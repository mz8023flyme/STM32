#include "bsp_SD.h"
u8  SD_Type=0;//SD��������

/**
  * @brief  �ȴ�SD��Ready
  * @retval   0���ɹ�       1��ʧ��
  * @note     SD��׼���û᷵��0XFF
  * */
u8 SD_WaitReady(void)
{
        u8 r1;
        u16 retry;
        retry = 0;
        do
        {
                r1 = SPI_SentByte(0xFF);
                if(retry==0xfffe)
                {
                    return 1;
                }
        }while(r1!=0xFF);

        return 0;
} 


/**
  * @brief  �ȴ�SD����Ӧ
  * @param  Response Ҫ�õ��Ļ�Ӧֵ
  * @retval   0���ɹ�       1��ʧ��
  * */
u8 SD_GetResponse(u8 Response)
{
        u16 Count=0xFFF;//�ȴ�����  
        while ((SPI_SentByte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ  
        if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
        else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}

/**
 * @brief    �ȴ�SD��д�����
 * @retval   0,�ɹ�;
 */
u8 SD_WaitDataReady(void)
{
        u8 r1=MSD_DATA_OTHER_ERROR;
        u32 retry;
        retry=0;
        do
        {
                r1=SPI_SentByte(0xFF)&0X1F;//������Ӧ
                if(retry==0xfffe)return 1; 
                retry++;
                switch (r1)
                {       
                        case MSD_DATA_OK://���ݽ�����ȷ��      
                                r1=MSD_DATA_OK;
                        break;  
                        case MSD_DATA_CRC_ERROR:  //CRCУ�����
                                return MSD_DATA_CRC_ERROR;  
                        case MSD_DATA_WRITE_ERROR://����д�����
                                return MSD_DATA_WRITE_ERROR;  
                        default://δ֪����    
                                r1=MSD_DATA_OTHER_ERROR;
                        break;  
                }   
        }while(r1==MSD_DATA_OTHER_ERROR); //���ݴ���ʱһֱ�ȴ�
        retry=0;
        while(SPI_SentByte(0XFF)==0)//��������Ϊ0,�����ݻ�δд���
        {
        retry++;
        //delay_us(10);//SD��д�ȴ���Ҫ�ϳ���ʱ��
        if(retry>=0XFFFFFFFE)return 0XFF;//�ȴ�ʧ����
        };      
        return 0;//�ɹ���
}       



/**
  * @brief  ѡ��SD�������ȴ�SD��׼����
  * @retval   0���ɹ�       -1��ʧ��
  * @note     SD��׼���û᷵��0XFF
  * */
u8 SD_Select(void)
{
        uint32_t t=0;
        u8 ack;
        SD_CS_LOW();  //ƬѡSD���͵�ƽʹ��
        do
        {
                if(SPI_SentByte(0XFF)==0XFF)
                {
                        ack=SPI_SentByte(0XFF);
                        printf("ack=%d",ack);
                        return 0;//OK
                }
                t++;
        }while(t<0XFFFF);//�ȴ�
//        SD_CS_HIGH();  //�ͷ�����
        return 1;//�ȴ�ʧ��
}

/** 
  * @brief   ��sd����ȡһ�����ݰ�������
  * @param   buf�����ݻ�������    
  * @param   len:Ҫ��ȡ�����ݵĳ���
  * @retval   0���ɹ�       ������ʧ��
  * @note   ��ȡʱ��Ҫ�ȴ�SD������������ʼ����0XFE
  */
u8 SD_RecvData(u8*buf,u16 len)
{ 
        u16 Count=0xF000;//�ȴ�����
        while ((SPI_SentByte(0XFF)!=0xFE)&&Count)Count--;//�ȴ��õ���ȡ��������0xfe
        if    (Count==0)   return MSD_RESPONSE_FAILURE;//��ȡ����ʧ��,����0XFF      
        while(len--)//��ʼ��������
        {
                *buf=SPI_SentByte(0xFF);
                buf++;
        }
        //������2��αCRC��dummy CRC������װ������2��CRC
        SPI_SentByte(0xFF);
        SPI_SentByte(0xFF);                     
        return 0;//��ȡ�ɹ�
}


/** 
  * @brief   ��sd��д��һ�����ݰ������� 512�ֽ�
  * @param   buf:���ݻ�����    cmd:���ݷ��͵����� 
  * @retval  0���ɹ�       ������ʧ��
  * @note    д����ʱ��Ҫ�ȷ���������ʼ����0XFE/0XFC/0XFD
  */
u8 SD_SendBlock(u8*buf,u8 cmd)
{ 
        u32 t,Count=0XFFFF; 
        while ((SPI_SentByte(0XFF)!=0xFF)&&Count)Count--;//�ȴ�SD��׼����
        if    (Count==0)   return MSD_RESPONSE_FAILURE;//SD��δ׼���ã�ʧ�ܣ�����
        SPI_SentByte(cmd); //����������ʼ��ֹͣ����
        if(cmd!=0XFD)//�ڲ��ǽ������Ƶ�����£���ʼ��������
        {
                for(t=0;t<512;t++) SPI_SentByte(buf[t]);//����ٶ�,���ٺ�������ʱ��
                SPI_SentByte(0xFF);//����2�ֽڵ�CRC
                SPI_SentByte(0xFF);
                t=SPI_SentByte(0xFF);//������CRC֮���������д��״̬
                if((t&0x1F)!=0x05)return MSD_DATA_WRITE_ERROR;//д�����                    
        }                           
        return 0;//д��ɹ�
}

/**
 * @brief ��SD����һ��block
 * @param u32 sector ȡ��ַ��sectorֵ���������ַ��
 * @param u8 *buffer ���ݴ洢��ַ����С����512byte��   
 * @retval 0�� �ɹ�
 *       other��ʧ��       
 */
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
        u8 r1; 
        //����Ϊ����ģʽ
        SPI_SetSpeed1(SPI_SPEED_4);     
        //�������SDHC����������sector��ַ������ת����byte��ַ
        if(SD_Type!=SD_TYPE_V2HC)
        {
                sector = sector<<9;
        } 
        r1 = SD_SendCmd(CMD17, sector, 0);//������     
        if(r1 != 0x00)return r1;        
        r1 = SD_RecvData(buffer, 512);      
        if(r1 != 0)return r1;   //�����ݳ���
        else return 0; 
}

/** 
 *  @brief  ��sd��д��һ�����ݰ������� 512�ֽ�
 *  @param  cmd������  arg���������  crc��crcУ��ֵ��ֹͣλ
 *  @retval ����ֵ:SD�����صĶ�Ӧ��Ӧ�������Ӧ
 *  @note   ��ӦΪR1-R7����SDЭ���ֲ�V2.0�棨2006��
 */
//u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
//{
//        u8 r1; 
//        u8 Retry=0; 
//        SD_CS_HIGH();//ȡ���ϴ�Ƭѡ�ͷ�����
//        SPI_SentByte(0xff);  //������ǰ�ȷ���8��ʱ��
//        SD_CS_LOW();
//        //����
//        SPI_SentByte(cmd | 0x40);//�ֱ�д������
//        SPI_SentByte(arg >> 24);
//        SPI_SentByte(arg >> 16);
//        SPI_SentByte(arg >> 8);
//        SPI_SentByte(arg);   
//        SPI_SentByte(crc); 
//        //�ȴ���Ӧ����ʱ�˳�
//        do
//        {
//               r1 = SPI_SentByte(0xff);
//               Retry++;
//               //����ʵ���ã�������Դ������
//                printf("r1=%d",r1);
//         } while((r1!=0xff) && Retry<100 );
//        //�ر�Ƭѡ
//        SD_CS_HIGH();
//        //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
//        SPI_SentByte(0xff);
//        printf("[%s][%d]\r\n", __func__, __LINE__);
//        //����״ֵ̬
//        return r1;
//}




u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
        u8 r1; 
        u8 Retry=0; 
        SD_CS_HIGH();
        SPI_SentByte(0x00);//����д������ʱ
        SPI_SentByte(0x00);
        SPI_SentByte(0x00); 
        //Ƭѡ���õͣ�ѡ��SD��
        SD_CS_LOW();
        //����
        SPI_SentByte(cmd | 0x40);//�ֱ�д������
        SPI_SentByte(arg >> 24);
        SPI_SentByte(arg >> 16);
        SPI_SentByte(arg >> 8);
        SPI_SentByte(arg);
        SPI_SentByte(crc|0x01); 
        //�ȴ���Ӧ����ʱ�˳�
        while((r1=SPI_SentByte(0xFF))==0xFF)
        {
                Retry++; 
                if(Retry>100)break; 
        }   
        //�ر�Ƭѡ
        SD_CS_HIGH();
        //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
        SPI_SentByte(0x00);
        //����״ֵ̬
        return r1;
}


