<<<<<<< HEAD
#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_at24c02.h"
#include "bsp_i2c.h"
#include <string.h>
#include <stdlib.h>

extern uint8_t USART_RX_DATA[];
extern uint8_t USART_TI;
extern uint8_t i;
int main(void)
{
        char *p;
        int address=0;
        int data=0;
        char *cmd=NULL;
        uint8_t writebuff[256];
        uint8_t readbuff[256];
        systick_init();
        usart_init();
        at24c02_init();
        led_init();
		printf("\r\niic  test\r\n");
        while (1)
        {      

                if(USART_TI)
                {
                        Usart_SendString(USART1, USART_RX_DATA);
                        printf ("  \r\n");
                        printf("receive  cmd\r\n");
                        p = strtok((char*)USART_RX_DATA, " ");
                        if(p)
                        {
                                cmd = p;
                        }
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                address = atoi(p);
                        }
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                data = atoi(p);
                        }
                        if(!strcmp("write", cmd)) 
                        {
                               writebuff[0]=data;
                               at24c02_byte_write(address,writebuff);
                               printf("write  succeed");
                        }
                        if(!strcmp("read", cmd))
                        {
                                at24c02_random_read(address,readbuff );
                                printf("the %d address's data =  %d \n ", address, readbuff[0]);
                        }   
                        //Usart_SendString(USART1, USART_RX_DATA);
                        i=0;
                        USART_TI=0;
                }      
        }
}
=======
#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_at24c02.h"
#include "bsp_i2c.h"
#include <string.h>
#include <stdlib.h>

extern uint8_t USART_RX_DATA[];
extern uint8_t USART_TI;
extern uint8_t i;
int main(void)
{
        char *p;
        int address=0;
        int data=0;
        char *cmd=NULL;
        uint8_t writebuff[256];
        uint8_t readbuff[256];
        systick_init();
        usart_init();
        at24c02_init();
        led_init();
	printf("\r\niic  test\r\n");
        while (1)
        {      

                if(USART_TI)
                {
                        Usart_SendString(USART1, USART_RX_DATA);
                        printf ("  \r\n");
                        printf("receive  cmd\r\n");
                        p = strtok((char*)USART_RX_DATA, " ");
                        if(p)
                        {
                                cmd = p;
                        }
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                address = atoi(p);
                        }
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                data = atoi(p);
                        }
                        if(!strcmp("write", cmd)) 
                        {
                               writebuff[0]=data;
                               at24c02_byte_write(address,writebuff);
                               printf("write  succeed");
                        }
                        if(!strcmp("read", cmd))
                        {
                                at24c02_random_read(address,readbuff );
                                printf("the %d address's data =  %d \n ", address, readbuff[0]);
                        }   
                        //Usart_SendString(USART1, USART_RX_DATA);
                        i=0;
                        USART_TI=0;
                }      
        }
}
>>>>>>> updata
