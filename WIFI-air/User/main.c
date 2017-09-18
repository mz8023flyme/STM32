/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   WF-ESP8266 WiFi模块测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_infrared_send.h"
#include "bsp_GeneralTim.h" 
extern u8 tem;


u8 Code_Tem_26_off[CODE_SIZE] = {0xb2,0x4d,0x7b,0x84,0xe0,0x1f};
u8 Code_Tem_26_on[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0xd0,0x2f};
u8 Code_Tem_26_up[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0xa0,0x5f};
u8 Code_Tem_26_down[CODE_SIZE] = {0xb2,0x4d,0xbf,0x40,0x70,0x8f};

u8 on_off_flag = 0;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
        
        /* 初始化 */
        USARTx_Config ();                   //初始化串口1
        systick_init ();                    //配置 SysTick 为 1ms 中断一次 
        ESP8266_Init ();                    //初始化WiFi模块使用的接口和外设
        LED_GPIO_Config();
        /*初始化DTT11的引脚*/
        DHT11_Init ();
        //初始化定时器
        GENERAL_TIM_Init();
        printf ( "\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n" );                          //打印测试例程提示信息


        ESP8266_StaTcpClient_UnvarnishTest ();


        while ( 1 );


}


/*********************************************END OF FILE**********************/
