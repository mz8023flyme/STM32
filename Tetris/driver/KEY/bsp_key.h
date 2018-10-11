#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "stm32f10x.h"
#include "main.h"


#define KEY_LEFT_GPIO_PIN           GPIO_Pin_6
#define KEY_LEFT_GPIO_PORT          GPIOC
#define KEY_LEFT_GPIO_CLK           RCC_APB2Periph_GPIOC

#define KEY_RIGHT_GPIO_PIN           GPIO_Pin_8
#define KEY_RIGHT_GPIO_PORT          GPIOC
#define KEY_RIGHT_GPIO_CLK           RCC_APB2Periph_GPIOC
/*
#define KEY_DOWN_GPIO_PIN           GPIO_Pin_7
#define KEY_DOWN_GPIO_PORT          GPIOC
#define KEY_DOWN_GPIO_CLK           RCC_APB2Periph_GPIOC

#define KEY_PAUSE_GPIO_PIN           GPIO_Pin_9
#define KEY_PAUSE_GPIO_PORT          GPIOC
#define KEY_PAUSE_GPIO_CLK           RCC_APB2Periph_GPIOC
*/
#define KEY_RESET_GPIO_PIN           GPIO_Pin_10
#define KEY_RESET_GPIO_PORT          GPIOC
#define KEY_RESET_GPIO_CLK           RCC_APB2Periph_GPIOC

#define KEY_ROTATE_GPIO_PIN           GPIO_Pin_11
#define KEY_ROTATE_GPIO_PORT          GPIOC
#define KEY_ROTATE_GPIO_CLK           RCC_APB2Periph_GPIOC



#define KEY_ON          0
#define KEY_OFF         1

void KEY_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);


#endif /* _BSP_KEY_H */

