<<<<<<< HEAD
#include "bsp_i2c.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/**
 * @brief 控制 I2C 的速度
 * @param I2Cx 指定操作的 I2C 对象
 */
void I2C_Delay(I2C_GpioDef* I2Cx)
{
        delay_us(I2Cx->Speed);
}

/**
 * @brief I2C初始化函数
 * @param I2Cx 指定操作的I2C对象
 * @note 设置 SCL 和 SDA 的 GPIO 模式
 */
void i2c_init(I2C_GpioDef* I2Cx)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        /* 计算 SCLGpioClk 和 SDAGpioClk */
        I2Cx->SCLGpioClk = RCC_APB2Periph_GPIOA << (((u32)(I2Cx->SCLGpioPort) - (u32) GPIOA_BASE) >> 10);
        I2Cx->SDAGpioClk = RCC_APB2Periph_GPIOA << (((u32)(I2Cx->SDAGpioPort) - (u32) GPIOA_BASE) >> 10);
        
        /* 开启 SCL 和 SDA 的时钟 */
        RCC_APB2PeriphClockCmd(I2Cx->SCLGpioClk, ENABLE);
        RCC_APB2PeriphClockCmd(I2Cx->SDAGpioClk, ENABLE);
        
        /* 配置 SCL 和 SDA 初始为输出模式 */
        GPIO_InitStructure.GPIO_Pin = I2Cx->SCLGpioPin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(I2Cx->SCLGpioPort, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = I2Cx->SDAGpioPin;
        GPIO_Init(I2Cx->SDAGpioPort, &GPIO_InitStructure);
        
        /* 接收数据时, SDA 为输入, 有特定的函数去改变 SDA 引脚的 GPIO 模式 */
        /* STM32 作为 IIC 设备(从机), SCL 也是输入, 但是我们本实验没有用到, 因此一直为输出 */

        I2Cx->SCLPinNum = i2c_get_pin_num(I2Cx->SCLGpioPin);
        I2Cx->SDAPinNum = i2c_get_pin_num(I2Cx->SDAGpioPin);
        
        I2Cx->SCL = (volatile unsigned long*) BITBAND((u32)(I2Cx->SCLGpioPort) + 12, I2Cx->SCLPinNum);
        I2Cx->SDA = (volatile unsigned long*) BITBAND((u32)(I2Cx->SDAGpioPort) + 12, I2Cx->SDAPinNum);
        I2Cx->REC = (volatile unsigned long*) BITBAND((u32)(I2Cx->SDAGpioPort) + 8, I2Cx->SDAPinNum);
        
        I2Cx->Speed = 2;
        
        /* 数据线和时钟线默认都是高电平 */
        *(I2Cx->SCL) = 1;
        *(I2Cx->SDA) = 1;
}

/**
 * @brief 设置 SDA 对应 GPIO 为输入模式
 * @param I2Cx  指定操作的 I2C 对象
 */
void i2c_set_sda_input(I2C_GpioDef* I2Cx)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = I2Cx->SDAGpioPin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(I2Cx->SDAGpioPort,&GPIO_InitStructure);
}

/**
 * @brief 设置 SDA 对应 GPIO 为输出模式
 * @param I2Cx 指定操作的 I2C 对象
 */
void i2c_set_sda_output(I2C_GpioDef* I2Cx)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = I2Cx->SDAGpioPin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(I2Cx->SDAGpioPort,&GPIO_InitStructure);
}

/**
 * @brief 产生 I2C 起始信号
 * @param I2Cx 指定操作的 I2C 对象
 * @note 在 SCL 为高电平的情况下, SDA 产生一个下降沿表示起始
 */
void i2c_start(I2C_GpioDef* I2Cx)
{
        i2c_set_sda_output(I2Cx);
        
        /* 这里需要注意顺序, SDA 先拉高, SCL 再拉高 */
        /* 因为 SCL 为高电平的情况下, SDA 的边沿动作是有意义的, 因此 SDA 要先拉高 */
        *(I2Cx->SDA) = 1;
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        
        /* 在 SCL 为高电平的情况下, SDA 产生一个下降沿表示起始*/
        *(I2Cx->SDA) = 0;
        I2C_Delay(I2Cx);
        
        /* 这里其实就是开始驱动传输的时钟了 */
        *(I2Cx->SCL) = 0;
}

