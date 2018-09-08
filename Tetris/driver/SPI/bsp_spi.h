#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h" 
 
#define      SPI_TIMEOUT                ((uint32_t)0x1000)
#define      DUMMY                              0xAB
#define      READ_JEDEC_ID                      0x9f
#define      ERASE_SECTRO                       0x20
#define      STATUS                             0x05
#define      READ_DATA                          0x03
#define      WRITE_ENABLE                       0X06 
#define      Dummy_Byte                         0xFF
#define      W25X_PageProgram                   0x02
#define SPI_PageSize              256
#define SPI_PerWritePageSize      256

/*SPI接口定义-开头****************************/
#define      FLASH_SPIx                        SPI1
#define      FLASH_SPI_APBxClock_FUN           RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define      FLASH_SPI_CS_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOC    
#define      FLASH_SPI_CS_PORT                 GPIOC
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_0

//SCK引脚
#define      FLASH_SPI_SCK_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      FLASH_SPI_SCK_PORT                GPIOA   
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_5
//MISO引脚
#define      FLASH_SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MISO_PORT               GPIOA 
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_6
//MOSI引脚
#define      FLASH_SPI_MOSI_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MOSI_PORT               GPIOA 
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_7

#define      SPI_FLASH_CS_LOW()               GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define      SPI_FLASH_CS_HIGH()              GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

void SPI_FLASH_Init(void);
uint32_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t data);
void SPI_Erase (uint32_t addr);
void SPI_Wite_Erase(void);
void SPI_Read_Data(uint32_t addr , uint8_t *buff,uint32_t num);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

#endif /* __BSP_SPI_H*/
