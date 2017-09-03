#include "bsp_i2c.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
 
/**
 * @brief 初始化I2C对应的接口引脚
 * @return null
 */
void IIC_Init(void)
{
        RCC->APB2ENR|=1<<3;         //先使能外设IO PORTB时钟 
        GPIOB->CRH&=0XFFFFFF00;     //PB8/9 推挽输出 
        GPIOB->CRH|=0X00000033; 
}

/**
 * @brief 产生IIC起始信号
 * @return 1 成功 0 失败
 */

int IIC_Start(void)
{
        SDA_OUT();     //sda线输出
        IIC_SDA=1;
//        if(!READ_SDA)return 0; //IDR 为 1
        IIC_SCL=1;
        delay_us(1);
        IIC_SDA=0;    //开始信号：时钟线保持高电平不变，数据线有下降沿 
//        if(READ_SDA)return 0; //IDR 为 0
        delay_us(1);
        IIC_SCL=0;    //钳住I2C总线，准备发送或接收数据 
        return 1;
}

/**
 * @brief 产生IIC停止信号
 */
void IIC_Stop(void)
{
        SDA_OUT();      //sda线输出
        IIC_SCL=0;
        IIC_SDA=0;      //停止信号：时钟线高电平不变，数据线有上升沿
        delay_us(1);
        IIC_SCL=1; 
        IIC_SDA=1;      //发送I2C总线结束信号
        delay_us(1); 
}

/**
 * @brief 等待应答信号到来
 * @return 0 接收应答失败 1 接收应答成功
 */
int IIC_Wait_Ack(void)
{
        u8 ucErrTime=0;
        SDA_IN();      //SDA设置为输入  
        IIC_SDA=1;
        delay_us(1); 
        IIC_SCL=1;
        delay_us(1); 
        while(READ_SDA)
        {
                ucErrTime++;
                if(ucErrTime>50)
                {
                        IIC_Stop();
                        return 0;
                }
                delay_us(1);
        }
        IIC_SCL=0;//时钟输出0  
        return 1;
} 

/**
 * @brief 产生应答信号
 */
void IIC_Ack(void)
{
        IIC_SCL=0;
        SDA_OUT();
        IIC_SDA=0;
        delay_us(1);
        IIC_SCL=1;
        delay_us(1);
        IIC_SCL=0;
}

/**
 * @brief 产生 NACK 应答
 */
void IIC_NAck(void)
{
        IIC_SCL=0;
        SDA_OUT();
        IIC_SDA=1;
        delay_us(1);
        IIC_SCL=1;
        delay_us(1);
        IIC_SCL=0;
}

/**
 * @brief 发送一个字节
 * @param 发送的字节
 */
void IIC_Send_Byte(u8 txd)
{                        
        u8 t;   
        SDA_OUT(); 
        IIC_SCL=0;              //拉低时钟开始数据传输
        for(t=0;t<8;t++)
        {              
                IIC_SDA=(txd&0x80)>>7;
                txd<<=1; 
                delay_us(1);   
                IIC_SCL=1;
                delay_us(1); 
                IIC_SCL=0; 
                delay_us(1);
        } 
} 
/**
 * @beief  i2c 写函数
 * @param addr 设备地址
 * @param reg 寄存器地址
 * @param len 要写入的长度
 * @param data 要写入的数据
 * @return 1 失败 0 成功
 */

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
        int i;
        if (!IIC_Start())
        return 1;
        IIC_Send_Byte(addr << 1 );
        if (!IIC_Wait_Ack()) 
        {
                IIC_Stop();
                return 1;
        }
        IIC_Send_Byte(reg);
        IIC_Wait_Ack();
        for (i = 0; i < len; i++)
        {
                IIC_Send_Byte(data[i]);
                if (!IIC_Wait_Ack())
                {
                        IIC_Stop();
                        return 0;
                }
        }
        IIC_Stop();
        return 0;
}

/**
 * @brief i2c 读取函数
 * @param addr 设备地址
 * @param reg 寄存器地址
 * @param len 读取的长度
 * @param buf 暂存数组
 * @return 0 成功 1 失败
 */
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
        if (!IIC_Start())
        return 1;
        IIC_Send_Byte(addr << 1);
        if (!IIC_Wait_Ack())
        {
                IIC_Stop();
                return 1;
        }
        IIC_Send_Byte(reg);
        IIC_Wait_Ack();
        IIC_Start();
        IIC_Send_Byte((addr << 1)+1);
        IIC_Wait_Ack();
        while (len)
        {
                if (len == 1)
                *buf = IIC_Read_Byte(0);
                else
                *buf = IIC_Read_Byte(1);
                buf++;
                len--;
        }
        IIC_Stop();
        return 0;
}


