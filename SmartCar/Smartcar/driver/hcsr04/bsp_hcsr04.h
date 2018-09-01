#ifndef __BSP_HCSR04_H
#define __BSP_HCSR04_H

#include "stm32f10x.h"

/**
* TRIG    TIM6    PB1 ‰≥ˆ
* ECHO     PB0  ‰»Î

*/

#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_1
#define HCSR04_ECHO     GPIO_Pin_0

#define TRIG_Send  PBout(1) 
#define ECHO_Reci  PBin(0)

void Hcsr04Init(void);
float Hcsr04GetLength(void );







#endif  /*__BSP_HCSR04_H*/
