#include "bsp_control.h"
#include "bsp_motor.h"
#include "bsp_usart.h"

extern int Motor1;
extern int Encoder;
extern int Target_position;
extern float Menu_MODE;

/**
 * @brief ��TIM2��ʼ��Ϊ�������ӿ�ģʽ
 */
 
void Encoder_Init_TIM2(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2��ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB�˿�ʱ��

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    //�˿�����
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
        GPIO_Init(GPIOA, &GPIO_InitStructure);               //�����趨������ʼ��GPIOB

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
        TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = 10;
        TIM_ICInit(TIM2, &TIM_ICInitStructure);
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        //Reset counter
        TIM_SetCounter(TIM2,0);
        if(Menu_MODE==1)TIM_SetCounter(TIM2,10000);
        else             TIM_SetCounter(TIM2,0);
        TIM_Cmd(TIM2, ENABLE); 
}
 
 /**
 * @brief ��TIM4��ʼ��Ϊ�������ӿ�ģʽ
 */
 

void Encoder_Init_TIM4(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;    //�˿�����
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
        GPIO_Init(GPIOB, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOB

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
        TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = 10;
        TIM_ICInit(TIM4, &TIM_ICInitStructure);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
        TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
        //Reset counter
        TIM_SetCounter(TIM4,10000);
        //TIM4->CNT = 10000; 
        TIM_Cmd(TIM4, ENABLE); 
}

/*
 * @brief TIM1 �������������ת��   ���� stm32c8t6
 * @param arr �Զ���װ��ֵ
 * @param psc ��Ƶ����
 */
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{ 
        RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ��   
        GPIOB->CRH&=0X0000FFFF;   //PORTB12 13 14 15�������
        GPIOB->CRH|=0X22220000;   //PORTB12 13 14 15�������  //��ʼ�������������IO
        
        RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��    
        RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��     
        GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11�������
        GPIOA->CRH|=0X0000B00B;    //PORTA8 11�������
        TIM1->ARR=arr;             //�趨�������Զ���װֵ 
        TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
        TIM1->CCMR2|=6<<12;        //CH4 PWM1ģʽ 
        TIM1->CCMR1|=6<<4;         //CH1 PWM1ģʽ 
        TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ�� 
        TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ�� 
        TIM1->CCER|=1<<12;         //CH4���ʹ�� 
        TIM1->CCER|=1<<0;          //CH1���ʹ�� 
        TIM1->BDTR |= 1<<15;       //TIM1����Ҫ��仰�������PWM
        TIM1->CR1=0x8000;          //ARPEʹ�� 
        TIM1->CR1|=0x01;           //ʹ�ܶ�ʱ��1 
}
/*
 * @brief TIM3 ��ʱ���ж�  10ms ÿ��   ����stm32c8t6
 * @param arr �Զ���װ��ֵ
 * @param psc ��Ƶ����
 */
void Timer3_Init(u16 arr,u16 psc)  
{  
        RCC->APB1ENR|=1<<1;     //TIM3ʱ��ʹ��    
        TIM3->ARR=arr;          //�趨�������Զ���װֵ   
        TIM3->PSC=psc;          //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
        TIM3->DIER|=1<<0;       //��������ж� 
        TIM3->DIER|=1<<6;       //�������ж�  
        TIM3->CR1|=0x01;        //ʹ�ܶ�ʱ��

        NVIC_InitTypeDef        NVIC_Initstuct;
        NVIC_Initstuct.NVIC_IRQChannel = TIM3_IRQn;
        NVIC_Initstuct.NVIC_IRQChannelPreemptionPriority = 1;   //�����ȼ�
        NVIC_Initstuct.NVIC_IRQChannelSubPriority = 1;          //�����ȼ�
        NVIC_Initstuct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_Initstuct);

        NVIC_Initstuct.NVIC_IRQChannel = EXTI15_10_IRQn;
        NVIC_Init(&NVIC_Initstuct);

} 
/*
 * @brief ��λʱ���ȡ����������
 * @param ��ʱ��
 * @return �ٶ�
 */
int Read_Encoder(u8 TIMX)
{
        int Encoder_TIM;
        switch(TIMX)
        {
                case 2:  Encoder_TIM= (short)TIM2 -> CNT; break;
                case 3:  Encoder_TIM= (short)TIM3 -> CNT; break; 
                case 4:  Encoder_TIM= (short)TIM4 -> CNT; break; 
                default:  Encoder_TIM=0;
        }
        return Encoder_TIM;
}

/*
 * @brief ���õ���� PWM ��
 * @return null
 */
void Set_Pwm(int motor1)
{
        M1_Revolve(motor1);
}
/*
 * @brief �޶� PWM ���ķ�ֵ
 */
void Xianfu_Pwm(void)
{
        int Amplitude=7100;    //===PWM������7200 ������7100
        if(Motor1<-Amplitude) Motor1=-Amplitude; 
        if(Motor1>Amplitude)  Motor1=Amplitude; 
}

/*
 *  @brief λ��ʽPID������
 *  @param ����������λ����Ϣ��Ŀ��λ��
 *  @return ���PWM
 *  @note   ����λ��ʽ��ɢPID��ʽ 
 *  pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
 *  e(k)������ƫ�� 
 *  e(k-1)������һ�ε�ƫ��  
 *  ��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
 *  pwm�������
 */
int Position_PID (int Encoder,int Target)
{ 
        float Position_KP=80,Position_KI=0.1,Position_KD=500;
        static float Bias,Pwm,Integral_bias,Last_Bias;
        Bias=Encoder-Target;                                    //����ƫ��
        Integral_bias+=Bias;                                    //���ƫ��Ļ���
        Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
        Last_Bias=Bias;                                       //������һ��ƫ�� 
        return Pwm;                                           //�������
}

int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}

