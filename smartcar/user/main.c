#include "main.h"
#include "bsp_motor.h"
//#include "bsp_sound.h"
#include "bsp_hcsr04.h"


u32  Distance;  // 超声波测距


int main(void)
{
        
	
	        float length;
 
      
        

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
	
	printf("串口初始化成功！\n");
    
        Hcsr04Init();    
        printf("超声波初始化成功！\n");//测试程序是否卡在下面两句上面
	
	
	
	
        TIM8_PWM_Init();     //    motor  初始化

        //TIM3_Cap_Init(0xffff,72-1);
         
        printf("[%s][%d]\r\n", __func__, __LINE__);
        while(1)
        {
//                TIM8->CCR3 = 25;
               //Read_Distane();
//                printf("distance = %d\r\n" ,Distance);
                //delay_ms(500);
					
					
								length = Hcsr04GetLength();
								printf("距离为:%.3f\n",length);
					
					
					
					
					
					
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





    




















