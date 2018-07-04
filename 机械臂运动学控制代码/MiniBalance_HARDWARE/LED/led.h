#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED 端口定义

#define ON  0
#define OFF 1
#define BEEP(a) if(a) \
											GPIO_SetBits(GPIOB,GPIO_Pin_13); \
								else \
											GPIO_ResetBits(GPIOB,GPIO_Pin_13)
								
#define	Beep PBout(13)  //选中FLASH										

void Beep_Init(void);
void Beep_Test(void);
#define LED PBout(14)  
void LED_Init(void);  //初始化
void Led_Flash(u16 time);
#endif
