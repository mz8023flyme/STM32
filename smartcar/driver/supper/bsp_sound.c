#include "bsp_sound.h"
#include "main.h"


void TIM3_Cap_Init(u16 arr,u16 psc)     
{       
        TIM_ICInitTypeDef  TIM3_ICInitStructure;
        GPIO_InitTypeDef GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能TIM3时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PB0 输入  
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;     
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     //PB1输出 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        //初始化定时器3 TIM3 
        TIM_TimeBaseStructure.TIM_Period = arr;         //设定计数器自动重装值 
        TIM_TimeBaseStructure.TIM_Prescaler =psc;       //预分频器   
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
        
        //初始化TIM3输入捕获参数
        TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;       //CC1S=03     选择输入端 IC3映射到TI1上
        TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
        TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
        TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  //配置输入分频,不分频 
        TIM3_ICInitStructure.TIM_ICFilter = 0x00;//配置输入滤波器 不滤波
        TIM_ICInit(TIM3, &TIM3_ICInitStructure);
        
        //中断分组初始化
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
        NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器  
        TIM_ClearFlag(TIM3, TIM_FLAG_Update|TIM_IT_CC3);
        TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC3,ENABLE);//允许更新中ww断 ,允许CC3IE捕获中断        
        TIM_Cmd(TIM3,ENABLE ); //使能定时器3
}
/**************************************************************************
函数功能：超声波接收回波函数
入口参数：无
返回  值：无
**************************************************************************/
u16 TIM3CH3_CAPTURE_STA,TIM3CH3_CAPTURE_VAL;


void Read_Distane(void)
{
        //printf("[%s][%d]\r\n", __func__, __LINE__);        
         PBout(1)=1;
         delay_us(20);  
         PBout(1)=0;    
                if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
                {
//                        Distance=TIM3CH3_CAPTURE_STA&0X3F;
//                        Distance*=65536;        //溢出时间总和
//                        Distance+=TIM3CH3_CAPTURE_VAL;  //得到总的高电平时间
//                        Distance=Distance*170/1000;
                        
                        Distance =  ((TIM3CH3_CAPTURE_STA&0X3F)*65536+TIM3CH3_CAPTURE_VAL)*170/1000;
                        
                        printf("distance   %d \r\n",Distance);
                        TIM3CH3_CAPTURE_STA=0;  //开启下一次捕获
                }
                printf("TIM3CH3_CAPTURE_VAL    %d\r\n",TIM3CH3_CAPTURE_VAL);
//                printf("[%s][%d]\r\n", __func__, __LINE__);
                
//                while(PBin(0)==1);
                
                        
                
                
                
}


void TIM3_IRQHandler(void)
{
        
}


//void TIM3_IRQHandler(void)
//{
//        u16 tsr;
//        tsr=TIM3->SR;
////        printf("[%s][%d]\r\n", __func__, __LINE__);
//        printf("TIM3CH3_CAPTURE_STA    %d\r\n",TIM3CH3_CAPTURE_STA);
//        if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获 
//        {
////                printf("[%s][%d]\r\n", __func__, __LINE__);
//                if(tsr&0X01)//溢出
//                {
////                printf("[%s][%d]\r\n", __func__, __LINE__);
//                        if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
//                        {
//                                if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//                                {
//                                        TIM3CH3_CAPTURE_STA|=0X80;//标记成功捕获了一次
//                                        TIM3CH3_CAPTURE_VAL=0XFFFF;
//                                }
//                                else 
//                                        TIM3CH3_CAPTURE_STA++;
//                        }
//                }
//                if(tsr&0x08)//捕获3发生捕获事件
//                {       
////                        printf("[%s][%d]\r\n", __func__, __LINE__);
//                        if(TIM3CH3_CAPTURE_STA&0X40)    //捕获到一个下降沿      
//                        {       
//                                TIM3CH3_CAPTURE_STA|=0X80;      //标记成功捕获到一次高电平脉宽
//                                TIM3CH3_CAPTURE_VAL=TIM3->CCR3;	//获取当前的捕获值.
//                                TIM3->CCER&=~(1<<9);    //CC1P=0 设置为上升沿捕获
//                        }
//                        else    //还未开始,第一次捕获上升沿
//                        {
//                                TIM3CH3_CAPTURE_STA=0;  //清空
//                                TIM3CH3_CAPTURE_VAL=0;
//                                TIM3CH3_CAPTURE_STA|=0X40;      //标记捕获到了上升沿
//                                TIM3->CNT=0;    //计数器清空
//                                TIM3->CCER|=1<<9;       //CC1P=1 设置为下降沿捕获
//                        }
//                }
//        }
//        TIM3->SR=0;//清除中断标志位    
//}