/**
 * @brief 产生 I2C 停止信号
 * @param I2Cx 指定操作的 I2C 对象
 * @note 在 SCL 为高电平的情况下, SDA 产生一个上升沿表示停止
 */
void i2c_stop(I2C_GpioDef* I2Cx)
{
        i2c_set_sda_output(I2Cx);

        /* 这里需要注意顺序, SCL 先拉低, SDA 再拉低 */
        /* 因为 SCL 为高电平的情况下, SDA 的边沿动作是有意义的, 因此 SCL 要先拉低 */
        *(I2Cx->SCL) = 0;
        *(I2Cx->SDA) = 0;
        I2C_Delay(I2Cx);

        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);

        *(I2Cx->SDA) = 1;
        I2C_Delay(I2Cx);
}

/**
 * @brief 等待应答信号到来
 * @param I2Cx 指定操作的 I2C 对象
 * @return u8 0,接收应答成功; 1,接收应答失败;
 * @note 主机发送完一帧数据之后, 需要等待从机给出响应才会继续发出下一帧数据
 * @note 这时候主机需要放出 SDA 的使用权, 由从机负责拉低 SDA (响应)
 * @note 主机在下一个 SCL 的上升沿(或说高电平)检测 SDA 是否为低电平, 低电平则表示有应答
 */
int i2c_wait_response(I2C_GpioDef* I2Cx)
{
        /* 定义一个变量, 作为超时的标记 */
        int ucErrTime = 0;
        
        /* 先默认的把 SDA 设置为高电平 */
        *(I2Cx->SDA) = 1;
        I2C_Delay(I2Cx);
        
        /* 这里需要接收从机的应答信号, 因此需要设置为输入 */
        i2c_set_sda_input(I2Cx);
        
        /* 下一个 SCL 时钟到了 */
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        
        /* 是时候去读取 SDA 看看有没有响应了 */
        /* 在没有超时之前只要读到了应答就可以自动跳出 while */
        while( *(I2Cx->REC))
        {
                ucErrTime++;
                
                printf("---------------------ucErrTime = %d\r\n", ucErrTime);

                /* 超时了, 既然从机在规定的时间不应答*/
                if(ucErrTime > 255)
                {
                        /* 主机就认为从机没有正确的接收, 就此作罢 */
                        i2c_stop(I2Cx);

                        /* 函数返回接收应答失败 */
                        return -1;
                }
        }
        
//        printf("ucErrTime = %d\r\n", ucErrTime);

        /* 到这里, 读取应答信号的 SCL 结束了 */
        *(I2Cx->SCL) = 0;

        /* 能执行到这里, 说明读取到了应答的 */
        return 0;
}

/**
 * @brief HOST 产生应答
 * @param I2Cx 指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_response(I2C_GpioDef* I2Cx)
{
        *(I2Cx->SCL) = 0;

        /* 要应答, 肯定要先获取SDA的使用权 */
        i2c_set_sda_output(I2Cx);

        /* 在从机发送完本帧最后一个位的低电平期间, 把 SDA 拉低 */
        /* 千万不能在 SCL 高电平期间拉低, 那就变成起始信号了 */
        *(I2Cx->SDA) = 0;
        I2C_Delay(I2Cx);

        /* 这里就是从机读取应答信号的一个 SCL 时钟周期了, SDA 不能动 */
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        *(I2Cx->SCL) = 0;
}

/**
 * @brief HOST 不产生应答
 * @param I2Cx  指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_no_response(I2C_GpioDef* I2Cx)
{
        *(I2Cx->SCL) = 0;
        i2c_set_sda_output(I2Cx);

        /* 既然拉低表示应答, 那我拉高不就行了 */
        *(I2Cx->SDA) = 1;
        I2C_Delay(I2Cx);

        /* 下一个时钟一检测, 就发现, 没有应答 */
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        *(I2Cx->SCL) = 0;
}

/**
 * @brief I2C 发送一个字节
 * @param I2Cx 指定操作的 I2C 对象
 * @param data 发送的数据
 */
