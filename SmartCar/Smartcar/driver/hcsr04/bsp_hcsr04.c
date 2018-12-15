#include "bsp_hcsr04.h"
#include "main.h"

u16 msHcCount = 0;//ms计数

void Hcsr04Init(void)
{  
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //生成用于定时器设置的结构体
eDef GPIO_InitStructure;

        //GPIO初始化
        RCC_APB2Peri
        GPIO_InitTypphClockCmd(HCSR04_CLK, ENABLE);
        //TRIG   PB5   输出10us高电平
        GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       //发送电平引脚
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
        GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
        GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
        //ECHO   PB6   发射并接收，通过时间计算距离
        GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     //返回电平引脚
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
        GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
        GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    

        //定时器初始化 使用基本定时器TIM6
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   //使能对应RCC时钟
        //配置定时器基础结构体

        TIM_TimeBaseStructure.TIM_Period = (1000-1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值         计数到1000为1ms
        TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
        TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位         

        TIM_ClearFlag(TIM6, TIM_FLAG_Update);   //清除更新中断，免得一打开中断立即产生中断
        TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);    //打开定时器更新中断


        MY_NVIC_Init(0,1,TIM6_IRQn);

        TIM_Cmd(TIM6,DISABLE);     
}


//tips：static函数的作用域仅限于定义它的源文件内，所以不需要在头文件里声明
static void OpenTimerForHc()        //打开定时器
{
        TIM_SetCounter(TIM6,0);//清除计数
        msHcCount = 0;
        TIM_Cmd(TIM6, ENABLE);  //使能TIMx外设
}
 
static void CloseTimerForHc()        //关闭定时器
{
        TIM_Cmd(TIM6, DISABLE);  //使能TIMx外设
}

//定时器6中断服务程序
void TIM6_IRQHandler (void)   //TIM3中断
{       

        if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
        {
               
                msHcCount++;
        }
         TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx更新中断标志 
}
 

//获取定时器时间
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//得到MS
        t += TIM_GetCounter(TIM6);//得到US
        TIM6->CNT = 0;  //将TIM2计数寄存器的计数值清零
        delay_ms(50);
        return t;
}
 

//一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号
//为了消除余震的影响，取五次数据的平均值进行加权滤波。
float Hcsr04GetLength(void )
{
        u32 t = 0;
        int i = 0;
        float lengthTemp = 0;
        float sum = 0;
        while(i!=2)
        {

                TRIG_Send = 1;      //发送口高电平输出
                delay_us(20);
                TRIG_Send = 0;
                while(ECHO_Reci == 0);      //等待接收口高电平输出
                OpenTimerForHc();        //打开定时器
                i = i + 1;
                while(ECHO_Reci == 1);

                CloseTimerForHc();        //关闭定时器

                t = GetEchoTimer();        //获取时间,分辨率为1US

                lengthTemp = ((float)t/58.0);//cm

                sum = lengthTemp + sum ;

        }
        lengthTemp = sum/2.0;
//        printf("距离为:%.3f CM\n",lengthTemp);
        return lengthTemp;

}

















