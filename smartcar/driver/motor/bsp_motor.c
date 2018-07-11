#include "bsp_motor.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/**
 * @brief 配置 TIM8 定时器输出 PWM 波
 * @return null
 * @note TIM3 的四个通道为 PC6、PC7、PC8、PC9
 */
static void PWM_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(MOTOR_PWM_CLOCK, ENABLE);

        GPIO_InitStructure.GPIO_Pin =  MOTOR1_PWM_PIN | MOTOR2_PWM_PIN|Steer_PWM_PIN ;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        
}

/**
 * @brief 配置电机普通 GPIO 引脚主要控制电机的正反转
 * @return null
 */
void MOTOR_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(MOTOR_DIR_CLOCK, ENABLE);  
        
        GPIO_InitStructure.GPIO_Pin =  MOTOR1_DIR_PIN1 | MOTOR1_DIR_PIN2 | MOTOR2_DIR_PIN1 | MOTOR2_DIR_PIN2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOE, &GPIO_InitStructure);
        
        MOTOR1_PIN1 = 0 ;
        MOTOR1_PIN2 = 0 ;
        MOTOR2_PIN1 = 0 ; 
        MOTOR2_PIN2 = 0 ;
                      
}
/**
 * @brief 配置 PWM 波的输出   配置定时器
 * @return null
 */
static void PWM_Mode_Config(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
        
        TIM_TimeBaseStructure.TIM_Prescaler =  7199;         //时钟预分频
        TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;  //向上计数
        TIM_TimeBaseStructure.TIM_Period = 200;                    //自动重装值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //时钟分频1
        TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
        
        TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
        
        /*--------------------输出比较结构体初始化-------------------*/	
        // 占空比配置
        uint8_t CCR1_Val = 0;
        uint8_t CCR2_Val = 0;
        uint8_t CCR4_Val = 0;
        
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //模式一
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出通道电平极性设置
//        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;    // 输出通道空闲电平极性配置   可以刹车
//        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        
        /*配置定时器通道1*/
        TIM_OCInitStructure.TIM_Pulse = CCR1_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC1Init(TIM8,&TIM_OCInitStructure);     
        TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
        TIM_OCInitStructure.TIM_Pulse = CCR2_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC2Init(TIM8,&TIM_OCInitStructure);     
        TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
         /*配置定时器通道4*/
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC4Init(TIM8,&TIM_OCInitStructure);
        TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
        
        
        /*------------------- 化-------------------*/
        // 有关刹车和死区结构体的成员具体可参考BDTR寄存器的描述
        TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
        TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
        TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
        TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
        // 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述
        // 这里配置的死区时间为152ns
        TIM_BDTRInitStructure.TIM_DeadTime = 11;
        TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
        // 当BKIN引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
        TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
        TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
        TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);
        
        
        TIM_CtrlPWMOutputs(TIM8, ENABLE);
        TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
        
        TIM_Cmd(TIM8,ENABLE);   //使能TIM3
       
}

/**
 * @brief PWM 初始化
 * @return null 
 */
void TIM8_PWM_Init(void) 
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
                MOTOR1_PIN1=1;
                MOTOR1_PIN2=0;
                TIM8->CCR2=val;
        }
        else
        {
                MOTOR1_PIN1=0;
                MOTOR1_PIN2=1;
                TIM8->CCR2=abs(val);
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
                MOTOR2_PIN1=1;
                MOTOR2_PIN2=0;
                TIM8->CCR4=val;
                
        }
        else
        {
                MOTOR2_PIN1=0;
                MOTOR2_PIN2=1;
                TIM8->CCR4=abs(val);
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




