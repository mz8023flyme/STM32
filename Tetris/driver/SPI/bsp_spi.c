#include "bsp_spi.h"
#include "bsp_usart.h"

static void GPIO_SPI_Config (void)
{
        GPIO_InitTypeDef  GPIO_InitStruct;      //配置连接到的GPIO
                
        FLASH_SPI_CS_APBxClock_FUN (FLASH_SPI_CS_CLK,ENABLE);
        FLASH_SPI_SCK_APBxClock_FUN(FLASH_SPI_SCK_CLK,ENABLE);
        FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK,ENABLE);

        /*MISO MOSI  SCK*/
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MISO_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(FLASH_SPI_MISO_PORT,&GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MOSI_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(FLASH_SPI_MOSI_PORT,&GPIO_InitStruct);

        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Pin = FLASH_SPI_SCK_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(FLASH_SPI_SCK_PORT,&GPIO_InitStruct);

        //配置CS片选引脚
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Pin = FLASH_SPI_CS_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(FLASH_SPI_CS_PORT,&GPIO_InitStruct);

        SPI_FLASH_CS_HIGH();
}

static void SPI_MODE(void)
{
        SPI_InitTypeDef   SPI_InitStruct;


        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge ; 
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStruct.SPI_CRCPolynomial = 0;
        SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b ;
        SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB ;
        SPI_InitStruct.SPI_Mode = SPI_Mode_Master ;
        SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;

        SPI_Init(FLASH_SPIx,&SPI_InitStruct);
        SPI_Cmd(FLASH_SPIx,ENABLE);
}

void SPI_FLASH_Init(void)
{
        GPIO_SPI_Config();
        SPI_MODE();
}

/*发送一个数据*/
uint8_t SPI_FLASH_SendByte(uint8_t data)
{
        uint32_t SPITIME;

        SPITIME=SPI_TIMEOUT;

        while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE)==RESET)
        {
                if((SPITIME--) == 0) 
                {
                        printf ("SPI 等待超时!");
                        return 0;
                }
        }
        SPI_I2S_SendData(FLASH_SPIx,data);
        while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE)==RESET)
        {
                if((SPITIME--) ==0) 
                {
                        printf ("SPI 等待超时!");
                        return 0;
                }
        }

        return SPI_I2S_ReceiveData(FLASH_SPIx);
}
/*读取一个数据*/

uint32_t SPI_FLASH_ReadByte(void)
{
        uint32_t ID; 

        SPI_FLASH_CS_LOW();

        SPI_FLASH_SendByte( READ_JEDEC_ID);

        ID =SPI_FLASH_SendByte( DUMMY);
        ID <<= 8;

        ID |=SPI_FLASH_SendByte( DUMMY);
        ID <<= 8;

        ID |=SPI_FLASH_SendByte( DUMMY);

        SPI_FLASH_CS_HIGH();

        return ID;
}

/*写使能*/

void SPI_Write_Enable (void)
{

        SPI_FLASH_CS_LOW();

        SPI_FLASH_SendByte( WRITE_ENABLE);

        SPI_FLASH_CS_HIGH();

}



//擦除函数
void SPI_Erase (uint32_t addr)
{

        SPI_Write_Enable();

        SPI_FLASH_CS_LOW();

        SPI_FLASH_SendByte( ERASE_SECTRO);

        SPI_FLASH_SendByte( (addr>>16)&0xff);

        SPI_FLASH_SendByte( (addr>>8)&0xff);

        SPI_FLASH_SendByte(addr&0xff);

        SPI_FLASH_CS_HIGH();

}
/*等待写入完成*/
void SPI_Wite_Erase(void)
{
        uint8_t status=0;

        SPI_FLASH_CS_LOW();

        SPI_FLASH_SendByte( STATUS);
        do
        {
        status=SPI_FLASH_SendByte( DUMMY);
        }while((status&0x01)==1);//忙碌

        SPI_FLASH_CS_HIGH();

}

