#include "bsp_mpu6050.h"
#include "bsp_i2c.h"
#include "bsp_usart.h"
#include <math.h>




extern char Flag_Show;
extern float Acceleration_Z;     //Z轴加速度计
extern float Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
extern int Temperature;     //显示温度
extern float angle, angle_dot; 


uint8_t buffer[14];


/**
  * @brief   写数据到MPU6050寄存器
  * @param   寄存器地址
  * @param   需要写入的数据
  * @return  null
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
        IIC_Start();
        IIC_Send_Byte(MPU6050_SLAVE_ADDRESS);
        IIC_Wait_Ack();
        IIC_Send_Byte(reg_add);
        IIC_Wait_Ack();
        IIC_Send_Byte(reg_dat);
        IIC_Wait_Ack();
        IIC_Stop();
}



/**
 * @brief   初始化MPU6050芯片
 * @param   null
 * @return  null
 */
void MPU6050_Init(void)
{
        int i=0,j=0;
        //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
        for(i=0;i<1000;i++)
        {
                for(j=0;j<1000;j++);
        }
        MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);       //解除休眠状态
        MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);      //陀螺仪采样率，1KHz
        MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);          //低通滤波器的设置，截止频率是1K，带宽是5K
        MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);    //配置加速度传感器工作在2G模式，不自检
        MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);      //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}

/**
  * @brief   从MPU6050寄存器读取数据
  * @param   寄存器地址
  * @param   存储数据指针
  * @param   读取的字节数
  * @return   null
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
        unsigned char i;

        IIC_Start();
        IIC_Send_Byte(MPU6050_SLAVE_ADDRESS);
        IIC_Wait_Ack();
        IIC_Send_Byte(reg_add);
        IIC_Wait_Ack();

        IIC_Start();
        IIC_Send_Byte(MPU6050_SLAVE_ADDRESS+1);
        IIC_Wait_Ack();

        for(i=0;i<(num-1);i++)
        {
                *Read=IIC_Read_Byte(1);
                Read++;
        }
        *Read=IIC_Read_Byte(0);
        IIC_Stop();
}

/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
        u8 buf[6];
        MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
        accData[0] = (buf[0] << 8) | buf[1];
        accData[1] = (buf[2] << 8) | buf[3];
        accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
        u8 buf[6];
        MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
        gyroData[0] = (buf[0] << 8) | buf[1];
        gyroData[1] = (buf[2] << 8) | buf[3];
        gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief 读取MPU6050内置温度传感器数据
  * @return 摄氏温度
  */
int Read_Temperature(void)
{ 
        float Temp;
        Temp=(I2C_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_TEMP_OUT_L);
        if(Temp>32768) Temp-=65536;
            Temp=(36.53+Temp/340)*10;
        return (int)Temp;
}




/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
float K1 =0.02; 
//float angle, angle_dot; 
float Q_angle=0.001;// 过程噪声的协方差
float Q_gyro=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle=0.5;// 测量噪声的协方差 既测量偏差
float dt=0.005;//                 
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

void Kalman_Filter(float Accel,float Gyro) 
{
        angle+=(Gyro - Q_bias) * dt;            //先验估计
        Pdot[0]=Q_angle - PP[0][1] - PP[1][0];  // Pk-先验估计误差协方差的微分

        Pdot[1]=-PP[1][1];
        Pdot[2]=-PP[1][1];
        Pdot[3]=Q_gyro;
        PP[0][0] += Pdot[0] * dt;                // Pk-先验估计误差协方差微分的积分
        PP[0][1] += Pdot[1] * dt;                // =先验估计误差协方差
        PP[1][0] += Pdot[2] * dt;
        PP[1][1] += Pdot[3] * dt;

        Angle_err = Accel - angle;      //zk-先验估计

        PCt_0 = C_0 * PP[0][0];
        PCt_1 = C_0 * PP[1][0];

        E = R_angle + C_0 * PCt_0;

        K_0 = PCt_0 / E;
        K_1 = PCt_1 / E;

        t_0 = PCt_0;
        t_1 = C_0 * PP[0][1];

        PP[0][0] -= K_0 * t_0;  //后验估计误差协方差
        PP[0][1] -= K_0 * t_1;
        PP[1][0] -= K_1 * t_0;
        PP[1][1] -= K_1 * t_1;

        angle   += K_0 * Angle_err;     //后验估计
        Q_bias  += K_1 * Angle_err;     //后验估计
        angle_dot   = Gyro - Q_bias;    //输出值(后验估计)的微分=角速度
}


