#ifndef  __BSP_TSL2561_H
#define  __BSP_TSL2561_H

#include "bsp_systick.h"


//Timing Register Value.Set integration time  
                                      //�����λ���û���ʱ��  
#define  TIMING_13MS         0x00     //����ʱ��13.7����  
#define  TIMING_101MS        0x01     //����ʱ��101����  
#define  TIMING_402MS        0x02     //����ʱ��402����  
#define  TIMING_GAIN_1X      0x10     //���汶�������ʱ����л�����  
#define  TIMING_GAIN_16X     0x00 

// TSL2561 registers  
  
#define TSL2561_CMD           0x80  
#define TSL2561_TIMING        0x81  
#define TSL2561_REG_ID        0x8A  
#define TSL2561_DATA0_LOW     0x8C  
#define TSL2561_DATA0_HIGH    0x8D  
#define TSL2561_DATA1_LOW     0x8E  
#define TSL2561_DATA1_HIGH    0x8F  

#define TSL2561_ON   0x03
#define TSL2561_OFF  0x00
#define INTERRUPT     0xC6 
#define TSL2561_ADDR0 0x29 // address with '0' shorted on board  
#define TSL2561_ADDR  0x39 // default address  
#define TSL2561_ADDR1 0x49 // address with '1' shorted on board

#define SLAVE_ADDR_WR 0x72 //�����ֲ����������ĵ�ַ�����ǻ�Ҫ����<<1λ�������ڵĵ�ַ��
#define SLAVE_ADDR_RD 0x73 

extern float light_Min;
extern float light_Max;
void TSL2561_Write(u8 addr,u8 cmd);
u8 TSL2561_Read(u8 addr);
void TSL2561_Init(void);
float Read_Light(void );

#endif  /*__BSP_TSL2561_H*/
