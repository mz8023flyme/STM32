#include "main.h"

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        systick_init();
        usart_init();
        
        while(1)
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                delay_ms(1000);
        }
}
