#include "bsp_GeneralTim.h" 

static void GENERAL_TIM_GPIO_Config(void) 
{
        GPIO_InitTypeDef GPIO_InitStructure;

        // ����Ƚ�ͨ��1 GPIO ��ʼ��
        RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
        GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN|GENERAL_TIM_CH4_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
        
}

///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *    TIM_CounterMode         TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (TIM_Period+1) = 72M/(TIM_Period+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(TIM_Period+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
        GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);

        /*--------------------ʱ���ṹ���ʼ��-------------------------*/
        // �������ڣ���������Ϊ100K

        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
        TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period; 
        // ����CNT��������ʱ�� = Fck_int/(psc+1)
        TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler; 
        // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
        // ����������ģʽ������Ϊ���ϼ���
        TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
        // �ظ���������ֵ��û�õ����ù�
        TIM_TimeBaseStructure.TIM_RepetitionCounter=0; 
        // ��ʼ����ʱ��
        TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

        u16 CCR3_Val = 0;
        u16 CCR4_Val = 0;
        
        /*--------------------����ȽϽṹ���ʼ��-------------------*/	

        TIM_OCInitTypeDef  TIM_OCInitStructure;
        // ����ΪPWMģʽ1 ��ģʽ�µ�����ֵС��CCRʱ����ߵ�ƽ��������͵�ƽ
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        // ���ʹ��
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        // ���ͨ����ƽ�������� 
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

        //��������ֵ�������������������ֵʱ����ƽ��������
        TIM_OCInitStructure.TIM_Pulse = CCR3_Val;  
        //����ʱ������ֵС��CCR3_ValʱΪ�ߵ�ƽ
        TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);

        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val;  
        //����ʱ������ֵС��CCR3_ValʱΪ�ߵ�ƽ
        TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
        
        // ʹ�ܼ�����
        TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init(void)
{
        GENERAL_TIM_GPIO_Config();
        GENERAL_TIM_Mode_Config(); 
}



//����CCR��ֵ�Ϳ��Կ��ƶ����λ��  ��ΧΪ5-25
/*********************************************END OF FILE**********************/
