#include "bsp_motor.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/**
 * @brief 配置 TIM3 定时器输出 PWM 波
 * @return null
 * @note TIM3 的四个通道为 PA6、PA7、PB0、PB1
 */
static void PWM_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 ;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 配置电机普通 GPIO 引脚主要控制电机的正反转
 * @return null
 */
void MOTOR_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        PBout(12)=0;
        PBout(13)=0;
        PBout(14)=0;
        PBout(15)=0;

}
/**
 * @brief 配置 PWM 波的输出   配置定时器
 * @return null
 */
static void PWM_Mode_Config(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
        
        TIM_TimeBaseStructure.TIM_Prescaler =  71;         //时钟预分频
        TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;  //向上计数
        TIM_TimeBaseStructure.TIM_Period = 99;                    //自动重装值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //时钟分频1
        TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
        
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
        
        /*--------------------输出比较结构体初始化-------------------*/	
        // 占空比配置
        uint8_t CCR1_Val = 50;
        uint8_t CCR4_Val = 50;
        
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //模式一
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出通道电平极性设置
        
        /*配置定时器通道1*/
        TIM_OCInitStructure.TIM_Pulse = CCR1_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC1Init(TIM3,&TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
         /*配置定时器通道4*/
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC4Init(TIM3,&TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
        
        
        TIM_Cmd(TIM3,ENABLE);   //使能TIM3
}

/**
 * @brief PWM 初始化
 * @return null 
 */
void TIM3_PWM_Init(void) 
{
        PWM_GPIO_Config();
        PWM_Mode_Config();
        MOTOR_GPIO_Config();
}


/**
 * @brief 电机1的转动
 * @param VAL占空比计数值其范围是（-100,100）
 *        正数表示正转，负数表示反转
 * @return null
 * @note VAL的绝对值越大转速越快
 */
void M1_Revolve(int val)
{
        if (val>0)
        {
                PBout(14)=1;
                PBout(15)=0;
                TIM1->CCR1=val;
        }
        else
        {
                PBout(14)=0;
                PBout(15)=1;
                TIM1->CCR1=abs(val);
        }

}
/**
 * @brief 电机2的转动
 * @param VAL占空比计数值其范围是（-100,100）
 *        正数表示正转，负数表示反转
 * @return null
 * @note VAL的绝对值越大转速越快
 */
void M2_Revolve(int val)
{
        if (val>0)
        {
                PBout(12)=1;
                PBout(13)=0;
                TIM3->CCR4=val;
        }
        else
        {
                PBout(12)=0;
                PBout(13)=1;
                TIM3->CCR4=abs(val);
        }

}
/**
 * @brief 绝对值函数
 * @param 需要取绝对值的数
 * @return 绝对值
 */

int abs (int val)
{
        int temp;
        if(val<0)
        temp=-val;
        else
        temp=val;
        return temp;
}




