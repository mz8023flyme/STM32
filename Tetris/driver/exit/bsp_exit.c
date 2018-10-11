#include "bsp_exit.h"


/**
 * @brief ��KEY1 ��KEY2 ���ó��ⲿ�ж�Դ �ĳ�ʼ��
 * @param 
 */
void EXTI_KEY_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStruct;
        EXTI_InitTypeDef  EXTI_InitStruct;
        
        //�����ж����ȼ�
        MY_NVIC_Init(2,3,EXTI9_5_IRQn);
        //��ʼ��GPIO
        RCC_APB2PeriphClockCmd(EXIT_CLK,ENABLE);
        
        GPIO_InitStruct.GPIO_Pin = KEY_DOWN_GPIO_PIN ;
        
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(KEY_DOWN_GPIO_PORT , &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin=KEY_PAUSE_GPIO_PIN;
        GPIO_Init(KEY_PAUSE_GPIO_PORT , &GPIO_InitStruct);
        
        //��ʼ��EXTI
        //���ⲿ�жϵ�ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
/*   ���·�ʽ��ʼ�� GPIOA �� GPIOC �ǽ� ���� GPIO �� 0�����ź�13 ���Ŷ���ʼ��   
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA | GPIO_PortSourceGPIOC , GPIO_PinSource0 | GPIO_PinSource13);
*/
        //������ѡIO��Ϊ�жϣ��¼����ߣ����ж�Դ
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
//        EXTI_InitStruct.EXTI_Line = EXTI_Line0;
        //���жϣ��¼����� �밴���� IO��ƥ��������������������Ϊ�ж�Դ
        EXTI_InitStruct.EXTI_Line = EXTI_Line9 | EXTI_Line7;
        EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
        //�����жϷ�ʽ����Ϊ�����ش���
        EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
        //ʹ���жϣ��¼�����
        EXTI_InitStruct.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStruct);

}





