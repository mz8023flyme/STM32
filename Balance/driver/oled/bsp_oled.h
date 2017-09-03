#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"

#define OLED_ADDRESS   0x78   //OLEDœ‘ æ∆¡µƒµÿ÷∑


int WriteCmd(u8 cmd);
int  WriteDat(u8 data );
void OLED_Init(void);
void OLED_SetPos(u8 x, u8 y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(u8 x, u8 y, u8 ch[], u8 TextSize);
void OLED_ShowCN(u8 x, u8 y, u8 N);

void test_oled (void);


#endif /*__BSP_OLED_H*/


