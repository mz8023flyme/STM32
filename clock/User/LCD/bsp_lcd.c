#include "bsp_lcd.h"
#include "bsp_usart.h"
#include "fonts.h"
#include <stdio.h>  
#include <string.h>
#include "ascii.h" 
#include <math.h>

extern uint8_t const *WEEK_STR[] ;
extern uint8_t const *zodiac_sign[] ;


//Һ����ɨ��ģʽ
//������ѡֵΪ0-7
uint8_t LCD_SCAN_MODE=6;
//����Һ��ɨ�跽����仯��XY���ؿ���
//����ILI9341_GramScan�������÷���ʱ���Զ�����
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

#define ZOOMMAXBUFF 16384
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};    //�������ŵĻ��棬���֧�ֵ�128*128
uint8_t zoomTempBuff[1024] = {0};

static sFONT *LCD_Currentfonts = &Font8x16;  //Ӣ������
static uint16_t CurrentTextColor   = BLACK;//ǰ��ɫ
static uint16_t CurrentBackColor   = WHITE;//����ɫ        
        
void LCD_GPIO_Config(void)
{

        GPIO_InitTypeDef  GPIO_InitStruct;      //�������ӵ���GPIO

        // ��GPIO��ʱ��
        RCC_APB2PeriphClockCmd(LCD_CS_CLK|LCD_RD_CLK|LCD_WE_CLK|LCD_RS_CLK|LCD_RESTE_CLK|LCD_BL_CLK|
                               ILI9341_D0_CLK|ILI9341_D1_CLK|ILI9341_D2_CLK|ILI9341_D3_CLK|ILI9341_D4_CLK|
                                ILI9341_D5_CLK|ILI9341_D6_CLK|ILI9341_D7_CLK|ILI9341_D8_CLK|ILI9341_D9_CLK|
                                ILI9341_D10_CLK|ILI9341_D11_CLK|ILI9341_D12_CLK|ILI9341_D13_CLK|ILI9341_D14_CLK|ILI9341_D15_CLK, ENABLE);

        /*RESTE��BL�����������ͨ���������*/

        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Pin = LCD_RESTE_GPIO_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LCD_RESTE_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = LCD_BL_GPIO_PIN;
        GPIO_Init(LCD_BL_GPIO_PORT, &GPIO_InitStruct);

        /*���ø����������*/

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

        /*������Ӧ��������*/

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

        //��ʱ��

        //��ַ����ʱ�䣨ADDSET��
        readTimStruct.FSMC_AddressSetupTime = 0x01;     //����ֵ
        //���ݱ���ʱ�䣨DATAST��
        readTimStruct.FSMC_DataSetupTime=0x04;  //����ֵΪ1 

        //ѡ��ƥ��SRAM��ģʽ
        readTimStruct.FSMC_AccessMode = FSMC_AccessMode_B ;
        //��ַ����ʱ�䣨ADDHLD��ģʽBδ�õ�
        readTimStruct.FSMC_AddressHoldTime = 0x00 ; //SRAMû���õ�	
        //��������ת�����ڣ������ڸ���ģʽ��NOR����
        readTimStruct.FSMC_BusTurnAroundDuration=0x00;	//SRAMû���õ�
        //����ʱ�ӷ�Ƶ��������ͬ�����͵Ĵ洢��
        readTimStruct.FSMC_CLKDivision=0x00;	//SRAMû���õ�
        //���ݱ���ʱ�䣬������NOR
        readTimStruct.FSMC_DataLatency=0x00;	//SRAMû���õ�
         

        //дʱ��
        writeTimStruct.FSMC_AccessMode = FSMC_AccessMode_B ;
        writeTimStruct.FSMC_AddressHoldTime = 0 ; //SRAMû���õ�
        writeTimStruct.FSMC_AddressSetupTime = 0; 	//����ֵ
        writeTimStruct.FSMC_BusTurnAroundDuration=0;	//SRAMû���õ�
        writeTimStruct.FSMC_CLKDivision=0;	//SRAMû���õ�
        writeTimStruct.FSMC_DataLatency=0;	//SRAMû���õ�
        writeTimStruct.FSMC_DataSetupTime=2;	//����ֵΪ2  

        SRAMInitStructure.FSMC_Bank = FSMC_Bank_NORSRAMx ;
        SRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable ;
        SRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b ;
        SRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable ;
        //SRAMû���õ�
        SRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
        SRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable ;
        SRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable ;	
        SRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;	
        SRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable  ;
        SRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState ;
        SRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low ;
        SRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable  ;
        SRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable ;
        //��дʱ��ṹ��
        SRAMInitStructure.FSMC_WriteTimingStruct = &writeTimStruct;
        SRAMInitStructure.FSMC_ReadWriteTimingStruct = &readTimStruct; //FSMC_ExtendedMode ����enable��Ч

        FSMC_NORSRAMInit(&SRAMInitStructure);
        FSMC_NORSRAMCmd(FSMC_Bank_NORSRAMx,ENABLE);

}

