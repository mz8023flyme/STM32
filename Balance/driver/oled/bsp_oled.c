#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_systick.h"
#include "codetab.h"
#include "bsp_usart.h"


int WriteCmd(u8 cmd)
{
        IIC_Start();
        IIC_Send_Byte(OLED_ADDRESS);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(0x00);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(cmd);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                IIC_Stop();
                return -1;
        }
        IIC_Stop();
        return 0;
}

int  WriteDat(u8 data )
{
        IIC_Start();
        IIC_Send_Byte(OLED_ADDRESS);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(0x40);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return -1;
        }
        IIC_Send_Byte(data);
        if (!IIC_Wait_Ack())
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                IIC_Stop();
                return -1;
        }
        IIC_Stop();
        return 0;
}

/**
 * @brief OLED 初始化
 */

void OLED_Init(void)
{
        delay_ms(10); //这里的延时很重要

        WriteCmd(0xAE); //关闭显示  清屏
        WriteCmd(0x20); //设置内存寻址模式
        WriteCmd(0x10); //00,水平寻址模式;01,垂直寻址模式;10,页面寻址模式;11,无效
        WriteCmd(0xb0); //为页面寻址设置页面起始地址 Mode,0-7
        WriteCmd(0xc8); //设置扫描方向
        WriteCmd(0x00); //设置低列地址
        WriteCmd(0x10); //设置高列地址
        WriteCmd(0x40); //设置开始行
        WriteCmd(0x81); //设置控制寄存器
        WriteCmd(0xff); //亮度调节 0x00~0xff
        WriteCmd(0xa1); //集合段重新映射0到127
        WriteCmd(0xa6); //设置正常显示
        WriteCmd(0xa8); //设置多路复用率(1到64)
        WriteCmd(0x3F); //
        WriteCmd(0xa4); //0xa4,输出跟踪内存的内容;0xa5,水平处理
        WriteCmd(0xd3); //设置显示抵消
        WriteCmd(0x00); //不抵消
        WriteCmd(0xd5); //设置显示时钟分频/振荡器频率
        WriteCmd(0xf0); //设置分率
        WriteCmd(0xd9); //--set pre-charge period
        WriteCmd(0x22); //
        WriteCmd(0xda); //设置com pin硬件配置
        WriteCmd(0x12);
        WriteCmd(0xdb); //--set vcomh
        WriteCmd(0x20); //0x20,0.77xVcc
        WriteCmd(0x8d); //--set DC-DC enable
        WriteCmd(0x14); //
        WriteCmd(0xaf); //打开oled面板
}

/**
 * @brief 设置起始点坐标
 */
void OLED_SetPos(u8 x, u8 y)
{ 
        WriteCmd(0xb0+y);
        WriteCmd(((x&0xf0)>>4)|0x10);
        WriteCmd((x&0x0f)|0x01);
}

/**
 * @brief 全屏填充
 */

void OLED_Fill(unsigned char fill_Data)
{
        unsigned char m,n;
        for(m=0;m<8;m++)
        {
                WriteCmd(0xb0+m);       //page0-page1
                WriteCmd(0x00);         //low column start address
                WriteCmd(0x10);         //high column start address
                for(n=0;n<128;n++)
                {
                        WriteDat(fill_Data);
                }
        }
}

void OLED_CLS(void)     //清屏
{
        OLED_Fill(0x00);
}
/**
 * @brief 将 OLED 从休眠中唤醒
 */

void OLED_ON(void)
{
        WriteCmd(0X8D);  //设置电荷泵
        WriteCmd(0X14);  //开启电荷泵
        WriteCmd(0XAF);  //OLED唤醒
}

/**
 * @brief OLED 进入休眠模式
 */
void OLED_OFF(void)
{
        WriteCmd(0X8D);  //设置电荷泵
        WriteCmd(0X10);  //关闭电荷泵
        WriteCmd(0XAE);  //OLED休眠
}
/**
 * @brief 显示codetab.h中的ASCII字符,有6*8和8*16可选择
 * @param x 起始横坐标  （0-127）
 * @param y 起始纵坐标  （0-7）
 * @param ch[] 要显示的字符串
 * @param TextSize 字符大小(1:6*8 ; 2:8*16)
 */
void OLED_ShowStr(u8 x, u8 y, u8 ch[], u8 TextSize)
{
        unsigned char c = 0,i = 0,j = 0;
        switch(TextSize)
        {
                case 1:
                {
                        while(ch[j] != '\0')
                        {
                                c = ch[j] - 32;
                                if(x > 126)
                                {
                                        x = 0;
                                        y++;
                                }
                                OLED_SetPos(x,y);
                                for(i=0;i<6;i++)
                                        WriteDat(F6x8[c][i]);
                                x += 6;
                                j++;
                        }
                }break;
                case 2:
                {
                        while(ch[j] != '\0')
                        {
                                c = ch[j] - 32;
                                if(x > 120)
                                {
                                        x = 0;
                                        y++;
                                }
                                OLED_SetPos(x,y);
                                for(i=0;i<8;i++)
                                        WriteDat(F8X16[c*16+i]);
                                OLED_SetPos(x,y+1);
                                for(i=0;i<8;i++)
                                        WriteDat(F8X16[c*16+i+8]);
                                x += 8;
                                j++;
                        }
                }break;
        }
}

/**
 * @brief 显示codetab.h中的汉字,16*16点阵
 * @param x 起始点坐标 (0-127)
 * @param y 起始纵坐标 (0-7)
 * @param N:汉字在codetab.h中的索引
 */
void OLED_ShowCN(u8 x, u8 y, u8 N)
{
        unsigned char wm=0;
        unsigned int  adder=32*N;
        OLED_SetPos(x , y);
        for(wm = 0;wm < 16;wm++)
        {
                WriteDat(F16x16[adder]);
                adder += 1;
        }
        OLED_SetPos(x,y + 1);
        for(wm = 0;wm < 16;wm++)
        {
                WriteDat(F16x16[adder]);
                adder += 1;
        }
}



