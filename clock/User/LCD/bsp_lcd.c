#include "bsp_lcd.h"
#include "bsp_usart.h"
#include "fonts.h"
#include <stdio.h>  
#include <string.h>
#include "ascii.h" 
#include <math.h>

extern uint8_t const *WEEK_STR[] ;
extern uint8_t const *zodiac_sign[] ;


//液晶屏扫描模式
//参数可选值为0-7
uint8_t LCD_SCAN_MODE=6;
//根据液晶扫描方向而变化的XY像素宽度
//调用ILI9341_GramScan函数设置方向时会自动更改
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

#define ZOOMMAXBUFF 16384
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};    //用于缩放的缓存，最大支持到128*128
uint8_t zoomTempBuff[1024] = {0};

static sFONT *LCD_Currentfonts = &Font8x16;  //英文字体
static uint16_t CurrentTextColor   = BLACK;//前景色
static uint16_t CurrentBackColor   = WHITE;//背景色        
        
void LCD_GPIO_Config(void)
{

        GPIO_InitTypeDef  GPIO_InitStruct;      //配置连接到的GPIO

        // 打开GPIO的时钟
        RCC_APB2PeriphClockCmd(LCD_CS_CLK|LCD_RD_CLK|LCD_WE_CLK|LCD_RS_CLK|LCD_RESTE_CLK|LCD_BL_CLK|
                               ILI9341_D0_CLK|ILI9341_D1_CLK|ILI9341_D2_CLK|ILI9341_D3_CLK|ILI9341_D4_CLK|
                                ILI9341_D5_CLK|ILI9341_D6_CLK|ILI9341_D7_CLK|ILI9341_D8_CLK|ILI9341_D9_CLK|
                                ILI9341_D10_CLK|ILI9341_D11_CLK|ILI9341_D12_CLK|ILI9341_D13_CLK|ILI9341_D14_CLK|ILI9341_D15_CLK, ENABLE);

        /*RESTE和BL背光灯设置普通的推挽输出*/

        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Pin = LCD_RESTE_GPIO_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LCD_RESTE_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = LCD_BL_GPIO_PIN;
        GPIO_Init(LCD_BL_GPIO_PORT, &GPIO_InitStruct);

        /*设置复用推挽输出*/

        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Pin = LCD_CS_GPIO_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = LCD_RD_GPIO_PIN;
        GPIO_Init(LCD_RD_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = LCD_WE_GPIO_PIN;
        GPIO_Init(LCD_WE_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = LCD_RS_GPIO_PIN;
        GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStruct);

        /*配置相应的数据线*/

        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF_PP;

        GPIO_InitStruct.GPIO_Pin = ILI9341_D0_PIN;
        GPIO_Init ( ILI9341_D0_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D1_PIN;
        GPIO_Init ( ILI9341_D1_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D2_PIN;
        GPIO_Init ( ILI9341_D2_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D3_PIN;
        GPIO_Init ( ILI9341_D3_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D4_PIN;
        GPIO_Init ( ILI9341_D4_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D5_PIN;
        GPIO_Init ( ILI9341_D5_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D6_PIN;
        GPIO_Init ( ILI9341_D6_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D7_PIN;
        GPIO_Init ( ILI9341_D7_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D8_PIN;
        GPIO_Init ( ILI9341_D8_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D9_PIN;
        GPIO_Init ( ILI9341_D9_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D10_PIN;
        GPIO_Init ( ILI9341_D10_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D11_PIN;
        GPIO_Init ( ILI9341_D11_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D12_PIN;
        GPIO_Init ( ILI9341_D12_PORT, & GPIO_InitStruct );      

        GPIO_InitStruct.GPIO_Pin = ILI9341_D13_PIN;
        GPIO_Init ( ILI9341_D13_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D14_PIN;
        GPIO_Init ( ILI9341_D14_PORT, & GPIO_InitStruct );

        GPIO_InitStruct.GPIO_Pin = ILI9341_D15_PIN;
        GPIO_Init ( ILI9341_D15_PORT, & GPIO_InitStruct );
}

void LCD_FSMC_Config(void)
{
        FSMC_NORSRAMInitTypeDef             SRAMInitStructure;
        FSMC_NORSRAMTimingInitTypeDef       readTimStruct;
        FSMC_NORSRAMTimingInitTypeDef       writeTimStruct;

        RCC_AHBPeriphClockCmd (RCC_AHBPeriph_FSMC,ENABLE);

        //读时序

        //地址建立时间（ADDSET）
        readTimStruct.FSMC_AddressSetupTime = 0x01;     //理论值
        //数据保持时间（DATAST）
        readTimStruct.FSMC_DataSetupTime=0x04;  //理论值为1 

        //选择匹配SRAM的模式
        readTimStruct.FSMC_AccessMode = FSMC_AccessMode_B ;
        //地址保持时间（ADDHLD）模式B未用到
        readTimStruct.FSMC_AddressHoldTime = 0x00 ; //SRAM没有用到	
        //设置总线转换周期，仅用于复用模式的NOR操作
        readTimStruct.FSMC_BusTurnAroundDuration=0x00;	//SRAM没有用到
        //设置时钟分频，仅用于同步类型的存储器
        readTimStruct.FSMC_CLKDivision=0x00;	//SRAM没有用到
        //数据保持时间，仅用于NOR
        readTimStruct.FSMC_DataLatency=0x00;	//SRAM没有用到
         

        //写时序
        writeTimStruct.FSMC_AccessMode = FSMC_AccessMode_B ;
        writeTimStruct.FSMC_AddressHoldTime = 0 ; //SRAM没有用到
        writeTimStruct.FSMC_AddressSetupTime = 0; 	//理论值
        writeTimStruct.FSMC_BusTurnAroundDuration=0;	//SRAM没有用到
        writeTimStruct.FSMC_CLKDivision=0;	//SRAM没有用到
        writeTimStruct.FSMC_DataLatency=0;	//SRAM没有用到
        writeTimStruct.FSMC_DataSetupTime=2;	//理论值为2  

        SRAMInitStructure.FSMC_Bank = FSMC_Bank_NORSRAMx ;
        SRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable ;
        SRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b ;
        SRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable ;
        //SRAM没有用到
        SRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
        SRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable ;
        SRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable ;	
        SRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;	
        SRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable  ;
        SRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState ;
        SRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low ;
        SRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable  ;
        SRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable ;
        //读写时序结构体
        SRAMInitStructure.FSMC_WriteTimingStruct = &writeTimStruct;
        SRAMInitStructure.FSMC_ReadWriteTimingStruct = &readTimStruct; //FSMC_ExtendedMode 配置enable有效

        FSMC_NORSRAMInit(&SRAMInitStructure);
        FSMC_NORSRAMCmd(FSMC_Bank_NORSRAMx,ENABLE);

}

void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
        
}

//复位函数
void ILI9341_Rst ( void )
{       
        GPIO_ResetBits ( LCD_RESTE_GPIO_PORT, LCD_RESTE_GPIO_PIN );     //低电平复位

        Delay ( 0xAFF );        
        
        GPIO_SetBits ( LCD_RESTE_GPIO_PORT, LCD_RESTE_GPIO_PIN );       

        Delay ( 0xAFF );        
        
}
//背光灯函数
void ILI9341_BackLed_Control ( FunctionalState enumState )
{
        if ( enumState )
        GPIO_ResetBits ( LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN );	
        else
        GPIO_SetBits ( LCD_BL_GPIO_PORT, LCD_BL_GPIO_PIN );
        
}

__inline void LCD_Write_Cmd ( uint16_t usCmd )
{
        *ILI9341_CMD_ADDR = usCmd;      
}

__inline void LCD_Write_Data ( uint16_t usData )
{
        * ILI9341_DATA_ADDR = usData;
        
}

__inline uint16_t LCD_Read_Data ( void )
{
        return ( *ILI9341_DATA_ADDR );

}

uint8_t Read_Format(void)
{
        LCD_Write_Cmd(0x0C);
        LCD_Read_Data();

        return LCD_Read_Data();
}       


static void LCD_REG_Config ( void )
{
        /*  Power control B (CFh)  */
        DEBUG_DELAY  ();
        LCD_Write_Cmd ( 0xCF  );
        LCD_Write_Data ( 0x00  );
        LCD_Write_Data ( 0x81  );
        LCD_Write_Data ( 0x30  );

        /*  Power on sequence control (EDh) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xED );
        LCD_Write_Data ( 0x64 );
        LCD_Write_Data ( 0x03 );
        LCD_Write_Data ( 0x12 );
        LCD_Write_Data ( 0x81 );

        /*  Driver timing control A (E8h) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xE8 );
        LCD_Write_Data ( 0x85 );
        LCD_Write_Data ( 0x10 );
        LCD_Write_Data ( 0x78 );

        /*  Power control A (CBh) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xCB );
        LCD_Write_Data ( 0x39 );
        LCD_Write_Data ( 0x2C );
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x34 );
        LCD_Write_Data ( 0x02 );

        /* Pump ratio control (F7h) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xF7 );
        LCD_Write_Data ( 0x20 );

        /* Driver timing control B */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xEA );
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x00 );

        /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xB1 );
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x1B );

        /*  Display Function Control (B6h) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xB6 );
        LCD_Write_Data ( 0x0A );
        LCD_Write_Data ( 0xA2 );

        /* Power Control 1 (C0h) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xC0 );
        LCD_Write_Data ( 0x35 );

        /* Power Control 2 (C1h) */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0xC1 );
        LCD_Write_Data ( 0x11 );

        /* VCOM Control 1 (C5h) */
        LCD_Write_Cmd ( 0xC5 );
        LCD_Write_Data ( 0x45 );
        LCD_Write_Data ( 0x45 );

        /*  VCOM Control 2 (C7h)  */
        LCD_Write_Cmd ( 0xC7 );
        LCD_Write_Data ( 0xA2 );

        /* Enable 3G (F2h) */
        LCD_Write_Cmd ( 0xF2 );
        LCD_Write_Data ( 0x00 );

        /* Gamma Set (26h) */
        LCD_Write_Cmd ( 0x26 );
        LCD_Write_Data ( 0x01 );
        DEBUG_DELAY ();

        /* Positive Gamma Correction */
        LCD_Write_Cmd ( 0xE0 ); //Set Gamma
        LCD_Write_Data ( 0x0F );
        LCD_Write_Data ( 0x26 );
        LCD_Write_Data ( 0x24 );
        LCD_Write_Data ( 0x0B );
        LCD_Write_Data ( 0x0E );
        LCD_Write_Data ( 0x09 );
        LCD_Write_Data ( 0x54 );
        LCD_Write_Data ( 0xA8 );
        LCD_Write_Data ( 0x46 );
        LCD_Write_Data ( 0x0C );
        LCD_Write_Data ( 0x17 );
        LCD_Write_Data ( 0x09 );
        LCD_Write_Data ( 0x0F );
        LCD_Write_Data ( 0x07 );
        LCD_Write_Data ( 0x00 );

        /* Negative Gamma Correction (E1h) */
        LCD_Write_Cmd ( 0XE1 ); //Set Gamma
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x19 );
        LCD_Write_Data ( 0x1B );
        LCD_Write_Data ( 0x04 );
        LCD_Write_Data ( 0x10 );
        LCD_Write_Data ( 0x07 );
        LCD_Write_Data ( 0x2A );
        LCD_Write_Data ( 0x47 );
        LCD_Write_Data ( 0x39 );
        LCD_Write_Data ( 0x03 );
        LCD_Write_Data ( 0x06 );
        LCD_Write_Data ( 0x06 );
        LCD_Write_Data ( 0x30 );
        LCD_Write_Data ( 0x38 );
        LCD_Write_Data ( 0x0F );

        /* memory access control set */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0x36 ); 	
        LCD_Write_Data ( 0xC8 );    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
        DEBUG_DELAY ();

        /* column address control set */
        LCD_Write_Cmd ( CMD_SetCoordinateX ); 
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0xEF );

        /* page address control set */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( CMD_SetCoordinateY ); 
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x00 );
        LCD_Write_Data ( 0x01 );
        LCD_Write_Data ( 0x3F );

        /*  Pixel Format Set (3Ah)  */
        DEBUG_DELAY ();
        LCD_Write_Cmd ( 0x3a ); 
        LCD_Write_Data ( 0x55 );

        /* Sleep Out (11h)  */
        LCD_Write_Cmd ( 0x11 );	
        Delay ( 0xAFFf<<2 );
        DEBUG_DELAY ();

        /* Display ON (29h) */
        LCD_Write_Cmd ( 0x29 ); 

}

void LCD_Init(void)
{
        LCD_GPIO_Config();
        LCD_FSMC_Config();
        ILI9341_BackLed_Control(ENABLE);
        ILI9341_Rst();
        LCD_REG_Config();
        LCD_GramScan(6);
        
}

/**
 * @brief  设置ILI9341的GRAM的扫描方向 
 * @param  ucOption ：选择GRAM的扫描方向 
 *     @arg 0-7 :参数可选值为0-7这八个方向
 *
 *	！！！其中0、3、5、6 模式适合从左至右显示文字，
 *				不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 *		
 *	其中0、2、4、6 模式的X方向像素为240，Y方向像素为320
 *	其中1、3、5、7 模式下X方向像素为320，Y方向像素为240
 *
 *	其中 6 模式为大部分液晶例程的默认显示方向
 *	其中 3 模式为摄像头例程使用的方向
 *	其中 0 模式为BMP图片显示例程使用的方向
 *
 * @retval 无
 * @note  坐标图例：A表示向上，V表示向下，<表示向左，>表示向右
					X表示X轴，Y表示Y轴

------------------------------------------------------------
模式0：				.		模式1：		.	模式2：			.	模式3：					
		A		.		A		.		A		.		A									
		|		.		|		.		|		.		|							
		Y		.		X		.		Y		.		X					
		0		.		1		.		2		.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
模式4：				.	模式5：			.	模式6：			.	模式7：					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
		4		.		5		.		6		.		7	
		Y		.		X		.		Y		.		X						
		|		.		|		.		|		.		|							
		V		.		V		.		V		.		V		
---------------------------------------------------------				
											 LCD屏示例
								|-----------------|
								|     秉火Logo    |
								|		  |
								|		  |
								|	          |
								|	          |
								|	          |
								|		  |
								|		  |
								|		  |
								|-----------------|
								屏幕正面（宽240，高320）

 *******************************************************/
void LCD_GramScan ( uint8_t ucOption )
{       
        //参数检查，只可输入0-7
        if(ucOption >7 )
                return;

        //根据模式更新LCD_SCAN_MODE的值，主要用于触摸屏选择计算参数
        LCD_SCAN_MODE = ucOption;

        //根据模式更新XY方向的像素宽度
        if(ucOption%2 == 0)     
        {
                //0 2 4 6模式下X方向像素宽度为240，Y方向为320
                LCD_X_LENGTH = ILI9341_LESS_PIXEL;
                LCD_Y_LENGTH = ILI9341_MORE_PIXEL;
        }
        else    
        {
                //1 3 5 7模式下X方向像素宽度为320，Y方向为240
                LCD_X_LENGTH = ILI9341_MORE_PIXEL;
                LCD_Y_LENGTH = ILI9341_LESS_PIXEL; 
        }

        //0x36命令参数的高3位可用于设置GRAM扫描方向     
        LCD_Write_Cmd ( 0x36 ); 
        LCD_Write_Data ( 0x08 |(ucOption<<5));//根据ucOption的值设置LCD参数，共0-7种模式
        LCD_Write_Cmd ( CMD_SetCoordinateX ); 
        LCD_Write_Data ( 0x00 );        /* x 起始坐标高8位 */
        LCD_Write_Data ( 0x00 );        /* x 起始坐标低8位 */
        LCD_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x 结束坐标高8位 */  
        LCD_Write_Data ( (LCD_X_LENGTH-1)&0xFF );       /* x 结束坐标低8位 */

        LCD_Write_Cmd ( CMD_SetCoordinateY ); 
        LCD_Write_Data ( 0x00 );        /* y 起始坐标高8位 */
        LCD_Write_Data ( 0x00 );        /* y 起始坐标低8位 */
        LCD_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );  /* y 结束坐标高8位 */ 
        LCD_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );       /* y 结束坐标低8位 */

        /* write gram start */
        LCD_Write_Cmd ( CMD_SetPixel );	
}




void LCD_OpenWindow(uint16_t usX,uint16_t usY,uint16_t usWidth,uint16_t usHeight)
{
        LCD_Write_Cmd ( CMD_SetCoordinateX );
        LCD_Write_Data( (usX & 0xff00)>>8);
        LCD_Write_Data( (usX & 0x00ff));
        LCD_Write_Data( ((usX+usWidth-1) & 0xff00 ) >>8 );
        LCD_Write_Data( (usX +usWidth-1) & 0x00ff );

        LCD_Write_Cmd ( CMD_SetCoordinateY );
        LCD_Write_Data( (usY & 0xff00) >> 8 );
        LCD_Write_Data( (usY & 0x00ff));
        LCD_Write_Data( ((usY+usHeight-1) & 0xff00 ) >> 8 );
        LCD_Write_Data( (usY+usHeight-1) & 0x00ff );
}



/**
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
  * 参数为以下值之一
  * @arg：Font24x32;
  * @arg：Font16x24;
  * @arg：Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}
/**
 * @brief  设定ILI9341的光标坐标
 * @param  usX ：在特定扫描方向下光标的X坐标
 * @param  usY ：在特定扫描方向下光标的Y坐标
 * @retval 无
 */
static void LCD_SetCursor ( uint16_t usX, uint16_t usY )        
{
        LCD_OpenWindow ( usX, usY, 1, 1 );
}


/**
 * @brief  在ILI9341显示器上以某一颜色填充像素点
 * @param  ulAmout_Point ：要填充颜色的像素点的总数目
 * @param  usColor ：颜色
 * @retval 无
 */
static __inline void LCD_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	/* memory write */
	LCD_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
	LCD_Write_Data ( usColor );	
}

/**
  * @brief  设置LCD的前景(字体)及背景颜色,RGB565
  * @param  TextColor: 指定前景(字体)颜色
  * @param  BackColor: 指定背景颜色
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
 * @brief  对ILI9341显示器的某一窗口以某种颜色进行清屏
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{
        LCD_OpenWindow ( usX, usY, usWidth, usHeight );

        LCD_FillColor ( usWidth * usHeight, CurrentBackColor );		

}

/**
 * @brief  在 ILI9341 显示器上显示一个英文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下该点的起始Y坐标
 * @param  cChar ：要显示的英文字符
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
        uint8_t  byteCount, bitCount,fontLength;        
        uint16_t ucRelativePositon;
        uint8_t *Pfont;

        //对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
        ucRelativePositon = cChar - ' ';

        //每个字模的字节数
        fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
                
        //字模首地址
        /*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
        Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];

        //设置显示窗口
        LCD_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);

        LCD_Write_Cmd ( CMD_SetPixel ); 

        //按字节读取字模数据
        //由于前面直接设置了显示窗口，显示数据会自动换行
        for ( byteCount = 0; byteCount < fontLength; byteCount++ )
        {
                        //一位一位处理要显示的颜色
                        for ( bitCount = 0; bitCount < 8; bitCount++ )
                        {
                                        if ( Pfont[byteCount] & (0x80>>bitCount) )
                                                LCD_Write_Data ( CurrentTextColor );    
                                        else
                                                LCD_Write_Data ( CurrentBackColor );
                        }       
        }       
}

/**
 * @brief  在 ILI9341 显示器上显示英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
 *       显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispStringLine_EN (  uint16_t line,  char * pStr )
{
        uint16_t usX = 0;

        while ( * pStr != '\0' )
        {
                if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
                {
                        usX = ILI9341_DispWindow_X_Star;
                        line += LCD_Currentfonts->Height;
                }
                
                if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
                {
                        usX = ILI9341_DispWindow_X_Star;
                        line = ILI9341_DispWindow_Y_Star;
                }
                
                LCD_DispChar_EN ( usX, line, * pStr);
                
                pStr ++;
                
                usX += LCD_Currentfonts->Width;
                
        }

}

/**
 * @brief  在 ILI9341 显示器上显示一个中文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  usChar ：要显示的中文字符（国标码）
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */ 
void LCD_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar )
{
        uint8_t rowCount, bitCount;
        uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];    
        uint16_t usTemp;        

        //设置显示窗口
        LCD_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR );

        LCD_Write_Cmd ( CMD_SetPixel );

        //取字模数据  
        GetGBKCode ( ucBuffer, usChar );

        for ( rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++ )
        {
        /* 取出两个字节的数据，在lcd上即是一个汉字的一行 */
                usTemp = ucBuffer [ rowCount * 2 ];
                usTemp = ( usTemp << 8 );
                usTemp |= ucBuffer [ rowCount * 2 + 1 ];
                
                for ( bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount ++ )
                {       
                        if ( usTemp & ( 0x8000 >> bitCount ) )  //高位在前 
                                LCD_Write_Data ( CurrentTextColor );    
                        else
                                LCD_Write_Data ( CurrentBackColor );    
                }       
        }

}

