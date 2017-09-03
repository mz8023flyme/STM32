#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H


#include "stm32f10x.h"


/********************TIM参数定义，只限TIM2、3、4或5**************************/
#define             macTIMx                                TIM3
#define             macTIM_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macTIM_CLK                             RCC_APB1Periph_TIM3
#define             macTIM_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macTIM_GPIO_CLK                        ( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB )
#define             macTIM_CH1_PORT                        GPIOA
#define             macTIM_CH1_PIN                         GPIO_Pin_6
#define             macTIM_CH2_PORT                        GPIOA
#define             macTIM_CH2_PIN                         GPIO_Pin_7
#define             macTIM_CH3_PORT                        GPIOB
#define             macTIM_CH3_PIN                         GPIO_Pin_0
#define             macTIM_CH4_PORT                        GPIOB
#define             macTIM_CH4_PIN                         GPIO_Pin_1


/************************** TIM 函数声明********************************/
void               TIMx_PWM_Init                           (void);


#endif /* __PWM_OUTPUT_H */


