#include "stm32f10x.h"   
#include "bsp_usart.h"   
#include "bsp_lcd.h"
#include "fonts.h"
#include "bsp_spi.h"
#include <stdio.h>
#include "bsp_led.h"
#include "bsp_rtc.h"
#include "bsp_bmp.h"
#include "bsp_sdfs_app.h"
#include "bsp_sdio_sdcard.h"

struct rtc_time systmtime;
u8 time_flag = 0;
int main(void)
{                
//        uint16_t test=0;
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        RTC_NVIC_Config();
        
        USART_Config();
        
        RTC_CheckAndConfig(&systmtime);
        
        LCD_Init();
        
        USART_Config ();
        
        LED_GPIO_Config();
        
        BEEP_GPIO_Config();
        
        Sd_fs_init();
        
        LCD_SetColors(RED,WHITE);
        LCD_Clear(0,0,240,320);
//        LCD_DispString_CH(88,50,"秉火物联");
//        LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"秉火物联",0);
//         LCD_DisplayStringEx(72,30,32,32,(uint8_t *)"劳力士",0);
        /*是否强制校准，若flash里有参数则不校准
        参数一为扫描方式参数二1为强制校准0不强制校准*/
        Lcd_show_bmp(0, 0,"/1.bmp");    //显示BMP图片
//        LCD_DrawLine(30,100,30,50);
//        printf("[%s][%d]",__func__,__LINE__);
//        LCD_Touch_Calibrate(3);//屏幕校准
        while(1)
        {
                Time_Show(&systmtime);
                if(time_flag==1)
                {
                        LCD_Time_Show(&systmtime);
                }
        }
        
}


