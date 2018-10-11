#include "bsp_key.h"

void KEY_GPIO_Config()
{
        GPIO_InitTypeDef  GPIO_InitStruct;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
        
        
        GPIO_InitStruct.GPIO_Pin = KEY_LEFT_GPIO_PIN;
        GPIO_Init(KEY_LEFT_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Pin = KEY_RIGHT_GPIO_PIN;
        GPIO_Init(KEY_RIGHT_GPIO_PORT, &GPIO_InitStruct);
        /*
        GPIO_InitStruct.GPIO_Pin = KEY_DOWN_GPIO_PIN;
        GPIO_Init(KEY_DOWN_GPIO_PORT, &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin = KEY_PAUSE_GPIO_PIN;
        GPIO_Init(KEY_PAUSE_GPIO_PORT, &GPIO_InitStruct);
        */
        GPIO_InitStruct.GPIO_Pin = KEY_RESET_GPIO_PIN;
        GPIO_Init(KEY_RESET_GPIO_PORT, &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin = KEY_ROTATE_GPIO_PIN;
        GPIO_Init(KEY_ROTATE_GPIO_PORT, &GPIO_InitStruct);
}

uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
        if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
        {
                //À… ÷ºÏ≤‚
//                while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
                return KEY_ON;
        }
        else return KEY_OFF;
}








