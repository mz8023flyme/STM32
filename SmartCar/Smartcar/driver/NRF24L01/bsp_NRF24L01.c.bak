#include "bsp_NRF24L01.h"


const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址

 /**
  * @brief  初始化判断NRF是接受模式还是发送模式用到的GPIO
  * @param  
  * @retval 
  */
void NRF_GPIO_Init(void)
{
        //定义一个GPIO结构体
        GPIO_InitTypeDef GPIO_InitStructure;

        //开启引脚时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = NRF_IRQ_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(NRF_IRQ_PORT,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = NRF_CE_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(NRF_CE_PORT,&GPIO_InitStructure);
        
        
        GPIO_InitStructure.GPIO_Pin = NRF_CS_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(NRF_CS_PORT,&GPIO_InitStructure);
        
        
        NRF24L01_CSN =  1;
        NRF24L01_CE  =  0;
}


 /**
  * @brief  检测24L01是否存在
  * @param  
  * @retval 返回值:0，成功;1，失败
  */
u8 NRF24L01_Check(void)
{
        u8 i;       
        u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
        u8 BUF[5]={0,0,0,0,0};

        
        NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	

        NRF24L01_Read_Buf(TX_ADDR,BUF,5); //读出写入的地址  
        
        for(i=0;i<5;i++)
        {
                if(BUF[i]!=0XA5)
                {
                    break;    
                }
        } 
        if(i!=5)return 1;//检测24L01错误    
        return 0;       //检测到24L01
}

 /**
  * @brief  在指定位置写指定长度的数据
  * @param  reg:寄存器(位置)
  * @param  writebuff:存储写入数据的指针
  * @param  NumByteToWrite，要写入数据长度
  * @retval 
  */
u8 NRF24L01_Write_Buf(u8 reg, u8 *writebuff, u8 NumByteToWrite)
{
        u8 status,u8_ctr;
        SPI_CS_LOW();         //使能SPI传输
        status = SPI_SendByte(reg);//发送寄存器值(位置),并读取状态值   
        //写入数据
        for(u8_ctr=0; u8_ctr<NumByteToWrite; u8_ctr++)
                SPI_SendByte(*writebuff++); //写入数据
        
        SPI_CS_HIGH();       //关闭SPI传输
        return status;          //返回读到的状态值
}

 /**
  * @brief  在指定位置读出指定长度的数据
  * @param  reg:寄存器(位置)
  * @param  readbuff:存储读出数据的指针
  * @param  NumByteToRead，要读取数据长度
  * @retval 
  */
u8 NRF24L01_Read_Buf(u8 reg,u8 *readbuff,u8 NumByteToRead)
{
        u8 status,u8_ctr;       
        SPI_CS_LOW();           //使能SPI传输
        status=SPI_SendByte(reg);//发送寄存器值(位置),并读取状态值   
//读数据

        for(u8_ctr=0;u8_ctr<NumByteToRead;u8_ctr++)readbuff[u8_ctr]=SPI_SendByte(0XFF);//读出数据

        SPI_CS_HIGH();       //关闭SPI传输
        return status;        //返回特定寄存器读取到的值
}

 /**
  * @brief  SPI 写寄存器
  * @param  reg:指定寄存器地址
  * @param  value:写入的值
  * @retval 
  */
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
        u8 status;      
        SPI_CS_LOW();                   //使能SPI传输
        status =SPI_SendByte(reg);      //发送寄存器号 
        SPI_SendByte(value);            //写入寄存器的值
        SPI_CS_HIGH();                  //禁止SPI传输 
        return(status);                 //返回特定寄存器读取到的值
}

 /**
  * @brief  SPI 读寄存器
  * @param  reg:指定寄存器地址
  * @retval 
  */
u8 NRF24L01_Read_Reg(u8 reg)
{
        u8 reg_val;     
        SPI_CS_LOW();           //使能SPI传输       
        SPI_SendByte(reg);      //发送寄存器号
        reg_val=SPI_SendByte(Dummy_Byte);//读取寄存器内容
        SPI_CS_HIGH();           //禁止SPI传输        
        return(reg_val);           //返回特定寄存器读取到的值
}

 /**
  * @brief  启动NRF24L01发送一次数据
  * @param  txbuf:存放发送的数据
  * @retval 返回值:发送完成状况
  */
u8 NRF24L01_TxPacket(u8 *txbuf)
{
        u8 sta;  
        NRF24L01_CE = 0; 
        NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节      
        NRF24L01_CE = 1;//启动发送  
        while(NRF24L01_IRQ!=0);//等待发送完成
        sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值       
        NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
        if(sta&MAX_TX)//达到最大重发次数
        {
                NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
                return MAX_TX; 
        }
        if(sta&TX_OK)//发送完成
        {
                return TX_OK;
        }
        return 0xff;//其他原因发送失败
}


 /**
  * @brief  启动NRF24L01接收一次数据
  * @param  rxbuf:用来存放接受到的数据
  * @retval 返回值:0，接收完成；其他，错误代码
  */
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
        u8 sta;
        NRF24L01_CE = 1; //进入接受状态
        while(NRF24L01_IRQ!=0);//等待接受中断
        NRF24L01_CE = 0; //进入待机状态
        sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值     
        NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
        if(sta&RX_OK)//接收到数据
        {
                NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
                NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
                return 0; 
        }       
        return 1;//没收到任何数据
}

 /**
  * @brief  该函数初始化NRF24L01到RX模式
  * @param  无
  * @retval 设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
  * @retval 当CE变高后,即进入RX模式,并可以接收数据了
  */
void RX_Mode(void)
{
        NRF24L01_CE=0;  
        NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
          
        NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
        NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址   
        NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);     //设置RF通信频率 
        NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度        
        NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
        NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
        NRF24L01_CE = 1; //CE为高,进入接收模式 
}

 /**
  * @brief  该函数初始化NRF24L01到TX模式
  * @param  
  * @retval 设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
  * @retval PWR_UP,CRC使能
  * @retval 当CE变高后,即进入RX模式,并可以接收数据了
  * @retval CE为高大于10us,则启动发送.
  */
void TX_Mode(void)
{       
        
        NRF24L01_CE=0; 
        NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
        NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置RX节点地址,主要为了使能ACK	  

        NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
        NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
        NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
        NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
        NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
        NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
        NRF24L01_CE=1;//CE为高,10us后启动发送
        delay_us(20);
}
