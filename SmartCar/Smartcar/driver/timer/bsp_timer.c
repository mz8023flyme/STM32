#include "bsp_timer.h"
#include "main.h"

static u16 TIM7_COUNT;


void TIM7_Init(void)
{  
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��

        //��ʱ����ʼ�� ʹ�û�����ʱ��TIM7
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
        //���ö�ʱ�������ṹ��

        TIM_TimeBaseStructure.TIM_Period = (1000-1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         ������1000Ϊ1ms
        TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         

        TIM_ClearFlag(TIM7, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
        TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);    //�򿪶�ʱ�������ж�


        MY_NVIC_Init(1,1,TIM7_IRQn);

        TIM_Cmd(TIM7,ENABLE);     
}

void TIM7_IRQHandler (void )
{
        if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM7�����жϷ������
        {
               TIM7_COUNT++;
               if(TIM7_COUNT>=50)
               {
                       TIM7_COUNT=0;
                       Distance = Hcsr04GetLength();
//                       printf("[%s][%d]\r\n", __func__, __LINE__ );
               }
        }
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIMx�����жϱ�־ 
}

