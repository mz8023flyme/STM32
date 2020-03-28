#ifndef  __MAIN_H
#define  __MAIN_H

#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_usart2.h"
#include "bsp_dht11.h"
#include "bsp_led.h"
#include "bsp_TiMbase.h" 
#include <string.h>
#include "bsp_i2c.h"
#include "bsp_OLED.h"
#include "bsp_tsl2561.h"
#include "stm32f10x_it.h"
#include "codetab.h"
#include "bsp_spi.h"
#include "bsp_rc522.h"
#include "bsp_GeneralTim.h" 
#include "bsp_adc.h"


#define   Base                 0x8000000
#define   Pos_Address         (Base+1204*31)
#define   Card_Address        (Base+1204*32)

typedef enum
{ 
        Abnormal_Mode  = 1,     //�쳣ģʽ     
        General_Mode ,          //��ͨģʽ
        Entering_Mode           //¼��ģʽ
}Mode_TypeDef;

extern u8 USart8266_temp[200];
extern u8 USART2_IT_Flag;
extern u8 Time_Flag;
extern u8 Mode;
extern u8 Door_Flag;
extern u8 Temp_flag;
extern u8 BEEP_Flag;
extern u16 Administ_Time;
extern u8 Show_flag ;
extern u8 Show_num;
extern u8 Irrigation_flag ;   //����־
extern u8 Exhaust_flag ;      //�ŷ��־
extern float Illumination;
extern u8 Step;
extern u8 anomaly;
extern u16 time_mode;
extern u8  Administ_Flag;
extern u8  Administ_Entering;
void Temperature_System(void);
void Network_System(void);
void System_Init(void);
uint32_t RFID_Number ( void );
void The_System_First_Run( void );
void updata (void);

#endif  /*__MAIN_H*/

