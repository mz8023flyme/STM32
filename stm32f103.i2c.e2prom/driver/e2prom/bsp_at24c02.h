<<<<<<< HEAD
#ifndef __BSP_AT24C02_H
#define __BSP_AT24C02_H

#include "stm32f10x.h"

/* AT24C02对应的器件地址为1010000x */
#define AT24C02_READ      0xA1            // 前7位为固定器件地址, 最后一位为1表示读
#define AT24C02_WRITE     0xA0            // 前7位为固定器件地址, 最后一位为0表示写

void at24c02_init(void);
int at24c02_set_current_address(u8 address);
int at24c02_byte_write(u8 address, u8* buffer);
int at24c02_page_write(u8 address, u8* buffer, u8 length);
int at24c02_current_address_read(u8* buffer);
int at24c02_random_read(u8 address, u8* buffer);
int at24c02_sequentia_read(u8* buffer, u16 length);

#endif /* __BSP_AT24C02_H */
=======
#ifndef __BSP_AT24C02_H
#define __BSP_AT24C02_H

#include "stm32f10x.h"

/* AT24C02对应的器件地址为1010000x */
#define AT24C02_READ      0xA1            // 前7位为固定器件地址, 最后一位为1表示读
#define AT24C02_WRITE     0xA0            // 前7位为固定器件地址, 最后一位为0表示写

void at24c02_init(void);
int at24c02_set_current_address(u8 address);
int at24c02_byte_write(u8 address, u8* buffer);
int at24c02_page_write(u8 address, u8* buffer, u8 length);
int at24c02_current_address_read(u8* buffer);
int at24c02_random_read(u8 address, u8* buffer);
int at24c02_sequentia_read(u8* buffer, u16 length);

#endif /* __BSP_AT24C02_H */
>>>>>>> updata
