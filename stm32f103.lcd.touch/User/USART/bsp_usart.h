<<<<<<< HEAD
#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "bsp_led.h"
#include <stdio.h>

// 串口1-USART1
#define  DEBUG_USARTx                   USART1				//配置串口
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1		//串口时钟
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd		//APB总线时钟	串口时钟
#define  DEBUG_USART_BAUDRATE           115200				//波特率

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)		//连接到GPIOA	
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd		//APB总线时钟	相应串口时钟
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA 				//发送的GPIO  
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9			//发送的GPIO的pin值
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA				//接收的GPIO 
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10			//接收的GPIO的pin值

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

void USART_Config (void);
void USART_sent( USART_TypeDef*pUSARTx, uint8_t Data );
void USART_Sent(USART_TypeDef*pUSARTx,char *str);
#endif /* __BSP_USART_H*/
=======
#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "bsp_led.h"
#include <stdio.h>

// 串口1-USART1
#define  DEBUG_USARTx                   USART1				//配置串口
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1		//串口时钟
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd		//APB总线时钟	串口时钟
#define  DEBUG_USART_BAUDRATE           115200				//波特率

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)		//连接到GPIOA	
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd		//APB总线时钟	相应串口时钟
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA 				//发送的GPIO  
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9			//发送的GPIO的pin值
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA				//接收的GPIO 
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10			//接收的GPIO的pin值

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

void USART_Config (void);
void USART_sent( USART_TypeDef*pUSARTx, uint8_t Data );
void USART_Sent(USART_TypeDef*pUSARTx,char *str);
#endif /* __BSP_USART_H*/
>>>>>>> updata
