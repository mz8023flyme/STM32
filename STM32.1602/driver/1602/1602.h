#ifndef __1602_H
#define __1602_H

#include "stm32f10x.h"



void LCD_Show(int y,int x,char *show);
int  Send_command(char cmd);
int LCD_Send_byte(char data);
void LCD_Init(void);

#endif /*__1602_H*/
