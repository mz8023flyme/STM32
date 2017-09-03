#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "bsp_control.h"
#include "bsp_motor.h"

extern int Motor1;
extern int Encoder;
extern int Target_position;

int TIM3_IRQHandler(void)  
{    
        if(TIM3->SR&0X0001)
        {   
                TIM3->SR&=~(1<<0);                           //===清除定时器1中断标志位  
                Encoder=Read_Encoder(4);                     //===读取编码器的位置数据 初始值是10000
                Motor1=Incremental_PI(Encoder,11000);          //===位置PID控制器
                Xianfu_Pwm();                                //===PWM限幅
                M1_Revolve(Motor1);                          //===赋值给PWM寄存器  
        }
        return 0; 
}

/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{
        if(TIM4->SR&0X0001)//溢出中断
        {
        }
        TIM4->SR&=~(1<<0);//清除中断标志位    
}
/**************************************************************************
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{
        if(TIM2->SR&0X0001)//溢出中断
        {
        }
        TIM2->SR&=~(1<<0);//清除中断标志位  
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
