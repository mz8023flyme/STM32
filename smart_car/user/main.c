#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "led.h"
#include "24l01.h"
#include "bsp_spi.h"

//int mode =0;


int main (void )
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        spi_Init();
        NRF24L01_Init();
        u8 tmp_buf[33];
        if(NRF24L01_Check())
        {
                 printf("error");
                 delay_ms(500);
        }
        else
        {
               printf("success");
        }
        while(1)
        {
                 
        }
        
        
        
}
//int main(void)
//{ 
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//        
//        systick_init();
//        usart_init();
//        
//        u16 t=0;			 
//        u8 tmp_buf[33];

//        NRF24L01_Init();    //初始化NRF24L01 
//        while(NRF24L01_Check())//检测不到24L01
//        {
//                delay_ms(500);
//                LED0=!LED0;//DS0闪烁
//        }
//        while(1)//在该部分确定进入哪个模式!
//        {

//        
//        if(mode==0)//RX模式
//        {
//                RX_Mode();		  
//                while(1)
//                {	  		    		    				 
//                        if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
//                        {
//                                tmp_buf[32]=0;//加入字符串结束符
//                        }else delay_us(100);	   
//                        t++;
//                        if(t==10000)//大约1s钟改变一次状态
//                        {
//                                t=0;
//                                LED0=!LED0;
//                        } 				    
//                };	
//        }else//TX模式
//        {							    
//                LCD_ShowString(60,150,"NRF24L01 TX_Mode");	
//                TX_Mode();
//                mode=' ';//从空格键开始  
//                while(1)
//                {	  		   				 
//                        if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
//                        {
//                                key=mode;
//                                for(t=0;t<32;t++)
//                                {
//                                        key++;
//                                        if(key>('~'))key=' ';
//                                        tmp_buf[t]=key;	
//                                }
//                                mode++; 
//                                if(mode>'~')mode=' ';  	  
//                                tmp_buf[32]=0;//加入结束符		   
//                        }else
//                        {									 
//                        };
//                        LED0=!LED0;
//                        delay_ms(1500);				    
//                };
//        }     	  
//}
//        }

        
