#ifndef _BSP_BASETIM_H_
#define _BSP_BASETIM_H_

#define Base_Tim_x              TIM7
#define Base_TIM_Prescaler      71
#define Base_TIM_Period         (10000-1)


void Base_Tim_Init(void);


#endif /* _BSP_BASETIM_H_ */