void   LCD_DispString_CH ( uint16_t usX , uint16_t usY, char * pStr )
{       
        uint16_t usCh;


        while( * pStr != '\0' )
        {       
                if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
                {
                        usX = ILI9341_DispWindow_X_Star;
                        usY += HEIGHT_CH_CHAR;
                }
                
                if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
                {
                        usX = ILI9341_DispWindow_X_Star;
                        usY = ILI9341_DispWindow_Y_Star;
                }       
                
                usCh = * ( uint16_t * ) pStr;   
          usCh = ( usCh << 8 ) + ( usCh >> 8 );

                LCD_DispChar_CH ( usX, usY, usCh );
                
                usX += WIDTH_CH_CHAR;
                
                pStr += 2;           //一个汉字两个字节 

        }       

}


/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
  *       显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispStringLine_EN_CH (  uint16_t line, char * pStr )
{
        uint16_t usCh;
        uint16_t usX = 0;

        while( * pStr != '\0' )
        {
                if ( * pStr <= 126 )    //英文字符
                {
                        if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                line += LCD_Currentfonts->Height;
                        }
                        
                        if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                line = ILI9341_DispWindow_Y_Star;
                        }       
                  
                  LCD_DispChar_EN ( usX, line, * pStr );
                        
                        usX +=  LCD_Currentfonts->Width;
                
                  pStr ++;

                }
                
                else    //汉字字符
                {
                        if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                line += HEIGHT_CH_CHAR;
                        }
                        
                        if ( ( line - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                line = ILI9341_DispWindow_Y_Star;
                        }       
                        
                        usCh = * ( uint16_t * ) pStr;   
                        
                        usCh = ( usCh << 8 ) + ( usCh >> 8 );   

                        LCD_DispChar_CH ( usX, line, usCh );
                        
                        usX += WIDTH_CH_CHAR;
                        
                        pStr += 2;           //一个汉字两个字节 
                
                }
                
        }     
} 

