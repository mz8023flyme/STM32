#include "string.h"
#include "USART_HMI.h"

/* 串口屏指令的结束标志常量 */
char END_flag[4] = {0xff,0xff,0xff,'\0'};


/**
  * @brief 向HMI发送字符串命令，改变显示的状态
  *       
  * @param 命令字符串        
  * @note  只需要发送命令实体，自动发送结束标志
  *       
  */
void HMI_Send_Command(char * Command)
{
        HMI_Usart_SendString(HMI_USARTx,Command);
        HMI_Usart_SendString(HMI_USARTx,END_flag);
}




/**
  * @brief 接收一个HMI_Control_Typtdef 结构体变量的指针，用于更新控件的参数
  *        该函数主要的适用于数字和文本控件
  * @param 串口屏空间结构体变量
  * @note 由于函数内部的处理没有自动分配内存，所以每个char指针所指向的字符串
  *       不能太长
  */
void HMI_Updata_Control_Property(HMI_Control_Typtdef* controlture)
{
        
        char buff[50]={0};       
        sprintf(buff,"%s",controlture->Control_Name);
        strcat(buff,".");
        strcat(buff,controlture->Control_Property);
        strcat(buff,"=");
        if(!strncmp(controlture->Control_Property,"txt",4))
        {
                strcat(buff,"\"");
                strcat(buff,controlture->Control_Param);
                strcat(buff,"\"");
                printf("%s",buff);
        }
        else
        {
                strcat(buff,controlture->Control_Param);
                printf("%s",buff);
        }
        HMI_Usart_SendString(HMI_USARTx,buff);
        HMI_Usart_SendString(HMI_USARTx,END_flag);

}