void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
        
}

//��λ����
void ILI9341_Rst ( void )
{       
        GPIO_ResetBits ( LCD_RESTE_GPIO_PORT, LCD_RESTE_GPIO_PIN );     //�͵�ƽ��λ

        Delay ( 0xAFF );        
        
        GPIO_SetBits ( LCD_RESTE_GPIO_PORT, LCD_RESTE_GPIO_PIN );       

        Delay ( 0xAFF );        
        
}
//����ƺ���
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
        LCD_Write_Data ( 0xC8 );    /*����  ���Ͻǵ� (���)�����½� (�յ�)ɨ�跽ʽ*/
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
 * @brief  ����ILI9341��GRAM��ɨ�跽�� 
 * @param  ucOption ��ѡ��GRAM��ɨ�跽�� 
 *     @arg 0-7 :������ѡֵΪ0-7��˸�����
 *
 *	����������0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 *				���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 *		
 *	����0��2��4��6 ģʽ��X��������Ϊ240��Y��������Ϊ320
 *	����1��3��5��7 ģʽ��X��������Ϊ320��Y��������Ϊ240
 *
 *	���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
 *	���� 3 ģʽΪ����ͷ����ʹ�õķ���
 *	���� 0 ģʽΪBMPͼƬ��ʾ����ʹ�õķ���
 *
 * @retval ��
 * @note  ����ͼ����A��ʾ���ϣ�V��ʾ���£�<��ʾ����>��ʾ����
					X��ʾX�ᣬY��ʾY��

------------------------------------------------------------
ģʽ0��				.		ģʽ1��		.	ģʽ2��			.	ģʽ3��					
		A		.		A		.		A		.		A									
		|		.		|		.		|		.		|							
		Y		.		X		.		Y		.		X					
		0		.		1		.		2		.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
ģʽ4��				.	ģʽ5��			.	ģʽ6��			.	ģʽ7��					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
		4		.		5		.		6		.		7	
		Y		.		X		.		Y		.		X						
		|		.		|		.		|		.		|							
		V		.		V		.		V		.		V		
---------------------------------------------------------				
											 LCD��ʾ��
								|-----------------|
								|     ����Logo    |
								|		  |
								|		  |
								|	          |
								|	          |
								|	          |
								|		  |
								|		  |
								|		  |
								|-----------------|
								��Ļ���棨��240����320��

 *******************************************************/
void LCD_GramScan ( uint8_t ucOption )
{       
        //������飬ֻ������0-7
        if(ucOption >7 )
                return;

        //����ģʽ����LCD_SCAN_MODE��ֵ����Ҫ���ڴ�����ѡ��������
        LCD_SCAN_MODE = ucOption;

        //����ģʽ����XY��������ؿ���
        if(ucOption%2 == 0)     
        {
                //0 2 4 6ģʽ��X�������ؿ���Ϊ240��Y����Ϊ320
                LCD_X_LENGTH = ILI9341_LESS_PIXEL;
                LCD_Y_LENGTH = ILI9341_MORE_PIXEL;
        }
        else    
        {
                //1 3 5 7ģʽ��X�������ؿ���Ϊ320��Y����Ϊ240
                LCD_X_LENGTH = ILI9341_MORE_PIXEL;
                LCD_Y_LENGTH = ILI9341_LESS_PIXEL; 
        }

        //0x36��������ĸ�3λ����������GRAMɨ�跽��     
        LCD_Write_Cmd ( 0x36 ); 
        LCD_Write_Data ( 0x08 |(ucOption<<5));//����ucOption��ֵ����LCD��������0-7��ģʽ
        LCD_Write_Cmd ( CMD_SetCoordinateX ); 
        LCD_Write_Data ( 0x00 );        /* x ��ʼ�����8λ */
        LCD_Write_Data ( 0x00 );        /* x ��ʼ�����8λ */
        LCD_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x ���������8λ */  
        LCD_Write_Data ( (LCD_X_LENGTH-1)&0xFF );       /* x ���������8λ */

        LCD_Write_Cmd ( CMD_SetCoordinateY ); 
        LCD_Write_Data ( 0x00 );        /* y ��ʼ�����8λ */
        LCD_Write_Data ( 0x00 );        /* y ��ʼ�����8λ */
        LCD_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );  /* y ���������8λ */ 
        LCD_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );       /* y ���������8λ */

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
  * @brief  ����Ӣ����������
  * @param  fonts: ָ��Ҫѡ�������
  * ����Ϊ����ֵ֮һ
  * @arg��Font24x32;
  * @arg��Font16x24;
  * @arg��Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  ��ȡ��ǰ��������
  * @param  None.
  * @retval ���ص�ǰ��������
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}
/**
 * @brief  �趨ILI9341�Ĺ������
 * @param  usX �����ض�ɨ�跽���¹���X����
 * @param  usY �����ض�ɨ�跽���¹���Y����
 * @retval ��
 */