/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispString_EN_CH ( uint16_t usX , uint16_t usY, char * pStr )
{
        uint16_t usCh;

        while( * pStr != '\0' )
        {
                if ( * pStr <= 126 )            //英文字符
                {
                        if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                usY += LCD_Currentfonts->Height;
                        }
                        
                        if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                usY = ILI9341_DispWindow_Y_Star;
                        }       
                
                  LCD_DispChar_EN ( usX, usY, * pStr );
                        
                        usX +=  LCD_Currentfonts->Width;
                
                  pStr ++;

                }
                
                else    //汉字字符
                {
                        if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                usY += HEIGHT_CH_CHAR;
                        }
                        
                        if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
                        {
                                usX = ILI9341_DispWindow_X_Star;
                                usY = ILI9341_DispWindow_Y_Star;
                        }       
                        
                        usCh = * ( uint16_t * ) pStr;   
                        
                        usCh = ( usCh << 8 ) + ( usCh >> 8 );   

                        LCD_DispChar_CH ( usX, usY, usCh );
                        
                        usX += WIDTH_CH_CHAR;
                        
                        pStr += 2;           //一个汉字两个字节 
                
                }
                
        }       
} 

/**
 * @brief  对ILI9341显示器的某一点以某种颜色进行填充
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_SetPointPixel ( uint16_t usX, uint16_t usY )	
{       
        if ( ( usX < LCD_X_LENGTH ) && ( usY < LCD_Y_LENGTH ) )
        {
                LCD_SetCursor ( usX, usY );
                
                LCD_FillColor ( 1, CurrentTextColor );
        }

}
/**
 * @brief  读取ILI9341 GRAN 的一个像素数据
 * @param  无
 * @retval 像素数据
 */