void i2c_send_byte(I2C_GpioDef* I2Cx, u8 data)
{
        u8 t;
        i2c_set_sda_output(I2Cx);

        /* 所有的数据的输出到 SDA 线上都是在 SCL 的低电平期间 */
        *(I2Cx->SCL) = 0;

        /* 依次发送8个数据值 */
        for(t = 0; t < 8; t++)
        {
                /* 写入数据的最高位 */
                *(I2Cx->SDA) = (data & 0x80) >> 7;

                /* 发送完了最高位, 数据左移一个, 次高位变成了新的最高位 */
                data <<= 1;
                I2C_Delay(I2Cx);

                /* 在 SCL 的上升沿(或者高电平期间), 数据被从机接收读取 */
                *(I2Cx->SCL) = 1;
                I2C_Delay(I2Cx);
                *(I2Cx->SCL) = 0;
                I2C_Delay(I2Cx);
        }
        /* 这函数结束的时候是不是 SCL = 0 */
        /* 一般这里接下来就会是 WaitResponse 操作了 */
        /* WaitResponse 的时候就是直接从 SCL = 0 开始的, 这样就不会多出来一个 SCL 的脉冲 */
}

/**
 * @brief I2C 读一个字节
 * @param I2Cx 指定操作的 I2C 对象
 * @note 根据参数决定要不要应答, 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
u8 i2c_receive_byte(I2C_GpioDef* I2Cx)
{
        u8 i, data = 0;
        i2c_set_sda_input(I2Cx);

        for(i = 0; i < 8; i++)
        {
                *(I2Cx->SCL) = 0;
                I2C_Delay(I2Cx);
                *(I2Cx->SCL) = 1;
                data <<= 1;
                if( *(I2Cx->REC))
                        data++;
                I2C_Delay(I2Cx);
        }

        return data;
}

/**
 * @brief 由GPIO_Pin_x 获取到引脚编号
 * @param GPIO_Pin_x 需要解析的 GPIO_Pin
 * @return u8 对应的引脚编号
 */
u8 i2c_get_pin_num(u16 GPIO_Pin_x)
{
        int num = 0;
        
        while(!(GPIO_Pin_x & 0x1))
        {
                GPIO_Pin_x = GPIO_Pin_x >> 1;
                num++;
        }
        
        return num;
}
=======
#include "bsp_i2c.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/**
 * @brief 控制 I2C 的速度
 * @param I2Cx 指定操作的 I2C 对象
 */
void I2C_Delay(I2C_GpioDef* I2Cx)
{
        delay_us(I2Cx->Speed);
}

/**
 * @brief I2C初始化函数
 * @param I2Cx 指定操作的I2C对象
 * @note 设置 SCL 和 SDA 的 GPIO 模式
 */
void i2c_init(I2C_GpioDef* I2Cx)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        /* 计算 SCLGpioClk 和 SDAGpioClk */
        I2Cx->SCLGpioClk = RCC_APB2Periph_GPIOA << (((u32)(I2Cx->SCLGpioPort) - (u32) GPIOA_BASE) >> 10);
        I2Cx->SDAGpioClk = RCC_APB2Periph_GPIOA << (((u32)(I2Cx->SDAGpioPort) - (u32) GPIOA_BASE) >> 10);
        
        /* 开启 SCL 和 SDA 的时钟 */
        RCC_APB2PeriphClockCmd(I2Cx->SCLGpioClk, ENABLE);
        RCC_APB2PeriphClockCmd(I2Cx->SDAGpioClk, ENABLE);
        
        /* 配置 SCL 和 SDA 初始为输出模式 */
        GPIO_InitStructure.GPIO_Pin = I2Cx->SCLGpioPin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(I2Cx->SCLGpioPort, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = I2Cx->SDAGpioPin;
        GPIO_Init(I2Cx->SDAGpioPort, &GPIO_InitStructure);
        
        /* 接收数据时, SDA 为输入, 有特定的函数去改变 SDA 引脚的 GPIO 模式 */
        /* STM32 作为 IIC 设备(从机), SCL 也是输入, 但是我们本实验没有用到, 因此一直为输出 */

        I2Cx->SCLPinNum = i2c_get_pin_num(I2Cx->SCLGpioPin);
        I2Cx->SDAPinNum = i2c_get_pin_num(I2Cx->SDAGpioPin);
        
        I2Cx->SCL = (volatile unsigned long*) BITBAND((u32)(I2Cx->SCLGpioPort) + 12, I2Cx->SCLPinNum);
        I2Cx->SDA = (volatile unsigned long*) BITBAND((u32)(I2Cx->SDAGpioPort) + 12, I2Cx->SDAPinNum);
        I2Cx->REC = (volatile unsigned long*) BITBAND((u32)(I2Cx->SDAGpioPort) + 8, I2Cx->SDAPinNum);
        
        I2Cx->Speed = 2;
        
        /* 数据线和时钟线默认都是高电平 */
        *(I2Cx->SCL) = 1;
        *(I2Cx->SDA) = 1;
}

