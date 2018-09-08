#ifndef      __BSP_LCD_H
#define      __BSP_LCD_H


#include "stm32f10x.h"
#include "fonts.h"


/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM1 连接 TFT，地址范围为0X6000 0000 ~ 0X63FF FFFF
FSMC_A16 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X60000000
RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/

/******************************* ILI9341 显示屏的 FSMC 参数定义 ***************************/
#define      LCD_FSMC_Addr_CMD         ( ( uint32_t ) 0x60000000 )	     //FSMC_Bank1_NORSRAM1用于LCD命令操作的地址
#define      LCD_FSMC_Addr_DATA        ( ( uint32_t ) 0x60020000 )      //FSMC_Bank1_NORSRAM1用于LCD数据操作的地址      
                
#define      FSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM1



/******************************* ILI9341 显示屏8080通讯引脚定义 ***************************/
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



/*************************************** 调试预用 ******************************************/
#define      DEBUG_DELAY()                



/***************** ILI934 显示屏全屏默认（扫描方向为1时）最大宽度和最大高度 *********************/
#define      LCD_Default_Max_Width       240     //起始点的X坐标
#define      LCD_Default_Max_Heigth      320     //起始点的Y坐标




/***************************** ILI934 显示区域的起始坐标和总行列数 ***************************/
#define      LCD_DispWindow_X_Star      0     //起始点的X坐标
#define      LCD_DispWindow_Y_Star      0     //起始点的Y坐标

#define      LCD_DispWindow_COLUMN                240     //总列数
#define      LCD_DispWindow_PAGE                  320     //总行数

#define      LCD_LESS_PIXEL          240    //液晶屏较短方向的像素宽度
#define      LCD_MORE_PIXEL         320     //液晶屏较长方向的像素宽度

/***************************** 在 ILI934 显示屏上显示的字符的大小 ***************************/
#define      WIDTH_EN_CHAR           8       //英文字符宽度 
#define      HEIGHT_EN_CHAR          16      //英文字符高度 


#define      WIDTH_CH_CHAR           16      //中文字符宽度 
#define      HEIGHT_CH_CHAR          16      //中文字符高度 

//#define      GetGBKCode( ucBuffer, usChar )     //定义获取中文字符字模数组的函数名，ucBuffer为存放字模数组名，usChar为中文字符（国标码）

#define RGB888_2_RGB565(R,G,B)  (uint16_t)(((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B & 0x1F) )

/******************************* 定义 ILI934 显示屏常用颜色 ********************************/
#define     BACKGROUND       BLACK   //默认背景颜色
#define     FOREGROUND       WHITE   //默认前景颜色

#define       WHITE                         0xFFFF            //白色
#define       BLACK                         0x0000            //黑色 
#define       GREY                          0xF7DE            //灰色 
#define       BLUE                          0x001F            //蓝色 
#define       BLUE2                         0x051F            //浅蓝色 
#define       RED                           0xF800            //红色 
#define       MAGENTA                       0xF81F            //红紫色，洋红色 
#define       GREEN                         0x07E0            //绿色 
#define       CYAN                          0x7FFF            //蓝绿色，青色 
#define       YELLOW                        0xFFE0            //黄色 
#define       BRED                          0xF81F
#define       GRED                          0xFFE0
#define       GBLUE                         0x07FF



/******************************* 定义 ILI934 常用命令 ********************************/
#define       CMD_SetCoordinateX                0x2A            //设置X坐标
#define       CMD_SetCoordinateY                0x2B            //设置Y坐标
#define       CMD_SetPixel                      0x2C            //填充像素



/********************************** 声明 ILI934 函数 ***************************************/
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

