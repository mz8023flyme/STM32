#ifndef  __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f10x.h"

#define KEY3_INT_GPIO_PIN        GPIO_Pin_0	
#define KEY3_INT_GPIO_PORT       GPIOA 
#define KEY3_INT_GPIO_CLK        RCC_APB2Periph_GPIOA  

#define KEY4_INT_GPIO_PIN        GPIO_Pin_13	
#define KEY4_INT_GPIO_PORT       GPIOC 
#define KEY4_INT_GPIO_CLK        RCC_APB2Periph_GPIOC 

void EXTI_Config (void);

#endif /* __BSP_EXTI_H*/

