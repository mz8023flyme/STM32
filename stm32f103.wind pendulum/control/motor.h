#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "stm32f10x.h"

#define MOTOR1_RCC_CMD                  RCC_APB2Periph_GPIOA                  
#define MOTOR1_IN1_PORT                 GPIOA
#define MOTOR1_IN1_PIN                  GPIO_Pin_4
#define MOTOR1_IN2_PORT                 GPIOA
#define MOTOR1_IN2_PIN                  GPIO_Pin_5

#define MOTOR2_RCC_CMD                  RCC_APB2Periph_GPIOB 
#define MOTOR2_IN1_PORT                 GPIOB
#define MOTOR2_IN1_PIN                  GPIO_Pin_12
#define MOTOR2_IN2_PORT                 GPIOB
#define MOTOR2_IN2_PIN                  GPIO_Pin_13

#define MOTOR3_RCC_CMD                  RCC_APB2Periph_GPIOC 
#define MOTOR3_IN1_PORT                 GPIOC
#define MOTOR3_IN1_PIN                  GPIO_Pin_6
#define MOTOR3_IN2_PORT                 GPIOC
#define MOTOR3_IN2_PIN                  GPIO_Pin_7

#define MOTOR4_RCC_CMD                  RCC_APB2Periph_GPIOB 
#define MOTOR4_IN1_PORT                 GPIOB
#define MOTOR4_IN1_PIN                  GPIO_Pin_14
#define MOTOR4_IN2_PORT                 GPIOB
#define MOTOR4_IN2_PIN                  GPIO_Pin_15


/* 设置控制引脚的输出状态 */
#define MOTOR1_IN1_1                    MOTOR1_IN1_PORT->BSRR = MOTOR1_IN1_PIN
#define MOTOR1_IN1_0                    MOTOR1_IN1_PORT->BRR = MOTOR1_IN1_PIN
#define MOTOR1_IN2_1                    MOTOR1_IN1_PORT->BSRR = MOTOR1_IN2_PIN
#define MOTOR1_IN2_0                    MOTOR1_IN1_PORT->BRR = MOTOR1_IN2_PIN

#define MOTOR2_IN1_1                    MOTOR2_IN1_PORT->BSRR = MOTOR2_IN1_PIN
#define MOTOR2_IN1_0                    MOTOR2_IN1_PORT->BRR = MOTOR2_IN1_PIN
#define MOTOR2_IN2_1                    MOTOR2_IN1_PORT->BSRR = MOTOR2_IN2_PIN
#define MOTOR2_IN2_0                    MOTOR2_IN1_PORT->BRR = MOTOR2_IN2_PIN

#define MOTOR3_IN1_1                    MOTOR3_IN1_PORT->BSRR = MOTOR3_IN1_PIN
#define MOTOR3_IN1_0                    MOTOR3_IN1_PORT->BRR = MOTOR3_IN1_PIN
#define MOTOR3_IN2_1                    MOTOR3_IN1_PORT->BSRR = MOTOR3_IN2_PIN
#define MOTOR3_IN2_0                    MOTOR3_IN1_PORT->BRR = MOTOR3_IN2_PIN

#define MOTOR4_IN1_1                    MOTOR4_IN1_PORT->BSRR = MOTOR4_IN1_PIN
#define MOTOR4_IN1_0                    MOTOR4_IN1_PORT->BRR = MOTOR4_IN1_PIN
#define MOTOR4_IN2_1                    MOTOR4_IN1_PORT->BSRR = MOTOR4_IN2_PIN
#define MOTOR4_IN2_0                    MOTOR4_IN1_PORT->BRR = MOTOR4_IN2_PIN



void motor_Control_GPIO_Init(void);
void Motor_Init(void);
void Set_motor1_Stop(void);

void Set_X_Positive_PWM(int32_t val);
void Set_X_Negative_PWM(int32_t val);
void Set_Y_Positive_PWM(int32_t val);
void Set_Y_Negative_PWM(int32_t val);


void Set_MotorX_PWM(int32_t pwm);
void Set_MotorY_PWM(int32_t pwm);

void Set_MotorX2_PWM(int32_t pwm);
void Set_MotorY2_PWM(int32_t pwm);
int32_t my_abs(int32_t val);

#endif /* _MOTOR_H_ */
