#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define Water_GPIO_PORT          GPIOB                   /* GPIO端口 */
#define Water_GPIO_CLK           RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define Water_GPIO_PIN           GPIO_Pin_12              /* 连接到SCL时钟线的GPIO */

// G-绿色
#define TemDown_GPIO_PORT          GPIOB                   /* GPIO端口 */
#define TemDown_GPIO_CLK           RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define TemDown_GPIO_PIN           GPIO_Pin_13              /* 连接到SCL时钟线的GPIO */

// B-蓝色
#define LED_GPIO_PORT          GPIOB                   /* GPIO端口 */
#define LED_GPIO_CLK           RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define LED_GPIO_PIN           GPIO_Pin_14              /* 连接到SCL时钟线的GPIO */

#define DOOR_GPIO_PORT          GPIOA                   /* GPIO端口 */
#define DOOR_GPIO_CLK           RCC_APB2Periph_GPIOA        /* GPIO端口时钟 */
#define DOOR_GPIO_PIN           GPIO_Pin_15              /* 连接到SCL时钟线的GPIO */
        
#define BEEP_GPIO_PORT          GPIOA                    /* GPIO端口 */
#define BEEP_GPIO_CLK           RCC_APB2Periph_GPIOA        /* GPIO端口时钟 */
#define BEEP_GPIO_PIN           GPIO_Pin_8              /* 连接到SCL时钟线的GPIO */


#define Wind_GPIO_PORT          GPIOB                            /* GPIO端口 */
#define Wind_GPIO_CLK           RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define Wind_GPIO_PIN           GPIO_Pin_0
/** the macro definition to trigger the led on or off 
  * 1 - off
  *1 - on
  */
#define ON  1
#define OFF 0

/* 使用标准的固件库控制IO*/
#define Water(a) if (a)  \
                                GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
                                else    \
                                GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define TemDown(a) if (a)  \
                                GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
                                else            \
                                GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED(a) if (a) \
                                GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
                                else     \
                                GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)  {p->BSRR=i;}    //输出为高电平        
#define digitalLo(p,i)  {p->BRR=i;}     //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态

#define READDOOR         ((DOOR_GPIO_PORT->IDR & DOOR_GPIO_PIN)==0)   //读到的是低电平


/* 定义控制IO的宏 */
#define Water_TOGGLE      digitalToggle(Water_GPIO_PORT,Water_GPIO_PIN)
#define Water_ON          digitalHi(Water_GPIO_PORT,Water_GPIO_PIN)
#define Water_OFF         digitalLo(Water_GPIO_PORT,Water_GPIO_PIN)
 
#define TemDown_TOGGLE    digitalToggle(TemDown_GPIO_PORT,TemDown_GPIO_PIN)
#define TemDown_ON        digitalHi(TemDown_GPIO_PORT,TemDown_GPIO_PIN)
#define TemDown_OFF       digitalLo(TemDown_GPIO_PORT,TemDown_GPIO_PIN)

#define LED_TOGGLE        digitalToggle(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_ON            digitalHi(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_OFF           digitalLo(LED_GPIO_PORT,LED_GPIO_PIN)


#define BEEP_TOGGLE        digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON            digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF           digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)


#define Wind_TOGGLE      digitalToggle(Water_GPIO_PORT,Water_GPIO_PIN)
#define Wind_ON          digitalHi(Water_GPIO_PORT,Water_GPIO_PIN)
#define Wind_OFF         digitalLo(Water_GPIO_PORT,Water_GPIO_PIN)

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

//红
#define LED_RED  \
                                LED1_ON;\
                                LED2_OFF\
                                LED3_OFF

//绿
#define LED_GREEN       \
                                LED1_OFF;\
                                LED2_ON\
                                LED3_OFF

//蓝
#define LED_BLUE        \
                                LED1_OFF;\
                                LED2_OFF\
                                LED3_ON

        
//黄(红+绿)        
#define LED_YELLOW      \
                                LED1_ON;\
                                LED2_ON\
                                LED3_OFF
//紫(红+蓝)
#define LED_PURPLE      \
                                LED1_ON;\
                                LED2_OFF\
                                LED3_ON

//青(绿+蓝)
#define LED_CYAN \
                                LED1_OFF;\
                                LED2_ON\
                                LED3_ON
                                
//白(红+绿+蓝)
#define LED_WHITE       \
                                LED1_ON;\
                                LED2_ON\
                                LED3_ON
                                
//黑(全部关闭)
#define LED_RGBOFF      \
                                LED1_OFF;\
                                LED2_OFF\
                                LED3_OFF

void LED_GPIO_Config(void);

#endif
