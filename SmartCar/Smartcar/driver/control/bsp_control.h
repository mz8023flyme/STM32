#ifndef __BSP_CONTROL_H
#define __BSP_CONTROL_H

#include "stm32f10x.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

void Set_Pwm(int motor1);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
int Position_PID (int Encoder,int Target);
int Incremental_PI (int Encoder,int Target);
void Xianfu_Pwm(void);
int Read_Position(u8 TIMX);
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void Timer3_Init(u16 arr,u16 psc)  ;

void test_control (void);

#endif /*__BSP_CONTROL_H*/
