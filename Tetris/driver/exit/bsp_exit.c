#include "bsp_exit.h"


/**
 * @brief 将KEY1 和KEY2 配置成外部中断源 的初始化
 * @param 
 */
void EXTI_KEY_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStruct;
        EXTI_InitTypeDef  EXTI_InitStruct;
        
        //配置中断优先级
        MY_NVIC_Init(2,3,EXTI9_5_IRQn);
        //初始化GPIO
        RCC_APB2PeriphClockCmd(EXIT_CLK,ENABLE);
        
        GPIO_InitStruct.GPIO_Pin = KEY_DOWN_GPIO_PIN ;
        
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(KEY_DOWN_GPIO_PORT , &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin=KEY_PAUSE_GPIO_PIN;
        GPIO_Init(KEY_PAUSE_GPIO_PORT , &GPIO_InitStruct);
        
        //初始化EXTI
        //打开外部中断的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
/*   如下方式初始化 GPIOA 和 GPIOC 是将 两个 GPIO 的 0号引脚和13 引脚都初始化   
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA | GPIO_PortSourceGPIOC , GPIO_PinSource0 | GPIO_PinSource13);
*/
        //配置所选IO口为中断（事件）线，即中断源
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
//        EXTI_InitStruct.EXTI_Line = EXTI_Line0;
        //将中断（事件）线 与按键的 IO口匹配起来，即将按键设置为中断源
        EXTI_InitStruct.EXTI_Line = EXTI_Line9 | EXTI_Line7;
        EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
        //触发中断方式设置为上升沿触发
        EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
        //使能中断（事件）线
        EXTI_InitStruct.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStruct);

}





