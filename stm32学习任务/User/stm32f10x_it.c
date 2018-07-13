/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_AdvanceTim.h" 
#include "bsp_usart.h"
#include "bsp_basic_tim.h"
#include "bsp_GeneralTim.h" 

extern uint32_t TimeDisplay;

extern uint16_t time;
uint8_t B=0;
__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;




//extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure ;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

int num = 0;

void ADVANCE_TIM_IRQHandler(void)
{
//	printf("%s enter\r\n",__FUNCTION__);
	/* 清除中断标志位 */
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

	/* 获取输入捕获值 */
	IC1Value = TIM_GetCapture1(ADVANCE_TIM);
	IC2Value = TIM_GetCapture2(ADVANCE_TIM);

	// 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
	if (IC1Value != 0)
	{
		/* 占空比计算 */  
		DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);
		
		/* 频率计算 */
		Frequency = (72000000/(ADVANCE_TIM_PSC+1))/(float)(IC1Value+1);
		
		
	}
	else
	{
		DutyCycle = 0;
		Frequency = 0;
	}
}

void RTC_IRQHandler(void)
{
//	printf("%s enter  \r\n",__FUNCTION__);
	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);
	
	    /* Enable time update */
	    TimeDisplay = 1;
	
	    /* Wait until last write operation on RTC registers has finished */
	    RTC_WaitForLastTask();
	  }
//	  printf("%s out  \r\n",__FUNCTION__);
}
//void  BASIC_TIM_IRQHandler (void)
//{
//	printf("%s enter %d \r\n",__FUNCTION__, num++);
//	
//	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
//	{	
//		time++;
//		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  		 
//	}
//	

////	printf("%s exit \r\n",__FUNCTION__);	
//}

void EXTI0_IRQHandler (void)     
{
	printf("%s enter\r\n",__FUNCTION__);

	if (EXTI_GetITStatus(EXTI_Line0)!= 0)
	{	
		TIM3->ARR=9999;
		TIM3->CCR3=4999;
		
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
	{	
		
	}
}


void EXTI15_10_IRQHandler (void)     
{

	printf("%s enter\r\n",__FUNCTION__);
	if (EXTI_GetITStatus(EXTI_Line13)!= 0)
	{	
		TIM3->ARR=19999;
		TIM3->CCR3=9999;
		
	}
	EXTI_ClearITPendingBit(EXTI_Line13);
	{	
		
	}

}


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	printf("%s enter\r\n",__FUNCTION__);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	printf("%s enter\r\n",__FUNCTION__);
  /* Go to infinite loop when Hard Fault exception occurs */
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
	printf("%s enter\r\n",__FUNCTION__);
  /* Go to infinite loop when Memory Manage exception occurs */
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
	printf("%s enter\r\n",__FUNCTION__);
  /* Go to infinite loop when Bus Fault exception occurs */
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
	printf("%s enter\r\n",__FUNCTION__);
  /* Go to infinite loop when Usage Fault exception occurs */
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
	printf("%s enter\r\n",__FUNCTION__);
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
	printf("%s enter\r\n",__FUNCTION__);
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	printf("%s enter\r\n",__FUNCTION__);
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	printf("%s enter\r\n",__FUNCTION__);
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
