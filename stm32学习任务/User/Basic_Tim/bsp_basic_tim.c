#include "bsp_basic_tim.h"
uint16_t time=0;

// 中断优先级配置
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void BASIC_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);
//	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
//	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up  ;
	TIM_TimeBaseStruct.TIM_Period = BASIC_TIM_Period ;	 //重装载，产生中断
	TIM_TimeBaseStruct.TIM_Prescaler = BASIC_TIM_Prescaler ; //分频因子
//	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0 ;
	TIM_TimeBaseInit(BASIC_TIM , &TIM_TimeBaseStruct);
	// 清除计数器中断标志位
        TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	// 开启计数器中断
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd( BASIC_TIM , ENABLE);
	
}	


void BASIC_TIM_Init(void)	
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Config();
}

