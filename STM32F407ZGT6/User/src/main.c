#include <stm32f4xx.h>               
#include "led.h"


void Delay(u32 t);

int main()
{
        LED_Init();
        KEY_Init();
        while(1)
        {
//                if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
//                {
//                        Delay(0XFFFF);
//                        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
//                        {
//                                GPIO_ResetBits(GPIOC,GPIO_Pin_0);
//                        }
//                        else 
//                        {
//                                GPIO_SetBits(GPIOC,GPIO_Pin_0);
//                        }
//                }
//                
//                if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
//                {
//                        Delay(0XFFFF);
//                        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
//                        {
//                                
//                                GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//                        }
//                        else 
//                        {
//                                GPIO_SetBits(GPIOF,GPIO_Pin_10);
//                        }
//                }
//                
//                if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0)
//                {
//                        Delay(0XFFFF);
//                        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0)
//                        {
//                                GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//                        }
//                        else 
//                        {
//                                GPIO_SetBits(GPIOF,GPIO_Pin_9);
//                        }
//                }
//                
//                if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
//                {
//                        Delay(0XFFFF);
//                        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
//                        {
//                                GPIO_ResetBits(GPIOF,GPIO_Pin_6);
//                        }
//                        else 
//                        {
//                                GPIO_SetBits(GPIOF,GPIO_Pin_6);
//                        }
//                }
                if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
                {
                        Delay(0XFFFFF);
                        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
                        {
                                GPIO_ToggleBits(GPIOC,GPIO_Pin_0);
                        }
                        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1);
                }
                if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
                {
                        Delay(0XFFFFF);
                        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
                        {
                                GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
                        }
                        while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0);
                }
                if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0)
                {
                        Delay(0XFFFFF);l.;xd'
                        
                        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0)
                        {
                                GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
                        }
                        while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0);
                }
                if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
                {
                        Delay(0XFFFFF);
                        if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0)
                        {
                                GPIO_ToggleBits(GPIOF,GPIO_Pin_6);
                        }
                        while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0);
                }
        }
}

void Delay(u32 t)
{
        while(t)
        {
                t--;
        }
}
