static uint16_t LCD_Read_PixelData ( void )	
{       
        uint16_t usR=0, usG=0, usB=0 ;


        LCD_Write_Cmd ( 0x2E );   /* 读数据 */

        usR = LCD_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/

        usR = LCD_Read_Data ();  	/*READ OUT RED DATA  */
        usB = LCD_Read_Data ();  	/*READ OUT BLUE DATA*/
        usG = LCD_Read_Data ();  	/*READ OUT GREEN DATA*/	

        return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );

}
/**
 * @brief  获取 ILI9341 显示器上某一个坐标点的像素数据
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @retval 像素数据
 */
uint16_t LCD_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
        uint16_t usPixelData;
        LCD_SetCursor ( usX, usY );

        usPixelData = LCD_Read_PixelData ();

        return usPixelData;

}





/**
  * @brief  清除某行文字
  * @param  Line: 指定要删除的行
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定要删除的行，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值，并删除当前字体高度的第x行。
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  LCD_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);      /* 清屏，显示全黑 */

}


/**
 * @brief  利用缩放后的字模显示字符串
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度，英文字符在此基础上/2。注意为偶数
 * @param  Font_Heig：字符高度，注意为偶数
 * @param  c ：要显示的字符串
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void LCD_DisplayStringEx(uint16_t x,  uint16_t y,uint16_t Font_width,uint16_t Font_Height,      uint8_t *ptr,uint16_t DrawModel)  

{
        uint16_t Charwidth = Font_width; //默认为Font_width，英文宽度为中文宽度的一半
        uint8_t *psr;
        uint8_t Ascii;	//英文
        uint16_t usCh;  //中文
        uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	

        while ( *ptr != '\0')
        {
                        /****处理换行*****/
                        if ( ( x - ILI9341_DispWindow_X_Star + Charwidth ) > LCD_X_LENGTH )
                        {
                                x = ILI9341_DispWindow_X_Star;
                                y += Font_Height;
                        }
                        
                        if ( ( y - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
                        {
                                x = ILI9341_DispWindow_X_Star;
                                y = ILI9341_DispWindow_Y_Star;
                        }	
                        
                if(*ptr > 0x80) //如果是中文
                {			
                        Charwidth = Font_width;
                        usCh = * ( uint16_t * ) ptr;				
                        usCh = ( usCh << 8 ) + ( usCh >> 8 );
                        GetGBKCode ( ucBuffer, usCh );	//取字模数据
                        //缩放字模数据，源字模为16*16
                        LCD_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
                        //显示单个字符
                        LCD_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
                        x+=Charwidth;
                        ptr+=2;
                }
                else
                {
                                Charwidth = Font_width / 2;
                                Ascii = *ptr - 32;
                                //使用16*24字体缩放字模数据
                                LCD_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
                          //显示单个字符
                                LCD_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
                                x+=Charwidth;
                                ptr++;
                }
        }
}
/**
 * @brief  利用缩放后的字模显示字符
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度
 * @param  Font_Heig：字符高度
 * @param  c ：要显示的字模数据
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void LCD_DrawChar_Ex(uint16_t usX, uint16_t usY, uint16_t Font_width, uint16_t Font_Height,uint8_t *c,uint16_t DrawModel)
{
  uint32_t index = 0, counter = 0;

        //设置显示窗口
        LCD_OpenWindow ( usX, usY, Font_width, Font_Height);

        LCD_Write_Cmd ( CMD_SetPixel ); 

        //按字节读取字模数据
        //由于前面直接设置了显示窗口，显示数据会自动换行
        for ( index = 0; index < Font_Height; index++ )
        {
                        //一位一位处理要显示的颜色
                        for ( counter = 0; counter < Font_width; counter++ )
                        {
                                        //缩放后的字模数据，以一个字节表示一个像素位
                                        //整个字节值为1表示该像素为笔迹
                                        //整个字节值为0表示该像素为背景
                                        if ( *c++ == DrawModel )
                                                LCD_Write_Data ( CurrentBackColor );    
                                        else
                                                LCD_Write_Data ( CurrentTextColor );
                        }       
        }       
}
/**
 * @brief  缩放字模，缩放后的字模由1个像素点由8个数据位来表示
                0x01表示笔迹，0x00表示空白区
 * @param  in_width ：原始字符宽度
 * @param  in_heig ：原始字符高度
 * @param  out_width ：缩放后的字符宽度
 * @param  out_heig：缩放后的字符高度
 * @param  in_ptr ：字库输入指针       注意：1pixel 1bit
 * @param  out_ptr ：缩放后的字符输出指针 注意: 1pixel 8bit
 *         out_ptr实际上没有正常输出，改成了直接输出到全局指针zoomBuff中
 * @param  en_cn ：0为英文，1为中文
 * @retval 无
 */
