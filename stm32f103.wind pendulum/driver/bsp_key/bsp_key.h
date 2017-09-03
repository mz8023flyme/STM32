#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_
/* 
* 模块：按键模块
* 实现方式：10ms 中断轮询
* 资源占用：定时器5，抢断优先级：1，亚优先级：1
*			4个通用IO
* 使用时在中断C文件中调用以下代码
* Key_Scan(void);返回值0 1 2 3 4，0表示没有按键按下
*/
#include "stm32f10x.h"

#define K1_GPIO_RCC             RCC_APB2Periph_GPIOB
#define K1_GPIO_PORT            GPIOB
#define K1_GPIO_PIN             GPIO_Pin_5

#define K2_GPIO_RCC             RCC_APB2Periph_GPIOB
#define K2_GPIO_PORT            GPIOB
#define K2_GPIO_PIN             GPIO_Pin_6

#define K3_GPIO_RCC             RCC_APB2Periph_GPIOA
#define K3_GPIO_PORT            GPIOA
#define K3_GPIO_PIN             GPIO_Pin_11

#define K4_GPIO_RCC             RCC_APB2Periph_GPIOA
#define K4_GPIO_PORT            GPIOA
#define K4_GPIO_PIN             GPIO_Pin_12


void TIM5_Config(void);
void Key_GPIO_Init(void);
u8 Key_Scan_10ms(void);
u8 Key_Scan(void);
#endif

