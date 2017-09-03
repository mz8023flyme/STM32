<<<<<<< HEAD
#ifndef __BSP_TOUCH_H
#define __BSP_TOUCH_H

#include "stm32f10x.h"


/******************************* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断) ***************************/
#define             Touch_PENIRQ_GPIO_CLK                        RCC_APB2Periph_GPIOE   
#define             Touch_PENIRQ_GPIO_PORT                       GPIOE
#define             Touch_PENIRQ_GPIO_PIN                        GPIO_Pin_4

//触屏信号有效电平
#define             Touch_PENIRQ_ActiveLevel                     0
#define             Touch_PENIRQ_Read()                          GPIO_ReadInputDataBit ( Touch_PENIRQ_GPIO_PORT, Touch_PENIRQ_GPIO_PIN )



/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define             Touch_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOD

#define             Touch_SPI_CS_PIN		                        GPIO_Pin_13
#define             Touch_SPI_CS_PORT		                      GPIOD

#define	            Touch_SPI_CLK_PIN	                        GPIO_Pin_0
#define             Touch_SPI_CLK_PORT	                        GPIOE

#define	            Touch_SPI_MOSI_PIN	                        GPIO_Pin_2
#define	            Touch_SPI_MOSI_PORT	                      GPIOE

#define	            Touch_SPI_MISO_PIN	                        GPIO_Pin_3
#define	            Touch_SPI_MISO_PORT	                      GPIOE


#define             Touch_CS_DISABLE()                 	 GPIO_SetBits ( Touch_SPI_CS_PORT, Touch_SPI_CS_PIN )    
#define             Touch_CS_ENABLE()                          GPIO_ResetBits ( Touch_SPI_CS_PORT, Touch_SPI_CS_PIN )  

#define             Touch_CLK_HIGH()                           GPIO_SetBits ( Touch_SPI_CLK_PORT, Touch_SPI_CLK_PIN )    
#define             Touch_CLK_LOW()                            GPIO_ResetBits ( Touch_SPI_CLK_PORT, Touch_SPI_CLK_PIN ) 

#define             Touch_MOSI_1()                             GPIO_SetBits ( Touch_SPI_MOSI_PORT, Touch_SPI_MOSI_PIN ) 
#define             Touch_MOSI_0()                             GPIO_ResetBits ( Touch_SPI_MOSI_PORT, Touch_SPI_MOSI_PIN )

#define             Touch_MISO()                               GPIO_ReadInputDataBit ( Touch_SPI_MISO_PORT, Touch_SPI_MISO_PIN )

#define	            LCD_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            LCD_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字


//检测 ：触摸屏的按下和释放

#define TOUCH_NOT_PRESSED  0
#define TOUCH_PRESSED      1

//触摸参数写到FLASH里的标志
#define		FLASH_TOUCH_PARA_FLAG_VALUE			 0xA5

//触摸标志写到FLASH里的地址
#define 	FLASH_TOUCH_PARA_FLAG_ADDR			(1*1024)

//触摸参数写到FLASH里的地址
#define 	FLASH_TOUCH_PARA_ADDR				(2*1024)

#define LCD_DEBUG_ON         0
#define LCD_INFO(fmt,arg...)           printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define LCD_ERROR(fmt,arg...)          printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)


//#define XPT2046_DEBUG(fmt,arg...)          do{\
//                                          if(XPT2046_DEBUG_ON)\
//                                          printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                          }while(0)

typedef enum
{
	TOUCH_STATE_RELEASE = 0,
	TOUCH_STATE_WAITING,
	TOUCH_STATE_PRESSED,
}Touch_State;


typedef struct         //校准因子结构体 
{
	 float 		An,  		 //注:sizeof(long double) = 8
			Bn,     
			Cn,   
			Dn,    
			En,    
			Fn,     
			Divider;
	
} strType_LCD_Calibration;



typedef struct         //校准系数结构体（最终使用）
{
	float 	dX_X,  			 
		dX_Y,     
		dX,   
		dY_X,    
		dY_Y,    
		dY;

} strType_LCD_TouchPara;

typedef	struct          //液晶坐标结构体 
{		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 
	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_LCD_Coordinate;  

/**************************函数声明********************/

uint16_t Touch_ReceiveData(void);
void Touch_SendCMD(uint8_t cmd)	;
void TOUCH_Init(void);
uint8_t touch_detect(void);
uint8_t LCD_Touch_Calibrate ( uint8_t LCD_Mode ) ;
void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal);
uint8_t LCD_Get_TouchedPoint ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_TouchPara * pTouchPara );
void LCD_TouchDown(strType_LCD_Coordinate * touch);
void LCD_TouchUp(strType_LCD_Coordinate * touch) ;
void LCD_TouchEvenHandler(void );
void Screen_Init(void);

#endif /* __BSP_TOUCH_H*/
=======
#ifndef __BSP_TOUCH_H
#define __BSP_TOUCH_H

#include "stm32f10x.h"


