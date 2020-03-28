#include "bsp_spi.h"
#include "bsp_usart.h"
#include "bsp_systick.h"


void spi_Init(void)
{
        SPI_InitTypeDef  SPI_InitStruct;
        GPIO_InitTypeDef GPIO_InitStruct;

        SPI_APBxClock(SPI_CLK,ENABLE);                           //�� SPI ʱ��
        SPI_CS2_APBxClock(SPI_CS2_CLK|SPI_CS1_CLK|SPI_GPIO_CLK,ENABLE);              //�� GPIOA ʱ��
//        SPI_SCK_GPIO_APBxClock(SPI_GPIO_CLK,ENABLE);    //�� SPI ���ù������ŵ�ʱ��
        //*SPI ���Žṹ������*/
        //Ƭѡ��
        GPIO_InitStruct.GPIO_Pin = SPI_CS1_PIN|SPI_CS2_PIN;
        GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SPI_CS1_PORT,&GPIO_InitStruct);
        GPIO_Init(SPI_CS2_PORT,&GPIO_InitStruct);
        //���ó�ʼ��Ϊ�ߵ�ƽ��ʧ��
        SPI_CS1_HIGH();
        SPI_CS2_HIGH();
        
        //spi ʱ����
        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Pin   = SPI_SCK_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStruct);
        //MISO
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Pin  = SPI_MISO_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SPI_MISO_PORT,&GPIO_InitStruct);
        //MOSI
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Pin  = SPI_MOSI_PIN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SPI_MISO_PORT,&GPIO_InitStruct);
        
        //*****SPI�ṹ������**********/
        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;  //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
        SPI_InitStruct.SPI_CRCPolynomial=7;     //CRCֵ����Ķ���ʽ
        SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
        SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
        SPI_InitStruct.SPI_FirstBit =  SPI_FirstBit_MSB  ;//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
        SPI_InitStruct.SPI_Mode = SPI_Mode_Master ;//����SPI����ģʽ:����Ϊ��SPI
        SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;  //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
        
        SPI_Init(SPIx,&SPI_InitStruct);
        SPI_Cmd(SPIx, ENABLE);
        
}

u8 SPI_SentByte(uint8_t Data)
{
         
        
        u32  SPITIME = SPI_TIMEOUT;

        while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE)==RESET)
        {
                if((SPITIME--) == 0) 
                {
                        printf ("SPI �ȴ���ʱ!");
                        printf("[%s][%d]\r\n", __func__, __LINE__);
                        return 0;
                }
        }
//        SPI_I2S_SendData(SPIx,Data);
        SPI1->DR = Data; //�Ĵ����汾���̼���������ĺ���������һ����Ϊ����Ը�Чһ���
        while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE)==RESET)
        {
                if( (SPITIME--)==0 ) 
                {
                        printf ("SPI �ȴ���ʱ!");
                        printf("[%s][%d]\r\n", __func__, __LINE__);
                        return 0;
                }
        }

//        return SPI_I2S_ReceiveData(SPIx);
        return SPI1->DR;
}

u8 SPI_ReadByte(void)
{
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  
//                SPI_I2S_SendData(SPI1, DUMMY);    //������Ӧ�ò���ص����ݣ�Ϊ�����Ķ������ṩʱ���ź�  
                SPI1->DR = DUMMY;////�Ĵ����汾���̼���������ĺ���������һ����Ϊ����Ը�Чһ���
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);   
//                return SPI_I2S_ReceiveData(SPI1); 
        return  SPI1->DR;
}

//u8 SPI_ReadByte(void)
//{
//        return SPI_SentByte(DUMMY);
//}

void SPI_SetSpeed1(u8 SpeedSet)
{
        SPI_InitTypeDef SPI_InitStruct;
        
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;  //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
        SPI_InitStruct.SPI_CRCPolynomial=7;     //CRCֵ����Ķ���ʽ
        SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
        SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
        SPI_InitStruct.SPI_FirstBit =  SPI_FirstBit_MSB  ;//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
        SPI_InitStruct.SPI_Mode = SPI_Mode_Master ;//����SPI����ģʽ:����Ϊ��SPI
        SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;  //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
        
        switch(SpeedSet)
        {
                case SPI_SPEED_2:
                        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
                break ;
                case SPI_SPEED_4:
                        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
                break ;
                case SPI_SPEED_8:
                        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
                break ;
                case SPI_SPEED_16:
                        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
                break ;
                case SPI_SPEED_256:
                        SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
                break ;
                
                
        }
       
        SPI_Init(SPI1, &SPI_InitStruct);
        SPI_Cmd(SPI1, ENABLE);
}



