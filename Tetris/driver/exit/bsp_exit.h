#ifndef __BSP_EXIT_H
#define __BSP_EXIT_H

#include "stm32f10x.h"
#include "bsp_systick.h"

#define KEY_DOWN_GPIO_PIN                       GPIO_Pin_7
#define KEY_DOWN_GPIO_PORT                      GPIOC
#define KEY_DOWN_GPIO_CLK                       RCC_APB2Periph_GPIOC

#define KEY_PAUSE_GPIO_PIN                      GPIO_Pin_9
#define KEY_PAUSE_GPIO_PORT                     GPIOC
#define KEY_PAUSE_GPIO_CLK                      RCC_APB2Periph_GPIOC

#define EXIT_CLK    (KEY_PAUSE_GPIO_CLK|KEY_DOWN_GPIO_CLK)

void EXTI_KEY_Init(void);



#endif /*__BSP_EXIT_H*/




