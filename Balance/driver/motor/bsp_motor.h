#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "stm32f10x.h"



int abs (int val);
void TIM3_PWM_Init(void);
void M1_Revolve(int val);
void M2_Revolve(int val);
void MOTOR_GPIO_Config(void);


#endif /*__BSP_MOTOR_H*/
