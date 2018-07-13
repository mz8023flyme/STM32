#include "bsp_exti.h"
#include "bsp_led.h"


static void EXTI_NVIC_Config (void)
{
	NVIC_InitTypeDef	 NVIC_Initstuct;
	NVIC_Initstuct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Initstuct.NVIC_IRQChannelPreemptionPriority = 1;  			//主优先级
	NVIC_Initstuct.NVIC_IRQChannelSubPriority = 1;				//子优先级
	NVIC_Initstuct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstuct);
	
	NVIC_Initstuct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_Init(&NVIC_Initstuct);
}
void EXTI_Config (void)
{
	GPIO_InitTypeDef	 GPIO_Initstuct;
	EXTI_InitTypeDef	 EXTI_Initstuct;
	EXTI_NVIC_Config ( );
	
	//初始化要连接到EXTI的GPIO
	RCC_APB2PeriphClockCmd(KEY3_INT_GPIO_CLK , ENABLE);		//开时钟
	GPIO_Initstuct.GPIO_Pin = KEY3_INT_GPIO_PIN  ;
	GPIO_Initstuct.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_Initstuct);
	
	RCC_APB2PeriphClockCmd(KEY4_INT_GPIO_CLK , ENABLE);		//开时钟
	GPIO_Initstuct.GPIO_Pin = KEY4_INT_GPIO_PIN  ;
	GPIO_Initstuct.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(KEY4_INT_GPIO_PORT, &GPIO_Initstuct);
	
	//初始化EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);	\
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_Initstuct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Initstuct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Initstuct.EXTI_Line = EXTI_Line0;
	EXTI_Initstuct.EXTI_LineCmd = ENABLE;
	EXTI_Init( &EXTI_Initstuct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_Initstuct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Initstuct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Initstuct.EXTI_Line = EXTI_Line13;
	EXTI_Initstuct.EXTI_LineCmd = ENABLE;
	EXTI_Init( &EXTI_Initstuct);
}