/**
 * @brief  ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
 * @param  u8 cmd   ���� 
 * @param  u32 arg  �������
 * @param  8 crc   crcУ��ֵ  
 * @retval SD�����ص���Ӧ  
 */
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
        u8 Retry=0;     
        u8 r1;  
        SPI_SentByte(0xff);//����д������ʱ
        SPI_SentByte(0xff);       
        SPI_CS1_LOW();//Ƭѡ���õͣ�ѡ��SD�� 
        //����
        SPI_SentByte(cmd | 0x40); //�ֱ�д������
        SPI_SentByte(arg >> 24);
        SPI_SentByte(arg >> 16);
        SPI_SentByte(arg >> 8);
        SPI_SentByte(arg);
        SPI_SentByte(crc);   
        //�ȴ���Ӧ����ʱ�˳�
        while((r1=SPI_SentByte(0xFF))==0xFF)
        {
                Retry++; 
                if(Retry>200)break; 
        } 
        //������Ӧֵ
        return r1;
}
/**
 * @brief  ��SD�����õ�����ģʽ
 * @retval 0,�ɹ�����
 *         1,����ʧ��
 */
u8 SD_Idle_Sta(void)
{
        u16 i;
        u8 retry;       
        //for(i=0;i<0xf00;i++);//����ʱ���ȴ�SD���ϵ����  
        //�Ȳ���>74�����壬��SD���Լ���ʼ�����
        //delay_ms(2000);
        printf("[%s][%d]\r\n", __func__, __LINE__);
        for(i=0;i<10;i++)SPI_SentByte(0xFF); 
        //-----------------SD����λ��idle��ʼ-----------------
        //ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
        //��ʱ��ֱ���˳�
        retry = 0;
        do
        {       
        //����CMD0����SD������IDLE״̬
        i = SD_SendCmd(CMD0, 0, 0x95);
        retry++;
        }while((i!=0x01)&&(retry<200));
        printf("The SD init ret %d\r\n",i);
        //����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
        if(retry==200)return 1; //ʧ��
        return 0;//�ɹ�   
}


