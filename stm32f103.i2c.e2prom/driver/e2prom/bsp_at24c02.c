<<<<<<< HEAD
#include "bsp_at24c02.h"
#include "bsp_systick.h"
#include "bsp_i2c.h"
#include "bsp_usart.h"

I2C_GpioDef i2c_at24c02;

/**
 * @brief 初始化AT24C02
 */
void at24c02_init()
{
        i2c_at24c02.SCLGpioPort = GPIOB;
        i2c_at24c02.SCLGpioPin = GPIO_Pin_6;
        i2c_at24c02.SDAGpioPort = GPIOB;
        i2c_at24c02.SDAGpioPin = GPIO_Pin_7;
        i2c_init(&i2c_at24c02);
}

int at24c02_set_current_address(u8 address)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write address fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 在指定的地址写一个字节
 * @param address 指定要写入的地址
 * @param buffer 要写入数据的数据缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_byte_write(u8 address, u8* buffer)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write address fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, *buffer);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write data fail line = %d \r\n", __FUNCTION__, __LINE__);
                i2c_stop(&i2c_at24c02);
                return -1;
        }
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 在指定的地址连续写入字节
 * @param address 指定要写入的首地址
 * @param buffer 要写入数据的数据缓存区
 * @param length 指定要写入的字节个数
 * @return u8 返回程序执行的状态
 * @note 此函数为页写入函数, 对于at24c02一页为8个字节
 */
int at24c02_page_write(u8 address, u8* buffer, u8 length)
{
        i2c_start(&i2c_at24c02);
        
        if((length <= 0) || (length > 8))
        {
                printf("[at24c02][%s]: length parameter values are not specification line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write address fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        for(int i = 0; i < length; i++)
        {
                i2c_send_byte(&i2c_at24c02, buffer[i]);
                if(i2c_wait_response(&i2c_at24c02))
                {
                        printf("[at24c02][%s]: write data error at %d line = %d \r\n", __FUNCTION__, i, __LINE__);
                        return -1;
                }
        }
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 直接读取当前地址的内容
 * @param buffer 保存读取到的数据的缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_current_address_read(u8* buffer)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_READ);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        buffer[0] = i2c_receive_byte(&i2c_at24c02);
        
        i2c_no_response(&i2c_at24c02);
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 在指定的地址读取一片连续的字节
 * @param address 指定要读取的首地址
 * @param buffer 保存读取到的数据的缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_random_read(u8 address, u8* buffer)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write adress fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        return at24c02_current_address_read(buffer);
}

/**
 * @brief 在指定的地址读取一片连续的字节
 * @param address 指定要读取的首地址
 * @param buffer 保存读取到的数据的缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_sequentia_read(u8* buffer, u16 length)
{
        u8 i;
        
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_READ);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        for(i = 0; i < (length - 1); i++)
        {
                buffer[i] = i2c_receive_byte(&i2c_at24c02);
                i2c_response(&i2c_at24c02);
        }
        
        buffer[i] = i2c_receive_byte(&i2c_at24c02);
        i2c_no_response(&i2c_at24c02);
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}
=======
#include "bsp_at24c02.h"
#include "bsp_systick.h"
#include "bsp_i2c.h"
#include "bsp_usart.h"

I2C_GpioDef i2c_at24c02;

/**
 * @brief 初始化AT24C02
 */
void at24c02_init()
{
        i2c_at24c02.SCLGpioPort = GPIOB;
        i2c_at24c02.SCLGpioPin = GPIO_Pin_6;
        i2c_at24c02.SDAGpioPort = GPIOB;
        i2c_at24c02.SDAGpioPin = GPIO_Pin_7;
        i2c_init(&i2c_at24c02);
}

int at24c02_set_current_address(u8 address)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write address fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 在指定的地址写一个字节
 * @param address 指定要写入的地址
 * @param buffer 要写入数据的数据缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_byte_write(u8 address, u8* buffer)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d\r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write address fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, *buffer);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write data fail line = %d \r\n", __FUNCTION__, __LINE__);
                i2c_stop(&i2c_at24c02);
                return -1;
        }
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 在指定的地址连续写入字节
 * @param address 指定要写入的首地址
 * @param buffer 要写入数据的数据缓存区
 * @param length 指定要写入的字节个数
 * @return u8 返回程序执行的状态
 * @note 此函数为页写入函数, 对于at24c02一页为8个字节
 */
int at24c02_page_write(u8 address, u8* buffer, u8 length)
{
        i2c_start(&i2c_at24c02);
        
        if((length <= 0) || (length > 8))
        {
                printf("[at24c02][%s]: length parameter values are not specification line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write address fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        for(int i = 0; i < length; i++)
        {
                i2c_send_byte(&i2c_at24c02, buffer[i]);
                if(i2c_wait_response(&i2c_at24c02))
                {
                        printf("[at24c02][%s]: write data error at %d line = %d \r\n", __FUNCTION__, i, __LINE__);
                        return -1;
                }
        }
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 直接读取当前地址的内容
 * @param buffer 保存读取到的数据的缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_current_address_read(u8* buffer)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_READ);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        buffer[0] = i2c_receive_byte(&i2c_at24c02);
        
        i2c_no_response(&i2c_at24c02);
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}

/**
 * @brief 在指定的地址读取一片连续的字节
 * @param address 指定要读取的首地址
 * @param buffer 保存读取到的数据的缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_random_read(u8 address, u8* buffer)
{
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_WRITE);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        i2c_send_byte(&i2c_at24c02, address);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: write adress fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        return at24c02_current_address_read(buffer);
}

/**
 * @brief 在指定的地址读取一片连续的字节
 * @param address 指定要读取的首地址
 * @param buffer 保存读取到的数据的缓存区
 * @return u8 返回程序执行的状态
 */
int at24c02_sequentia_read(u8* buffer, u16 length)
{
        u8 i;
        
        i2c_start(&i2c_at24c02);
        
        i2c_send_byte(&i2c_at24c02, AT24C02_READ);
        if(i2c_wait_response(&i2c_at24c02))
        {
                printf("[at24c02][%s]: device address mapping fail line = %d \r\n", __FUNCTION__, __LINE__);
                return -1;
        }
        
        for(i = 0; i < (length - 1); i++)
        {
                buffer[i] = i2c_receive_byte(&i2c_at24c02);
                i2c_response(&i2c_at24c02);
        }
        
        buffer[i] = i2c_receive_byte(&i2c_at24c02);
        i2c_no_response(&i2c_at24c02);
        
        i2c_stop(&i2c_at24c02);
        
        return 0;
}
>>>>>>> updata