static void LCD_SetCursor ( uint16_t usX, uint16_t usY )        
{
        LCD_OpenWindow ( usX, usY, 1, 1 );
}


/**
 * @brief  ��ILI9341��ʾ������ĳһ��ɫ������ص�
 * @param  ulAmout_Point ��Ҫ�����ɫ�����ص������Ŀ
 * @param  usColor ����ɫ
 * @retval ��
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
  * @brief  ����LCD��ǰ��(����)��������ɫ,RGB565
  * @param  TextColor: ָ��ǰ��(����)��ɫ
  * @param  BackColor: ָ��������ɫ
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
 * @brief  ��ILI9341��ʾ����ĳһ������ĳ����ɫ��������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ���
 * @param  usHeight �����ڵĸ߶�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void LCD_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{
        LCD_OpenWindow ( usX, usY, usWidth, usHeight );

        LCD_FillColor ( usWidth * usHeight, CurrentBackColor );		

}

/**
 * @brief  �� ILI9341 ��ʾ������ʾһ��Ӣ���ַ�
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽���¸õ����ʼY����
 * @param  cChar ��Ҫ��ʾ��Ӣ���ַ�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void LCD_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
        uint8_t  byteCount, bitCount,fontLength;        
        uint16_t ucRelativePositon;
        uint8_t *Pfont;

        //��ascii���ƫ�ƣ���ģ��������ASCII����ǰ32����ͼ�η��ţ�
        ucRelativePositon = cChar - ' ';

        //ÿ����ģ���ֽ���
        fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
                
        //��ģ�׵�ַ
        /*ascii���ƫ��ֵ����ÿ����ģ���ֽ����������ģ��ƫ��λ��*/
        Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];

        //������ʾ����
        LCD_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);

        LCD_Write_Cmd ( CMD_SetPixel ); 

        //���ֽڶ�ȡ��ģ����
        //����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
        for ( byteCount = 0; byteCount < fontLength; byteCount++ )
        {
                        //һλһλ����Ҫ��ʾ����ɫ
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
 * @brief  �� ILI9341 ��ʾ������ʾӢ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
 *       ��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 * @brief  �� ILI9341 ��ʾ������ʾһ�������ַ�
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  usChar ��Ҫ��ʾ�������ַ��������룩
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */ 
void LCD_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar )
{
        uint8_t rowCount, bitCount;
        uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];    
        uint16_t usTemp;        

        //������ʾ����
        LCD_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR );

        LCD_Write_Cmd ( CMD_SetPixel );

        //ȡ��ģ����  
        GetGBKCode ( ucBuffer, usChar );

        for ( rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++ )
        {
        /* ȡ�������ֽڵ����ݣ���lcd�ϼ���һ�����ֵ�һ�� */
                usTemp = ucBuffer [ rowCount * 2 ];
                usTemp = ( usTemp << 8 );
                usTemp |= ucBuffer [ rowCount * 2 + 1 ];
                
                for ( bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount ++ )
                {       
                        if ( usTemp & ( 0x8000 >> bitCount ) )  //��λ��ǰ 
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
                
                pStr += 2;           //һ�����������ֽ� 

        }       

}