/** 
 * @brief   ��ȡSD����CID��Ϣ��������������Ϣ
 * @param   cid_data(���CID���ڴ棬����16Byte
 * @retval  0���ɹ�       ������ʧ��
 * @note    CID�Ĵ����������SDЭ���ֲ�V2.0�棨2006��
 */
u8 SD_GetCID(u8 *cid_data)
{
        printf("[%s][%d]\r\n", __func__, __LINE__);
        u8 r1;    
        //��CMD10�����CID
        r1=SD_SendCmd(CMD10,0,0x01);
        if(r1==0x00)
        {
                r1=SD_RecvData(cid_data,16);//����16���ֽڵ�����  
        }
        SD_CS_HIGH();//ȡ��Ƭѡ
        if(r1)return 1;
        else return 0;
}

/** 
 * @brief   ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
 * @param   cid_data(���CID���ڴ棬����16Byte
 * @retval  0���ɹ�       ������ʧ��
 * @note    CID�Ĵ����������SDЭ���ֲ�V2.0�棨2006��
 */
u8 SD_GetCSD(u8 *csd_data)
{
        u8 r1;  
        r1=SD_SendCmd(CMD9,0,0x01);//��CMD9�����CSD 
        if(r1==0)
        {
                r1=SD_RecvData(csd_data,16);//����16���ֽڵ����� 
        }
        SD_CS_HIGH();//ȡ��Ƭѡ
        if(r1)return 1;
        else return 0;
} 

/** 
 * @brief   ��ȡSD��������������������
 * @param   cid_data(���CSD���ڴ棬����16Byte
 * @retval  0����ȡ��������       ������SD������������ֵ
 * @note    CID�Ĵ����������SDЭ���ֲ�V2.0�棨2006��
 */
