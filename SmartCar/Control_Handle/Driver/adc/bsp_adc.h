#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

extern __IO uint16_t ADC_ConvertedValue[5];
extern float ADC_Value[5];

// ADC ���ѡ��
// ������ ADC1/2�����ʹ��ADC3���ж���ص�Ҫ�ĳ�ADC3��
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA
#define    ADC_PIN0                      GPIO_Pin_0
#define    ADC_PIN1                      GPIO_Pin_1
#define    ADC_PIN2                      GPIO_Pin_2
#define    ADC_PIN3                      GPIO_Pin_3
#define    ADC_PIN4                      GPIO_Pin_4


#define    ADC_DMA_CHANNEL               DMA1_Channel1

// ADC ͨ���궨��
#define    ADC_CHANNEL0                   ADC_Channel_0
#define    ADC_CHANNEL1                   ADC_Channel_1
#define    ADC_CHANNEL2                   ADC_Channel_2
#define    ADC_CHANNEL3                   ADC_Channel_3
#define    ADC_CHANNEL4                   ADC_Channel_4

// ADC �ж���غ궨��
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler


void ADCx_Init(void);

#endif /*__BSP_ADC_H*/


