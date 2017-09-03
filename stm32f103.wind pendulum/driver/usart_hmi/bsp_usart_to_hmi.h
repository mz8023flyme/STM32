#ifndef _BSP_USART_TO_HMI_H
#define	_BSP_USART_TO_HMI_H


#include "stm32f10x.h"
#include <stdio.h>

#define  HMI_USARTx                   USART3
#define  HMI_USART_CLK                RCC_APB1Periph_USART3
#define  HMI_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  HMI_USART_BAUDRATE           9600

// USART GPIO Òý½Åºê¶¨Òå
#define  HMI_USART_GPIO_CLK            (RCC_APB2Periph_GPIOB)
#define  HMI_USART_GPIO_APBxClkCmd     RCC_APB2PeriphClockCmd
    
#define  HMI_USART_TX_GPIO_PORT         GPIOB   
#define  HMI_USART_TX_GPIO_PIN          GPIO_Pin_10
#define  HMI_USART_RX_GPIO_PORT         GPIOB
#define  HMI_USART_RX_GPIO_PIN          GPIO_Pin_11

#define  HMI_USART_IRQ                  USART3_IRQn
#define  HMI_USART_IRQHandler           USART3_IRQHandler




void HMI_USART_Init(void);
void HMI_Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void HMI_Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void HMI_Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* 	_BSP_USART_TO_HMI_H */
