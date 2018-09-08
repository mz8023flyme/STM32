#include "bsp_spi.h"

 /**
  * @brief  
  * @param  
  * @retval 
  */

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



/**
 * @brief SPI 的 GPIO 初始化
 * @param 无
 * @retval 无
 */
static void SPI_GPIO_Init(void)
{
        //定义一个GPIO结构体
        GPIO_InitTypeDef GPIO_InitStructure;

        /* 使能 SPI 相关引脚的时钟 */

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

        /* 配置 SPI 的 SCK 引脚*/
        GPIO_InitStructure.GPIO_Pin = NRF_SPI_SCK_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(NRF_SPI_SCK_PORT,&GPIO_InitStructure); 

        /* 配置 SPI 的 MOSI 引脚*/
        GPIO_InitStructure.GPIO_Pin = NRF_SPI_MOSI_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(NRF_SPI_MOSI_PORT,&GPIO_InitStructure); 

        /* 配置 SPI 的 MISO 引脚*/
        GPIO_InitStructure.GPIO_Pin = NRF_SPI_MISO_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(NRF_SPI_MISO_PORT,&GPIO_InitStructure);

        PAout(5)=1;
        PAout(6)=1;
        PAout(7)=1;
        
        /* 停止信号 NRF：CS 引脚高电平*/ 
        SPI_CS_HIGH();
}
 
  /**
  * @brief SPI 的模式配置
  * @param  
  * @retval 
  */
 static void SPI_Mode_Init(void)
 {
         SPI_InitTypeDef SPI_InitStructure;
//         /* 使能 SPI 时钟 */
         NRF_SPI_APBxClock_FUN(RCC_APB2Periph_SPI1,ENABLE);

//                 /* 配置 SPI 的工作模式*/
         // NRF芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
          SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
          /*  配置 SPI 的模式为主机模式 */
          SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
          /*  配置 SPI 一次传输数据的位数为8位（不是说每次只能传八位，可以一次性传很多个八位） */
          SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
          /*  配置 SPI 的时钟极性（高，低电平两种） 让时钟在起始位置处于低电平 */
          SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
          /*  配置 SPI 的时钟相位（奇数次，偶数次边沿采样）此地配置成奇数次边沿采样  */
          SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
         /* 配置 SPI 片选段为软件控制*/
          SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
         /* 配置 SPI 的波特率为 32 分频*/
          SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
         /* 配置 SPI 为高位数据先传*/
          SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
          /* 校验位 （此值是通过公式计算出来的，在这个地方因为W25Q64BV不使用校验位，所以校验位的值为任意值都没有影响）*/
          SPI_InitStructure.SPI_CRCPolynomial = 7;
         //初始化 SPI（将配置的结构体内容写入相应的寄存器）
          SPI_Init(SPI1,&SPI_InitStructure);
        //使能 SPI
          SPI_Cmd(NRF_SPIx,ENABLE);
          SPI_SendByte(0xff);//启动传输 
 }

  /**
  * @brief  初始化 SPI 的 GPIO 以及其模式，将上面的两个函数封装起来
  * @param  
  * @retval 
  */
 void SPI_bsp_Init(void)
 {
         SPI_GPIO_Init();
         SPI_Mode_Init();
         
 }
 
  /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
u8 SPI_SendByte(u8 byte)
{
        SPITimeout = SPIT_FLAG_TIMEOUT;
        /* 等待发送缓冲区为空，TXE事件 */
        while (SPI_I2S_GetFlagStatus(NRF_SPIx , SPI_I2S_FLAG_TXE) == RESET)
        {
                if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
        }

        /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
        SPI_I2S_SendData(NRF_SPIx , byte);

        SPITimeout = SPIT_FLAG_TIMEOUT;
        /* 等待接收缓冲区非空，RXNE事件 */
        while (SPI_I2S_GetFlagStatus(NRF_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
        {
                if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
        }

        /* 读取数据寄存器，获取接收缓冲区数据 */
        return SPI_I2S_ReceiveData(NRF_SPIx );
}
 
 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据     *->(还是有疑问)<-*
  */
u8 SPI_ReadByte(void)
{
        return (SPI_SendByte(Dummy_Byte));
}

/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
          /* 等待超时后的处理,输出错误信息 */
          NRF_ERROR("SPI 等待超时!errorCode = %d",errorCode);
          return 0;
}

