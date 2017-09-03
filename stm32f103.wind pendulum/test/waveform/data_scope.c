#include "data_scope.h"
#include <stdarg.h>
#include <string.h>

/* 串口发送的数据帧的帧缓冲区 */
/* 此数据帧使用以 '$' 符号开头, 之后每 4 个字节分别为每一个通道的 float 数据值 */
/* buffer[1-4]: 通道 1 的数据, buffer[5-8] 通道 2 的数据, 以此类推 */
/* 数据帧以 buffer[i] = i 形式结尾, 即倘若当前 buffer 的值与其下标相等, 意味着数据帧结束 */
u8 data_scope_buffer[42] = {0};

/**
 * @brief 将单精度浮点数据转成 4 字节数据并存入 buffer 中指定的位置
 * @param target 传入的单精度浮点数, 上位机显示的就是它
 * @param buffer 将但精度浮点数转化后保存在此 buffer 中
 * @param begin  指定保存在 buffer 中的位置
 * @return null
 */
void data_scope_float_to_byte(float *target, u8 *buffer, u8 begin)
{
        /* 得到 float 数据的首地址 */
        u8 *point = (u8*) target;

        /* float 类型的数据在内存中本就是占据 4 个字节 */
        /*这里将 float 以单字节的形式以此存入 buffer */
        buffer[begin] = point[0];
        buffer[begin + 1] = point[1];
        buffer[begin + 2] = point[2];
        buffer[begin + 3] = point[3];
}

/**
 * @brief 将待发送通道的单精度浮点数据写入发送缓冲区
 * @param channel 指定此但精度浮点数对应的显示通道 (1 - 10)
 * @param data 传入的单精度浮点数, 上位机显示的就是它
 * @return null
 * @note data_scope 上位机最多支持 10 个通道
 */
void data_scope_write_buffer(u8 channel, float data)
{
        /* 将指定通道的数据写入 buffer 中对应位置 */
        /* 通道 i 的数据对应 buffer[4*i-3 ~ 4*i] */
        if ((channel >= 1) && (channel <= 10))
                data_scope_float_to_byte(&data, data_scope_buffer, 4 * channel - 3);
}

/**
 * @brief 生成 data_scope 上位机能正确识别的帧格式
 * @param channel 指定本次格式化帧中使用到了多少个通道 (1 - 10)
 * @return succed: 发送缓冲区数据个数 fail: -1
 */
int data_scope_ready_to_send(u8 channel)
{
        if ((channel >= 1) && (channel <= 10))
        {
                /* 一帧数据帧以 $ 符号开始 */
                data_scope_buffer[0] = '$';

                /* 一帧数据帧以 buffer[i] = i 结尾 */
                data_scope_buffer[4 * channel + 1] = 4 * channel + 1;

                /* 发送缓冲区数据个数: 通道数据(4*i) + 帧头(1) + 帧尾(1) */
                return (4 * channel + 2);
        }

        return -1;
}

/**
 * @brief 发送波形数据到上位机
 * @param num 同时发送的通道数
 * @return ... 每一个通道的波形数据公式
 * @note 尽量不要使用过多的通道, 3个左右就好了, 波形容易乱
 */
void data_scope_wave(int num, ...)
{
        int i;
        int length;

        /* 定义保存函数参数的结构 */
        va_list list;

        /* 存放取出的字符串参数 */
        float param;

        /* list 指向传入的第一个可选参数, num 是最后一个确定的参数 */
        va_start(list, num);

        for (i = 1; i <= num; i++)
        {
                /* 取出当前的参数 */
                param = va_arg(list, double);

                /* 将参数值写入对应的通道中 */
                data_scope_write_buffer(i, param);
        }

        /* 将 list 置为 NULL */
        va_end(list);

        /* 格式化数据帧 */
        length = data_scope_ready_to_send(num);

        /* 通过查询的方式发送数据帧 */
        for (i = 0; i < length; i++)
        {
                while ((USART1->SR & 0x40) == 0)
                        ;
                USART1->DR = data_scope_buffer[i];
        }
}
