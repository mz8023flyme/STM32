#ifndef  __MAIN_H_
#define  __MAIN_H_

#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_lcd.h"
#include "fonts.h"
#include "bsp_spi.h"
#include "bsp_basictime.h"
#include "bsp_tetris.h"
#include "bsp_adc.h"
#include "bsp_key.h"
#include "bsp_exit.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>


extern u8 Down_Flag;
extern u8 Pause_Flag;
extern u8 Game_Over_flag;
extern u16 Score;
u8 Make_Random (void);
void Key_Board_Scan(void);
void Reset_Game(void);

#endif   /* __MAIN_H_ */


