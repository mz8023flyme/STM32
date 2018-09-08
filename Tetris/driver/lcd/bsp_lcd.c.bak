#include "bsp_lcd.h"
#include "main.h"

static sFONT *LCD_Currentfonts = &Font8x16;  //英文字体
static uint16_t CurrentTextColor   = WHITE;//前景色
static uint16_t CurrentBackColor   = BLACK;//背景色 

uint16_t LCD_X_LENGTH = LCD_DispWindow_PAGE;
uint16_t LCD_Y_LENGTH = LCD_DispWindow_COLUMN;

#define ZOOMMAXBUFF 16384
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};    //用于缩放的缓存，最大支持到128*128
uint8_t zoomTempBuff[1024] = {0};

static void                   LCD_Delay               ( __IO uint32_t nCount );
static void                   LCD_GPIO_Config         ( void );
static void                   LCD_FSMC_Config         ( void );
static void                   LCD_REG_Config          ( void );
static void                   LCD_SetCursor           ( uint16_t usX, uint16_t usY );
static __inline void          LCD_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );
static uint16_t               LCD_Read_PixelData      ( void );



/**
  * @brief  向ILI9341写入命令
  * @param  usCmd :要写入的命令（表寄存器地址）
  * @retval 无
  */ 
__inline void LCD_Write_Cmd ( uint16_t usCmd )
{
        * ( __IO uint16_t * ) ( LCD_FSMC_Addr_CMD ) = usCmd;

}


/**
  * @brief  向ILI9341写入数据
  * @param  usData :要写入的数据
  * @retval 无
  */ 
__inline void LCD_Write_Data ( uint16_t usData )
{
        * ( __IO uint16_t * ) ( LCD_FSMC_Addr_DATA ) = usData;

}


/**
  * @brief  从ILI9341读取数据
  * @param  无
  * @retval 读取到的数据
  */	
__inline uint16_t LCD_Read_Data ( void )
{
        return ( * ( __IO uint16_t * ) ( LCD_FSMC_Addr_DATA ) );
 
}


/**
  * @brief  用于 ILI9341 简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */    
static void LCD_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
        
}


/**
  * @brief  初始化ILI9341的IO引脚
  * @param  无
  * @retval 无
  */
static void LCD_GPIO_Config ( void )
{
        GPIO_InitTypeDef GPIO_InitStructure;


        /* 使能复用IO时钟*/
        RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );

        /* 使能FSMC对应相应管脚时钟*/
        RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE );
                
        /* 配置FSMC相对应的数据线,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10 */	
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_8 | GPIO_Pin_9 | 
                                                                                                                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_Init ( GPIOD, & GPIO_InitStructure );

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                                                                                                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                                                                                                                GPIO_Pin_15;
        GPIO_Init ( GPIOE,  & GPIO_InitStructure ); 


        /* 配置FSMC相对应的控制线
         * PD4-FSMC_NOE   :LCD-RD
         * PD5-FSMC_NWE   :LCD-WR
         * PD7-FSMC_NE1   :LCD-CS
         * PD11-FSMC_A16  :LCD-DC
         */
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
        GPIO_Init (GPIOD, & GPIO_InitStructure );

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
        GPIO_Init (GPIOD, & GPIO_InitStructure );

        LCD_CS_APBxClock_FUN ( LCD_CS_CLK, ENABLE );
        GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN; 
        GPIO_Init ( LCD_CS_PORT, & GPIO_InitStructure );  

        LCD_DC_APBxClock_FUN ( LCD_DC_CLK, ENABLE );
        GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN; 
        GPIO_Init ( LCD_DC_PORT, & GPIO_InitStructure );


        /* 配置LCD复位RST控制管脚*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        LCD_RST_APBxClock_FUN ( LCD_RST_CLK, ENABLE );
        GPIO_InitStructure.GPIO_Pin =  LCD_RST_PIN; 
        GPIO_Init ( LCD_RST_PORT, & GPIO_InitStructure );


        /* 配置LCD背光控制管脚BK*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  

        LCD_BK_APBxClock_FUN ( LCD_BK_CLK, ENABLE );
        GPIO_InitStructure.GPIO_Pin = LCD_BK_PIN; 
        GPIO_Init ( LCD_BK_PORT, & GPIO_InitStructure );


}


 /**
  * @brief  LCD  FSMC 模式配置
  * @param  无
  * @retval 无
  */
