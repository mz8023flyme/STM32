#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_control.h"
#include "DataScope_DP.h"

extern int Encoder;

void test_control (void)
{
////        Encoder=Read_Encoder(4);
//        printf("Encoder:%d \r\n",Encoder);
        data_scope_wave(2,(float)Encoder);
        delay_ms(20);
}