u32 SD_GetSectorCount(void)
{
        u8 csd[16];
        u32 Capacity;  
        u8 n;
        u16 csize;           
        //ȡCSD��Ϣ������ڼ��������0 
        if(SD_GetCSD(csd)!=0) return 0;  //��ȡ����ʧ��   
        //���ΪSDHC�����������淽ʽ����
        if((csd[0]&0xC0)==0x40)  //V2.00�Ŀ�
        { 
                csize = csd[9] + ((u16)csd[8] << 8) + 1;
                Capacity = (u32)csize << 10;//�õ�������       
        }
        else//V1.XX�Ŀ�
        { 
                n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
                Capacity= (u32)csize << (n - 9);//�õ�������   
        }
        return Capacity;
}

/** 
 * @brief    ��SD��
 * @param    buf:���ݻ�����        sector:����ȡ�ĵ�ַ      cnt:����ȡ�������� 
 * @retvsl   0���ɹ�       ������ʧ��
 * @note     1.��ȡ�ĵ�ַ������һ����������ʼ
 *          2.������SD2.0���������Ŀ�������
 */
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
        u8 r1;
        if(cnt==1)
        {
                r1=SD_SendCmd(CMD17,sector,0X01);//������
                if(r1==0)//ָ��ͳɹ�
                {
                        r1=SD_RecvData(buf,512);//����512���ֽ�    
                }
        }
        else
        {
                r1=SD_SendCmd(CMD18,sector,0X01);//����������
                do
                {
                        r1=SD_RecvData(buf,512);//����512���ֽ�  
                        buf+=512;  
                }while(--cnt && r1==0);  
                SD_SendCmd(CMD12,0,0X01); //����ֹͣ����
        }   
        SD_CS_HIGH();//ȡ��Ƭѡ
        return r1;//
}

/** 
 * @brief       дSD��
 * @param       buf:���ݻ�����        sector:��д�ĵ�ַ      cnt:��д�������� 
 * @retvsl      0���ɹ�       ������ʧ��
 * @note        1.д�ĵ�ַ������һ����������ʼ
 *             2.������SD2.0���������Ŀ�������
 */
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)
{
        u8 r1;
        if(cnt==1)
        {
                r1=SD_SendCmd(CMD24,sector,0X01);//��������д����
                if(r1==0)//ָ��ͳɹ�
                {
                        r1=SD_SendBlock(buf,0xFE);//д512���ֽ�    
                }
        }
        else
        {
                if(SD_Type!=SD_TYPE_MMC)
                {
                        SD_SendCmd(CMD55,0,0X01); 
                        SD_SendCmd(ACMD23,cnt,0X01);//���ʹ�д�������������������������Ԥ�������д�д������� 
                }
                r1=SD_SendCmd(CMD25,sector,0X01);//����д���������ʼ��ַ
                if(r1==0)
                {
                        do
                        {
                                r1=SD_SendBlock(buf,0xFC);//����512���ֽ�  
                                buf+=512;  
                        }while(--cnt && r1==0);
                        r1=SD_SendBlock(0,0xFD);//����ֹͣλ
                }
        }   
        SD_CS_HIGH();//ȡ��Ƭѡ
        return r1;//
}

