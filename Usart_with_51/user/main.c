#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_key.h"

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        LED_GPIO_Config ();
        KEY_GPIO_Config();
        systick_init();
        usart_init();
        extern u8 CH;
        printf ("test\n");
        printf("[%s][%d]", __func__, __LINE__);
        while(1)
        {
                switch (CH)
                {
                        case 'R': LED_R(ON);
                          break;
                        case 'r': LED_R(OFF);
                          break;
                        case 'B': LED_B(ON);
                          break;
                        case 'b': LED_B(OFF);
                          break;
                        case 'G': LED_G(ON);
                          break;
                        case 'g': LED_G(OFF);
                          break;
                        default: LED_OFF;
                          break;
                }
                if ( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
                {
                        USART_sent(USART1,'o');
                }
                if ( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON )
                {
                        USART_sent(USART1,'c');
                }
        }
}




