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
        

////                printf("[%s][%d]\r\n", __func__, __LINE__);
        while(1)
        {
                TIM8->CCR1=15;
                M1_Revolve(50);
                M2_Revolve(50);
                delay_ms(500);
                M1_Revolve( 0 );
                M2_Revolve( 0 );
                delay_ms(500);
                TIM8->CCR1=10;
                M1_Revolve(-50);
                M2_Revolve(-50);
                delay_ms(500);
                M1_Revolve( 0 );
                M2_Revolve( 0 );
                delay_ms(500);
                TIM8->CCR1=20;
                M1_Revolve(50);
                M2_Revolve(50);
                delay_ms(500);
                TIM8->CCR1=15;
                M1_Revolve( 0 );
                M2_Revolve( 0 );
        }
}
