#include "bsp_basictime.h"


/**
 * @brief �����ж����ȼ�
 * @param 
 */
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;        
                // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;        
                // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   
          // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**
 * @brief  ������ʱ����ģʽ����
 * @param  
 * @note   ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * @note   TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * @note   ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.  
 */

 /*-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
static void BASIC_TIM_Mode_Config(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
                
        // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
        BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);

        // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
        TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;    

        // ʱ��Ԥ��Ƶ��Ϊ
        TIM_TimeBaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;

        // ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
        //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
                
        // ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
        //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
                
        // �ظ���������ֵ��������ʱ��û�У����ù�
        //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

        // ��ʼ����ʱ��
        TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
                
        // ����������жϱ�־λ
        TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
          
        // �����������ж�
        TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
                
        // ʹ�ܼ�����
        TIM_Cmd(BASIC_TIM, ENABLE); 
}

/**
 * @brief ��ʼ��������ʱ��
 * @param 
 */
void BASIC_TIM_Init(void)
{
        BASIC_TIM_NVIC_Config();
        BASIC_TIM_Mode_Config();
}

