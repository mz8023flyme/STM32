#include "usart_mpu.h"
#include "stdio.h"
AccTypedef Acc = {0,0,0};
GyroTypedef Gyro= {0,0,0};
AngleTypedef Angle= {0,0,0};
float  Temperature=0;
u8 updata_flag = 0;
u8 Re_buf[11] = {0};
u8 Re_temp[11] = {0};
void get_up_all_data(void)
{
        if(updata_flag == 1)
        {
                switch(Re_buf[1])
                {
                        case 0x51: //标识这个包是加速度包
                           Acc.Acc_X = ((short)(Re_buf[3]<<8 | Re_buf[2]))/32768.0*16;      //X轴加速度
                           Acc.Acc_Y= ((short)(Re_buf[5]<<8 | Re_buf[4]))/32768.0*16;      //Y轴加速度
                           Acc.Acc_Z = ((short)(Re_buf[7]<<8 | Re_buf[6]))/32768.0*16;      //Z轴加速度
                           Temperature = ((short)(Re_buf[9]<<8 | Re_buf[8]))/340.0+36.25;      //温度
                           break;
                        
                        case 0x52: //标识这个包是角速度包
                           Gyro.Gyro_X = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*2000;      //X轴角速度
                           Gyro.Gyro_Y = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*2000;      //Y轴角速度
                           Gyro.Gyro_Z = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*2000;      //Z轴角速度
                           Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;      //温度
                           break;
                        
                        case 0x53: //标识这个包是角度包
                           Angle.roll = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*180;   //X轴滚转角（x 轴）
                           Angle.pitch = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
                           Angle.yaw = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*180;   //Z轴偏航角（z 轴）
                           Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;   //温度
                           //printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
                           break;
                        
                         default:  
                           break;
                }
        updata_flag  = 0;
        }
}

void get_angle(void)
{

        if(Re_buf[1] == 0x53)
        {                
                Angle.roll = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*180;   //X轴滚转角（x 轴）
                Angle.pitch = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
                Angle.yaw = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*180;   //Z轴偏航角（z 轴）
                Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;   //温度 
                printf("Angle.roll:%0.2f, Angle.pitch:%0.2f\n",Angle.roll,Angle.pitch);
        }
}
        


