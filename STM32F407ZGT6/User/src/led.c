#include "led.h"

/* 寄存器初始化(LED4 -- PC0) */
//void LED_Init(void)
//{
//	RCC->AHB1ENR |= 1 << 2;
//	GPIOC->MODER &= ~(3 << (2 * 0));
//	GPIOC->MODER |= 1 << (2 * 0);
//	GPIOC->OTYPER &= ~(1 << 0);
//	GPIOC->OSPEEDR &= ~(3 << (2 * 0));
//	GPIOC->OSPEEDR |= 3 << (2 * 0);
//	GPIOC->PUPDR &= ~(3 << (2 * 0));
//	GPIOC->ODR |= 1 << 0;
//}


/* 库函数初始化(LED4 -- PC0) */
void LED_Init(void)
{
        /* 库函数结构体变量 */
        GPIO_InitTypeDef GPIO_InitStructure;                   //定义GPIO口结构体变量

        /* 设置GPIO工作模式 */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //使能GPIOC时钟
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  //使能GPIOC时钟
        
        GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_OUT;         //通用输出模式
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;       //输出速度50M
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无需要上/下拉

        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //设置GPIOC.0端口
        GPIO_Init(GPIOC,&GPIO_InitStructure);                   //初始化GPIOC
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;              //设置GPIOC.0端口
        GPIO_Init(GPIOF,&GPIO_InitStructure);                   //初始化GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;              //设置GPIOC.0端口
        GPIO_Init(GPIOF,&GPIO_InitStructure);                   //初始化GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //设置GPIOC.0端口
        GPIO_Init(GPIOF,&GPIO_InitStructure);                   //初始化GPIOC
        
        GPIO_SetBits(GPIOC,GPIO_Pin_0);
        GPIO_SetBits(GPIOF,GPIO_Pin_6);
        GPIO_SetBits(GPIOF,GPIO_Pin_9);
        GPIO_SetBits(GPIOF,GPIO_Pin_10);
        
        
} 
void KEY_Init(void)
{
        /* 库函数结构体变量 */
        GPIO_InitTypeDef GPIO_InitStructure;                   //定义GPIO口结构体变量

        /* 设置GPIO工作模式 */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOC时钟
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //使能GPIOC时钟
        
        GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;         //通用输入模式
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无需要上/下拉

        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //设置GPIOC.0端口
        GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化GPIOC
        
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;              //设置GPIOC.0端口
        GPIO_Init(GPIOE,&GPIO_InitStructure);                   //初始化GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;              //设置GPIOC.0端口
        GPIO_Init(GPIOE,&GPIO_InitStructure);                   //初始化GPIOC
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;              //设置GPIOC.0端口
        GPIO_Init(GPIOE,&GPIO_InitStructure);                   //初始化GPIOC
} 


















