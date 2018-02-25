#include "1602.h"
#include "bsp_i2c.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
/************************** 输入字符 **************************/
void LCD_Show(int y,int x,char *show)//先输入行再输入列
{
        char addr ;
        addr=0x80+0x40*y+x;
        Send_command(addr);
        while (*show)
        {
                LCD_Send_byte(*show++);
        }
}

/************************** 发送命令 **************************/
int Send_command(char cmd)
{
        char temp;
        I2C_Start();
        I2C_Sent_Byte(0x7e);
        if(I2C_Wait_Ask())
        {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
/************************** 发送前四位 **************************/
        temp = cmd & 0xF0;
        temp = temp|0x0C;   //EN = 1,  RW = 0 , RS =1
        I2C_Sent_Byte(temp);
        if(I2C_Wait_Ask())
        {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        delay_ms(1);
        temp &= 0xFB;           //Make EN = 0
        I2C_Sent_Byte(temp);
        if(I2C_Wait_Ask())
        {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
/************************** 发送后四位 **************************/
        temp = (cmd & 0x0F)<<4;
        temp = temp|0x0C;   //EN = 1,  RW = 0 , RS =1
        I2C_Sent_Byte(temp);
        if(I2C_Wait_Ask())
        {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        delay_ms(1);
        temp &= 0xFB;           //Make EN = 0
        I2C_Sent_Byte(temp);
        if(I2C_Wait_Ask())
        {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        I2C_Stop();
        return 0;
}

/************************** 发送数据 **************************/
int  LCD_Send_byte(char data)
{
            char temp;
            I2C_Start();
            I2C_Sent_Byte(0x7e);
            if(I2C_Wait_Ask())
            {
                    printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                    return -1;
            }
/************************** 发送前四位 **************************/
            temp = data & 0xF0;
            temp |= 0x0D;        //  EN = 1,  RW = 0 , RS =1
            I2C_Sent_Byte(temp);
            if(I2C_Wait_Ask())
            {
                    printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                    return -1;
            }
            delay_ms(1); 
            temp &= 0xFB;        //Make EN = 0
            I2C_Sent_Byte(temp); 
            if(I2C_Wait_Ask())
            {
                    printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                    return -1;
            }
/************************** 发送后四位 **************************/
            temp = (data & 0x0F) << 4    ;
            temp |= 0x0D;        //EN = 1,  RW = 0 , RS =1
            I2C_Sent_Byte(temp); 
            if(I2C_Wait_Ask())
            {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
            }
            delay_ms(1);
            temp &= 0xFB;        // Make EN = 0
            I2C_Sent_Byte(temp); 
            if(I2C_Wait_Ask())
            {
                printf("[1602][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
            }
            I2C_Stop();
            return 0;
}

void LCD_Init(void)
{
        Send_command(0x32);
        Send_command(0x28);
        Send_command(0x0C);
        Send_command(0x06);
        Send_command(0x01);
}