/******************************* XPT2046 触摸屏触摸信号指示引脚定义(不使用中断) ***************************/
#define             Touch_PENIRQ_GPIO_CLK                        RCC_APB2Periph_GPIOE   
#define             Touch_PENIRQ_GPIO_PORT                       GPIOE
#define             Touch_PENIRQ_GPIO_PIN                        GPIO_Pin_4

//触屏信号有效电平
#define             Touch_PENIRQ_ActiveLevel                     0
#define             Touch_PENIRQ_Read()                          GPIO_ReadInputDataBit ( Touch_PENIRQ_GPIO_PORT, Touch_PENIRQ_GPIO_PIN )



/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define             Touch_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOD

#define             Touch_SPI_CS_PIN		                        GPIO_Pin_13
#define             Touch_SPI_CS_PORT		                      GPIOD

#define	            Touch_SPI_CLK_PIN	                        GPIO_Pin_0
#define             Touch_SPI_CLK_PORT	                        GPIOE

#define	            Touch_SPI_MOSI_PIN	                        GPIO_Pin_2
#define	            Touch_SPI_MOSI_PORT	                      GPIOE

#define	            Touch_SPI_MISO_PIN	                        GPIO_Pin_3
#define	            Touch_SPI_MISO_PORT	                      GPIOE


#define             Touch_CS_DISABLE()                 	 GPIO_SetBits ( Touch_SPI_CS_PORT, Touch_SPI_CS_PIN )    
#define             Touch_CS_ENABLE()                          GPIO_ResetBits ( Touch_SPI_CS_PORT, Touch_SPI_CS_PIN )  

#define             Touch_CLK_HIGH()                           GPIO_SetBits ( Touch_SPI_CLK_PORT, Touch_SPI_CLK_PIN )    
#define             Touch_CLK_LOW()                            GPIO_ResetBits ( Touch_SPI_CLK_PORT, Touch_SPI_CLK_PIN ) 

#define             Touch_MOSI_1()                             GPIO_SetBits ( Touch_SPI_MOSI_PORT, Touch_SPI_MOSI_PIN ) 
#define             Touch_MOSI_0()                             GPIO_ResetBits ( Touch_SPI_MOSI_PORT, Touch_SPI_MOSI_PIN )

#define             Touch_MISO()                               GPIO_ReadInputDataBit ( Touch_SPI_MISO_PORT, Touch_SPI_MISO_PIN )

#define	            LCD_CHANNEL_X 	                          0x90 	          //通道Y+的选择控制字	
#define	            LCD_CHANNEL_Y 	                          0xd0	          //通道X+的选择控制字


//检测 ：触摸屏的按下和释放

#define TOUCH_NOT_PRESSED  0
#define TOUCH_PRESSED      1

//触摸参数写到FLASH里的标志
#define		FLASH_TOUCH_PARA_FLAG_VALUE			 0xA5

//触摸标志写到FLASH里的地址
#define 	FLASH_TOUCH_PARA_FLAG_ADDR			(1*1024)

//触摸参数写到FLASH里的地址
#define 	FLASH_TOUCH_PARA_ADDR				(2*1024)

#define LCD_DEBUG_ON         0
#define LCD_INFO(fmt,arg...)           printf("<<-XPT2046-INFO->> "fmt"\n",##arg)
#define LCD_ERROR(fmt,arg...)          printf("<<-XPT2046-ERROR->> "fmt"\n",##arg)


//#define XPT2046_DEBUG(fmt,arg...)          do{\
//                                          if(XPT2046_DEBUG_ON)\
//                                          printf("<<-XPT2046-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                          }while(0)

typedef enum
{
	TOUCH_STATE_RELEASE = 0,
	TOUCH_STATE_WAITING,
	TOUCH_STATE_PRESSED,
}Touch_State;


typedef struct         //校准因子结构体 
{
	 float 		An,  		 //注:sizeof(long double) = 8
			Bn,     
			Cn,   
			Dn,    
			En,    
			Fn,     
			Divider;
	
} strType_LCD_Calibration;



typedef struct         //校准系数结构体（最终使用）
{
	float 	dX_X,  			 
		dX_Y,     
		dX,   
		dY_X,    
		dY_Y,    
		dY;

} strType_LCD_TouchPara;

typedef	struct          //液晶坐标结构体 
{		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 
	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_LCD_Coordinate;  

/**************************函数声明********************/

uint16_t Touch_ReceiveData(void);
void Touch_SendCMD(uint8_t cmd)	;
void TOUCH_Init(void);
uint8_t touch_detect(void);
uint8_t LCD_Touch_Calibrate ( uint8_t LCD_Mode ) ;
void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal);
uint8_t LCD_Get_TouchedPoint ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_TouchPara * pTouchPara );
void LCD_TouchDown(strType_LCD_Coordinate * touch);
void LCD_TouchUp(strType_LCD_Coordinate * touch) ;
void LCD_TouchEvenHandler(void );
void Screen_Init(void);

#endif /* __BSP_TOUCH_H*/
>>>>>>> updata
