#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_systick.h"
//#include "codetab.h"
#include "bsp_usart.h"

void test_oled (void)
{
        unsigned char i;
        OLED_Init();
        IIC_Init();
        
        while(1)
        {
//                OLED_Fill(0xFF);//È«ÆÁµãÁÁ
//                delay_ms(2000);
                OLED_Fill(0x00);//È«ÆÁÃð
                delay_ms(200);
                for(i=0;i<4;i++)
                {
                        OLED_ShowCN(30+i*16,0,i);//²âÊÔÏÔÊ¾ÖÐÎÄ
                }
                delay_ms(2000);
                OLED_ShowStr(0,3,"Flying embeddedstudio",1);//²âÊÔ6*8×Ö·û
                OLED_ShowStr(0,4,"Hello Studio ",2);       //²âÊÔ8*16×Ö·û
                delay_ms(2000);
                OLED_CLS();//ÇåÆÁ
                
        }
}

