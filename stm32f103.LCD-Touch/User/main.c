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
        
//        TOUCH_Init();
        
        USART_Config ();
        
//        LED_GPIO_Config();
//        
//        BEEP_GPIO_Config();
        
        Screen_Init();
        
        printf("\r\n ����һ��LCD����ʵ�� \r\n");
        
        LCD_SetColors(RED,WHITE);
        
//        LCD_DispString_CH(88,50,"��������");
        LCD_DrawRectangle(29,100,60,50,0);
        LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"��������",0);
        /*�Ƿ�ǿ��У׼����flash���в�����У׼
        ����һΪɨ�跽ʽ������1Ϊǿ��У׼0��ǿ��У׼*/
//        Calibrate_or_Get_TouchParaWithFlash(6,0);
//        LCD_DispString_CH(0,0,"��");
        
//        LCD_DrawLine(30,100,30,50);
        
//        LCD_Touch_Calibrate(3);//��ĻУ׼
        while(1)
        {
//                LCD_TouchEvenHandler();
                
                
                
//                if (touch_detect() == TOUCH_PRESSED )
//                {
//                        printf("\r\n����������");
//                        
//                        Touch_SendCMD(0x90);
//                        Delay(0xF);
//                        test = Touch_ReceiveData();
//                
//                        printf("\r\n value_test = 0x%x",test);
//                }
//                else
//                {
//                        printf("\r\n����δ����");                
//                }
//                
//                Delay(0x1FFFF);
        }
        
}