/**
 * @brief 设置 SDA 对应 GPIO 为输入模式
 * @param I2Cx  指定操作的 I2C 对象
 */
void i2c_set_sda_input(I2C_GpioDef* I2Cx)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = I2Cx->SDAGpioPin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(I2Cx->SDAGpioPort,&GPIO_InitStructure);
}

/**
 * @brief 设置 SDA 对应 GPIO 为输出模式
 * @param I2Cx 指定操作的 I2C 对象
 */
void i2c_set_sda_output(I2C_GpioDef* I2Cx)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = I2Cx->SDAGpioPin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(I2Cx->SDAGpioPort,&GPIO_InitStructure);
}

/**
 * @brief 产生 I2C 起始信号
 * @param I2Cx 指定操作的 I2C 对象
 * @note 在 SCL 为高电平的情况下, SDA 产生一个下降沿表示起始
 */
void i2c_start(I2C_GpioDef* I2Cx)
{
        i2c_set_sda_output(I2Cx);
        
        /* 这里需要注意顺序, SDA 先拉高, SCL 再拉高 */
        /* 因为 SCL 为高电平的情况下, SDA 的边沿动作是有意义的, 因此 SDA 要先拉高 */
        *(I2Cx->SDA) = 1;
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        
        /* 在 SCL 为高电平的情况下, SDA 产生一个下降沿表示起始*/
        *(I2Cx->SDA) = 0;
        I2C_Delay(I2Cx);
        
        /* 这里其实就是开始驱动传输的时钟了 */
        *(I2Cx->SCL) = 0;
}

/**
 * @brief 产生 I2C 停止信号
 * @param I2Cx 指定操作的 I2C 对象
 * @note 在 SCL 为高电平的情况下, SDA 产生一个上升沿表示停止
 */
void i2c_stop(I2C_GpioDef* I2Cx)
{
        i2c_set_sda_output(I2Cx);

        /* 这里需要注意顺序, SCL 先拉低, SDA 再拉低 */
        /* 因为 SCL 为高电平的情况下, SDA 的边沿动作是有意义的, 因此 SCL 要先拉低 */
        *(I2Cx->SCL) = 0;
        *(I2Cx->SDA) = 0;
        I2C_Delay(I2Cx);

        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);

        *(I2Cx->SDA) = 1;
        I2C_Delay(I2Cx);
}

/**
 * @brief 等待应答信号到来
 * @param I2Cx 指定操作的 I2C 对象
 * @return u8 0,接收应答成功; 1,接收应答失败;
 * @note 主机发送完一帧数据之后, 需要等待从机给出响应才会继续发出下一帧数据
 * @note 这时候主机需要放出 SDA 的使用权, 由从机负责拉低 SDA (响应)
 * @note 主机在下一个 SCL 的上升沿(或说高电平)检测 SDA 是否为低电平, 低电平则表示有应答
 */
int i2c_wait_response(I2C_GpioDef* I2Cx)
{
        /* 定义一个变量, 作为超时的标记 */
        int ucErrTime = 0;
        
        /* 先默认的把 SDA 设置为高电平 */
        *(I2Cx->SDA) = 1;
        I2C_Delay(I2Cx);
        
        /* 这里需要接收从机的应答信号, 因此需要设置为输入 */
        i2c_set_sda_input(I2Cx);
        
        /* 下一个 SCL 时钟到了 */
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        
        /* 是时候去读取 SDA 看看有没有响应了 */
        /* 在没有超时之前只要读到了应答就可以自动跳出 while */
        while( *(I2Cx->REC))
        {
                ucErrTime++;
                
                printf("---------------------ucErrTime = %d\r\n", ucErrTime);

                /* 超时了, 既然从机在规定的时间不应答*/
                if(ucErrTime > 255)
                {
                        /* 主机就认为从机没有正确的接收, 就此作罢 */
                        i2c_stop(I2Cx);

                        /* 函数返回接收应答失败 */
                        return -1;
                }
        }
        
//        printf("ucErrTime = %d\r\n", ucErrTime);

        /* 到这里, 读取应答信号的 SCL 结束了 */
        *(I2Cx->SCL) = 0;

        /* 能执行到这里, 说明读取到了应答的 */
        return 0;
}

