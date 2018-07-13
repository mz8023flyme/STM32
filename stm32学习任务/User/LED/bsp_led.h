#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_G_GPIO_PIN        GPIO_Pin_7	/*指定三原色的输入输出引脚*/
#define LED_B_GPIO_PIN        GPIO_Pin_1
#define LED_R_GPIO_PIN        GPIO_Pin_5
#define LED_G_GPIO_PORT       GPIOA 
#define LED_R_GPIO_PORT       GPIOB 
#define LED_B_GPIO_PORT       GPIOB 
#define LED_G_GPIO_CLK        RCC_APB2Periph_GPIOA  //对时钟进行宏替换
#define LED_R_GPIO_CLK        RCC_APB2Periph_GPIOB
#define LED_B_GPIO_CLK        RCC_APB2Periph_GPIOB


#define   ON      1
#define   OFF     0

#define  LED_R(a)   if(a)   \
	                         GPIO_ResetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);\
                     else    GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
#define  LED_G(a)   if(a)   \
				GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);\
                    else    GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
#define  LED_B(a)   if(a)   \
	                        GPIO_ResetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);\
                    else    GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);
					
#define  LED_RED           		  LED_R(ON)LED_B(OFF)LED_G(OFF)
#define  LED_BLUE       		  LED_R(OFF)LED_B(ON)LED_G(OFF)
#define  LED_GREEN    		  	  LED_R(OFF)LED_B(OFF)LED_G(ON)
#define  LED_YELLOW      		  LED_R(ON)LED_B(OFF)LED_G(ON)
#define  LED_WHITE      	 	  LED_R(ON)LED_B(ON)LED_G(ON)
#define  LED_VIOLET   			  LED_R(ON)LED_B(ON)LED_G(OFF)
#define  LED_OFF      	  		  LED_R(OFF)LED_B(OFF)LED_G(OFF)
void LED_GPIO_Config (void);



/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED_G_TOGGLE digitalToggle(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED1_ON			   digitalLo(LED_R_GPIO_PORT,LED_R_GPIO_PIN)


#endif
