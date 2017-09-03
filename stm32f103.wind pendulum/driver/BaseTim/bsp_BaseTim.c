#include "bsp_BaseTim.h"
#include "stm32f10x.h"

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}
void Base_Tim_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_Configuration();
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
        TIM_TimeBaseStructure.TIM_Period = Base_TIM_Period;
        TIM_TimeBaseStructure.TIM_Prescaler = Base_TIM_Prescaler;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(Base_Tim_x, &TIM_TimeBaseStructure);
        TIM_ITConfig(Base_Tim_x,TIM_IT_Update,ENABLE);
        TIM_Cmd(Base_Tim_x, ENABLE);
}