static void LCD_FSMC_Config ( void )
{
        FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
        FSMC_NORSRAMTimingInitTypeDef  p; 


        /* 使能FSMC时钟*/
        RCC_AHBPeriphClockCmd ( RCC_AHBPeriph_FSMC, ENABLE );


        p.FSMC_AddressSetupTime      = 0x02;    //地址建立时间
        p.FSMC_AddressHoldTime       = 0x00;    //地址保持时间
        p.FSMC_DataSetupTime         = 0x05;    //数据建立时间
        p.FSMC_BusTurnAroundDuration = 0x00;
        p.FSMC_CLKDivision           = 0x00;
        p.FSMC_DataLatency           = 0x00;
        p.FSMC_AccessMode            = FSMC_AccessMode_B;       //模式B比较适用于ILI9341

        FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAMx;
        FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
        //FSMC_NORSRAMInitStructure.FSMC_MemoryType          = FSMC_MemoryType_SRAM;
        FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
        FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
        FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
        FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
        FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
        FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
        FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = & p;
        FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = & p;  

        FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 


        /* 使能 FSMC_Bank1_NORSRAM4 */
        FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAMx, ENABLE );  
                
                
}


/**
 * @brief  初始化ILI9341寄存器
 * @param  无
 * @retval 无
 */
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
        LCD_Delay ( 0xAFFf<<2 );
        DEBUG_DELAY ();
        /* Display ON (29h) */
        LCD_Write_Cmd ( 0x29 ); 


}


/**
 * @brief  ILI9341初始化函数，如果要用到lcd，一定要调用这个函数
 * @param  无
 * @retval 无
 */
void LCD_Init ( void )
{
        
        LCD_GPIO_Config ();
        printf("[%s][%d]\r\n", __func__, __LINE__);
        LCD_FSMC_Config ();
        LCD_BackLed_Control ( ENABLE );      //点亮LCD背光灯
        LCD_Rst ();
        LCD_REG_Config ();
        LCD_GramScan(6);
        
}


/**
 * @brief  ILI9341背光LED控制
 * @param  enumState ：决定是否使能背光LED
  *   该参数为以下值之一：
  *     @arg ENABLE :使能背光LED
  *     @arg DISABLE :禁用背光LED
 * @retval 无
 */
void LCD_BackLed_Control ( FunctionalState enumState )
{
        if ( enumState )
                GPIO_ResetBits (LCD_BK_PORT, LCD_BK_PIN );

        else
                GPIO_SetBits ( LCD_BK_PORT, LCD_BK_PIN );
                
}



/**
 * @brief  ILI9341 软件复位
 * @param  无
 * @retval 无
 */
void LCD_Rst ( void )
{       
        GPIO_ResetBits ( LCD_RST_PORT, LCD_RST_PIN ); //低电平复位

        LCD_Delay ( 0xAFFf<<2 );    

        GPIO_SetBits ( LCD_RST_PORT, LCD_RST_PIN );       

        LCD_Delay ( 0xAFFf<<2 ); 

}

void LCD_GramScan ( uint8_t ucOption )
{ 
        //参数检查，只可输入0-7
        if(ucOption >7 )
                return;

        //根据模式更新XY方向的像素宽度
        if(ucOption%2 == 0)     
        {
                //0 2 4 6模式下X方向像素宽度为240，Y方向为320
                LCD_X_LENGTH = LCD_LESS_PIXEL;
                LCD_Y_LENGTH = LCD_MORE_PIXEL;
        }
        else    
        {
                //1 3 5 7模式下X方向像素宽度为320，Y方向为240
                LCD_X_LENGTH = LCD_MORE_PIXEL;
                LCD_Y_LENGTH = LCD_LESS_PIXEL; 
        }

        //0x36命令参数的高3位可用于设置GRAM扫描方向     
        LCD_Write_Cmd ( 0x36 ); 
        LCD_Write_Data ( 0x08 |(ucOption<<5));//根据ucOption的值设置LCD参数，共0-7种模式
        LCD_Write_Cmd ( CMD_SetCoordinateX ); 
        LCD_Write_Data ( 0x00 );                /* x 起始坐标高8位 */
        LCD_Write_Data ( 0x00 );                /* x 起始坐标低8位 */
        LCD_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x 结束坐标高8位 */  
        LCD_Write_Data ( (LCD_X_LENGTH-1)&0xFF );               /* x 结束坐标低8位 */

        LCD_Write_Cmd ( CMD_SetCoordinateY ); 
        LCD_Write_Data ( 0x00 );                /* y 起始坐标高8位 */
        LCD_Write_Data ( 0x00 );                /* y 起始坐标低8位 */
        LCD_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );  /* y 结束坐标高8位 */ 
        LCD_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );               /* y 结束坐标低8位 */

        /* write gram start */
        LCD_Write_Cmd ( CMD_SetPixel ); 
}


