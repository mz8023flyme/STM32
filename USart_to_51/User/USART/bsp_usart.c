#include "bsp_usart.h"

static void NVIC_Configuration(void)
{
        NVIC_InitTypeDef NVIC_InitStructure;

        /* 嵌套向量中断控制器组选择 */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

        /* 配置USART为中断源 */
        NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
        /* 抢断优先级*/
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        /* 子优先级 */
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        /* 使能中断 */
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        /* 初始化配置NVIC */
        NVIC_Init(&NVIC_InitStructure);
}

void USART_Config (void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;      //配置连接到的GPIO
        USART_InitTypeDef  USART_InitStructure;         //配置串口初始化

        // 打开串口GPIO的时钟
        DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
        // 打开串口外设的时钟
        DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //推挽输出
        GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;  //PA9   PA10
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;         //配置硬件控制流
        USART_InitStructure.USART_Mode  =   USART_Mode_Rx | USART_Mode_Tx;                      //接收发送引脚
        USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;                              //波特率
        USART_InitStructure.USART_Parity = USART_Parity_No;                                     //校验
        USART_InitStructure.USART_StopBits = USART_StopBits_1;                                  //停止位
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;                             //字长
        USART_Init(DEBUG_USARTx, &USART_InitStructure);
        // 串口中断优先级配置
        NVIC_Configuration();

        // 使能串口接收中断
        USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE); 

        // 使能串口
        USART_Cmd(DEBUG_USARTx, ENABLE);
}


void USART_sent( USART_TypeDef*pUSARTx, uint8_t Data )                  //发送字符函数
{
        USART_SendData(pUSARTx,Data); 
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);  //等待发送完成
}


void USART_Sentstring(USART_TypeDef*pUSARTx,char *str)                        //发送字符串函数
{
        unsigned int k = 0;
        do
        {
                USART_sent( pUSARTx, *(str+k) );
                k++;
        }while (*(str+k)!='\0');
        while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);       //等待发送完成
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
        /* 发送一个字节数据到串口 */
        USART_sent(DEBUG_USARTx, (uint8_t) ch);
        
        /* 等待发送完毕 */
        while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET); 
        return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
        /* 等待串口输入数据 */
        while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

        return (int)USART_ReceiveData(DEBUG_USARTx);
}