void LCD_zoomChar(uint16_t in_width,uint16_t in_heig,uint16_t out_width,	
			uint16_t out_heig,uint8_t *in_ptr,uint8_t *out_ptr, uint8_t en_cn)	
{
        uint8_t *pts,*ots;
        //根据源字模及目标字模大小，设定运算比例因子，左移16是为了把浮点运算转成定点运算
        unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
        unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;

        unsigned int srcy_16=0;
        unsigned int y,x;
        uint8_t *pSrcLine;

        uint16_t byteCount,bitCount;

        //检查参数是否合法
        if(in_width >= 32) return;												//字库不允许超过32像素
        if(in_width * in_heig == 0) return;	
        if(in_width * in_heig >= 1024 ) return; 					//限制输入最大 32*32

        if(out_width * out_heig == 0) return;	
        if(out_width * out_heig >= ZOOMMAXBUFF ) return; //限制最大缩放 128*128
        pts = (uint8_t*)&zoomTempBuff;

        //为方便运算，字库的数据由1 pixel/1bit 映射到1pixel/8bit
        //0x01表示笔迹，0x00表示空白区
        if(en_cn == 0x00)//英文
        {
                //英文和中文字库上下边界不对，可在此处调整。需要注意tempBuff防止溢出
                        for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
                        {
                                for(bitCount=0;bitCount<8;bitCount++)
                                        {						
                                                //把源字模数据由位映射到字节
                                                //in_ptr里bitX为1，则pts里整个字节值为1
                                                //in_ptr里bitX为0，则pts里整个字节值为0
                                                *pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
                                        }
                        }				
        }
        else //中文
        {			
                        for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
                        {
                                for(bitCount=0;bitCount<8;bitCount++)
                                        {						
                                                //把源字模数据由位映射到字节
                                                //in_ptr里bitX为1，则pts里整个字节值为1
                                                //in_ptr里bitX为0，则pts里整个字节值为0
                                                *pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
                                        }
                        }		
        }

        //zoom过程
        pts = (uint8_t*)&zoomTempBuff;	//映射后的源数据指针
        ots = (uint8_t*)&zoomBuff;	//输出数据的指针
        for (y=0;y<out_heig;y++)	/*行遍历*/
        {
                                unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);				
        for (x=0;x<out_width;x++) /*行内像素遍历*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //把源字模数据复制到目标指针中
            srcx_16+=xrIntFloat_16;			//按比例偏移源像素点
        }
        srcy_16+=yrIntFloat_16;				  //按比例偏移源像素点
        ots+=out_width;						
        }
        /*！！！缩放后的字模数据直接存储到全局指针zoomBuff里了*/
        out_ptr = (uint8_t*)&zoomBuff;	//out_ptr没有正确传出，后面调用直接改成了全局变量指针！

        /*实际中如果使用out_ptr不需要下面这一句！！！
                只是因为out_ptr没有使用，会导致warning。强迫症*/
        out_ptr++; 
}

