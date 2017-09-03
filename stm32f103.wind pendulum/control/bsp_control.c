#include "bsp_control.h"
#include "Motor_PID.h"
#include "math.h"
#include "motor.h"
#include "data_scope.h"
#include "bsp_usart.h"
#include "usart_mpu.h"
#include "data_scope.h"


#define SWING_L         44 // 摆杆的长度，用于计算单摆的周期，单位为cm
#define SWING_T         1331.35 //单摆的周期，单位为ms
#define SWING_H         70 //万向节到地面的高度为70cm
#define PI              3.1415




void Mode0(void)
{
        
}

u8 Offset_rotation = 0;//设置偏移的角度
u8 line_length=30;              //摆动的直线一半的距离
u8  radius_of_circle = 30;//模式三中圆的半径

void Mode1(float pitch,float roll)
{
        static float Cycle_normalized=0;        //周期归一化变量
        static float Point_normalized=0;        //角度归一化变量
        static unsigned int Time_step=0;        //时间增量，用于计算归一化的值
        int32_t M1_PWM=0,M2_PWM=0;
        float set_x=0,set_y=0;
        float Angle_max=0;                       //摆动幅度的最大值
        Time_step += 10; //10ms中断
        Cycle_normalized = Time_step/SWING_T;   //对周期进行归一化处理
        Point_normalized = Cycle_normalized*2*3.14159;  //对2pi 进行归一化处理
        Angle_max = atan2(line_length,70.0)*57.2958; //摆动的最大角度
        
        set_x = Angle_max*cos(Offset_rotation * PI/180.0)*sin(Point_normalized);//将理论值分解到电机X方向
        set_y = Angle_max*sin(Offset_rotation * PI/180.0)*sin(Point_normalized);;//将理论值分解到电机Y方向
        
        //printf("set_x = %0.2f,set_y = %0.2f",set_x,set_y);
        PID_M1_SetPoint(set_x);//设置x方向的电机的理论值
        PID_M1_SetKp(60);
        PID_M1_SetKi(0.6);
        PID_M1_SetKd(20000);
        
        PID_M2_SetPoint(set_y); //设置y方向的电机的理论值
        PID_M2_SetKp(60);
        PID_M2_SetKi(0.6);
        PID_M2_SetKd(20000);
        M1_PWM = PID_M1_PosLocCalc(pitch);//根据实际值，计算x轴pwm的方向和大小
        M2_PWM = PID_M2_PosLocCalc(roll);//根据实际值，计算y轴pwm的方向和大小
        if(my_abs(M1_PWM)> 3200)
        {
                if(M1_PWM>0)
                {
                        M1_PWM = 3200;
                }
                else
                {
                        M1_PWM = -3200;
                }
        }
        
        if (my_abs(M2_PWM)> 3200)
        {
                if(M2_PWM>0)
                {
                        M2_PWM = 3200;
                }
                else
                {
                        M2_PWM = -3200;
                }
        }
        Set_MotorX_PWM(M1_PWM);
        Set_MotorY_PWM(M2_PWM);
        
}


void Mode2(float pitch,float roll)
{
                        
        int32_t M1_PWM=0,M2_PWM=0;
        //printf("mode2__runing\n");
        if(my_abs(pitch) < 45 && my_abs(roll) < 45)//拉起一定角度，当风力摆的角度小于45度时，开始制动
        {
                
                PID_M1_SetPoint(0);
                PID_M1_SetKp(60);
                PID_M1_SetKi(0.6);
                PID_M1_SetKd(20000);
                
                PID_M2_SetPoint(0);
                PID_M2_SetKp(60); 
                PID_M2_SetKi(0.6);
                PID_M2_SetKd(20000);
                M1_PWM = PID_M1_PosLocCalc(pitch);
                M2_PWM = PID_M2_PosLocCalc(roll);
                if(my_abs(M1_PWM)> 3000)
                {
                        if(M1_PWM>0)
                        {
                                M1_PWM = 3000;
                        }
                        else
                        {
                                M1_PWM = -3600;
                        }
                }
                if (my_abs(M2_PWM)> 3000)
                {
                        if(M2_PWM>0)
                        {
                                M2_PWM = 3000;
                        }
                        else
                        {
                                M2_PWM = -3000;
                        }
                }
                Set_MotorX_PWM(M1_PWM);
                Set_MotorY_PWM(M2_PWM);
        }
        
        else
        {
                Set_motor1_Stop();
        }  
}



void Mode3(float pitch,float roll)
{
        static float Cycle_normalized=0;        //周期归一化变量
        static float Point_normalized=0;        //角度归一化变量
        static unsigned int Time_step=0;        //时间增量，用于计算归一化的值
        int32_t M1_PWM=0,M2_PWM=0;
        float set_x=0,set_y=0;
        Time_step += 10; //10ms中断
        Cycle_normalized = Time_step/SWING_T;   //对周期进行归一化处理
        Point_normalized = Cycle_normalized*2*3.14159;  //对2pi 进行归一化处理
        
        set_x = radius_of_circle*sin(Point_normalized + PI/2.0);//将理论值分解到电机X方向
        set_y = radius_of_circle*sin(Point_normalized);//将理论值分解到电机Y方向
         
        //printf("set_x = %0.2f,set_y = %0.2f",set_x,set_y);
        PID_M1_SetPoint(set_x);//设置x方向的电机的理论值
        PID_M1_SetKp(60);
        PID_M1_SetKi(0.6);
        PID_M1_SetKd(20000);
        
        PID_M2_SetPoint(set_y); //设置y方向的电机的理论值
        PID_M2_SetKp(60);
        PID_M2_SetKi(0.6);
        PID_M2_SetKd(20000);
        
        M1_PWM = PID_M1_PosLocCalc(pitch);//根据实际值，计算x轴pwm的方向和大小
        M2_PWM = PID_M2_PosLocCalc(roll);//根据实际值，计算y轴pwm的方向和大小
        
//        data_scope_wave(2,set_x,pitch);
        data_scope_wave(2,set_y,roll);
        
        if(my_abs(M1_PWM)> 3200)
        {
                if(M1_PWM>0)
                {
                        M1_PWM = 3200;
                }
                else
                {
                        M1_PWM = -3200;
                }
        }
        
        if (my_abs(M2_PWM)> 3200)
        {
                if(M2_PWM>0)
                {
                        M2_PWM = 3200;
                }
                else
                {
                        M2_PWM = -3200;
                }
        }
        Set_MotorX_PWM(M1_PWM);
        Set_MotorY_PWM(M2_PWM);
        
}
        