/**
 * @brief  在ILI9341显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void LCD_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{ 
        LCD_Write_Cmd ( CMD_SetCoordinateX );    /* 设置X坐标 */
        LCD_Write_Data ( usX >> 8  );       /* 先高8位，然后低8位 */
        LCD_Write_Data ( usX & 0xff  );     /* 设置起始点和结束点*/
        LCD_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
        LCD_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

        LCD_Write_Cmd ( CMD_SetCoordinateY );    /* 设置Y坐标*/
        LCD_Write_Data ( usY >> 8  );
        LCD_Write_Data ( usY & 0xff  );
        LCD_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
        LCD_Write_Data ( ( usY + usHeight - 1) & 0xff );

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
 * @brief  对ILI9341显示器的某一窗口以某种颜色进行清屏
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @param  usColor ：颜色
 * @retval 无
 */
void LCD_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight, uint16_t usColor )
{
        LCD_OpenWindow ( usX, usY, usWidth, usHeight );

        LCD_FillColor ( usWidth * usHeight, usColor ); 

}


/**
 * @brief  对ILI9341显示器的某一点以某种颜色进行填充
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @param  usColor ：用于填充的颜色
 * @retval 无
 */
void LCD_SetPointPixel ( uint16_t usX, uint16_t usY, uint16_t usColor ) 
{ 
        if ( ( usX < LCD_DispWindow_COLUMN ) && ( usY < LCD_DispWindow_PAGE ) )
        {
                LCD_SetCursor ( usX, usY );
                
                LCD_FillColor ( 1, usColor );
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

        usR = LCD_Read_Data ();     /*FIRST READ OUT DUMMY DATA*/

        usR = LCD_Read_Data ();     /*READ OUT RED DATA  */
        usB = LCD_Read_Data ();     /*READ OUT BLUE DATA*/
        usG = LCD_Read_Data ();     /*READ OUT GREEN DATA*/ 

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
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画线段 
 * @param  usX1 ：在特定扫描方向下线段的一个端点X坐标
 * @param  usY1 ：在特定扫描方向下线段的一个端点Y坐标
 * @param  usX2 ：在特定扫描方向下线段的另一个端点X坐标
 * @param  usY2 ：在特定扫描方向下线段的另一个端点Y坐标
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
                LCD_SetPointPixel ( usX_Current, usY_Current, CurrentTextColor );//画点 
                
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
 * @brief  在 ILI9341 显示器上画一个矩形
 * @param  usX_Start ：在特定扫描方向下矩形的起始点X坐标
 * @param  usY_Start ：在特定扫描方向下矩形的起始点Y坐标
 * @param  usWidth：矩形的宽度（单位：像素）
 * @param  usHeight：矩形的高度（单位：像素）
 * @param  usColor ：矩形的颜色
 * @param  ucFilled ：选择是否填充该矩形
  *   该参数为以下值之一：
  *     @arg 0 :空心矩形
  *     @arg 1 :实心矩形
 * @retval 无
 */
void LCD_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled )
{
        if ( ucFilled )
          LCD_Clear ( usX_Start, usY_Start, usWidth, usHeight, CurrentTextColor);

        else
        {
                LCD_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start );
                LCD_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );
                LCD_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 );
                LCD_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );        
        }

}


/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画圆
 * @param  usX_Center ：在特定扫描方向下圆心的X坐标
 * @param  usY_Center ：在特定扫描方向下圆心的Y坐标
 * @param  usRadius：圆的半径（单位：像素）
 * @param  usColor ：圆的颜色
 * @param  ucFilled ：选择是否填充该圆
  *   该参数为以下值之一：
  *     @arg 0 :空心圆
  *     @arg 1 :实心圆
 * @retval 无
 */
