#include "bsp_hcsr04.h"
#include "main.h"

u16 msHcCount = 0;//ms����

void Hcsr04Init(void)
{  
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��
eDef GPIO_InitStructure;

        //GPIO��ʼ��
        RCC_APB2Peri
        GPIO_InitTypphClockCmd(HCSR04_CLK, ENABLE);
        //TRIG   PB5   ���10us�ߵ�ƽ
        GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       //���͵�ƽ����
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
        GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
        GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
        //ECHO   PB6   ���䲢���գ�ͨ��ʱ��������
        GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     //���ص�ƽ����
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
        GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
        GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    

        //��ʱ����ʼ�� ʹ�û�����ʱ��TIM6
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
        //���ö�ʱ�������ṹ��

        TIM_TimeBaseStructure.TIM_Period = (1000-1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         ������1000Ϊ1ms
        TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
        TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         

        TIM_ClearFlag(TIM6, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
        TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);    //�򿪶�ʱ�������ж�


        MY_NVIC_Init(0,1,TIM6_IRQn);

        TIM_Cmd(TIM6,DISABLE);     
}


//tips��static����������������ڶ�������Դ�ļ��ڣ����Բ���Ҫ��ͷ�ļ�������
static void OpenTimerForHc()        //�򿪶�ʱ��
{
        TIM_SetCounter(TIM6,0);//�������
        msHcCount = 0;
        TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx����
}
 
static void CloseTimerForHc()        //�رն�ʱ��
{
        TIM_Cmd(TIM6, DISABLE);  //ʹ��TIMx����
}

//��ʱ��6�жϷ������
void TIM6_IRQHandler (void)   //TIM3�ж�
{       

        if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
        {
               
                msHcCount++;
        }
         TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx�����жϱ�־ 
}
 

//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//�õ�MS
        t += TIM_GetCounter(TIM6);//�õ�US
        TIM6->CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����
        delay_ms(50);
        return t;
}
 

//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
//Ϊ�����������Ӱ�죬ȡ������ݵ�ƽ��ֵ���м�Ȩ�˲���
float Hcsr04GetLength(void )
{
        u32 t = 0;
        int i = 0;
        float lengthTemp = 0;
        float sum = 0;
        while(i!=2)
        {

                TRIG_Send = 1;      //���Ϳڸߵ�ƽ���
                delay_us(20);
                TRIG_Send = 0;
                while(ECHO_Reci == 0);      //�ȴ����տڸߵ�ƽ���
                OpenTimerForHc();        //�򿪶�ʱ��
                i = i + 1;
                while(ECHO_Reci == 1);

                CloseTimerForHc();        //�رն�ʱ��

                t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US

                lengthTemp = ((float)t/58.0);//cm

                sum = lengthTemp + sum ;

        }
        lengthTemp = sum/2.0;
//        printf("����Ϊ:%.3f CM\n",lengthTemp);
        return lengthTemp;

}

















