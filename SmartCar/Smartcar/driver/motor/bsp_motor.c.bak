#include "bsp_motor.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "main.h"

/**
 * @brief 配置 TIM8 定时器输出 PWM 波
 * @return null
 * @note TIM3 的四个通道为 PC6、PC7、PC8、PC9
 */
static void PWM_GPIO_Config(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(MOTOR_PWM_CLOCK, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin =  MOTOR1_PWM_PIN | MOTOR2_PWM_PIN |Steer1_PWM_PIN|Steer2_PWM_PIN;
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
        uint8_t CCR3_Val = 0;
        uint8_t CCR4_Val = 0;
        
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //模式一
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出通道电平极性设置
//        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;    // 输出通道空闲电平极性配置   可以刹车

        
        /*配置定时器通道1*/
        TIM_OCInitStructure.TIM_Pulse = CCR1_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC1Init(TIM8,&TIM_OCInitStructure);     
//        TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
        TIM_OCInitStructure.TIM_Pulse = CCR2_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC2Init(TIM8,&TIM_OCInitStructure);     
//        TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
        TIM_OCInitStructure.TIM_Pulse = CCR3_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC3Init(TIM8,&TIM_OCInitStructure);     
//        TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
        
         /*配置定时器通道4*/
        TIM_OCInitStructure.TIM_Pulse = CCR4_Val;    //配置CCR寄存器      占空比 = （CCR/ARR）
        TIM_OC4Init(TIM8,&TIM_OCInitStructure);
//        TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

        TIM_CtrlPWMOutputs(TIM8, ENABLE);
//        TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
        
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
 * @param VAL占空比计数值其范围是（-200,200）
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
 * @param VAL占空比计数值其范围是（-200,200）
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

/**
 * @brief  距离扫描函数
 * @retval 返回距最大的那个方向
 */
unsigned char Distance_Scan(void )
{
        u8 i,direction ;
        
        u16 max=0, distance[12]={0};
        
        for (i=0;i<6;i++)
        {
                TIM8->CCR3 = (15-i) ;
//                Distance=Hcsr04GetLength();
                distance[TIM8->CCR3-9]+=(u16)Distance;
                delay_ms(100);
        }
        for (i=0;i<12;i++)
        {
                TIM8->CCR3 = (i+9) ;
                distance[TIM8->CCR3-9]+=(u16)Distance;
                delay_ms(100);
        }
        for (i=0;i<6;i++)
        {
                TIM8->CCR3 = (21-i) ;
                distance[TIM8->CCR3-9]+=(u16)Distance;
                delay_ms(100);
        }
        
        for (i=0;i<12;i++)
        {
                printf("distance%d  %d  ",i,distance[i]);
        }
        for (i=0;i<12;i++)   //因为大舵机不能满转，所以小舵机的比较也就没有必要加上两边的范围
        {
                if(distance[i]>max)
                {
                        max=distance[i];
                        direction=i;
                }
        }
        
        if(max<70)
        {
                //发现被包围了没有找到可以出去的路只有倒车
                
        }
        
        printf("\n  max %d direction %d",max,direction+9);
        
        
        TIM8->CCR3 = 15 ;
        return (direction+9);
        
}

/**
 * @brief  因避障而转弯的函数
 * @param  direction 这个方向的距离最远
 */
void BZ_Turn(u8 direction)
{
        TIM8->CCR1 = direction;
        printf("\n   direction   %d",direction);
        
        M1_Revolve(-100);
        M2_Revolve(-100);
        delay_ms(10000);
        TIM8->CCR1 = 30-direction;
        printf("\n   30-direction   %d",(30-direction));
        M1_Revolve(100);
        M2_Revolve(100);
        delay_ms(10000);
        TIM8->CCR1=15;
}