void LCD_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint16_t usColor, uint8_t ucFilled )
{
        int16_t sCurrentX, sCurrentY;
        int16_t sError;


        sCurrentX = 0; sCurrentY = usRadius;    

        sError = 3 - ( usRadius << 1 );     //判断下个点位置的标志


        while ( sCurrentX <= sCurrentY )
        {
                int16_t sCountY;
                
                
                if ( ucFilled ) 
                        for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
                        {                      
                                LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY,   usColor );           //1，研究对象 
                                LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCountY,   usColor );           //2       
                                LCD_SetPointPixel ( usX_Center - sCountY,   usY_Center + sCurrentX, usColor );           //3
                                LCD_SetPointPixel ( usX_Center - sCountY,   usY_Center - sCurrentX, usColor );           //4
                                LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCountY,   usColor );           //5    
        LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCountY,   usColor );           //6
                                LCD_SetPointPixel ( usX_Center + sCountY,   usY_Center - sCurrentX, usColor );           //7 	
        LCD_SetPointPixel ( usX_Center + sCountY,   usY_Center + sCurrentX, usColor );           //0				
                        }
                
                else
                {          
                        LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY, usColor );             //1，研究对象
                        LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCurrentY, usColor );             //2      
                        LCD_SetPointPixel ( usX_Center - sCurrentY, usY_Center + sCurrentX, usColor );             //3
                        LCD_SetPointPixel ( usX_Center - sCurrentY, usY_Center - sCurrentX, usColor );             //4
                        LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCurrentY, usColor );             //5       
                        LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCurrentY, usColor );             //6
                        LCD_SetPointPixel ( usX_Center + sCurrentY, usY_Center - sCurrentX, usColor );             //7 
                        LCD_SetPointPixel ( usX_Center + sCurrentY, usY_Center + sCurrentX, usColor );             //0
        }       
                
                
                sCurrentX ++;

                
                if ( sError < 0 ) 
                        sError += 4 * sCurrentX + 6; 
                
                else
                {
                        sError += 10 + 4 * ( sCurrentX - sCurrentY );   
                        sCurrentY --;
                }       
                
                
        }


}

void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
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
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
  *       参数为以下值之一
  *     @arg：Font24x32;
  *     @arg：Font16x24;
  *     @arg：Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
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
 * @param  usX ：在特定扫描方向下字符串的起始X坐标
 * @param  usY ：在特定扫描方向下字符串的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @retval 无
 */