/**
 * @brief HOST 产生应答
 * @param I2Cx 指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_response(I2C_GpioDef* I2Cx)
{
        *(I2Cx->SCL) = 0;

        /* 要应答, 肯定要先获取SDA的使用权 */
        i2c_set_sda_output(I2Cx);

        /* 在从机发送完本帧最后一个位的低电平期间, 把 SDA 拉低 */
        /* 千万不能在 SCL 高电平期间拉低, 那就变成起始信号了 */
        *(I2Cx->SDA) = 0;
        I2C_Delay(I2Cx);

        /* 这里就是从机读取应答信号的一个 SCL 时钟周期了, SDA 不能动 */
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        *(I2Cx->SCL) = 0;
}

/**
 * @brief HOST 不产生应答
 * @param I2Cx  指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_no_response(I2C_GpioDef* I2Cx)
{
        *(I2Cx->SCL) = 0;
        i2c_set_sda_output(I2Cx);

        /* 既然拉低表示应答, 那我拉高不就行了 */
        *(I2Cx->SDA) = 1;
        I2C_Delay(I2Cx);

        /* 下一个时钟一检测, 就发现, 没有应答 */
        *(I2Cx->SCL) = 1;
        I2C_Delay(I2Cx);
        *(I2Cx->SCL) = 0;
}

/**
 * @brief I2C 发送一个字节
 * @param I2Cx 指定操作的 I2C 对象
 * @param data 发送的数据
 */
void i2c_send_byte(I2C_GpioDef* I2Cx, u8 data)
{
        u8 t;
        i2c_set_sda_output(I2Cx);

        /* 所有的数据的输出到 SDA 线上都是在 SCL 的低电平期间 */
        *(I2Cx->SCL) = 0;

        /* 依次发送8个数据值 */
        for(t = 0; t < 8; t++)
        {
                /* 写入数据的最高位 */
                *(I2Cx->SDA) = (data& 0x80) >> 7;

                /* 发送完了最高位, 数据左移一个, 次高位变成了新的最高位 */
                data <<= 1;
                I2C_Delay(I2Cx);

                /* 在 SCL 的上升沿(或者高电平期间), 数据被从机接收读取 */
                *(I2Cx->SCL) = 1;
                I2C_Delay(I2Cx);
                *(I2Cx->SCL) = 0;
                I2C_Delay(I2Cx);
        }
        /* 这函数结束的时候是不是 SCL = 0 */
        /* 一般这里接下来就会是 WaitResponse 操作了 */
        /* WaitResponse 的时候就是直接从 SCL = 0 开始的, 这样就不会多出来一个 SCL 的脉冲 */
}

/**
 * @brief I2C 读一个字节
 * @param I2Cx 指定操作的 I2C 对象
 * @note 根据参数决定要不要应答, 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
u8 i2c_receive_byte(I2C_GpioDef* I2Cx)
{
        u8 i, data = 0;
        i2c_set_sda_input(I2Cx);

        for(i = 0; i < 8; i++)
        {
                *(I2Cx->SCL) = 0;
                I2C_Delay(I2Cx);
                *(I2Cx->SCL) = 1;
                data <<= 1;
                if( *(I2Cx->REC))
                        data++;
                I2C_Delay(I2Cx);
        }

        return data;
}

/**
 * @brief 由GPIO_Pin_x 获取到引脚编号
 * @param GPIO_Pin_x 需要解析的 GPIO_Pin
 * @return u8 对应的引脚编号
 */
u8 i2c_get_pin_num(u16 GPIO_Pin_x)
{
        int num = 0;
        
        while(!(GPIO_Pin_x & 0x1))
        {
                GPIO_Pin_x = GPIO_Pin_x >> 1;
                num++;
        }
        
        return num;
}
>>>>>>> updata
