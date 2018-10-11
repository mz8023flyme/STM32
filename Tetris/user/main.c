#include "main.h"


//u8 Random;

Shape Shape_Structer={0};
u8 Down_Flag=0;
u8 Pause_Flag=0;


int main(void)
{
        
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        ADCx_Init();
        LCD_Init();
        printf("[%s][%d]\r\n", __func__, __LINE__);
        LCD_SetColors(RED,BLACK);
        LCD_Clear(0,0,240,320,GREY);   //����
        LCD_DrawLine(0,49,151,49);      //������Ϸ��
        LCD_DrawLine(151,48,151,320);   //������Ϸ��
        LCD_SetTextColor(BLUE);
        LCD_DisplayStringEx(40,10,32,32,(uint8_t *)"����˹����",0);
        
        
        Shape_Structer.CurNum = Make_Random();
        Shape_Structer.NextNum = Make_Random();
        
        BASIC_TIM_Init();
        KEY_GPIO_Config();
        EXTI_KEY_Init();
        Creat_Shap( );
        while(1)
        {
                
                LCD_Tetris_Show();
                Key_Board_Scan();
//                printf("[%s][%d]\r\n", __func__, __LINE__);
//                printf("\n\n\n\n");
                delay_ms(100);
        }
}



void Key_Board_Scan(void)
{
//        static u8 Pause_Flag=0;//1 ��ͣ 0��ʼ
        
        if(Key_Scan(KEY_LEFT_GPIO_PORT,KEY_LEFT_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_LEFT_GPIO_PORT,KEY_LEFT_GPIO_PIN)==KEY_ON)
                {
                        //���ƺ���
                        if(!Pause_Flag)
                        Tetris_Left();
                }
        }
        
        if(Key_Scan(KEY_RIGHT_GPIO_PORT,KEY_RIGHT_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_RIGHT_GPIO_PORT,KEY_RIGHT_GPIO_PIN)==KEY_ON)
                {
                        //���ƺ���
                        if(!Pause_Flag)
                        Tetris_Right();
                }
        }
        /*
        if(Key_Scan(KEY_DOWN_GPIO_PORT,KEY_DOWN_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_DOWN_GPIO_PORT,KEY_DOWN_GPIO_PIN)==KEY_ON)
                {
                        //�����½�����
                        Down_Flag=1;
//                        printf("[%s][%d]\r\n", __func__, __LINE__);
                }
//                Down_Flag=0;
        }
//        Down_Flag=0;
        if(Key_Scan(KEY_PAUSE_GPIO_PORT,KEY_PAUSE_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_PAUSE_GPIO_PORT,KEY_PAUSE_GPIO_PIN)==KEY_ON)
                {
                        //��ͣ����
                        Pause_Flag=!Pause_Flag;
                        if(Pause_Flag)
                                TIM_Cmd(BASIC_TIM, DISABLE);
                        else
                                TIM_Cmd(BASIC_TIM, ENABLE);
                }
        }
        */
        if(Key_Scan(KEY_RESET_GPIO_PORT,KEY_RESET_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_RESET_GPIO_PORT,KEY_RESET_GPIO_PIN)==KEY_ON)
                {
                        //��λ����
                        Clean_Reset();
                        Pause_Flag=0;
                        TIM_Cmd(BASIC_TIM, ENABLE);
                }
        }

        if(Key_Scan(KEY_ROTATE_GPIO_PORT,KEY_ROTATE_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_ROTATE_GPIO_PORT,KEY_ROTATE_GPIO_PIN)==KEY_ON)
                {
                        //���κ���
                        Tetris_Rotate();
                }
        }
}

/**
 * @brief ����һ�������
 * @retval ����һ�������   0-19
 */
u8 Make_Random (void)
{
        srand(ADC_ConvertedValue);
        return (rand()%19);
}





