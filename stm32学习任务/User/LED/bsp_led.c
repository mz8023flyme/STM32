#include "bsp_led.h"

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
