
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

void led_init(void);
void led_light_color(u32 rgb);

#endif /* __BSP_LED_H */

