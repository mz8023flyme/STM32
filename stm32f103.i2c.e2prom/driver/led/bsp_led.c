<<<<<<< HEAD
#include "bsp_led.h"

void led_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
        
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);            // IO设置
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;                      // 红 PB5
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;                      // 绿 PB0
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;                      // 蓝 PB1
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStructure;
        
        /* 设置TIM CLK 时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            // 使能COLOR_TIMx时钟
        
        /* 基本定时器配置 */
        TIM_TimeBaseStructure.TIM_Period = 255;                         // 当定时器从 0 计数到 255 , 即为 256 次, 为一个定时周期
        TIM_TimeBaseStructure.TIM_Prescaler = 1999;                     // 设置预分频
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        // 设置时钟分频系数：不分频(这里用不到)
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // 向上计数模式
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
        
        /* PWM模式配置 */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // 配置为 PWM 模式1
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   // 使能输出
        TIM_OCInitStructure.TIM_Pulse = 0;                              // 设置初始PWM脉冲宽度为0
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        // 当定时器计数值小于 CCR_Val 时为低电平
        
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);                        // 使能通道
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);               // 使能预装载
        
        TIM_ARRPreloadConfig(TIM3, ENABLE);                             // 使能COLOR_TIMx重载寄存器ARR
        
        /* COLOR_TIMx enable counter */
        TIM_Cmd(TIM3, ENABLE);                                          // 使能定时器
}

void led_light_color(u32 rgb)
{
        TIM3->CCR2 = (uint8_t)(rgb >> 16);                              // R
        TIM3->CCR3 = (uint8_t)(rgb >> 8);                               // G
        TIM3->CCR4 = (uint8_t)rgb;                                      // B
}
=======
#include "bsp_led.h"

void led_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
        
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);            // IO设置
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;                      // 红 PB5
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;                      // 绿 PB0
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;                      // 蓝 PB1
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        TIM_OCInitTypeDef TIM_OCInitStructure;
        
        /* 设置TIM CLK 时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            // 使能COLOR_TIMx时钟
        
        /* 基本定时器配置 */
        TIM_TimeBaseStructure.TIM_Period = 255;                         // 当定时器从 0 计数到 255 , 即为 256 次, 为一个定时周期
        TIM_TimeBaseStructure.TIM_Prescaler = 1999;                     // 设置预分频
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        // 设置时钟分频系数：不分频(这里用不到)
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // 向上计数模式
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
        
        /* PWM模式配置 */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // 配置为 PWM 模式1
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   // 使能输出
        TIM_OCInitStructure.TIM_Pulse = 0;                              // 设置初始PWM脉冲宽度为0
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        // 当定时器计数值小于 CCR_Val 时为低电平
        
        TIM_OC2Init(TIM3, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
        TIM_OC3Init(TIM3, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
        TIM_OC4Init(TIM3, &TIM_OCInitStructure);                        // 使能通道
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);               // 使能预装载
        
        TIM_ARRPreloadConfig(TIM3, ENABLE);                             // 使能COLOR_TIMx重载寄存器ARR
        
        /* COLOR_TIMx enable counter */
        TIM_Cmd(TIM3, ENABLE);                                          // 使能定时器
}

void led_light_color(u32 rgb)
{
        TIM3->CCR2 = (uint8_t)(rgb >> 16);                              // R
        TIM3->CCR3 = (uint8_t)(rgb >> 8);                               // G
        TIM3->CCR4 = (uint8_t)rgb;                                      // B
}
>>>>>>> updata