/**
 * @brief 读1个字节
 * @param 1 发送 ASK 0 发送 NASK
 * @return 读取的字节
 */
u8 IIC_Read_Byte(unsigned char ack)
{
        unsigned char i,receive=0;
        SDA_IN();//SDA设置为输入
        for(i=0;i<8;i++ )
        {
                IIC_SCL=0; 
                delay_us(2);
                IIC_SCL=1;
                receive<<=1;
                if(READ_SDA)receive++;   
                delay_us(2); 
        } 
        if (ack)
        IIC_Ack(); //发送ACK 
        else
        IIC_NAck();//发送nACK  
        return receive;
}

/**
 * @brife 读取指定设备 指定寄存器的一个值
 * @param I2C_Addr i2c 的设备地址
 * @param addr 设备的存储地址  寄存器地址
 * @return 读取到的值
 */ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
        unsigned char res=0;

        IIC_Start();    
        IIC_Send_Byte(I2C_Addr);        //发送写命令
        res++;
        IIC_Wait_Ack();
        IIC_Send_Byte(addr); res++;  //发送地址
        IIC_Wait_Ack(); 
        //IIC_Stop();//产生一个停止条件 
        IIC_Start();
        IIC_Send_Byte(I2C_Addr+1); res++;    //进入接收模式  
        IIC_Wait_Ack();
        res=IIC_Read_Byte(0);  
        IIC_Stop();//产生一个停止条件

        return res;
}


/**
 * @brief 读取指定设备 指定寄存器的 length个值
 * @param dev 目标设备地址
 * @param reg 寄存器地址
 * @param length 要读的字节数
 * @param *data  读出的数据将要存放的指针
 * @return 读出来的字节数量
 */
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
        u8 count = 0;

        IIC_Start();
        IIC_Send_Byte(dev);     //发送写命令
        IIC_Wait_Ack();
        IIC_Send_Byte(reg);   //发送地址
        IIC_Wait_Ack();   
        IIC_Start();
        IIC_Send_Byte(dev+1);  //进入接收模式 
        IIC_Wait_Ack();

        for(count=0;count<length;count++)
        {

                if(count!=length-1)data[count]=IIC_Read_Byte(1);  //带ACK的读数据
                else  data[count]=IIC_Read_Byte(0);             //最后一个字节NACK
        }
        IIC_Stop();             //产生一个停止条件
        return count;
}

/**
 * @brief 将多个字节写入指定设备 指定寄存器
 * @param dev  目标设备地址
 * @param reg 寄存器地址
 * @param length 要写的字节数
 * @param *data  将要写的数据的首地址
 * @return 1 成功 0 失败
 */

u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
{

        u8 count = 0;
        IIC_Start();
        IIC_Send_Byte(dev);     //发送写命令
        IIC_Wait_Ack();
        IIC_Send_Byte(reg);   //发送地址
        IIC_Wait_Ack(); 
        for(count=0;count<length;count++)
        {
                IIC_Send_Byte(data[count]); 
                IIC_Wait_Ack(); 
        }
        IIC_Stop();     //产生一个停止条件

        return 1;       //status == 0;
}

/**
 * @brief 读取指定设备 指定寄存器的一个值
 * @param dev  目标设备地址
 * @param reg 寄存器地址
 * @param *data  读出的数据将要存放的地址
 * @return 1 成功
 */
u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
        *data=I2C_ReadOneByte(dev, reg);
        return 1;
}

/**
 * @brief 写入指定设备 指定寄存器一个字节
 * @param dev  目标设备地址
 * @param reg 寄存器地址
 * @param data  将要写入的字节
 * @return 读取到的值
 */
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
        return IICwriteBytes(dev, reg, 1, &data);
}

/*
 * @brief 读 修改 写 指定设备 指定寄存器一个字节 中的多个位
 * @param dev  目标设备地址
 * @param reg 寄存器地址
 * @param data  将要写入的字节
 * @param bitStart  目标字节的起始位
 * @param length   位长度
 * @return 1 成功 0 失败
 */
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

        u8 b;
        if (IICreadByte(dev, reg, &b) != 0) 
        {
                u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
                data <<= (8 - length);
                data >>= (7 - bitStart);
                b &= mask;
                b |= data;
                return IICwriteByte(dev, reg, b);
        } 
        else 
        return 0;
        
}

/**
 * @brief 读 修改 写 指定设备 指定寄存器一个字节 中的1个位
 * @param dev  目标设备地址
 * @param reg 寄存器地址
 * @param bitNum  要修改目标字节的 bitNum 位
 * @param data  将要写入的字节
 * @return 1 成功 0 失败
 */
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
        u8 b;
        IICreadByte(dev, reg, &b);
        b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
        return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
