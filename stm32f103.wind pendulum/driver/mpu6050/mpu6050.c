/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   MPU6050驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */


#include "mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"


/**
  * @brief   写数据到MPU6050寄存器
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	i2c_WaitAck();
	i2c_Stop();
}


/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}


/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
        int i=0,j=0;
        //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
        for(i=0;i<1000;i++)
        {
                for(j=0;j<1000;j++);
               
        }
        MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
        MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
        MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	    //低通滤波器的设置，截止频率是1K，带宽是5K
        MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);   //配置加速度传感器工作在2G模式，不自检
        MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围0x00表示（-250-+250度/秒），典型值：0x18(不自检，2000deg/s)
}



/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
        MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
        printf("%d\n",Re);
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
                printf("%d",Re);
                
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
                printf("%d",Re);
                
		return 1;
	}
		
}
/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(MPU6050_AccTypeDef *AccTypeDef)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    AccTypeDef->AccX = (buf[0] << 8) | buf[1];
    AccTypeDef->AccY = (buf[2] << 8) | buf[3];
    AccTypeDef->AccZ = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(MPU6050_GyroTypeDef *GyroTypeDef)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    GyroTypeDef->GyroX = (float)((buf[0] << 8) | buf[1]);
    GyroTypeDef->GyroY = (float)((buf[2] << 8) | buf[3]);
    GyroTypeDef->GyroZ = (float)((buf[4] << 8) | buf[5]);
}


/**
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
        u8 buf[2];
        MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
        *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
        temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53; 

}
/**
  * @brief   读取mpu6050的寄存器中X轴加速度的原始值
  * @param   
  * @return  返回值一个16位的数值  
  */
u16 Get_MPU_Reg_Acc_X(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_ACC_XOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   读取mpu6050的寄存器中Y轴加速度的原始值
  * @param   
  * @return  返回值一个16位的数值  
  */
u16 Get_MPU_Reg_Acc_Y(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_ACC_YOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   读取mpu6050的寄存器中Z轴加速度的原始值
  * @param   
  * @return  返回值一个16位的数值  
  */
u16 Get_MPU_Reg_Acc_Z(void)
{
        unsigned char temp[2]={0,0};
        short result=0;
        MPU6050_ReadData(MPU6050_ACC_ZOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   读取mpu6050的寄存器中X轴角速度的原始值
  * @param   
  * @return  返回值一个16位的数值  
  */
u16 Get_MPU_Reg_Gyro_X(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_GYRO_XOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   读取mpu6050的寄存器中Y轴角速度的原始值
  * @param   
  * @return  返回值一个无符号位的数值  
  */
u16 Get_MPU_Reg_Gyro_Y(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_GYRO_YOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}
/**
  * @brief   读取mpu6050的寄存器中Z轴角速度的原始值
  * @param   
  * @return  返回值一个无符号16位的数值  
  */
u16 Get_MPU_Reg_Gyro_Z(void)
{
        unsigned char temp[2]={0,0};
        u16 result=0;
        MPU6050_ReadData(MPU6050_GYRO_ZOUT_H,temp,2);
        result = temp[0];
        result = (result<<8)|temp[1];
        return result;
}




/**
  * @brief   读取mpu6050X轴方向的加速度
  * @param   
  * @return  返回一个浮点数 
  */
float Get_MPU_Acc_X(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Acc_X();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*39.2);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*78.4);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*156.8);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

/**
  * @brief   读取mpu6050Y轴方向的加速度
  * @param   
  * @return  返回一个浮点数 
  */
float Get_MPU_Acc_Y(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Acc_Y();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*39.2);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*78.4);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*156.8);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

/**
  * @brief   读取mpu6050Z轴方向的加速度
  * @param   
  * @return  返回一个浮点数 
  */
float Get_MPU_Acc_Z(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Acc_Z();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*39.2);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*78.4);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*156.8);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

float Get_MPU_Gyro_X(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Gyro_X();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*250.0);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*500.0);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*1000.0);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*2000.0);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}

float Get_MPU_Gyro_Y(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Gyro_Y();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*250.0);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*500.0);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*1000.0);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*2000.0);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}
float Get_MPU_Gyro_Z(void)
{
        short temp=0;
        float result=0;
        temp = (short)Get_MPU_Reg_Gyro_Z();
        switch(MPU6050_Set_RA_Acc)
        {
                case 0:
                        result =  (float)((temp/32768.0)*250.0);
                        break;
                case 1:
                        result =  (float)((temp/32768.0)*500.0);
                        break;
                case 2:
                        result =  (float)((temp/32768.0)*1000.0);
                        break;
                case 3:
                        result =  (float)((temp/32768.0)*2000.0);
                        break;
                default:
                        result =  (float)((temp/32768.0)*19.6);
                        break;
        }               
        return result;
}



