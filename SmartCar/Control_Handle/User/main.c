#include "main.h"

u8 Mode = 1 ; // NRF的工作模式 1 TX  0 RX 

int main(void)
{
        //advance作为电机的（0-200）voice 超声波舵机（5-25）turn作为转向舵机（10-20）
        u8 advance,turn,voice;
        char str[100];
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        SPI_bsp_Init();
        NRF_GPIO_Init();
        ADCx_Init();
        
        while( NRF24L01_Check() )
        {
               printf("[%s][%d]\r\n", __func__, __LINE__);
               delay_ms(1000);
        }
        TX_Mode();
        while(1)
        {
                printf("[%s][%d]\n", __func__, __LINE__);
                ADC_Value[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
                ADC_Value[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
                ADC_Value[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
                ADC_Value[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
                ADC_Value[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
//                printf("\r\n CH0 value = %f V \r\n",ADC_Value[0]);
//                printf("\r\n CH1 value = %f V \r\n",ADC_Value[1]);
//                printf("\r\n CH2 value = %f V \r\n",ADC_Value[2]);
//                printf("\r\n CH3 value = %f V \r\n",ADC_Value[3]);
//                printf("\r\n CH4 value = %f V \r\n",ADC_Value[4]);
                
                advance = ADC_Value[2]/3.3*200+1;   //+1处理1.63v与1.65v带来的偏差
                printf("advance : %d   ",advance);
                
                turn = (char)((ADC_Value[1]*10)/3.3+10);
                printf("turn : %d   ",turn);
                
                voice = ADC_Value[3]/3.3*20+5;
                printf("voice : %d   \n",voice);
                
                sprintf(str,"advance_%d_turn_%d_voice_%d_#",advance ,turn ,voice );
                printf("%s",str);
                NRF24L01_TxPacket((u8*)str);
                delay_ms(100);
        }
}

//advance:*_turn:*_voice:*_lkey:*_rkey:*_#(传输格式)
/*
int main(void)
{
        u8 str[32] ="Hello Word" ; //发送端要发送的数据
         
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        LED_GPIO_Config();
        SPI_bsp_Init();
        //判断NRF为收、发模式的引脚初始化
        NRF_GPIO_Init();
        
        printf("这是一个SPI测试程序\n");
        if(NRF24L01_Check())
        {
               printf("[%s][%d]\r\n", __func__, __LINE__);
        }
        
        while(1)
        {
//                printf("[%s][%d]\r\n", __func__, __LINE__);
//                delay_ms(1000);
                
                //判断NRF是接受端还是发送端，如果引脚 NRF_Judge_PORT 检测到为低电平，则为接受端
                  if(GPIO_ReadInputDataBit(NRF_Judge_PORT, NRF_Judge_PIN) == 0) 
                  {
                          //配置NRF的工作模式为接受模式
                          RX_Mode();
//                          printf("\n NRF要接受数据了！\n");
                          while(1)
                          {
                                  if(NRF24L01_RxPacket(Read) == 0)
                                  {
//                                          printf("\n NRF接受到数据了！\n");
                                          //如果数据接受成功则亮绿灯
                                          LED_GREEN
//                                          Usart_SendString( USART1,Read);
                                          printf("NRF 接受到的数据为：%s\n",Read);
                                  }
                                  else
                                  {
                                          //如果数据没有接受成功则亮红灯
                                          LED_RED
                                  }
                          }
                          
                         
                  }
                  else
                  {
                          //配置NRF的工作模式为发送模式
                          TX_Mode();
//                          printf("\n NRF要发送数据了！\n");
                          while(1)
                          {
                                  if(NRF24L01_TxPacket(str) == TX_OK)
                                  {
                                          //如果数据发送成功则亮蓝灯
                                          LED_BLUE
//                                          printf("数据发送完成！\n");
                                  }
                                  else
                                  {
                                          //如果数据没有发送成功则亮黄灯
                                          LED_YELLOW
                                  }
                          }
                          
                  }
        }
}

*/


