#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

#define KEY1_GPIO_PIN        GPIO_Pin_6	/*指定按键1的输入输出引脚*/
#define KEY1_GPIO_PORT       GPIOC 
#define KEY1_GPIO_CLK        RCC_APB2Periph_GPIOC  //对时钟进行宏替换

#define KEY2_GPIO_PIN        GPIO_Pin_7	/*指定按键2的输入输出引脚*/
#define KEY2_GPIO_PORT       GPIOC 
#define KEY2_GPIO_CLK        RCC_APB2Periph_GPIOC  //对时钟进行宏替换

#define   KEY_ON      1
#define   KEY_OFF     0

void KEY_GPIO_Config (void);
uint8_t Key_Scan(GPIO_TypeDef*GPIOx,uint16_t GPIO_Pin);


#endif
