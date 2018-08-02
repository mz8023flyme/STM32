#include "main.h"

float Distance;


int main(void)
{
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        
        TIM8_PWM_Init();        //motor   初始化
        Hcsr04Init();           //超声波的初始化
        TIM7_Init();
        printf("[%s][%d]\r\n", __func__, __LINE__ );
        
        
        TIM8->CCR3 = 15;
        
        Distance = Hcsr04GetLength();
        while(1)
        {
                
                
                if(Distance>=30)
                {
                        M1_Revolve(200);
                        M2_Revolve(200);
                }
                
                else
                {     
                        M1_Revolve(0);
                        M2_Revolve(0);
                        
                        BZ_Turn(Distance_Scan());
                }
                
                
                
                
                
                
                
//                TIM8->CCR1=15;
//                M1_Revolve(70);
//                M2_Revolve(70);
//                delay_ms(500);
//                M1_Revolve(0);
//                M2_Revolve(0);
//                delay_ms(500);
//                TIM8->CCR1=10;
//                M1_Revolve(-70);
//                M2_Revolve(-70);
//                delay_ms(500);
//                M1_Revolve(0);
//                M2_Revolve(0);
//                delay_ms(500);
//                TIM8->CCR1=20;
//                M1_Revolve(70);
//                M2_Revolve(70);
//                delay_ms(500);
//                M1_Revolve(0);
//                M2_Revolve(0);
        }
}























