#ifndef _USART_MPU_H_
#define _USART_MPU_H_
#include "stm32f10x.h"
typedef struct{
        float Acc_X;
        float Acc_Y;
        float Acc_Z;
}AccTypedef;

typedef struct{
        float Gyro_X;
        float Gyro_Y;
        float Gyro_Z;
}GyroTypedef;

typedef struct{
        float yaw;
        float pitch;
        float roll;
}AngleTypedef;

extern AccTypedef Acc;
extern GyroTypedef Gyro;
extern AngleTypedef Angle;
extern float  Temperature;
extern u8 updata_flag;
extern u8 Re_buf[11];
extern u8 Re_temp[11]; 

void get_up_all_data(void);
void get_angle(void);



#endif /* _USART_MPU_H_ */

