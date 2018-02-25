#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "1602.h"
#include "bsp_at24c02.h"
int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        I2C_init();
        LCD_Init();
        int num=0;
        char NUM[5]={'a','b','c','d','e'};
        sprintf(NUM,"%d",num);
        LCD_Show(0,0,"  hello  world  ");
        LCD_Show(1,0,NUM);
        while(1)
        {
//                printf("[%s][%d]", __func__, __LINE__);
//                delay_ms(1000);
                delay_ms(1000);
                num+=1;
                printf("[%s][%d]", __func__, __LINE__);
                LCD_Show(0,0,"  hello  world  ");
                LCD_Show(1,0,NUM);
//                LCD_Show(1,0,NUM);
                
        }
}





//#include "stm32f10x.h"
//#include "bsp_systick.h"
//#include "bsp_usart.h"
//#include "bsp_at24c02.h"
//#include "bsp_i2c.h"
//#include <string.h>
//#include <stdlib.h>
//char buf[]={20,30};
//char buf2[]={0};
//int main(void)
//{
//        systick_init();
//        usart_init();
//        I2C_init();
//        
//        printf("\r\niic  test\r\n");
//        while (1)
//        {
//                 at24c02_byte_write(100,buf);
//                 at24c02_current_address_read(buf2);
//                printf("read=%d",buf2[0]);
//                delay_ms(500);
//        }
//}


