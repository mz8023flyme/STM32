#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "stm32f10x.h"


#define   MOTOR_PWM_CLOCK       RCC_APB2Periph_GPIOC     

#define   MOTOR1_PWM_PIN        GPIO_Pin_7
#define   MOTOR2_PWM_PIN        GPIO_Pin_9

#define   Steer1_PWM_PIN         GPIO_Pin_6
#define   Steer2_PWM_PIN         GPIO_Pin_8


#define   MOTOR_DIR_CLOCK       RCC_APB2Periph_GPIOE

#define   MOTOR1_DIR_PIN1       GPIO_Pin_7
#define   MOTOR1_DIR_PIN2       GPIO_Pin_9
#define   MOTOR2_DIR_PIN1       GPIO_Pin_13
#define   MOTOR2_DIR_PIN2       GPIO_Pin_15


#define   MOTOR1_PIN1           PEout(7)
#define   MOTOR1_PIN2           PEout(9)
#define   MOTOR2_PIN1           PEout(13)
#define   MOTOR2_PIN2           PEout(15)


int abs (int val);
void TIM8_PWM_Init(void);
void M1_Revolve(int val);
void M2_Revolve(int val);
void MOTOR_GPIO_Config(void);
unsigned char Distance_Scan(void );
void BZ_Turn(u8 direction);


#endif /*__BSP_MOTOR_H*/
