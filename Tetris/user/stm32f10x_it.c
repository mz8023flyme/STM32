#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "main.h"


static u16 Time ; 
static u16 Down_Time_F ; 
void  BASIC_TIM_IRQHandler (void)
{
//        printf("[%s][%d]\r\n", __func__, __LINE__);
        if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
        {
                if(!Pause_Flag)
                {
                        if(Down_Flag==0)
                        {
                                Down_Time_F=0;
                                Time++;
                                if(Time>=1000)
                                {
                                        Time=0;
                                        Tetris_Dowm(); 
                                }
                        }
                        else
                        {
                                Time=0;
                                Down_Time_F++;
                                if(Down_Time_F>=5)//此数字较大则下降为快速下降，较小为瞬间到底
                                {
                                        Down_Time_F=0;
                                        
                                        Tetris_Dowm();
                                }
                        }
                }
        }
        TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);
}


void EXTI9_5_IRQHandler()
{
        if(EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
                Down_Flag=1;
                delay_ms(50);

                
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
        
        if(EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
                Pause_Flag=!Pause_Flag;
                delay_ms(50);
                
        }
         EXTI_ClearITPendingBit(EXTI_Line9);
}


void ADC_IRQHandler(void)
{ 
        if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
        {
                // 读取ADC的转换值
                ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
        }
        ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
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
