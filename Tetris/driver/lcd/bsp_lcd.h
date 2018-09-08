#ifndef      __BSP_LCD_H
#define      __BSP_LCD_H


#include "stm32f10x.h"
#include "fonts.h"


/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X6000 0000 ~ 0X63FF FFFF
FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X60000000
RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/

/******************************* ILI9341 ��ʾ���� FSMC �������� ***************************/
#define      LCD_FSMC_Addr_CMD         ( ( uint32_t ) 0x60000000 )	     //FSMC_Bank1_NORSRAM1����LCD��������ĵ�ַ
#define      LCD_FSMC_Addr_DATA        ( ( uint32_t ) 0x60020000 )      //FSMC_Bank1_NORSRAM1����LCD���ݲ����ĵ�ַ      
                
#define      FSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM1



/******************************* ILI9341 ��ʾ��8080ͨѶ���Ŷ��� ***************************/
#define      LCD_CS_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      LCD_CS_CLK                RCC_APB2Periph_GPIOD    
#define      LCD_CS_PORT               GPIOD   
#define      LCD_CS_PIN                GPIO_Pin_7

#define      LCD_DC_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      LCD_DC_CLK                RCC_APB2Periph_GPIOD    
#define      LCD_DC_PORT               GPIOD   
#define      LCD_DC_PIN                GPIO_Pin_11

#define      LCD_RST_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      LCD_RST_CLK               RCC_APB2Periph_GPIOE   
#define      LCD_RST_PORT              GPIOE
#define      LCD_RST_PIN               GPIO_Pin_1

#define      LCD_BK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      LCD_BK_CLK                RCC_APB2Periph_GPIOD    
#define      LCD_BK_PORT               GPIOD
#define      LCD_BK_PIN                GPIO_Pin_12



/*************************************** ����Ԥ�� ******************************************/
#define      DEBUG_DELAY()                



/***************** ILI934 ��ʾ��ȫ��Ĭ�ϣ�ɨ�跽��Ϊ1ʱ������Ⱥ����߶� *********************/
#define      LCD_Default_Max_Width       240     //��ʼ���X����
#define      LCD_Default_Max_Heigth      320     //��ʼ���Y����




/***************************** ILI934 ��ʾ�������ʼ������������� ***************************/
#define      LCD_DispWindow_X_Star      0     //��ʼ���X����
#define      LCD_DispWindow_Y_Star      0     //��ʼ���Y����

#define      LCD_DispWindow_COLUMN                240     //������
#define      LCD_DispWindow_PAGE                  320     //������

#define      LCD_LESS_PIXEL          240    //Һ�����϶̷�������ؿ��
#define      LCD_MORE_PIXEL         320     //Һ�����ϳ���������ؿ��

/***************************** �� ILI934 ��ʾ������ʾ���ַ��Ĵ�С ***************************/
#define      WIDTH_EN_CHAR           8       //Ӣ���ַ���� 
#define      HEIGHT_EN_CHAR          16      //Ӣ���ַ��߶� 


#define      WIDTH_CH_CHAR           16      //�����ַ���� 
#define      HEIGHT_CH_CHAR          16      //�����ַ��߶� 

//#define      GetGBKCode( ucBuffer, usChar )     //�����ȡ�����ַ���ģ����ĺ�������ucBufferΪ�����ģ��������usCharΪ�����ַ��������룩

#define RGB888_2_RGB565(R,G,B)  (uint16_t)(((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B & 0x1F) )

/******************************* ���� ILI934 ��ʾ��������ɫ ********************************/
#define     BACKGROUND       BLACK   //Ĭ�ϱ�����ɫ
#define     FOREGROUND       WHITE   //Ĭ��ǰ����ɫ

#define       WHITE                         0xFFFF            //��ɫ
#define       BLACK                         0x0000            //��ɫ 
#define       GREY                          0xF7DE            //��ɫ 
#define       BLUE                          0x001F            //��ɫ 
#define       BLUE2                         0x051F            //ǳ��ɫ 
#define       RED                           0xF800            //��ɫ 
#define       MAGENTA                       0xF81F            //����ɫ�����ɫ 
#define       GREEN                         0x07E0            //��ɫ 
#define       CYAN                          0x7FFF            //����ɫ����ɫ 
#define       YELLOW                        0xFFE0            //��ɫ 
#define       BRED                          0xF81F
#define       GRED                          0xFFE0
#define       GBLUE                         0x07FF



/******************************* ���� ILI934 �������� ********************************/
#define       CMD_SetCoordinateX                0x2A            //����X����
#define       CMD_SetCoordinateY                0x2B            //����Y����
#define       CMD_SetPixel                      0x2C            //�������



/********************************** ���� ILI934 ���� ***************************************/
void               LCD_Init             ( void );
void               LCD_Rst              ( void );
void               LCD_BackLed_Control  ( FunctionalState enumState );
__inline void      LCD_Write_Cmd        ( uint16_t usCmd );
__inline void      LCD_Write_Data       ( uint16_t usData );
__inline uint16_t  LCD_Read_Data        ( void );
void               LCD_GramScan         ( uint8_t ucOtion );
void               LCD_OpenWindow       ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void               LCD_Clear            ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight, uint16_t usColor );
void               LCD_SetPointPixel    ( uint16_t usX, uint16_t usY, uint16_t usColor );
uint16_t           LCD_GetPointPixel    ( uint16_t usX , uint16_t usY );
void               LCD_DrawLine         ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 );
void               LCD_DrawRectangle    ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth,uint16_t usHeight, uint8_t ucFilled );
void               LCD_DrawCircle       ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint16_t usColor, uint8_t ucFilled );
void               LCD_DispChar_EN      ( uint16_t usX, uint16_t usY, const char cChar );
void               LCD_DispString_EN    ( uint16_t usX, uint16_t usY, const char * pStr );
void               LCD_DispChar_CH      ( uint16_t usX, uint16_t usY, uint16_t usChar  );
void               LCD_DispString_CH    ( uint16_t usX, uint16_t usY, const uint8_t * pStr   );
void               LCD_DispString_EN_CH ( uint16_t usX, uint16_t usY, const uint8_t * pStr );
void               LCD_SetTextColor     (uint16_t Color);
void               LCD_SetFont          (sFONT *fonts);
void               LCD_SetColors        (uint16_t TextColor, uint16_t BackColor) ;
void               LCD_DispStringLine_EN(  uint16_t line,  char * pStr );
void               LCD_DisplayStringEx  ( uint16_t x,  uint16_t y,uint16_t Font_width,uint16_t Font_Height,      uint8_t *ptr,uint16_t DrawModel);
void               LCD_DrawChar_Ex      (uint16_t usX, uint16_t usY, uint16_t Font_width, uint16_t Font_Height,uint8_t *c,uint16_t DrawModel);
void               LCD_zoomChar         (uint16_t in_width,uint16_t in_heig,uint16_t out_width,uint16_t out_heig,uint8_t *in_ptr,uint8_t *out_ptr, uint8_t en_cn);

#endif /* __BSP_ILI9341_ILI9341_H */

