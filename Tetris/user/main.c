#include "main.h"


//u8 Random;

Shape Shape_Structer={0};
u8 Down_Flag=0;
u8 Pause_Flag=0;
u8 Game_Over_flag=0;
u16 Score=0;

int main(void)
{
        char Score_str[5]={0};
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        ADCx_Init();
        LCD_Init();
        printf("[%s][%d]\r\n", __func__, __LINE__);
        LCD_SetColors(RED,BLACK);
        LCD_Clear(0,0,240,320,BLACK);   //清屏
        LCD_DrawLine(0,49,150,49);      //设置游戏区
        LCD_DrawLine(150,49,150,320);   //设置游戏区
        LCD_SetTextColor(BLUE);
        LCD_DisplayStringEx(40,10,32,32,(uint8_t *)"俄罗斯方块",0);
        LCD_DisplayStringEx(163,50,32,32,(uint8_t *)"分数",0);
        
        Shape_Structer.CurNum = Make_Random();
        Shape_Structer.NextNum = Make_Random();
        
        BASIC_TIM_Init();
        KEY_GPIO_Config();
        EXTI_KEY_Init();
        Creat_Shap();
        
        while(1)
        {
                Reset_Game();
                if(0==Game_Over_flag)
                {
                        LCD_Tetris_Show();
                        Key_Board_Scan();
                        sprintf(Score_str,"%04d",Score);
                        LCD_SetTextColor(RED);
                        LCD_DisplayStringEx(163,90,32,32,(uint8_t *)Score_str,0);
                        delay_ms(100);
                }
        }
}

void Key_Board_Scan(void)
{
//        static u8 Pause_Flag=0;//1 暂停 0开始
        
        if(Key_Scan(KEY_LEFT_GPIO_PORT,KEY_LEFT_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_LEFT_GPIO_PORT,KEY_LEFT_GPIO_PIN)==KEY_ON)
                {
                        //左移函数
                        if(!Pause_Flag)
                        Tetris_Left();
                }
        }
        
        if(Key_Scan(KEY_RIGHT_GPIO_PORT,KEY_RIGHT_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_RIGHT_GPIO_PORT,KEY_RIGHT_GPIO_PIN)==KEY_ON)
                {
                        //右移函数
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
                        //快速下降函数
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
                        //暂停函数
                        Pause_Flag=!Pause_Flag;
                        if(Pause_Flag)
                                TIM_Cmd(BASIC_TIM, DISABLE);
                        else
                                TIM_Cmd(BASIC_TIM, ENABLE);
                }
        }
        */
        

        if(Key_Scan(KEY_ROTATE_GPIO_PORT,KEY_ROTATE_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_ROTATE_GPIO_PORT,KEY_ROTATE_GPIO_PIN)==KEY_ON)
                {
                        //变形函数
                        if(!Pause_Flag)
                        Tetris_Rotate();
                }
        }
}

void Reset_Game(void)
{
        if(Key_Scan(KEY_RESET_GPIO_PORT,KEY_RESET_GPIO_PIN)==KEY_ON)
        {
                delay_ms(10);
                if(Key_Scan(KEY_RESET_GPIO_PORT,KEY_RESET_GPIO_PIN)==KEY_ON)
                {
                        //复位函数
                        Clean_Reset();
                        Score=0;
                        Pause_Flag=0;
                        Game_Over_flag=0;
                        TIM_Cmd(BASIC_TIM, ENABLE);
                }
        }
}

/**
 * @brief 产生一个随机数
 * @retval 返回一个随机数   0-19
 */
u8 Make_Random (void)
{
        srand(ADC_ConvertedValue);
        return (rand()%19);
}






