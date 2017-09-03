#include "bsp_mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"
#include <math.h>
#include "DataScope_DP.h"

#define   PI        3.14159265

float Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z,X_Accel,Y_Accel,Z_Accel;
extern float angle;
float X_angle,Y_angle,Z_angle;

void test_mpu6050(void)
{
        static u8 re1,re2;
        
        MPU6050_ReadData( 0x3B ,&re1 , 1);
        MPU6050_ReadData( 0x3C ,&re2 , 1);
        Accel_X = (re1<<8)|re2;
        
        MPU6050_ReadData( 0x3D ,&re1 , 1);
        MPU6050_ReadData( 0x3E ,&re2 , 1);
        Accel_Y = (re1<<8)|re2;
        
        MPU6050_ReadData( 0x3F ,&re1 , 1);
        MPU6050_ReadData( 0x40 ,&re2 , 1);
        Accel_Z = (re1<<8)|re2;
        
        MPU6050_ReadData( 0x43 ,&re1 , 1);
        MPU6050_ReadData( 0x44 ,&re2 , 1);
        Gyro_X = (re1<<8)|re2;
        
        MPU6050_ReadData( 0x45 ,&re1 , 1);
        MPU6050_ReadData( 0x46 ,&re2 , 1);
        Gyro_Y = (re1<<8)|re2;
        
        MPU6050_ReadData( 0x47 ,&re1 , 1);
        MPU6050_ReadData( 0x48 ,&re2 , 1);
        Gyro_Z = (re1<<8)|re2;
        
        
        
        if(Gyro_X>32768)  Gyro_X-=65536;
        if(Gyro_Y>32768)  Gyro_Y-=65536;
        if(Gyro_Z>32768)  Gyro_Z-=65536;
        if(Accel_X>32768) Accel_X-=65536;
        if(Accel_Y>32768) Accel_Y-=65536; 
        if(Accel_Z>32768) Accel_Z-=65536;

        X_Accel=atan2(Accel_Y,Accel_Z)*180/PI; //实时角度绕x轴方向
        Y_Accel=atan2(Accel_X,Accel_Z)*180/PI;
        Z_Accel=atan2(Accel_X,Accel_Y)*180/PI;
        
        
        Gyro_X = Gyro_X/16.4;           //量程换算
        Gyro_Y = Gyro_Y/16.4;           //量程换算
        Gyro_Z = Gyro_Z/16.4;           //量程换算
//        angle = 0.02*X_Accel + 0.98*(angle + Gyro_X * 0.005);     //一阶互补滤波
        Kalman_Filter(X_Accel,Gyro_X);
        X_angle=angle;
        Kalman_Filter(Y_Accel,Gyro_Y);
        Y_angle=angle;
        Kalman_Filter(Z_Accel,Gyro_Z);
        Z_angle=angle;
        printf("实时角度:X:%0.2f  Y:%0.2f  Z:%0.2f\r\n",X_Accel,Y_Accel,Z_Accel);
        printf("滤波角度:X:%0.2f  Y:%0.2f  Z:%0.2f\r\n\r\n",X_angle,Y_angle,Z_angle);
        
        
        
        
//        data_scope_wave(1,X_Accel);    //原始
//        data_scope_wave(2,Y_Accel);
//        data_scope_wave(3,Z_Accel);
//        data_scope_wave(4,X_angle);
//        data_scope_wave(5,Y_angle);
//        data_scope_wave(6,Z_angle);         //一阶滤波
        delay_ms(500);
        
}





