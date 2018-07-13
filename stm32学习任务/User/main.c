#include "stm32f10x.h"   
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h" 
#include "bsp_AdvanceTim.h" 
#include "bsp_exti.h"
#include "bsp_rtc.h"
#include "bsp_date.h"

extern __IO uint16_t ADC_ConvertedValue;
extern __IO uint32_t TimeDisplay;
float ADC_ConvertedValueLocal; 
extern  __IO float DutyCycle ;
extern __IO float Frequency ;
extern uint16_t time;

uint8_t flag=0;
uint8_t  P=0;

struct rtc_time rtc_demo_time = 
{
	.tm_sec = 0,
	.tm_min = 20,
	.tm_hour = 10,
	.tm_mday = 14,
	.tm_mon = 5,
	.tm_year = 2017,
	.tm_wday = 0,
};
	
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


void jieshu(void)
{
	Usart_SendByte(USART1,0xff);
	Usart_SendByte(USART1,0xff);
	Usart_SendByte(USART1,0xff);
}


int num_count = 0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART_Config();
	
	LED_GPIO_Config();
	
	KEY_GPIO_Config();
	
	EXTI_Config();
	
	ADCx_Init();
	
	ADVANCE_TIM_Init();

	GENERAL_TIM_Init();
	
	RTC_NVIC_Config();

	RTC_CheckAndConfig(&rtc_demo_time);

	while (1)
	{
		
		printf("%s runing %d\r\n", __FUNCTION__,num_count++);
		
	       if (TimeDisplay == 1)
	       {
					
		      Time_Display( RTC_GetCounter(), &rtc_demo_time); 		  
		      TimeDisplay = 0;
	       }

		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // 读取转换的AD值
	
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
	       
		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
		printf("占空比：%0.2f%%   频率：%0.2fHz\r\n",DutyCycle,Frequency);

	       __ASM("CPSID I");        //关中断
                jieshu();
	        printf("t8.txt=\"%0.3f\"",ADC_ConvertedValueLocal);
		jieshu();
	        printf("t6.txt=\"%0.2f%%\"",DutyCycle);
	        jieshu();
	        printf("t9.txt=\"%0.2fHz\"",Frequency);
	        jieshu();
	      __ASM("CPSIE I"); //开中断
	}	
}