/**
 * @brief    дSD��
 * @param    ��
 * @retvsl   0���ɹ�       ������ʧ��
 * @note     1.д�ĵ�ַ������һ����������ʼ
 *           2.������SD2.0���������Ŀ�������
*/
u8 SD_Initialize(void)
{
        u8 r1;      // ���SD���ķ���ֵ
        u16 retry;  // �������г�ʱ����
        u8 buf[4];  
        u16 i;

        //SPI_SD_Init();  //��ʼ��IO
       // SD_SPI_SpeedLow(); //���õ�����ģʽ 
        SPI_SetSpeed1(SPI_SPEED_256);        
        for(i=0;i<10;i++)SPI_SentByte(0XFF);//��������74������,��ʱ����Ƭѡ���Ǹߵ�ƽ
        retry=20;
        do
        {
                r1=SD_SendCmd(CMD0,0,0x95);//���븴λ��ͬʱѡ����SPIģʽ������CMD0ʱ��CSNΪ�͵�ƽ��
        }while((r1!=0X01) && retry--);
        
        SD_Type=0;//Ĭ���޿�
        if(r1==0X01)
        {
                if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//����V2.0��SD�����е�����CMD8����Ƿ�Ϊ2.0��
                {
                        for(i=0;i<4;i++)buf[i]=SPI_SentByte(0XFF); //Get trailing return value of R7 resp
                        if(buf[2]==0X01&&buf[3]==0XAA)//���Ƿ�֧��2.7~3.6V
                        {
                                retry=0XFFFE;
                                do
                                {
                                        SD_SendCmd(CMD55,0,0X01); //����CMD55
                                        r1=SD_SendCmd(ACMD41,0x40000000,0X01);//����CMD41
                                }while(r1&&retry--);
                                if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//����SD2.0���汾,��ȡOCR��ֵ
                                {
                                        for(i=0;i<4;i++)buf[i]=SPI_SentByte(0XFF);//�õ�OCRֵ
                                        if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //���CCS (��30λ)
                                        else SD_Type=SD_TYPE_V2;   
                                }
                        }
                }
                else//����2.0��������£�����Ƿ�Ϊ1.0������mmc��
                {
                        SD_SendCmd(CMD55,0,0X01);  //����CMD55
                        r1=SD_SendCmd(ACMD41,0,0X01); //����CMD41
                        if(r1<=1)//����CMD55��CMD41�ɹ�����ʾ����1.0��
                        {  
                                SD_Type=SD_TYPE_V1;
                                retry=0XFFFE;
                                do //�ȴ��˳�IDLEģʽ
                                {
                                        SD_SendCmd(CMD55,0,0X01); //����CMD55
                                        r1=SD_SendCmd(ACMD41,0,0X01);//����CMD41 ���г�ʼ��
                                }while(r1&&retry--);
                        }
                        else //����1.0����������MMC��
                        {
                                SD_Type=SD_TYPE_MMC;//�ȼ�����MMC��
                                retry=0XFFFE;
                                do //�ȴ��˳�IDLEģʽ
                                {               
                                        r1=SD_SendCmd(CMD1,0,0X01);//����CMD1�����ø�λ�����ж��Ƿ�ΪMMC��
                                }while(r1&&retry--); //���͸�λ�����ʱ��λʧ�� 
                        }
                if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0) 
                        SD_Type=SD_TYPE_ERR;//MMC����λʧ��
                }
        }
        SD_CS_HIGH();//ȡ��Ƭѡ
        if(SD_Type)return 0; //��ʼ���ɹ�
        else if(r1)return r1; //��ʼ��ʧ��    
        return 0xaa;//��������
}


//��ȡSD�����������ֽڣ�   
//����ֵ:0�� ȡ�������� 
//       ����:SD��������(�ֽ�)  
u32 SD_GetCapacity(void)
{
        u8 csd[16];
        u32 Capacity;
        u8 r1;
        u16 i;
        u16 temp;       
        //ȡCSD��Ϣ������ڼ��������0
        if(SD_GetCSD(csd)!=0) 
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return 0;
        }                
        //���ΪSDHC�����������淽ʽ����
        if((csd[0]&0xC0)==0x40)
        {       
                Capacity=((u32)csd[8])<<8;
                Capacity+=(u32)csd[9]+1;        
                Capacity = (Capacity)*1024;//�õ�������
                Capacity*=512;//�õ��ֽ���   
        }
        else
        {       
                i = csd[6]&0x03;
                i<<=8;
                i += csd[7];
                i<<=2;
                i += ((csd[8]&0xc0)>>6);
                //C_SIZE_MULT
                r1 = csd[9]&0x03;
                r1<<=1;
                r1 += ((csd[10]&0x80)>>7);      
                r1+=2;//BLOCKNR
                temp = 1;
                while(r1)
                {
                        temp*=2;
                        r1--;
                }
                Capacity = ((u32)(i+1))*((u32)temp);    
                // READ_BL_LEN
                i = csd[5]&0x0f;
                //BLOCK_LEN
                temp = 1;
                while(i)
                {
                        temp*=2;
                        i--;
                }
                //The final result
                Capacity *= (u32)temp;//�ֽ�Ϊ��λ   
        }
        return (u32)Capacity;
}

