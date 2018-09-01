#include "bsp_timer.h"
#include "main.h"

static u16 TIM7_COUNT;


void TIM7_Init(void)
{  
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //生成用于定时器设置的结构体

        //定时器初始化 使用基本定时器TIM7
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);   //使能对应RCC时钟
        //配置定时器基础结构体

        TIM_TimeBaseStructure.TIM_Period = (1000-1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值         计数到1000为1ms
        TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位         

        TIM_ClearFlag(TIM7, TIM_FLAG_Update);   //清除更新中断，免得一打开中断立即产生中断
        TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);    //打开定时器更新中断


        MY_NVIC_Init(1,1,TIM7_IRQn);

        TIM_Cmd(TIM7,ENABLE);     
}

void TIM7_IRQHandler (void )
{
        if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM7更新中断发生与否
        {
               TIM7_COUNT++;
               if(TIM7_COUNT>=50)
               {
                       TIM7_COUNT=0;
                       Distance = Hcsr04GetLength();
//                       printf("[%s][%d]\r\n", __func__, __LINE__ );
               }
        }
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除TIMx更新中断标志 
}

