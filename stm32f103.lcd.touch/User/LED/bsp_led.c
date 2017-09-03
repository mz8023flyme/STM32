<<<<<<< HEAD
#include "bsp_led.h"

static void DelayMS ( __IO uint32_t ulCount );

void LED_GPIO_Config (void)
{
	
	GPIO_InitTypeDef	 GPIO_Initstuct;
	
	RCC_APB2PeriphClockCmd(LED_G_GPIO_CLK, ENABLE);		//开时钟
	
	GPIO_Initstuct.GPIO_Pin = LED_R_GPIO_PIN| LED_G_GPIO_PIN|LED_B_GPIO_PIN   ;
	GPIO_Initstuct.GPIO_Mode = GPIO_Mode_Out_PP;    //设置模式为推挽输出
	GPIO_Initstuct.GPIO_Speed = GPIO_Speed_50MHz;	//设置速度
	GPIO_Init(LED_G_GPIO_PORT, &GPIO_Initstuct);	//
	GPIO_Init(LED_R_GPIO_PORT, &GPIO_Initstuct);
	GPIO_Init(LED_B_GPIO_PORT, &GPIO_Initstuct);

	
	GPIO_SetBits(LED_R_GPIO_PORT, LED_G_GPIO_PIN);		//关闭所有的灯，，进行初始化
	GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
	GPIO_SetBits(LED_R_GPIO_PORT, LED_B_GPIO_PIN);
	
}

void BEEP_GPIO_Config(void)
{
	GPIO_InitTypeDef	 GPIO_Initstuct;
	RCC_APB2PeriphClockCmd(LED_BEEP_GPIO_CLK, ENABLE);	
	GPIO_Initstuct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstuct.GPIO_Pin = LED_BEEP_GPIO_PIN;
	GPIO_Initstuct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(LED_BEEP_GPIO_PORT, &GPIO_Initstuct);
	
	GPIO_ResetBits(LED_BEEP_GPIO_PORT, LED_BEEP_GPIO_PIN);	
}

void Water_Lape(void)
{
	static uint8_t i;
	for(i=4;i>0;i--)
	{
		LED_RED;
		DelayMS(250);
		LED_BLUE  ;     
		DelayMS(250);		
		LED_GREEN ;
		DelayMS(250);		
		LED_YELLOW ;
		DelayMS(250);
		LED_WHITE ;
		DelayMS(250);		
		LED_VIOLET ;  			
		DelayMS(250);
	}
	LED_OFF;
}

static void DelayMS ( __IO uint32_t ulCount )
{
	uint32_t i;


	for ( i = 0; i < ulCount; i ++ )
	{
		uint32_t uc = 12000;     //设置值为12，大约延1毫秒  
	      
		while ( uc -- );     //延1微秒	

	}
	
}










=======
#include "bsp_led.h"

static void DelayMS ( __IO uint32_t ulCount );

void LED_GPIO_Config (void)
{
	
	GPIO_InitTypeDef	 GPIO_Initstuct;
	
	RCC_APB2PeriphClockCmd(LED_G_GPIO_CLK, ENABLE);		//开时钟
	
	GPIO_Initstuct.GPIO_Pin = LED_R_GPIO_PIN| LED_G_GPIO_PIN|LED_B_GPIO_PIN   ;
	GPIO_Initstuct.GPIO_Mode = GPIO_Mode_Out_PP;    //设置模式为推挽输出
	GPIO_Initstuct.GPIO_Speed = GPIO_Speed_50MHz;	//设置速度
	GPIO_Init(LED_G_GPIO_PORT, &GPIO_Initstuct);	//
	GPIO_Init(LED_R_GPIO_PORT, &GPIO_Initstuct);
	GPIO_Init(LED_B_GPIO_PORT, &GPIO_Initstuct);

	
	GPIO_SetBits(LED_R_GPIO_PORT, LED_G_GPIO_PIN);		//关闭所有的灯，，进行初始化
	GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);
	GPIO_SetBits(LED_R_GPIO_PORT, LED_B_GPIO_PIN);
	
}

void BEEP_GPIO_Config(void)
{
	GPIO_InitTypeDef	 GPIO_Initstuct;
	RCC_APB2PeriphClockCmd(LED_BEEP_GPIO_CLK, ENABLE);	
	GPIO_Initstuct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstuct.GPIO_Pin = LED_BEEP_GPIO_PIN;
	GPIO_Initstuct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(LED_BEEP_GPIO_PORT, &GPIO_Initstuct);
	
	GPIO_ResetBits(LED_BEEP_GPIO_PORT, LED_BEEP_GPIO_PIN);	
}

void Water_Lape(void)
{
	static uint8_t i;
	for(i=4;i>0;i--)
	{
		LED_RED;
		DelayMS(250);
		LED_BLUE  ;     
		DelayMS(250);		
		LED_GREEN ;
		DelayMS(250);		
		LED_YELLOW ;
		DelayMS(250);
		LED_WHITE ;
		DelayMS(250);		
		LED_VIOLET ;  			
		DelayMS(250);
	}
	LED_OFF;
}

static void DelayMS ( __IO uint32_t ulCount )
{
	uint32_t i;


	for ( i = 0; i < ulCount; i ++ )
	{
		uint32_t uc = 12000;     //设置值为12，大约延1毫秒  
	      
		while ( uc -- );     //延1微秒	

	}
	
}










>>>>>>> updata
