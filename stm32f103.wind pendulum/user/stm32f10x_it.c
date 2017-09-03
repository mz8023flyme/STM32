#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "test.h"
#include "bsp_control.h"
#include "data_scope.h"
#include "usart_mpu.h"
#include "string.h"



//void USART2_IRQHandler(void)
//{
//        if( USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET )
//        {
//                static u8 counter = 0;
//                Re_temp[counter] = USART_ReceiveData(USART2);   //接收数据
//                if(counter == 0 && Re_temp[0] != 0x55) 
//                        return;      //第 0 号数据不是帧头，跳过
//                else
//                {
//                        counter++;
//                        if(counter>=2)
//                        {
//                                if(Re_temp[1] != 0x53)
//                                {
//                                        counter=0;
//                                        return;
//                                }
//                                else
//                                {                                        
//                                        if(counter==11) //接收到 11 个数据
//                                        { 
//                                                memcpy(Re_buf,Re_temp,11);
//                                               
//                                                counter=0; //重新赋值，准备下一帧数据的接收

////                                                printf("Angle.pitch = %0.2f,Angle.roll = %0.2f\n",Angle.pitch,Angle.roll);
//                                        }
//                                }
//                        }
//                }
//                
//        }
//}



void USART2_IRQHandler(void)
{
        if( USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET )
        {
                static u8 counter = 0;
                Re_temp[counter] = USART_ReceiveData(USART2);   //接收数据
                if(counter == 0 && Re_temp[0] != 0x55) 
                        return;      //第 0 号数据不是帧头，跳过
                else
                {
                        counter++; 
                }
                if(counter==11) //接收到 11 个数据
                { 
                        memcpy(Re_buf,Re_temp,11);
                        counter=0; //重新赋值，准备下一帧数据的接收
                        if(Re_buf[1] == 0x53)
                        {                
                                Angle.roll = ((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*180;   //X轴滚转角（x 轴）
                                Angle.pitch = ((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
                                Angle.yaw = ((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*180;   //Z轴偏航角（z 轴）
                                Temperature = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;   //温度 
                                printf("Angle.roll:%0.2f, Angle.pitch:%0.2f\n",Angle.roll,Angle.pitch);
                        }
                }
        }

}


void USART3_IRQHandler(void)
{
        if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != RESET )
        {
                //printf("%c\n",USART_ReceiveData(USART3));
                USART_ReceiveData(USART3);
        }
}




u8 Current_Mode = 1;
void TIM7_IRQHandler(void)
{

        if(TIM_GetITStatus(TIM7,TIM_IT_Update) ==  SET)
        {
               TIM_ClearFlag(TIM7,TIM_FLAG_Update);
               //printf("roll = %0.2f pitch = %0.2f\n\n",Angle.roll,Angle.pitch);
               //printf("\nTIM7_IRQHandler runing\n");
                switch(Current_Mode)
                {
                        case 1:
                                Mode1(Angle.pitch+0.99,Angle.roll-1.94);
                                break;
                        case 2:
                                Mode1(Angle.pitch+0.99,Angle.roll-1.94);
                                break;
                        case 3:
                                Mode2(Angle.pitch+0.99,Angle.roll-1.94);
                                break;
                        case 4:
                                Mode3(Angle.pitch+0.99,Angle.roll-1.94);
                        default: 
                                Mode0();
                                break;
                }
                //test_get_angle();
        }
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
        /* Go to infinite loop when Hard Fault exception occurs */
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
          
        }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
        /* Go to infinite loop when Memory Manage exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
        /* Go to infinite loop when Bus Fault exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
        /* Go to infinite loop when Usage Fault exception occurs */
        
        printf("[%s][%d]", __func__, __LINE__);
        
        while (1)
        {
                
        }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
        printf("[%s][%d]", __func__, __LINE__);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
        
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
