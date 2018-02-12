#include "bsp_key.h"
#include "bsp_systick.h"

void KEY_GPIO_Config (void)  //初始化
{
        GPIO_InitTypeDef        GPIO_Initstuct;
        RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK , ENABLE); 
        RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK , ENABLE);         //开时钟
        GPIO_Initstuct.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN ;
        GPIO_Initstuct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(KEY1_GPIO_PORT, &GPIO_Initstuct);
        GPIO_Init(KEY2_GPIO_PORT, &GPIO_Initstuct);
}

uint8_t Key_Scan(GPIO_TypeDef*GPIOx,uint16_t GPIO_Pin)
{
        if ( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON )
        {
                delay_ms(10);
                if ( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON )
                {
                        while ( GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)== KEY_ON );  //松手检测
                        return KEY_ON;
                }
                else return KEY_OFF;
        }
        
}