u8 SD_Init(void)
{
        printf("[%s][%d]\r\n", __func__, __LINE__);
        u8 i;
        u8 r1;      // ���SD���ķ���ֵ
        u16 retry;  // �������г�ʱ����
        u8 buff[6];
        SPI_SetSpeed1(SPI_SPEED_256);
        SD_CS_HIGH();
        
        delay_ms(100);
        for(i=0;i<10;i++)  //���ʹ���74��ʱ������
        {
                SPI_SentByte(0xFF); //80clks
        }
        /********************SD����λ��idle״̬******************/
        retry=0;
        do 
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                r1=SD_SendCmd(CMD0,0,0x95);
                retry++;
                printf("r1=%d\n",r1);
                
        }while ( (r1!=0x01) && (retry<100) );
        if(retry==100)
        {
                printf("[%s][%d]\r\n", __func__, __LINE__);
                return 1;
        }
        /*****************SD����λ��idle����**********************/
        r1 = SD_SendCommand_NoDeassert(CMD8,0x1aa,0x87);
        if(r1==0x05)  
        {
                SD_Type = SD_TYPE_V1;
                //�����V1.0����CMD8ָ���û�к�������
                //Ƭѡ�øߣ�������������
                SD_CS_HIGH();
                SPI_SentByte(0xff);
                retry = 0;
                do
                {
                        //�ȷ�CMD55��Ӧ����0x01���������
                       r1 = SD_SendCmd(CMD55,0,0);
                       if(r1 !=0x01)
                       return r1;
                       //�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������400��
                       r1 = SD_SendCmd(ACMD41,0,0);
                       retry++;
                 }while((r1!=0x00)&&(retry<400));
                      //�ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
                      // ���л�Ӧ����SD����û�л�Ӧ����MMC��
                if(retry==400)
                {
                 retry =0;
                     //����MMC����ʼ������(û�в���)
                     do
                      {
                         r1=SD_SendCmd(CMD1,0,0);
                         retry++;
                       }while((r1!=0x00)&&(retry<400));
                       if(retry==400)
                       {
                                //д�뿨����
                                SD_Type=SD_TYPE_MMC;
                               return 1;//MMC����ʼ����ʱ
                       }
                 }
                       //----------MMC�������ʼ����������---------------
                       //����SPIΪ����ģʽ
                       SPI_SetSpeed1(SPI_SPEED_4);
                       SPI_SentByte(0xff);

                       //��ֹCRCУ��
                       r1=SD_SendCmd(CMD59,0,0x95);
                       if(r1!=0x00)return r1;//������󣬷���r1
                       //-------------SD����MMC����ʼ������-------------

           }//SD��ΪV1.0�汾�ĳ�ʼ������
                   //������V2.0���ĳ�ʼ��
                       //������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC
        else if(r1==0x01)
        {
                //v2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ڽ���������
                buff[0]=SPI_SentByte(0xff);//shoule be 0x00
                buff[1]=SPI_SentByte(0xff);//shoule be 0x00
                buff[2]=SPI_SentByte(0xff);//shoule be 0x11
                SD_CS_HIGH();
                SPI_SentByte(0xff);
                if(buff[2]==0x01&&buff[3]==0xaa)
                {
                        retry = 0;
                        do
                        {
                                retry++;
                                r1=SD_SendCmd(CMD55,0,0);
                                if(r1!=0x01)return r1;
                                r1=SD_SendCmd(ACMD41,0x40000000,1);
                                if(retry>200)return r1;
                        }while(r1!=0);
                        r1=SD_SendCommand_NoDeassert(CMD58,0,0);
                        if(r1!=0x00)return r1;
                        //��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
                          buff[0]=SPI_SentByte(0xff);
                          buff[1]=SPI_SentByte(0xff);
                          buff[2]=SPI_SentByte(0xff);
                          buff[3]=SPI_SentByte(0xff);
                        //OCR������ɣ�Ƭѡ�ø�
                        SD_CS_HIGH();
                        SPI_SentByte(0xff);
                }
                        
                  if(buff[0]&0x40)
                          SD_Type = SD_TYPE_V2HC;//���CCS
                  else 
                          SD_Type=SD_TYPE_V2;
                  //------------------����SD2.0���汾����------------------
                  //����SPIΪ����ģʽ
                  SPI_SetSpeed1(SPI_SPEED_4);
         }
                 return r1;
}
                
       
        
        
   
        