//读取flash内容
void SPI_Read_Data(uint32_t addr , uint8_t *buff,uint32_t num)
{
        SPI_FLASH_CS_LOW();

        SPI_FLASH_SendByte( READ_DATA);

        SPI_FLASH_SendByte( (addr>>16)&0xff);

        SPI_FLASH_SendByte( (addr>>8)&0xff);

        SPI_FLASH_SendByte(addr&0xff);

        while(num--)
        {
                *buff=SPI_FLASH_SendByte( DUMMY);
                buff++;
        }

        SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
        /* 发送FLASH写使能命令 */
        SPI_Write_Enable();
        SPI_Wite_Erase();

        /* 擦除扇区 */
        /* 选择FLASH: CS低电平 */
        SPI_FLASH_CS_LOW();

        /* 发送扇区擦除指令*/
        SPI_FLASH_SendByte(ERASE_SECTRO);

        /*发送擦除扇区地址的高位*/
        SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);

        /* 发送擦除扇区地址的中位 */
        SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);

        /* 发送擦除扇区地址的低位 */
        SPI_FLASH_SendByte(SectorAddr & 0xFF);

        /* 停止信号 FLASH: CS 高电平 */
        SPI_FLASH_CS_HIGH();

        /* 等待擦除完毕*/
        SPI_Wite_Erase();
}

 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param      pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
        /* 发送FLASH写使能命令 */
        SPI_Write_Enable();

        /* 选择FLASH: CS低电平 */
        SPI_FLASH_CS_LOW();
        /* 写页写指令*/
        SPI_FLASH_SendByte(W25X_PageProgram);
        /*发送写地址的高位*/
        SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
        /*发送写地址的中位*/
        SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
        /*发送写地址的低位*/
        SPI_FLASH_SendByte(WriteAddr & 0xFF);

        if(NumByteToWrite > SPI_PerWritePageSize)
        {
                NumByteToWrite = SPI_PerWritePageSize;
                printf("<<-FLASH-ERROR->>");
        }

        /* 写入数据*/
        while (NumByteToWrite--)
        {
                /* 发送当前要写入的字节数据 */
                SPI_FLASH_SendByte(*pBuffer);
                /* 指向下一字节数据 */
                pBuffer++;
        }

        /* 停止信号 FLASH: CS 高电平 */
        SPI_FLASH_CS_HIGH();

        /* 等待写入完毕*/
        SPI_Wite_Erase();
}


 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param      pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
        u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

        /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
        Addr = WriteAddr % SPI_PageSize;

        /*差count个数据值，刚好可以对齐到页地址*/
        count = SPI_PageSize - Addr;
        /*计算出要写多少整数页*/
        NumOfPage =  NumByteToWrite / SPI_PageSize;
        /*mod运算求余，计算出剩余不满一页的字节数*/
        NumOfSingle = NumByteToWrite % SPI_PageSize;

        /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
        if (Addr == 0)
        {
                        /* NumByteToWrite < SPI_FLASH_PageSize */
                if (NumOfPage == 0) 
                {
                        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
                }
                else /* NumByteToWrite > SPI_FLASH_PageSize */
                { 
                                /*先把整数页都写了*/
                        while (NumOfPage--)
                        {
                                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_PageSize);
                                WriteAddr +=  SPI_PageSize;
                                pBuffer += SPI_PageSize;
                        }
                                        /*若有多余的不满一页的数据，把它写完*/
                        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                }
        }
        /* 若地址与 SPI_FLASH_PageSize 不对齐  */
        else 
        {
                        /* NumByteToWrite < SPI_FLASH_PageSize */
                if (NumOfPage == 0)
                {
                                /*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
                        if (NumOfSingle > count) 
                        {
                                temp = NumOfSingle - count;
                                                        /*先写满当前页*/
                                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                                                        
                                WriteAddr +=  count;
                                pBuffer += count;
                                                        /*再写剩余的数据*/
                                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
                        }
                        else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
                        {
                                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
                        }
                }
                else /* NumByteToWrite > SPI_FLASH_PageSize */
                {
                                        /*地址不对齐多出的count分开处理，不加入这个运算*/
                        NumByteToWrite -= count;
                        NumOfPage =  NumByteToWrite / SPI_PageSize;
                        NumOfSingle = NumByteToWrite % SPI_PageSize;
                                        
                                        /* 先写完count个数据，为的是让下一次要写的地址对齐 */
                        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                                        
                                        /* 接下来就重复地址对齐的情况 */
                        WriteAddr +=  count;
                        pBuffer += count;
                                        /*把整数页都写了*/
                        while (NumOfPage--)
                        {
                                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_PageSize);
                                WriteAddr +=  SPI_PageSize;
                                pBuffer += SPI_PageSize;
                        }
                                        /*若有多余的不满一页的数据，把它写完*/
                        if (NumOfSingle != 0)
                        {
                                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                        }
                }
        }
}


/**
  * @brief  读取FLASH数据
  * @param      pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
        /* 选择FLASH: CS低电平 */
        SPI_FLASH_CS_LOW();

        /* 发送 读 指令 */
        SPI_FLASH_SendByte(READ_DATA);

        /* 发送 读 地址高位 */
        SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
        /* 发送 读 地址中位 */
        SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
        /* 发送 读 地址低位 */
        SPI_FLASH_SendByte(ReadAddr & 0xFF);

        /* 读取数据 */
        while (NumByteToRead--) /* while there is data to be read */
        {
                /* 读取一个字节*/
                *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
                /* 指向下一个字节缓冲区 */
                pBuffer++;
        }

        /* 停止信号 FLASH: CS 高电平 */
        SPI_FLASH_CS_HIGH();
}