/**
 * @brief  �� ILI9341 ��ʾ������ʾ��Ӣ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
  *       ��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void LCD_DispStringLine_EN_CH (  uint16_t line, char * pStr )
{
        uint16_t usCh;
        uint16_t usX = 0;

        while( * pStr != '\0' )
        {
                if ( * pStr <= 126 )    //Ӣ���ַ�
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
                
                else    //�����ַ�
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
                        
                        pStr += 2;           //һ�����������ֽ� 
                
                }
                
        }     
} 

/**
 * @brief  �� ILI9341 ��ʾ������ʾ��Ӣ���ַ���
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void LCD_DispString_EN_CH ( uint16_t usX , uint16_t usY, char * pStr )
{
        uint16_t usCh;

        while( * pStr != '\0' )
        {
                if ( * pStr <= 126 )            //Ӣ���ַ�
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
                
                else    //�����ַ�
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
                        
                        pStr += 2;           //һ�����������ֽ� 
                
                }
                
        }       
} 

/**
 * @brief  ��ILI9341��ʾ����ĳһ����ĳ����ɫ�������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 * @brief  ��ȡILI9341 GRAN ��һ����������
 * @param  ��
 * @retval ��������
 */
static uint16_t LCD_Read_PixelData ( void )	
{       
        uint16_t usR=0, usG=0, usB=0 ;


        LCD_Write_Cmd ( 0x2E );   /* ������ */

        usR = LCD_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/

        usR = LCD_Read_Data ();  	/*READ OUT RED DATA  */
        usB = LCD_Read_Data ();  	/*READ OUT BLUE DATA*/
        usG = LCD_Read_Data ();  	/*READ OUT GREEN DATA*/	

        return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );

}
/**
 * @brief  ��ȡ ILI9341 ��ʾ����ĳһ����������������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @retval ��������
 */
uint16_t LCD_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
        uint16_t usPixelData;
        LCD_SetCursor ( usX, usY );

        usPixelData = LCD_Read_PixelData ();

        return usPixelData;

}





/**
  * @brief  ���ĳ������
  * @param  Line: ָ��Ҫɾ������
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ��Ҫɾ�����У�
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ����ɾ����ǰ����߶ȵĵ�x�С�
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  LCD_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);      /* ��������ʾȫ�� */

}


/**
 * @brief  �������ź����ģ��ʾ�ַ���
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ����ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
 * @param  Font_Heig���ַ��߶ȣ�ע��Ϊż��
 * @param  c ��Ҫ��ʾ���ַ���
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void LCD_DisplayStringEx(uint16_t x,  uint16_t y,uint16_t Font_width,uint16_t Font_Height,      uint8_t *ptr,uint16_t DrawModel)  

{
        uint16_t Charwidth = Font_width; //Ĭ��ΪFont_width��Ӣ�Ŀ���Ϊ���Ŀ��ȵ�һ��
        uint8_t *psr;
        uint8_t Ascii;	//Ӣ��
        uint16_t usCh;  //����
        uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	

        while ( *ptr != '\0')
        {
                        /****��������*****/
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
                        
                if(*ptr > 0x80) //���������
                {			
                        Charwidth = Font_width;
                        usCh = * ( uint16_t * ) ptr;				
                        usCh = ( usCh << 8 ) + ( usCh >> 8 );
                        GetGBKCode ( ucBuffer, usCh );	//ȡ��ģ����
                        //������ģ���ݣ�Դ��ģΪ16*16
                        LCD_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
                        //��ʾ�����ַ�
                        LCD_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
                        x+=Charwidth;
                        ptr+=2;
                }
                else
                {
                                Charwidth = Font_width / 2;
                                Ascii = *ptr - 32;
                                //ʹ��16*24����������ģ����
                                LCD_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
                          //��ʾ�����ַ�
                                LCD_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
                                x+=Charwidth;
                                ptr++;
                }
        }
}
/**
 * @brief  �������ź����ģ��ʾ�ַ�
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ�����
 * @param  Font_Heig���ַ��߶�
 * @param  c ��Ҫ��ʾ����ģ����
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void LCD_DrawChar_Ex(uint16_t usX, uint16_t usY, uint16_t Font_width, uint16_t Font_Height,uint8_t *c,uint16_t DrawModel)
{
  uint32_t index = 0, counter = 0;

        //������ʾ����
        LCD_OpenWindow ( usX, usY, Font_width, Font_Height);

        LCD_Write_Cmd ( CMD_SetPixel ); 

        //���ֽڶ�ȡ��ģ����
        //����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
        for ( index = 0; index < Font_Height; index++ )
        {
                        //һλһλ����Ҫ��ʾ����ɫ
                        for ( counter = 0; counter < Font_width; counter++ )
                        {
                                        //���ź����ģ���ݣ���һ���ֽڱ�ʾһ������λ
                                        //�����ֽ�ֵΪ1��ʾ������Ϊ�ʼ�
                                        //�����ֽ�ֵΪ0��ʾ������Ϊ����
                                        if ( *c++ == DrawModel )
                                                LCD_Write_Data ( CurrentBackColor );    
                                        else
                                                LCD_Write_Data ( CurrentTextColor );
                        }       
        }       
}
/**
 * @brief  ������ģ�����ź����ģ��1�����ص���8������λ����ʾ
                0x01��ʾ�ʼ���0x00��ʾ�հ���
 * @param  in_width ��ԭʼ�ַ�����
 * @param  in_heig ��ԭʼ�ַ��߶�
 * @param  out_width �����ź���ַ�����
 * @param  out_heig�����ź���ַ��߶�
 * @param  in_ptr ���ֿ�����ָ��       ע�⣺1pixel 1bit
 * @param  out_ptr �����ź���ַ����ָ�� ע��: 1pixel 8bit
 *         out_ptrʵ����û������������ĳ���ֱ�������ȫ��ָ��zoomBuff��
 * @param  en_cn ��0ΪӢ�ģ�1Ϊ����
 * @retval ��
 */
