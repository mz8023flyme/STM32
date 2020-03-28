#ifndef __BSP_TIMEBASE_H
#define __BSP_TIMEBASE_H


#include "stm32f10x.h"


/********************������ʱ��TIM�������壬ֻ��TIM6��7************/

#define            BASIC_TIM                   TIM2
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM2
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM2_IRQn
#define            BASIC_TIM_IRQHandler        TIM2_IRQHandler


/**************************��������********************************/

void BASIC_TIM_Init(void);


#endif /* __BSP_TIMEBASE_H */


