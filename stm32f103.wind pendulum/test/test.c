#include "test.h"
#include "bsp_pwm_output.h"
#include "bsp_usart.h"
#include "mpu6050.h"
#include <math.h>
#include "ahrs.h"
#include "data_scope.h"
#include "motor.h"


void test_PWM(void)
{
        printf("%d\n",(u16)macTIMx->CCR1);
        printf("%d\n",(u16)macTIMx->CCR2);
        printf("%d\n",(u16)macTIMx->CCR3);
        printf("%d\n",(u16)macTIMx->CCR4);
        
}




//u16 Get_MPU_Direct_Acc_X(void);
//u16 Get_MPU_Direct_Acc_Y(void);
//u16 Get_MPU_Direct_Acc_Z(void);

//u16 Get_MPU_Direct_Gyro_X(void);
//u16 Get_MPU_Direct_Gyro_Y(void);
//u16 Get_MPU_Direct_Gyro_Z(void);

#if 0
void test_mpu6050(void)
{
        float num=0;
        float Val[6]={0};
        float Val1[6] = {0};
        u16 temp[6]={0};
        temp[0] = Get_MPU_Reg_Acc_X();
        temp[1] = Get_MPU_Reg_Acc_Y();
        temp[2] = Get_MPU_Reg_Acc_Z();
        if(temp[2]>32768)
        {
                num = (float)(temp[2]-65536);
                num = (num*19.6/32768.0);
        }
        else
        {
                num = (temp[2]*19.6/32768.0);
        }
        Val[2] = Get_MPU_Acc_Z();
        
        if(temp[1]>32768)
        {
                Val[1] = (float)(temp[1]-65536);
                Val[1] = (Val[1]*19.6/32768.0);
        }
        else
        {
                Val[1] = (temp[1]*19.6/32768.0);
        }
        
        if(temp[0]>32768)
        {
                Val[0] = (float)(temp[2]-65536);
                Val[0] = (Val[0]*19.6/32768.0);
        }
        else
        {
                Val[0] = (temp[0]*19.6/32768.0);
        }
        Val1[0] = Get_MPU_Acc_X();
        Val1[1] = Get_MPU_Acc_Y();
        Val1[2] = Get_MPU_Acc_Z();
//        printf("Acc_x = %d\n",temp[0]);
//        printf("Acc_y = %d\n",temp[1]);
//        printf("Acc_z = %d\n",temp[2]);
//        printf("Acc_x_val = %0.2f\n",Val[0]);
//        printf("Acc_y_val = %0.2f\n",Val[1]);
//        printf("Acc_z_val = %0.2f\n\n",Val[2]);
        printf("Acc_x_val11 = %0.2f\n",Val1[0]);
        printf("Acc_y_val11 = %0.2f\n",Val1[1]);
        printf("Acc_z_val11 = %0.2f\n\n",Val1[2]);
        printf("angle_x=%0.2f",(atan2(Val1[1],Val1[2]))*57.2958);
        printf("angle_Y=%0.2f",(atan2(Val1[0],Val1[2]))*57.2958);
        printf("num = %0.2f\n",num);
        temp[3] = Get_MPU_Reg_Gyro_X();
        temp[4] = Get_MPU_Reg_Gyro_Y();
        temp[5] = Get_MPU_Reg_Gyro_Z();
        printf("Gyro_x = %d\n",temp[3]);
        printf("Gyro_y = %d\n",temp[4]);
        printf("Gyro_z = %d\n",temp[5]);        
}

void test_get_angle
        (void)
{
        float angle_X=0,angle_Y=0,angle_Z=0;
        u8 i=0;
        float Fangun_Angle=0,Fuyan_Angle=0;
        float pitch_temp1 = 0.0;
        float roll_temp1 = 0.0;
        float pitch_sum=0;
        float roll_sum=0; 
        float Gyro_X=0,Gyro_Y=0,Gyro_Z=0;
        for(i=0;i<3;i++)
        {	
                
                pitch_temp1 = ((atan2(Get_MPU_Acc_X(),Get_MPU_Acc_Z()))*57.2958-0.4);   //¼ÆËãPitch½Ç¶È 0.4Îª¾²Ì¬Æ«²î½Ç
                roll_temp1  = ((atan2(Get_MPU_Acc_Y(),Get_MPU_Acc_Z()))*57.2958-0.3);   //¼ÆËãRoll½Ç¶È  0.3Îª¾²Ì¬Æ«²î½Ç
                
                pitch_sum += pitch_temp1;
                roll_sum  += roll_temp1;
        }
        angle_Y = pitch_sum/3.0;
        angle_X = roll_sum/3.0;
//        printf("angle_Y = %0.2f\n",angle_Y);
//        printf("angle_X = %0.2f\n",angle_X);
        Gyro_Y = Get_MPU_Gyro_Y();
        Gyro_X = Get_MPU_Gyro_X();
        Fuyan_Angle = Kalman_Filter1(angle_Y,Gyro_Y);
        Fangun_Angle = Kalman_Filter2(angle_X,-Gyro_X);
//        printf("Fangun_Angle = %0.2f\n",Fuyan_Angle);
//        printf("Fuyan_Angle = %0.2f\n",Fangun_Angle);
//        data_scope_wave(4,angle_Y,angle_X,Fuyan_Angle,Fangun_Angle);
        
}

#endif

void test_control_motor_gpio(void)
{
//        GPIO_SetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
//        GPIO_ResetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
//        
//        GPIO_SetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
//        GPIO_ResetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
//        

//        
//        GPIO_SetBits(MOTOR4_IN1_PORT,MOTOR4_IN1_PIN);
//        GPIO_ResetBits(MOTOR4_IN2_PORT,MOTOR4_IN2_PIN);

//        printf("CCR1 = %d\n",TIM3->CCR1);
//        printf("CCR2 = %d\n",TIM3->CCR2);
//        printf("CCR3 = %d\n",TIM3->CCR3);
//        printf("CCR4 = %d\n",TIM3->CCR4);
        
     //   Set_motor1_Stop();
//        Set_X_Negative_PWM(1000);
//        Set_X_Positive_PWM(1000);
//        Set_Y_Negative_PWM(1000);
//        printf("CCR3 = %d\n",TIM3->CCR1);
//        TIM3->CCR3 = 2000;
////        printf("CCR3 = %d\n",TIM3->CCR3);
//        MOTOR1_IN1_0;
//        MOTOR1_IN2_1;
//        TIM3->CCR1 = 1000;
//        MOTOR1_IN1_1;
//        MOTOR1_IN2_0;
//        MOTOR3_IN1_1;
//        MOTOR3_IN2_0;
//        TIM3->CCR1 = 2000;
//        TIM3->CCR3 = 2000;
        
//        Set_Y_Positive_PWM(1000);
//        Set_MotorX_PWM(-1000);
//        Set_MotorY_PWM(-1000);
        MOTOR4_IN1_1;
        MOTOR4_IN2_0;
        
        MOTOR3_IN1_1;
        MOTOR3_IN2_0;
        
        MOTOR2_IN1_1;
        MOTOR2_IN2_0;
        
        MOTOR1_IN1_1;
        MOTOR1_IN2_0;
        
        TIM3->CCR2 = 2000;
        TIM3->CCR4 = 2000;
        TIM3->CCR1 = 2000;
        TIM3->CCR3 = 2000;
        
}
        
        

