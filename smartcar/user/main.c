#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_motor.h"


int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        TIM8_PWM_Init();
        
//        int i;
////                printf("[%s][%d]\r\n", __func__, __LINE__);
        while(1)
        {
//                MOTOR2_PIN2=0;
//                MOTOR2_PIN1=0;
//                TIM8->CCR4=100;
//                delay_ms(1000);
//                TIM8->CCR1=5;
//                delay_ms(1000);
//                TIM8->CCR4=0;
//                TIM8->CCR1=10;
                M1_Revolve(1);
                M2_Revolve(1);
//                
////                TIM8->CCR1=25;
//                delay_ms(1000);
////                M1_Revolve(-199);
////                M2_Revolve(-199);
//                TIM8->CCR1=15;
//                delay_ms(1000);
//                delay_ms(1000);
//                M1_Revolve(100);
//                delay_ms(1000);
        }
}
