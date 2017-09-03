#include "DataScope_DP.h"
#include <stdarg.h>
#include <string.h>
#include "bsp_usart.h"
#include "bsp_systick.h"

unsigned char DataScope_OutPut_Buffer[42] = {0};        //串口发送缓冲区


/**
 * @brief 将单精度浮点数据转成 4 字节数据并存入 buffer 中指定的位置
 * @param target 传入的单精度浮点数, 上位机显示的就是它
 * @param buf 将单精度浮点数转化后保存在此 buffer 中
 * @param beg 指定保存在 buffer 中的位置
 * @return null
 */
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
        unsigned char *point;
        point = (unsigned char*)target;     //得到float的地址
        buf[beg]   = point[0];
        buf[beg+1] = point[1];
        buf[beg+2] = point[2];
        buf[beg+3] = point[3];
}
 
 
//函数说明：将待发送通道的单精度浮点数据写入发送缓冲区
//Data：通道数据
//Channel：选择通道（1-10）
//函数无返回 
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
        if ( (Channel <= 10) && (Channel > 0) ) 
        Float2Byte(&Data,DataScope_OutPut_Buffer,4 * Channel - 3);
}

/**
 * @brief 生成 data_scope 上位机能正确识别的帧格式
 * @param channel 指定本次格式化帧中使用到了多少个通道 (1 - 10)
 * @return succed: 发送缓冲区数据个数 fail: 0
 */
unsigned char DataScope_Data_Generate(unsigned char Channel)
{
        if ( (Channel <= 10) && (Channel > 0) ) 
        {
                DataScope_OutPut_Buffer[0] = '$';  //帧头
                
                DataScope_OutPut_Buffer[4 * Channel + 1]  =  4 * Channel + 1 ;
                
                return (4 * Channel + 2);
        }
        return 0;
}

/**
 * @brief 发送波形数据到上位机
 * @param num 要发送的通道
 * @return Data 每一个通道的波形数据公式
 * @note 尽量不要使用过多的通道, 3个左右就好了, 波形容易乱
 */

void data_scope_wave(int num, float Data)
{
        int i;
        int length;
                /* 将参数值写入对应的通道中 */
        DataScope_Get_Channel_Data(Data, i);
        
        /* 格式化数据帧 */
        length = DataScope_Data_Generate(num);

        /* 通过查询的方式发送数据帧 */
        for (i = 0; i < length; i++)
        {
                while ((USART1->SR & 0x40) == 0);
                
                USART1->DR = DataScope_OutPut_Buffer[i];
        }
        delay_ms(50); //20HZ
}









