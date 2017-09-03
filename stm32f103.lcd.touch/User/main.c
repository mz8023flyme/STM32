<<<<<<< HEAD
#include "stm32f10x.h"   
#include "bsp_usart.h"   
#include "bsp_lcd.h"
#include "fonts.h"
#include "bsp_spi.h"
#include <stdio.h>
#include "bsp_touch.h"
#include "bsp_led.h"

int main(void)
{                
//        uint16_t test=0;
        
        LCD_Init();
        
        TOUCH_Init();
        
        USART_Config ();
        
        LED_GPIO_Config();
        
        BEEP_GPIO_Config();
        
        Screen_Init();
        
        printf("\r\n 这是一个LCD触摸实验 \r\n");
        
        LCD_SetColors(RED,WHITE);
        
//        LCD_DispString_CH(88,50,"秉火物联");
        LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"秉火物联",0);
        /*是否强制校准，若flash里有参数则不校准
        参数一为扫描方式参数二1为强制校准0不强制校准*/
        Calibrate_or_Get_TouchParaWithFlash(6,0);
        
        
//        LCD_DrawLine(30,100,30,50);
        
//        LCD_Touch_Calibrate(3);//屏幕校准
        while(1)
        {
                LCD_TouchEvenHandler();
                
                
                
//                if (touch_detect() == TOUCH_PRESSED )
//                {
//                        printf("\r\n触摸被按下");
//                        
//                        Touch_SendCMD(0x90);
//                        Delay(0xF);
//                        test = Touch_ReceiveData();
//                
//                        printf("\r\n value_test = 0x%x",test);
//                }
//                else
//                {
//                        printf("\r\n触摸未按下");                
//                }
//                
//                Delay(0x1FFFF);
        }
        
}


=======
#include "stm32f10x.h"   
#include "bsp_usart.h"   
#include "bsp_lcd.h"
#include "fonts.h"
#include "bsp_spi.h"
#include <stdio.h>
#include "bsp_touch.h"
#include "bsp_led.h"

int main(void)
{                
//        uint16_t test=0;
        
        LCD_Init();
        
        TOUCH_Init();
        
        USART_Config ();
        
        LED_GPIO_Config();
        
        BEEP_GPIO_Config();
        
        Screen_Init();
        
        printf("\r\n 这是一个LCD触摸实验 \r\n");
        
        LCD_SetColors(RED,WHITE);
        
//        LCD_DispString_CH(88,50,"秉火物联");
        LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"秉火物联",0);
        /*是否强制校准，若flash里有参数则不校准
        参数一为扫描方式参数二1为强制校准0不强制校准*/
        Calibrate_or_Get_TouchParaWithFlash(6,0);
        
        
//        LCD_DrawLine(30,100,30,50);
        
//        LCD_Touch_Calibrate(3);//屏幕校准
        while(1)
        {
                LCD_TouchEvenHandler();
                
                
                
//                if (touch_detect() == TOUCH_PRESSED )
//                {
//                        printf("\r\n触摸被按下");
//                        
//                        Touch_SendCMD(0x90);
//                        Delay(0xF);
//                        test = Touch_ReceiveData();
//                
//                        printf("\r\n value_test = 0x%x",test);
//                }
//                else
//                {
//                        printf("\r\n触摸未按下");                
//                }
//                
//                Delay(0x1FFFF);
        }
        
}


>>>>>>> updata