/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画线段 
 * @param  usX1 ：在特定扫描方向下线段的一个端点X坐标
 * @param  usY1 ：在特定扫描方向下线段的一个端点Y坐标
 * @param  usX2 ：在特定扫描方向下线段的另一个端点X坐标
 * @param  usY2 ：在特定扫描方向下线段的另一个端点Y坐标
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 )
{
        uint16_t us; 
        uint16_t usX_Current, usY_Current;

        int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
        int32_t lIncrease_X, lIncrease_Y; 	


        lDelta_X = usX2 - usX1; //计算坐标增量 
        lDelta_Y = usY2 - usY1; 

        usX_Current = usX1; 
        usY_Current = usY1; 


        if ( lDelta_X > 0 ) 
                lIncrease_X = 1; //设置单步方向 

        else if ( lDelta_X == 0 ) 
                lIncrease_X = 0;//垂直线 

        else 
        { 
        lIncrease_X = -1;
        lDelta_X = - lDelta_X;
        } 


        if ( lDelta_Y > 0 )
                lIncrease_Y = 1; 

        else if ( lDelta_Y == 0 )
                lIncrease_Y = 0;//水平线 

        else 
        {
        lIncrease_Y = -1;
        lDelta_Y = - lDelta_Y;
        } 


        if (  lDelta_X > lDelta_Y )
                lDistance = lDelta_X; //选取基本增量坐标轴 

        else 
                lDistance = lDelta_Y; 


        for ( us = 0; us <= lDistance + 1; us ++ )//画线输出 
        {  
                LCD_SetPointPixel ( usX_Current, usY_Current );//画点 
                
                lError_X += lDelta_X ; 
                lError_Y += lDelta_Y ; 
                
                if ( lError_X > lDistance ) 
                { 
                        lError_X -= lDistance; 
                        usX_Current += lIncrease_X; 
                }  
                
                if ( lError_Y > lDistance ) 
                { 
                        lError_Y -= lDistance; 
                        usY_Current += lIncrease_Y; 
                } 
                
        }  


}   
/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画线段 
 * @param  usX1 ：在特定扫描方向下线段的一个端点X坐标
 * @param  usY1 ：在特定扫描方向下线段的一个端点Y坐标
 * @param  usL ：在特定扫描方向下线段的长度
 * @param  usW ：在特定扫描方向下线段的角度
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DrawLine_Polar(uint16_t X1, uint16_t Y1, uint16_t L, uint16_t angle )
{
        u16 X2,Y2;  //另一个点的坐标
        float radian;  //弧度
        radian = angle*Pi/180;
        X2=(u16)(X1+L*cos(radian));
        Y2=(u16)(Y1+L*sin(radian));
        LCD_DrawLine(X1,Y1,X2,Y2);
}



