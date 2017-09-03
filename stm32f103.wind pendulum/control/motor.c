#include "motor.h"
#include "bsp_pwm_output.h"


void motor_Control_GPIO_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(MOTOR1_RCC_CMD|MOTOR2_RCC_CMD|MOTOR3_RCC_CMD|MOTOR4_RCC_CMD, ENABLE); 

        GPIO_InitStructure.GPIO_Pin =  MOTOR1_IN1_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // ¸´ÓÃÍÆÍìÊä³ö
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        
        GPIO_Init(MOTOR1_IN1_PORT, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin =  MOTOR1_IN2_PIN;
        GPIO_Init(MOTOR1_IN2_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin =  MOTOR2_IN1_PIN;
        GPIO_Init(MOTOR2_IN1_PORT, &GPIO_InitStructure);       
        GPIO_InitStructure.GPIO_Pin =  MOTOR2_IN2_PIN;
        GPIO_Init(MOTOR2_IN2_PORT, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  MOTOR3_IN1_PIN;
        GPIO_Init(MOTOR3_IN1_PORT, &GPIO_InitStructure);        
        GPIO_InitStructure.GPIO_Pin =  MOTOR3_IN2_PIN;
        GPIO_Init(MOTOR3_IN2_PORT, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  MOTOR4_IN1_PIN;
        GPIO_Init(MOTOR4_IN1_PORT, &GPIO_InitStructure);        
        GPIO_InitStructure.GPIO_Pin =  MOTOR4_IN2_PIN;
        GPIO_Init(MOTOR4_IN2_PORT, &GPIO_InitStructure);
}


void Set_motor1_Stop(void)
{
        MOTOR1_IN1_0;
        MOTOR1_IN2_0;
        TIM3->CCR1 = 0;
        
        MOTOR2_IN1_0;
        MOTOR2_IN2_0;
        TIM3->CCR2 = 0;
        
        MOTOR3_IN1_0;
        MOTOR3_IN2_0;
        TIM3->CCR3 = 0;
        
        MOTOR4_IN1_0;
        MOTOR4_IN2_0;
        TIM3->CCR4 = 0;
        
}

void Set_X_Positive_PWM(int32_t val)
{
        MOTOR2_IN1_1;
        MOTOR2_IN2_0;
        MOTOR4_IN1_0;
        MOTOR4_IN2_1;
        TIM3->CCR2 = val;
        TIM3->CCR4 = val;
}

void Set_X_Negative_PWM(int32_t val)
{
        MOTOR2_IN1_0;
        MOTOR2_IN2_1;
        MOTOR4_IN1_1;
        MOTOR4_IN2_0;
        TIM3->CCR2 = val;
        TIM3->CCR4 = val;
}

void Set_Y_Positive_PWM(int32_t val)
{
        MOTOR1_IN1_1;
        MOTOR1_IN2_0;
        
        MOTOR3_IN1_0;
        MOTOR3_IN2_1;
        TIM3->CCR1 = val;
        TIM3->CCR3 = val;
}

void Set_Y_Negative_PWM(int32_t val)
{
        MOTOR1_IN1_0;
        MOTOR1_IN2_1;
        
        MOTOR3_IN1_1;
        MOTOR3_IN2_0;
        TIM3->CCR1 = val;
        TIM3->CCR3 = val;
}

void Motor_Init(void)
{
        TIMx_PWM_Init();
        motor_Control_GPIO_Init();
        Set_motor1_Stop();
        
}

void Set_MotorX_PWM(int32_t pwm)
{
        if(pwm>0)
        {
                Set_X_Positive_PWM(pwm);
        }
        else
        {
                Set_X_Negative_PWM(-pwm);
        }
}

void Set_MotorY_PWM(int32_t pwm)
{
        if(pwm>0)
        {
                Set_Y_Positive_PWM(pwm);
        }
        else
        {
                Set_Y_Negative_PWM(-pwm);
        }
}

int32_t my_abs(int32_t val)
{
        if(val>=0)
        {
                return val;
        }
        else
        {
                return -val;
        }
        
        
}
void Set_MotorX2_PWM(int32_t pwm)
{
        if(pwm>0)
        {
                Set_X_Positive_PWM(pwm);
        }
        else
        {
                Set_X_Negative_PWM(-pwm);
        }
}

void Set_MotorY2_PWM(int32_t pwm)
{
        if(pwm>0)
        {
                Set_Y_Positive_PWM(pwm);
        }
        else
        {
                Set_Y_Negative_PWM(-pwm);
        }
}




        