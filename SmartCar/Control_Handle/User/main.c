#include "main.h"

u8 Mode = 1 ; // NRF�Ĺ���ģʽ 1 TX  0 RX 

int main(void)
{
        //advance��Ϊ����ģ�0-200��voice �����������5-25��turn��Ϊת������10-20��
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
                
                advance = ADC_Value[2]/3.3*200+1;   //+1����1.63v��1.65v������ƫ��
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

//advance:*_turn:*_voice:*_lkey:*_rkey:*_#(�����ʽ)
/*
int main(void)
{
        u8 str[32] ="Hello Word" ; //���Ͷ�Ҫ���͵�����
         
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        LED_GPIO_Config();
        SPI_bsp_Init();
        //�ж�NRFΪ�ա���ģʽ�����ų�ʼ��
        NRF_GPIO_Init();
        
        printf("����һ��SPI���Գ���\n");
        if(NRF24L01_Check())
        {
               printf("[%s][%d]\r\n", __func__, __LINE__);
        }
        
        while(1)
        {
//                printf("[%s][%d]\r\n", __func__, __LINE__);
//                delay_ms(1000);
                
                //�ж�NRF�ǽ��ܶ˻��Ƿ��Ͷˣ�������� NRF_Judge_PORT ��⵽Ϊ�͵�ƽ����Ϊ���ܶ�
                  if(GPIO_ReadInputDataBit(NRF_Judge_PORT, NRF_Judge_PIN) == 0) 
                  {
                          //����NRF�Ĺ���ģʽΪ����ģʽ
                          RX_Mode();
//                          printf("\n NRFҪ���������ˣ�\n");
                          while(1)
                          {
                                  if(NRF24L01_RxPacket(Read) == 0)
                                  {
//                                          printf("\n NRF���ܵ������ˣ�\n");
                                          //������ݽ��ܳɹ������̵�
                                          LED_GREEN
//                                          Usart_SendString( USART1,Read);
                                          printf("NRF ���ܵ�������Ϊ��%s\n",Read);
                                  }
                                  else
                                  {
                                          //�������û�н��ܳɹ��������
                                          LED_RED
                                  }
                          }
                          
                         
                  }
                  else
                  {
                          //����NRF�Ĺ���ģʽΪ����ģʽ
                          TX_Mode();
//                          printf("\n NRFҪ���������ˣ�\n");
                          while(1)
                          {
                                  if(NRF24L01_TxPacket(str) == TX_OK)
                                  {
                                          //������ݷ��ͳɹ���������
                                          LED_BLUE
//                                          printf("���ݷ�����ɣ�\n");
                                  }
                                  else
                                  {
                                          //�������û�з��ͳɹ������Ƶ�
                                          LED_YELLOW
                                  }
                          }
                          
                  }
        }
}

*/