void LCD_zoomChar(uint16_t in_width,uint16_t in_heig,uint16_t out_width,	
			uint16_t out_heig,uint8_t *in_ptr,uint8_t *out_ptr, uint8_t en_cn)	
{
        uint8_t *pts,*ots;
        //����Դ��ģ��Ŀ����ģ��С���趨����������ӣ�����16��Ϊ�˰Ѹ�������ת�ɶ�������
        unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
        unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;

        unsigned int srcy_16=0;
        unsigned int y,x;
        uint8_t *pSrcLine;

        uint16_t byteCount,bitCount;

        //�������Ƿ�Ϸ�
        if(in_width >= 32) return;												//�ֿⲻ��������32����
        if(in_width * in_heig == 0) return;	
        if(in_width * in_heig >= 1024 ) return; 					//����������� 32*32

        if(out_width * out_heig == 0) return;	
        if(out_width * out_heig >= ZOOMMAXBUFF ) return; //����������� 128*128
        pts = (uint8_t*)&zoomTempBuff;

        //Ϊ�������㣬�ֿ��������1 pixel/1bit ӳ�䵽1pixel/8bit
        //0x01��ʾ�ʼ���0x00��ʾ�հ���
        if(en_cn == 0x00)//Ӣ��
        {
                //Ӣ�ĺ������ֿ����±߽粻�ԣ����ڴ˴���������Ҫע��tempBuff��ֹ���
                        for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
                        {
                                for(bitCount=0;bitCount<8;bitCount++)
                                        {						
                                                //��Դ��ģ������λӳ�䵽�ֽ�
                                                //in_ptr��bitXΪ1����pts�������ֽ�ֵΪ1
                                                //in_ptr��bitXΪ0����pts�������ֽ�ֵΪ0
                                                *pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
                                        }
                        }				
        }
        else //����
        {			
                        for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
                        {
                                for(bitCount=0;bitCount<8;bitCount++)
                                        {						
                                                //��Դ��ģ������λӳ�䵽�ֽ�
                                                //in_ptr��bitXΪ1����pts�������ֽ�ֵΪ1
                                                //in_ptr��bitXΪ0����pts�������ֽ�ֵΪ0
                                                *pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
                                        }
                        }		
        }

        //zoom����
        pts = (uint8_t*)&zoomTempBuff;	//ӳ����Դ����ָ��
        ots = (uint8_t*)&zoomBuff;	//������ݵ�ָ��
        for (y=0;y<out_heig;y++)	/*�б���*/
        {
                                unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);				
        for (x=0;x<out_width;x++) /*�������ر���*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //��Դ��ģ���ݸ��Ƶ�Ŀ��ָ����
            srcx_16+=xrIntFloat_16;			//������ƫ��Դ���ص�
        }
        srcy_16+=yrIntFloat_16;				  //������ƫ��Դ���ص�
        ots+=out_width;						
        }
        /*���������ź����ģ����ֱ�Ӵ洢��ȫ��ָ��zoomBuff����*/
        out_ptr = (uint8_t*)&zoomBuff;	//out_ptrû����ȷ�������������ֱ�Ӹĳ���ȫ�ֱ���ָ�룡

        /*ʵ�������ʹ��out_ptr����Ҫ������һ�䣡����
                ֻ����Ϊout_ptrû��ʹ�ã��ᵼ��warning��ǿ��֢*/
        out_ptr++; 
}

