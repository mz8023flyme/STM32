#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_NRF24L01.h"

/*SPI接口定义-开头****************************/
#define      NRF_SPIx                        SPI1
#define      NRF_SPI_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define      NRF_SPI_CLK                     RCC_APB2Periph_SPI1

//SPI相关引脚
#define      NRF_SPI_GPIO_APBxClock_FUN       RCC_APB2PeriphClockCmd

//SCK引脚
#define      NRF_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      NRF_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      NRF_SPI_SCK_PORT                GPIOA   
#define      NRF_SPI_SCK_PIN                 GPIO_Pin_5
//MISO引脚
#define      NRF_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      NRF_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      NRF_SPI_MISO_PORT               GPIOA 
#define      NRF_SPI_MISO_PIN                GPIO_Pin_6
//MOSI引脚
#define      NRF_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      NRF_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      NRF_SPI_MOSI_PORT               GPIOA 
#define      NRF_SPI_MOSI_PIN                GPIO_Pin_7

#define  SPI_CS_LOW()           PCout(14) = 0; //GPIO_ResetBits( NRF_SPI_CS_PORT, NRF_SPI_CS_PIN )
#define  SPI_CS_HIGH()          PCout(14) = 1; //GPIO_SetBits( NRF_SPI_CS_PORT, NRF_SPI_CS_PIN )


/*信息输出*/
#define NRF_DEBUG_ON         1

#define NRF_INFO(fmt,arg...)           printf("<<-NRF-INFO->> "fmt"\n",##arg)
#define NRF_ERROR(fmt,arg...)          printf("<<-NRF-ERROR->> "fmt"\n",##arg)
#define NRF_DEBUG(fmt,arg...)          do{\
                                          if(NRF_DEBUG_ON)\
                                          printf("<<-NRF-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define Dummy_Byte                0xFF
#define WIP_Flag                  0x01
#define W25X_JedecDeviceID	  0x9F
#define W25X_SectorErase	  0x20 
#define W25X_ReadStatusReg	  0x05 
#define W25X_ReadData		  0x03
#define W25X_WriteEnable	  0x06 
#define W25X_PageProgram	  0x02 

void SPI_bsp_Init(void);
u8 SPI_SendByte(u8 byte);
u8 SPI_ReadByte(void);
uint32_t SPI_FLASH_ReadID(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_ReadDate(u32 ReadAddr,u8 *readbuff,u32 NumByteToRead);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDate(u32 WriteAddr,u8 *writebuff,u32 NumByteToWrite);
        


#endif /*__BSP_SPI_H*/