void LCD_DispString_EN ( uint16_t usX, uint16_t usY, const char * pStr)
{
        while ( * pStr != '\0' )
        {
                if ( ( usX - LCD_DispWindow_X_Star + WIDTH_EN_CHAR ) > LCD_DispWindow_COLUMN )
                {
                        usX = LCD_DispWindow_X_Star;
                        usY += HEIGHT_EN_CHAR;
                }
                
                if ( ( usY - LCD_DispWindow_Y_Star + HEIGHT_EN_CHAR ) > LCD_DispWindow_PAGE )
                {
                        usX = LCD_DispWindow_X_Star;
                        usY = LCD_DispWindow_Y_Star;
                }
                
                LCD_DispChar_EN ( usX, usY, * pStr );
                
                pStr ++;
                
                usX += WIDTH_EN_CHAR;
                
        }

}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
  *  显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void LCD_DispStringLine_EN (  uint16_t line,  char * pStr )
{
        uint16_t usX = 0;

        while ( * pStr != '\0' )
        {
                if ( ( usX - LCD_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
                {
                        usX = LCD_DispWindow_X_Star;
                        line += LCD_Currentfonts->Height;
                }
                
                if ( ( line - LCD_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
                {
                        usX = LCD_DispWindow_X_Star;
                        line = LCD_DispWindow_Y_Star;
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
 * @retval 无
 */ 
void LCD_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar)
{
        uint8_t ucPage, ucColumn;
        uint8_t ucBuffer [ 32 ];        

        uint16_t usTemp;        


        LCD_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR );

        LCD_Write_Cmd ( CMD_SetPixel );
           
        GetGBKCode ( ucBuffer, usChar );     //取字模数据

        for ( ucPage = 0; ucPage < HEIGHT_CH_CHAR; ucPage ++ )
        {
        /* 取出两个字节的数据，在lcd上即是一个汉字的一行 */
                usTemp = ucBuffer [ ucPage * 2 ];
                usTemp = ( usTemp << 8 );
                usTemp |= ucBuffer [ ucPage * 2 + 1 ];
                
                for ( ucColumn = 0; ucColumn < WIDTH_CH_CHAR; ucColumn ++ )
                {       
                        if ( usTemp & ( 0x01 << 15 ) )  //高位在前 
                          LCD_Write_Data ( CurrentTextColor );
                                
                        else
                                LCD_Write_Data ( CurrentBackColor );
                        
                        usTemp <<= 1;
                        
                }
                
        }

}


/**
 * @brief  在 ILI9341 显示器上显示中文字符串
 * @param  usX ：在特定扫描方向下字符串的起始X坐标
 * @param  usY ：在特定扫描方向下字符串的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址 
 * @retval 无
 */
void LCD_DispString_CH ( uint16_t usX, uint16_t usY, const uint8_t * pStr   )
{ 
        uint16_t usCh;


        while( * pStr != '\0' )
        {       
                if ( ( usX - LCD_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_DispWindow_COLUMN )
                {
                        usX = LCD_DispWindow_X_Star;
                        usY += HEIGHT_CH_CHAR;
                }
                
                if ( ( usY - LCD_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_DispWindow_PAGE )
                {
                        usX = LCD_DispWindow_X_Star;
                        usY = LCD_DispWindow_Y_Star;
                }       
                
                usCh = * ( uint16_t * ) pStr;   
                usCh = ( usCh << 8 ) + ( usCh >> 8 );

                LCD_DispChar_CH ( usX, usY, usCh  );
                
                usX += WIDTH_CH_CHAR;
                
                pStr += 2;           //一个汉字两个字节 

        }       

}


/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  usX ：在特定扫描方向下字符串的起始X坐标
 * @param  usY ：在特定扫描方向下字符串的起始Y坐标
 * @param  pStr ：要显示的字符串的首地址 
 * @retval 无
 */
void LCD_DispString_EN_CH ( uint16_t usX, uint16_t usY, const uint8_t * pStr  )
{
        uint16_t usCh;


        while( * pStr != '\0' )
        {
                if ( * pStr <= 126 )    //英文字符
                {
                        if ( ( usX - LCD_DispWindow_X_Star +  WIDTH_EN_CHAR ) >LCD_DispWindow_COLUMN )
                        {
                                usX = LCD_DispWindow_X_Star;
                                usY +=  HEIGHT_EN_CHAR;
                        }
                        
                        if ( ( usY - LCD_DispWindow_Y_Star +  HEIGHT_EN_CHAR ) > LCD_DispWindow_PAGE )
                        {
                                usX = LCD_DispWindow_X_Star;
                                usY = LCD_DispWindow_Y_Star;
                        }       
                
                  LCD_DispChar_EN ( usX, usY, * pStr );
                        
                        usX += WIDTH_EN_CHAR;
                
                  pStr ++;

                }
                
                else    //汉字字符
                {
                        if ( ( usX - LCD_DispWindow_X_Star +  WIDTH_CH_CHAR ) > LCD_DispWindow_COLUMN )
                        {
                                usX = LCD_DispWindow_X_Star;
                                usY +=  HEIGHT_CH_CHAR;
                        }
                        
                        if ( ( usY - LCD_DispWindow_Y_Star +  HEIGHT_CH_CHAR ) > LCD_DispWindow_PAGE )
                        {
                                usX = LCD_DispWindow_X_Star;
                                usY = LCD_DispWindow_Y_Star;
                        }       
                        
                        usCh = * ( uint16_t * ) pStr;   
                        
                        usCh = ( usCh << 8 ) + ( usCh >> 8 );   

                        LCD_DispChar_CH ( usX, usY, usCh   );
                        
                        usX += WIDTH_CH_CHAR;
                        
                        pStr += 2;           //一个汉字两个字节 
                
        }
                
        }


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
        uint8_t Ascii;  //英文
        uint16_t usCh;  //中文
        uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];    

        while ( *ptr != '\0')
        {
                        /****处理换行*****/
                        if ( ( x - LCD_DispWindow_X_Star + Charwidth ) > LCD_X_LENGTH )
                        {
                                x = LCD_DispWindow_X_Star;
                                y += Font_Height;
                        }
                        
                        if ( ( y - LCD_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
                        {
                                x = LCD_DispWindow_X_Star;
                                y = LCD_DispWindow_Y_Star;
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
 * @param  in_ptr ：字库输入指针	注意：1pixel 1bit
 * @param  out_ptr ：缩放后的字符输出指针 注意: 1pixel 8bit
 *		out_ptr实际上没有正常输出，改成了直接输出到全局指针zoomBuff中
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
        if(in_width >= 32) return;      //字库不允许超过32像素
        if(in_width * in_heig == 0) return;     
        if(in_width * in_heig >= 1024 ) return; //限制输入最大 32*32

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
        pts = (uint8_t*)&zoomTempBuff;  //映射后的源数据指针
        ots = (uint8_t*)&zoomBuff;      //输出数据的指针
        for (y=0;y<out_heig;y++)        /*行遍历*/
        {
                                unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);    
        for (x=0;x<out_width;x++) /*行内像素遍历*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //把源字模数据复制到目标指针中
            srcx_16+=xrIntFloat_16;     //按比例偏移源像素点
        }
        srcy_16+=yrIntFloat_16;         //按比例偏移源像素点
        ots+=out_width; 
        }
        /*！！！缩放后的字模数据直接存储到全局指针zoomBuff里了*/
        out_ptr = (uint8_t*)&zoomBuff;  //out_ptr没有正确传出，后面调用直接改成了全局变量指针！

        /*实际中如果使用out_ptr不需要下面这一句！！！
                只是因为out_ptr没有使用，会导致warning。强迫症*/
        out_ptr++; 
}