/**
  * @brief  设置LCD的前景(字体)颜色,RGB565
  * @param  Color: 指定前景(字体)颜色 
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
 * @brief  在 ILI9341 显示器上画一个矩形
 * @param  usX_Start ：在特定扫描方向下矩形的起始点X坐标
 * @param  usY_Start ：在特定扫描方向下矩形的起始点Y坐标
 * @param  usWidth：矩形的宽度（单位：像素）
 * @param  usHeight：矩形的高度（单位：像素）
 * @param  ucFilled ：选择是否填充该矩形
  *   该参数为以下值之一：
  *     @arg 0 :空心矩形
  *     @arg 1 :实心矩形 
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled )
{
        if ( ucFilled )
        {
                LCD_OpenWindow ( usX_Start, usY_Start, usWidth, usHeight );
                LCD_FillColor ( usWidth * usHeight ,CurrentTextColor);	
        }
        else
        {
                LCD_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start );
                LCD_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );
                LCD_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 );
                LCD_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );		
        }

}

/**
 * @brief  在屏幕上显示时间
 *
 */
void LCD_Time_Show(struct rtc_time *tm)
{
        char dataStr [ 1500 ] = { 0 };
        char TimeStr [ 1500 ] = { 0 };
        u16  anagle,temp;
        int sec_anagle,hour_anagle,min_anagle;
        sprintf(dataStr,"%d年 (%s年) %d月%d日 星期%s",tm->tm_year,zodiac_sign[(tm->tm_year-3)%12],
        tm->tm_mon, tm->tm_mday,WEEK_STR[tm->tm_wday]);
        sprintf(TimeStr,"%0.2d:%0.2d:%0.2d", tm->tm_hour,tm->tm_min, tm->tm_sec);

        LCD_SetTextColor(RED);
        LCD_DispString_EN_CH(88,240,TimeStr);
        LCD_DispString_EN_CH(8,270,dataStr);
        
        anagle = tm->tm_sec*6;
        
        if(anagle<90)
        {
                sec_anagle=anagle+360-90;
        }
        else
        {
                sec_anagle=anagle-90;
        }
        
        //檫除上一次的痕迹
        LCD_SetTextColor(WHITE);
        if(sec_anagle==0)
        {
                temp=354;
                LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, temp);
        }
        else
        {
                LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, sec_anagle-6);
        }