/**
 * @brief  �� ILI9341 ��ʾ����ʹ�� Bresenham �㷨���߶� 
 * @param  usX1 �����ض�ɨ�跽�����߶ε�һ���˵�X����
 * @param  usY1 �����ض�ɨ�跽�����߶ε�һ���˵�Y����
 * @param  usX2 �����ض�ɨ�跽�����߶ε���һ���˵�X����
 * @param  usY2 �����ض�ɨ�跽�����߶ε���һ���˵�Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void LCD_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 )
{
        uint16_t us; 
        uint16_t usX_Current, usY_Current;

        int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
        int32_t lIncrease_X, lIncrease_Y; 	


        lDelta_X = usX2 - usX1; //������������ 
        lDelta_Y = usY2 - usY1; 

        usX_Current = usX1; 
        usY_Current = usY1; 


        if ( lDelta_X > 0 ) 
                lIncrease_X = 1; //���õ������� 

        else if ( lDelta_X == 0 ) 
                lIncrease_X = 0;//��ֱ�� 

        else 
        { 
        lIncrease_X = -1;
        lDelta_X = - lDelta_X;
        } 


        if ( lDelta_Y > 0 )
                lIncrease_Y = 1; 

        else if ( lDelta_Y == 0 )
                lIncrease_Y = 0;//ˮƽ�� 

        else 
        {
        lIncrease_Y = -1;
        lDelta_Y = - lDelta_Y;
        } 


        if (  lDelta_X > lDelta_Y )
                lDistance = lDelta_X; //ѡȡ�������������� 

        else 
                lDistance = lDelta_Y; 


        for ( us = 0; us <= lDistance + 1; us ++ )//������� 
        {  
                LCD_SetPointPixel ( usX_Current, usY_Current );//���� 
                
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
 * @brief  �� ILI9341 ��ʾ����ʹ�� Bresenham �㷨���߶� 
 * @param  usX1 �����ض�ɨ�跽�����߶ε�һ���˵�X����
 * @param  usY1 �����ض�ɨ�跽�����߶ε�һ���˵�Y����
 * @param  usL �����ض�ɨ�跽�����߶εĳ���
 * @param  usW �����ض�ɨ�跽�����߶εĽǶ�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void LCD_DrawLine_Polar(uint16_t X1, uint16_t Y1, uint16_t L, uint16_t angle )
{
        u16 X2,Y2;  //��һ���������
        float radian;  //����
        radian = angle*Pi/180;
        X2=(u16)(X1+L*cos(radian));
        Y2=(u16)(Y1+L*sin(radian));
        LCD_DrawLine(X1,Y1,X2,Y2);
}



/**
  * @brief  ����LCD��ǰ��(����)��ɫ,RGB565
  * @param  Color: ָ��ǰ��(����)��ɫ 
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
 * @brief  �� ILI9341 ��ʾ���ϻ�һ������
 * @param  usX_Start �����ض�ɨ�跽���¾��ε���ʼ��X����
 * @param  usY_Start �����ض�ɨ�跽���¾��ε���ʼ��Y����
 * @param  usWidth�����εĿ��ȣ���λ�����أ�
 * @param  usHeight�����εĸ߶ȣ���λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ����þ���
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :���ľ���
  *     @arg 1 :ʵ�ľ��� 
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
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
 * @brief  ����Ļ����ʾʱ��
 *
 */
void LCD_Time_Show(struct rtc_time *tm)
{
        char dataStr [ 1500 ] = { 0 };
        char TimeStr [ 1500 ] = { 0 };
        u16  anagle,temp;
        int sec_anagle,hour_anagle,min_anagle;
        sprintf(dataStr,"%d�� (%s��) %d��%d�� ����%s",tm->tm_year,zodiac_sign[(tm->tm_year-3)%12],
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
        
        //�߳���һ�εĺۼ�
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
        //���µ�ָ��
        LCD_SetTextColor(RED);
        LCD_DrawLine_Polar(Xlabel, Ylabel, Sec_length, sec_anagle);
        
        //���벿��
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
                //�߳���һ�εĺۼ�
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
                //���µ�ָ��
                LCD_SetTextColor(BLUE);
                LCD_DrawLine_Polar(Xlabel, Ylabel, Min_length, min_anagle);
        
                //ʱ�벿��
        
                
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
                //�߳���һ�εĺۼ�
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
                //���µ�ָ��
                LCD_SetTextColor(BLACK);
                LCD_DrawLine_Polar(Xlabel, Ylabel, Hour_length, hour_anagle);
        
}