#include "bsp_spi.h"

 /**
  * @brief  
  * @param  
  * @retval 
  */

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);



/**
 * @brief SPI �� GPIO ��ʼ��
 * @param ��
 * @retval ��
 */
static void SPI_GPIO_Init(void)
{
        //����һ��GPIO�ṹ��
        GPIO_InitTypeDef GPIO_InitStructure;

        /* ʹ�� SPI ������ŵ�ʱ�� */

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

        /* ���� SPI �� SCK ����*/
        GPIO_InitStructure.GPIO_Pin = NRF_SPI_SCK_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(NRF_SPI_SCK_PORT,&GPIO_InitStructure); 

        /* ���� SPI �� MOSI ����*/
        GPIO_InitStructure.GPIO_Pin = NRF_SPI_MOSI_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(NRF_SPI_MOSI_PORT,&GPIO_InitStructure); 

        /* ���� SPI �� MISO ����*/
        GPIO_InitStructure.GPIO_Pin = NRF_SPI_MISO_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(NRF_SPI_MISO_PORT,&GPIO_InitStructure);

        PAout(5)=1;
        PAout(6)=1;
        PAout(7)=1;
        
        /* ֹͣ�ź� NRF��CS ���Ÿߵ�ƽ*/ 
        SPI_CS_HIGH();
}
 
  /**
  * @brief SPI ��ģʽ����
  * @param  
  * @retval 
  */
 static void SPI_Mode_Init(void)
 {
         SPI_InitTypeDef SPI_InitStructure;
//         /* ʹ�� SPI ʱ�� */
         NRF_SPI_APBxClock_FUN(RCC_APB2Periph_SPI1,ENABLE);

//                 /* ���� SPI �Ĺ���ģʽ*/
         // NRFоƬ ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
          SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
          /*  ���� SPI ��ģʽΪ����ģʽ */
          SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
          /*  ���� SPI һ�δ������ݵ�λ��Ϊ8λ������˵ÿ��ֻ�ܴ���λ������һ���Դ��ܶ����λ�� */
          SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
          /*  ���� SPI ��ʱ�Ӽ��ԣ��ߣ��͵�ƽ���֣� ��ʱ������ʼλ�ô��ڵ͵�ƽ */
          SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
          /*  ���� SPI ��ʱ����λ�������Σ�ż���α��ز������˵����ó������α��ز���  */
          SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
         /* ���� SPI Ƭѡ��Ϊ�������*/
          SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
         /* ���� SPI �Ĳ�����Ϊ 32 ��Ƶ*/
          SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
         /* ���� SPI Ϊ��λ�����ȴ�*/
          SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
          /* У��λ ����ֵ��ͨ����ʽ��������ģ�������ط���ΪW25Q64BV��ʹ��У��λ������У��λ��ֵΪ����ֵ��û��Ӱ�죩*/
          SPI_InitStructure.SPI_CRCPolynomial = 7;
         //��ʼ�� SPI�������õĽṹ������д����Ӧ�ļĴ�����
          SPI_Init(SPI1,&SPI_InitStructure);
        //ʹ�� SPI
          SPI_Cmd(NRF_SPIx,ENABLE);
          SPI_SendByte(0xff);//�������� 
 }

  /**
  * @brief  ��ʼ�� SPI �� GPIO �Լ���ģʽ�������������������װ����
  * @param  
  * @retval 
  */
 void SPI_bsp_Init(void)
 {
         SPI_GPIO_Init();
         SPI_Mode_Init();
         
 }
 
  /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
u8 SPI_SendByte(u8 byte)
{
        SPITimeout = SPIT_FLAG_TIMEOUT;
        /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
        while (SPI_I2S_GetFlagStatus(NRF_SPIx , SPI_I2S_FLAG_TXE) == RESET)
        {
                if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
        }

        /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
        SPI_I2S_SendData(NRF_SPIx , byte);

        SPITimeout = SPIT_FLAG_TIMEOUT;
        /* �ȴ����ջ������ǿգ�RXNE�¼� */
        while (SPI_I2S_GetFlagStatus(NRF_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
        {
                if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
        }

        /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
        return SPI_I2S_ReceiveData(NRF_SPIx );
}
 
 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������     *->(����������)<-*
  */
u8 SPI_ReadByte(void)
{
        return (SPI_SendByte(Dummy_Byte));
}

/**
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
          /* �ȴ���ʱ��Ĵ���,���������Ϣ */
          NRF_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
          return 0;
}

