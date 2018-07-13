#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_x                         ADC1
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC

// PC1-通道11 独立IO
#define    ADC_PIN                       GPIO_Pin_1
#define    ADC_CHANNEL                   ADC_Channel_11
#define    ADC_DMA_CHANNEL               DMA1_Channel1

/************************函数声明***************************/

void ADCx_Init(void);

#endif   /*  __BSP-ADC_H  */