//        LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, sec_anagle);
//        LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, sec_anagle+6);
        //画新的指针
        LCD_SetTextColor(RED);
        LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, sec_anagle);
        
        //分针部分
        anagle = tm->tm_min*6;
                
        if(anagle<90)
        {
                min_anagle=anagle+360-90;
        }
        else
        {
                min_anagle=anagle-90;
        }
        
        if(tm->tm_sec==0)
        {
                //檫除上一次的痕迹
                LCD_SetTextColor(WHITE);
                if(min_anagle==0)
                {
                        temp=354;
                        LCD_DrawLine_Polar(Xlabel, Ylabel, Min_length, temp);
                }
                else
                {
                        LCD_DrawLine_Polar(Xlabel, Ylabel, Min_length, min_anagle-6);
                }
//                LCD_DrawLine_Polar(Xlabel, Ylabel, Min_length, min_anagle);
//                LCD_DrawLine_Polar(Xlabel, Ylabel, Min_length, min_anagle+6);
        }
                //画新的指针
                LCD_SetTextColor(BLUE);
                LCD_DrawLine_Polar(Xlabel, Ylabel, Min_length, min_anagle);
        
                //时针部分
        
                
                anagle = ((tm->tm_hour%12)*30+tm->tm_min/2);
                
                if(hour_anagle<90)
                {
                        hour_anagle=anagle+360-90;
                }
                else
                {
                        hour_anagle=anagle-90;
                }
//        if(tm->tm_min==0)
//        {
                //檫除上一次的痕迹
                LCD_SetTextColor(WHITE);
                if(hour_anagle==0)
                {
                        temp=359;
                        LCD_DrawLine_Polar(Xlabel, Ylabel, Hour_length, temp);
                }
                else
                {
                        LCD_DrawLine_Polar(Xlabel, Ylabel, Hour_length, hour_anagle-1);
                }
//                LCD_DrawLine_Polar(Xlabel, Ylabel, Hour_length, hour_anagle);
//                LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, hour_anagle+1);
//        }
                //画新的指针
                LCD_SetTextColor(BLACK);
                LCD_DrawLine_Polar(Xlabel, Ylabel, Hour_length, hour_anagle);
        
}
