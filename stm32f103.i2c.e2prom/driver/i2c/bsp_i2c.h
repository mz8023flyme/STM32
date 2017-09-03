<<<<<<< HEAD
#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

typedef struct
{
        volatile unsigned long* SCL;
        volatile unsigned long* SDA;
        volatile unsigned long* REC;
        u32 SCLGpioClk;
        u32 SDAGpioClk;
        GPIO_TypeDef* SCLGpioPort;
        GPIO_TypeDef* SDAGpioPort;
        u16 SCLGpioPin;
        u16 SDAGpioPin;
        u8 SCLPinNum;
        u8 SDAPinNum;
        u8 Speed;
} I2C_GpioDef;

u8 i2c_get_pin_num(u16 GPIO_Pin_x);
void i2c_init(I2C_GpioDef* I2Cx);

void i2c_set_sda_input(I2C_GpioDef* I2Cx);
void i2c_set_sda_output(I2C_GpioDef* I2Cx);
void i2c_start(I2C_GpioDef* I2Cx);
void i2c_stop(I2C_GpioDef* I2Cx);
int i2c_wait_response(I2C_GpioDef* I2Cx);
void i2c_response(I2C_GpioDef* I2Cx);
void i2c_no_response(I2C_GpioDef* I2Cx);
void i2c_send_byte(I2C_GpioDef* I2Cx, u8 data);
u8 i2c_receive_byte(I2C_GpioDef* I2Cx);

#endif /* __BSP_I2C_H */
=======
#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

typedef struct
{
        volatile unsigned long* SCL;
        volatile unsigned long* SDA;
        volatile unsigned long* REC;
        u32 SCLGpioClk;
        u32 SDAGpioClk;
        GPIO_TypeDef* SCLGpioPort;
        GPIO_TypeDef* SDAGpioPort;
        u16 SCLGpioPin;
        u16 SDAGpioPin;
        u8 SCLPinNum;
        u8 SDAPinNum;
        u8 Speed;
} I2C_GpioDef;

u8 i2c_get_pin_num(u16 GPIO_Pin_x);
void i2c_init(I2C_GpioDef* I2Cx);

void i2c_set_sda_input(I2C_GpioDef* I2Cx);
void i2c_set_sda_output(I2C_GpioDef* I2Cx);
void i2c_start(I2C_GpioDef* I2Cx);
void i2c_stop(I2C_GpioDef* I2Cx);
int i2c_wait_response(I2C_GpioDef* I2Cx);
void i2c_response(I2C_GpioDef* I2Cx);
void i2c_no_response(I2C_GpioDef* I2Cx);
void i2c_send_byte(I2C_GpioDef* I2Cx, u8 data);
u8 i2c_receive_byte(I2C_GpioDef* I2Cx);

#endif /* __BSP_I2C_H */
>>>>>>> updata
