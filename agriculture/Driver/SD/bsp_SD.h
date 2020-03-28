#ifndef __BSP_SD_H
#define __BSP_SD_H

#include "bsp_spi.h"
#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"



//SD�������ݽ������Ƿ��ͷ����ߺ궨��  
#define NO_RELEASE      0
#define RELEASE         1       
// SD�����Ͷ���  
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_ERR     3
#define SD_TYPE_V2HC    4       
// SD��ָ���     
#define CMD0    0x00       //����λ
#define CMD8    8
#define CMD1    1
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

#define BLOCK_SIZE 512 

#define SD_CS_LOW()            SPI_CS1_LOW() 
#define SD_CS_HIGH()           SPI_CS1_HIGH()

/***************��������*********************/

u8 SD_Select(void);
u8 SD_RecvData(u8*buf,u16 len);
u8 SD_SendBlock(u8*buf,u8 cmd);
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc);
u8 SD_GetCID(u8 *cid_data);
u8 SD_GetCSD(u8 *csd_data);
u32 SD_GetSectorCount(void);

u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);
u8 SD_Initialize(void);
u8 SD_WaitReady(void);
u8 SD_GetResponse(u8 Response);
u8 SD_WaitDataReady(void);
u8 SD_Idle_Sta(void);
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);
u32 SD_GetCapacity(void);
u8 SD_Init1(void);
u8 SD_Init(void);


#endif   /*__BSP_SD_H*/


