#include "main.h"

float Distance;
u8 NRF_RX_Buf[100]; //用来存储发送端发来了数据
void Handle_Control(u8 *buff);


int main(void)
{
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        printf("[%s][%d]\n",__func__,__LINE__);
        TIM8_PWM_Init();        //motor   初始化
        Hcsr04Init();           //超声波的初始化
        TIM7_Init();
        SPI_bsp_Init();
        NRF_GPIO_Init();
//        ADCx_Init();
        TIM8->CCR3 = 15; 
        
        Distance = Hcsr04GetLength();
        while(NRF24L01_Check())
        {
               printf("can't find NRF24L01\n");
                delay_ms(1000);
        }
        RX_Mode();
        while(1)
        {
                NRF24L01_RxPacket(NRF_RX_Buf);
                printf("%s\n",NRF_RX_Buf);
                Handle_Control(NRF_RX_Buf);
        }
}


void Handle_Control(u8 *buff)
{
        char *res;
        char *motor=NULL;
        char *steer1=NULL;  //转向舵机
        char *steer2=NULL;  //超声波舵机
        int advance=0;
        int turn=0;
        int voice=0;
        
        res = strtok((char*)buff, "_");
        if(res)
        {
                motor = res;
        }
        res = strtok(NULL, "_");
        if(res)
        {
                advance = atoi(res);
        }
        res = strtok(NULL, "_");
        if(res)
        {
                steer1 = res;
        }
        res = strtok(NULL, "_");
        if(res)
        {
                turn = atoi(res);
        }
        res = strtok(NULL, "_");
        if(res)
        {
                steer2 = res;
        }
        res = strtok(NULL, "_");
        if(res)
        {
                voice = atoi(res);
        }
        if(!strcmp("advance", motor))
        {
                if(advance>100 && Distance<30)
                {
                        M1_Revolve(0);
                        M2_Revolve(0);
                        return ;
                }
                M1_Revolve((advance-100)*2);
                M2_Revolve((advance-100)*2);
        }
        if(!strcmp("turn", steer1))
        {
                TIM8->CCR1 = turn ;
        }
        if(!strcmp("voice", steer2))
        {
                TIM8->CCR3 = voice;
        }
}





//                delay_ms(500);
//                if(Distance>=30)
//                {
//                        M1_Revolve(200);
//                        M2_Revolve(200);
//                }
//                
//                else
//                {     
//                        M1_Revolve(0);
//                        M2_Revolve(0);
//                        
//                        BZ_Turn(Distance_Scan());
//                }
//                
























