#include "bsp_control.h"
#include "bsp_motor.h"
#include "bsp_usart.h"

extern int Motor1;
extern int Encoder;
extern int Target_position;
extern float Menu_MODE;

/**
 * @brief 把TIM2初始化为编码器接口模式
 */
 
void Encoder_Init_TIM2(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PB端口时钟

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    //端口配置
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);               //根据设定参数初始化GPIOB

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
        TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = 10;
        TIM_ICInit(TIM2, &TIM_ICInitStructure);
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        //Reset counter
        TIM_SetCounter(TIM2,0);
        if(Menu_MODE==1)TIM_SetCounter(TIM2,10000);
        else             TIM_SetCounter(TIM2,0);
        TIM_Cmd(TIM2, ENABLE); 
}
 
 /**
 * @brief 把TIM4初始化为编码器接口模式
 */
 

void Encoder_Init_TIM4(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;    //端口配置
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
        GPIO_Init(GPIOB, &GPIO_InitStructure);                  //根据设定参数初始化GPIOB

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
        TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = 10;
        TIM_ICInit(TIM4, &TIM_ICInitStructure);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
        TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
        //Reset counter
        TIM_SetCounter(TIM4,10000);
        //TIM4->CNT = 10000; 
        TIM_Cmd(TIM4, ENABLE); 
}

/*
 * @brief TIM1 用于驱动电机的转动   配套 stm32c8t6
 * @param arr 自动重装载值
 * @param psc 分频因子
 */
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{ 
        RCC->APB2ENR|=1<<3;       //PORTB时钟使能   
        GPIOB->CRH&=0X0000FFFF;   //PORTB12 13 14 15推挽输出
        GPIOB->CRH|=0X22220000;   //PORTB12 13 14 15推挽输出  //初始化电机控制所需IO
        
        RCC->APB2ENR|=1<<11;       //使能TIM1时钟    
        RCC->APB2ENR|=1<<2;        //PORTA时钟使能     
        GPIOA->CRH&=0XFFFF0FF0;    //PORTA8 11复用输出
        GPIOA->CRH|=0X0000B00B;    //PORTA8 11复用输出
        TIM1->ARR=arr;             //设定计数器自动重装值 
        TIM1->PSC=psc;             //预分频器不分频
        TIM1->CCMR2|=6<<12;        //CH4 PWM1模式 
        TIM1->CCMR1|=6<<4;         //CH1 PWM1模式 
        TIM1->CCMR2|=1<<11;        //CH4预装载使能 
        TIM1->CCMR1|=1<<3;         //CH1预装载使能 
        TIM1->CCER|=1<<12;         //CH4输出使能 
        TIM1->CCER|=1<<0;          //CH1输出使能 
        TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
        TIM1->CR1=0x8000;          //ARPE使能 
        TIM1->CR1|=0x01;           //使能定时器1 
}
/*
 * @brief TIM3 定时器中断  10ms 每次   配套stm32c8t6
 * @param arr 自动重装载值
 * @param psc 分频因子
 */
void Timer3_Init(u16 arr,u16 psc)  
{  
        RCC->APB1ENR|=1<<1;     //TIM3时钟使能    
        TIM3->ARR=arr;          //设定计数器自动重装值   
        TIM3->PSC=psc;          //预分频器7200,得到10Khz的计数时钟
        TIM3->DIER|=1<<0;       //允许更新中断 
        TIM3->DIER|=1<<6;       //允许触发中断  
        TIM3->CR1|=0x01;        //使能定时器

        NVIC_InitTypeDef        NVIC_Initstuct;
        NVIC_Initstuct.NVIC_IRQChannel = TIM3_IRQn;
        NVIC_Initstuct.NVIC_IRQChannelPreemptionPriority = 1;   //主优先级
        NVIC_Initstuct.NVIC_IRQChannelSubPriority = 1;          //子优先级
        NVIC_Initstuct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_Initstuct);

        NVIC_Initstuct.NVIC_IRQChannel = EXTI15_10_IRQn;
        NVIC_Init(&NVIC_Initstuct);

} 
/*
 * @brief 单位时间读取编码器计数
 * @param 定时器
 * @return 速度
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
 * @brief 设置电机的 PWM 波
 * @return null
 */
void Set_Pwm(int motor1)
{
        M1_Revolve(motor1);
}
/*
 * @brief 限定 PWM 波的幅值
 */
void Xianfu_Pwm(void)
{
        int Amplitude=7100;    //===PWM满幅是7200 限制在7100
        if(Motor1<-Amplitude) Motor1=-Amplitude; 
        if(Motor1>Amplitude)  Motor1=Amplitude; 
}

/*
 *  @brief 位置式PID控制器
 *  @param 编码器测量位置信息，目标位置
 *  @return 电机PWM
 *  @note   根据位置式离散PID公式 
 *  pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
 *  e(k)代表本次偏差 
 *  e(k-1)代表上一次的偏差  
 *  ∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
 *  pwm代表输出
 */
int Position_PID (int Encoder,int Target)
{ 
        float Position_KP=80,Position_KI=0.1,Position_KD=500;
        static float Bias,Pwm,Integral_bias,Last_Bias;
        Bias=Encoder-Target;                                    //计算偏差
        Integral_bias+=Bias;                                    //求出偏差的积分
        Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
        Last_Bias=Bias;                                       //保存上一次偏差 
        return Pwm;                                           //增量输出
}

int Incremental_PI (int Encoder,int Target)
{ 	
   float Kp=20,Ki=30;	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}

