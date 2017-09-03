#include "bsp_key.h"
#include "bsp_systick.h"

static void TIM_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM5_Config(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        TIM_NVIC_Init();
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
        TIM_TimeBaseStructure.TIM_Period = 10000;
        TIM_TimeBaseStructure.TIM_Prescaler = 71;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
        TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
        TIM_Cmd(TIM5, ENABLE);
}

void Key_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(K1_GPIO_RCC|K2_GPIO_RCC|K3_GPIO_RCC|K4_GPIO_RCC, ENABLE); 

	GPIO_InitStructure.GPIO_Pin =  K1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //带上拉输入，空闲时为高电平，当按键按下输入为底
	GPIO_Init(K1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  K2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //带上拉输入，空闲时为高电平，当按键按下输入为底
	GPIO_Init(K2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  K3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //带上拉输入，空闲时为高电平，当按键按下输入为底
	GPIO_Init(K3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  K4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //带上拉输入，空闲时为高电平，当按键按下输入为底
	GPIO_Init(K4_GPIO_PORT, &GPIO_InitStructure);
}



/*
* @brief 扫描按键输入的状态，确定是否真的有按键按下，有则返回具体按键
* @param none
* @return 0  没有按键被按下
*         1  按键1被按下
*         2  按键2被按下
*         3  按键3被按下
*         4  按键4被按下
* @note 同一时间，只能一个按键被按下，该函数放在10ms中断中使用
*/
u8 Key_Scan_10ms(void)
{
        u8 key_state = 0;
        static u8 K1_CNT = 0,K2_CNT = 0,K3_CNT = 0,K4_CNT = 0; 
        if(GPIO_ReadInputDataBit(K1_GPIO_PORT,K1_GPIO_PIN) != SET)
        {
                K1_CNT++;
                if(K1_CNT > 2)
                {
                        while(GPIO_ReadInputDataBit(K1_GPIO_PORT,K1_GPIO_PIN) == RESET)
                                ;
                        K1_CNT = 0;
                        key_state = 1;
                }
        }
        else
        {
                K1_CNT = 0;
        }

        if(GPIO_ReadInputDataBit(K2_GPIO_PORT,K2_GPIO_PIN) != SET)
        {
                K2_CNT++;
                if(K2_CNT > 2)
                {
                        while(GPIO_ReadInputDataBit(K2_GPIO_PORT,K2_GPIO_PIN) != SET)
                                ;
                        K2_CNT = 0;
                        key_state = 2;
                }
        }
        else
        {
                K2_CNT = 0;
        }
                
        if(GPIO_ReadInputDataBit(K3_GPIO_PORT,K3_GPIO_PIN) != SET)
        {
                K3_CNT++;
                if(K3_CNT > 2)
                {
                        while(GPIO_ReadInputDataBit(K3_GPIO_PORT,K3_GPIO_PIN) != SET)
                                ;
                        K3_CNT = 0;
                        key_state = 3;
                }
        }
        else
        {
                K3_CNT = 0;
        }
                
        if(GPIO_ReadInputDataBit(K4_GPIO_PORT,K4_GPIO_PIN) != SET)
        {
                K4_CNT++;
                if(K4_CNT > 2)
                {
                        while(GPIO_ReadInputDataBit(K4_GPIO_PORT,K4_GPIO_PIN) != SET)
                                ;
                        K4_CNT = 0;
                        key_state = 4;
                }
        }
        else
        {
                K4_CNT = 0;
        }
        return key_state;
}

/*
* @brief 扫描按键输入的状态，确定是否真的有按键按下，有则返回具体按键
* @param none
* @return 0  没有按键被按下
*         1  按键1被按下
*         2  按键2被按下
*         3  按键3被按下
*         4  按键4被按下
* @note 同一时间，只能一个按键被按下，该函数放在while(1) 循环中使用
*/
u8 Key_Scan(void)
{
        u8 key_state = 0;
        if(GPIO_ReadInputDataBit(K1_GPIO_PORT,K1_GPIO_PIN) != SET)
        {
                delay_ms(20);
                if(GPIO_ReadInputDataBit(K1_GPIO_PORT,K1_GPIO_PIN) != SET)
                {
                        while(GPIO_ReadInputDataBit(K1_GPIO_PORT,K1_GPIO_PIN) == RESET)
                                ;
                        key_state = 1;
                }
        }
        
        if(GPIO_ReadInputDataBit(K2_GPIO_PORT,K2_GPIO_PIN) != SET)
        {
                delay_ms(20);
                if(GPIO_ReadInputDataBit(K2_GPIO_PORT,K2_GPIO_PIN) != SET)
                {
                        while(GPIO_ReadInputDataBit(K2_GPIO_PORT,K2_GPIO_PIN) != SET)
                                ;
                        key_state = 2;
                }
        }
                
        if(GPIO_ReadInputDataBit(K3_GPIO_PORT,K3_GPIO_PIN) != SET)
        {
                delay_ms(20);
                if(GPIO_ReadInputDataBit(K3_GPIO_PORT,K3_GPIO_PIN) != SET)
                {
                        while(GPIO_ReadInputDataBit(K3_GPIO_PORT,K3_GPIO_PIN) != SET)
                                ;
                        key_state = 3;
                }
        }
                
        if(GPIO_ReadInputDataBit(K4_GPIO_PORT,K4_GPIO_PIN) != SET)
        {
                delay_ms(20);
                if(GPIO_ReadInputDataBit(K4_GPIO_PORT,K4_GPIO_PIN) != SET)
                {
                        while(GPIO_ReadInputDataBit(K4_GPIO_PORT,K4_GPIO_PIN) != SET)
                                ;
                        key_state = 4;
                }
        }
        return key_state;
}		
				
