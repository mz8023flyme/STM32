#include "bsp_i2c.h"
#include "stm32f10x.h"
#include "bsp_usart.h"

/**
 * @brief I2C��ʼ������
 * @param void
 * @note ���� SCL �� SDA �� GPIO ģʽ
 */
void I2C_init (void)
{
        GPIO_InitTypeDef        GPIO_Initstuct;
        /* ���� SCL �� SDA ��ʱ�� */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
        
        /* ���� SCL �� SDA ��ʼΪ���ģʽ */
        GPIO_Initstuct.GPIO_Pin = I2C_SCL_Pin|I2C_SDA_Pin;
        GPIO_Initstuct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Initstuct.GPIO_Mode = GPIO_Mode_Out_OD;    //��©���
        GPIO_Init(I2C_PORT,&GPIO_Initstuct);
        SCL=1;
        SDA=1;
}
/**
 * @brief ���� SDA ��Ӧ GPIO Ϊ����ģʽ
 */
void I2C_set_sda_input(void)
{
        GPIO_InitTypeDef        GPIO_Initstuct;
        GPIO_Initstuct.GPIO_Pin = I2C_SDA_Pin;
        GPIO_Initstuct.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(I2C_PORT,&GPIO_Initstuct);
}

/**
 * @brief ���� SDA ��Ӧ GPIO Ϊ���ģʽ
 */
void I2C_set_sda_output(void)
{
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.GPIO_Pin = I2C_SDA_Pin;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(I2C_PORT,&GPIO_InitStruct);
}

/**
 * @brief ���� I2C ��ʼ�ź�
 * @note �� SCL Ϊ�ߵ�ƽ�������, SDA ����һ���½��ر�ʾ��ʼ
 */
void I2C_Start(void)
{
        //I2C_set_sda_output();
        
        /* ������Ҫע��˳��, SDA ������, SCL ������ */
        /* ��Ϊ SCL Ϊ�ߵ�ƽ�������, SDA �ı��ض������������, ��� SDA Ҫ������ */
        SCL=0;
        SDA=1;
        delay_us(1);
        SCL=1;
        delay_us(1);
        /* �� SCL Ϊ�ߵ�ƽ�������, SDA ����һ���½��ر�ʾ��ʼ*/
        SDA=0;
        delay_us(1);
        /* ������ʵ���ǿ�ʼ���������ʱ���� */
        SCL=0;
}

/**
 * @brief ���� I2C ֹͣ�ź�
 * @note �� SCL Ϊ�ߵ�ƽ�������, SDA ����һ�������ر�ʾֹͣ
 */
void I2C_Stop(void)
{
        //I2C_set_sda_output();
        
        /* ������Ҫע��˳��, SCL ������, SDA ������ */
        /* ��Ϊ SCL Ϊ�ߵ�ƽ�������, SDA �ı��ض������������, ��� SCL Ҫ������ */
        SCL=0;
        SDA=0;
        delay_us(1);
        SCL=1;
        delay_us(1);
        
        SDA=1;
        delay_us(1);
        
}

/**
 * @brief �ȴ�Ӧ���źŵ���
 * @return u8 0,����Ӧ��ɹ�; -1,����Ӧ��ʧ��;
 * @note ����������һ֡����֮��, ��Ҫ�ȴ��ӻ�������Ӧ�Ż����������һ֡����
 * @note ��ʱ��������Ҫ�ų� SDA ��ʹ��Ȩ, �ɴӻ��������� SDA (��Ӧ)
 * @note ��������һ�� SCL ��������(��˵�ߵ�ƽ)��� SDA �Ƿ�Ϊ�͵�ƽ, �͵�ƽ���ʾ��Ӧ��
 */

int I2C_WaitAsk(void)
{
        /* ����һ������, ��Ϊ��ʱ�ı�� */
        int ucErrTime = 0;
        /* ��Ĭ�ϵİ� SDA ����Ϊ�ߵ�ƽ */
        SDA=1;
        delay_us(1);
        /* ������Ҫ���մӻ���Ӧ���ź�, �����Ҫ����Ϊ���� */
        //I2C_set_sda_input();
        
        /* ��һ�� SCL ʱ�ӵ��� */
        SCL=1;
        delay_us(1);
        /* ��ʱ��ȥ��ȡ SDA ������û����Ӧ�� */
        /* ��û�г�ʱ֮ǰֻҪ������Ӧ��Ϳ����Զ����� while */
        __ASM("CPSID I");        //���ж�  �������ж�����Ϊû��Ӧ��
        while(READSDA)
        {
                ucErrTime++;
                
                if(ucErrTime > 255)
                {
                        /* ��������Ϊ�ӻ�û����ȷ�Ľ���, �ʹ����� */
                        I2C_Stop();
                        printf("[%s][%d]", __func__, __LINE__);
                        /* �������ؽ���Ӧ��ʧ�� */
                        return -1;
                }
                
        }
        __ASM("CPSIE I"); //���ж�
        /* ������, ��ȡӦ���źŵ� SCL ������ */
        SCL=0;
        /* ��ִ�е�����, ˵����ȡ����Ӧ��� */
        return 0;
}

/**
 * @brief HOST ����Ӧ��
 * @note �˺������� HOST(STM32) ��������ʱ����ʹ��
 * @note ���ֻҪ����4���ֽ�, ǰ3��Ӧ��, ���һ�β�Ӧ����Զ������ˡ�
 */
void I2C_Ask(void)
{
        SCL=0;
        delay_us(1);
        
        /* ҪӦ��, �϶�Ҫ�Ȼ�ȡSDA��ʹ��Ȩ */
       // I2C_set_sda_output();
        /* �ڴӻ������걾֡���һ��λ�ĵ͵�ƽ�ڼ�, �� SDA ���� */
        /* ǧ������ SCL �ߵ�ƽ�ڼ�����, �Ǿͱ����ʼ�ź��� */
        
        SDA=0;
        delay_us(1);
        
        /* ������Ǵӻ���ȡӦ���źŵ�һ�� SCL ʱ��������, SDA ���ܶ� */
        
        SCL=1;
        delay_us(1);
        SCL=0;
        
}

/**
 * @brief HOST ������Ӧ��
 * @note �˺������� HOST(STM32) ��������ʱ����ʹ��
 * @note ���ֻҪ����4���ֽ�, ǰ3��Ӧ��, ���һ�β�Ӧ����Զ������ˡ�
 */

void I2C_NotAsk(void)
{
        SCL=0;
        delay_us(1);
        
        //I2C_set_sda_output();
        
        /* ��Ȼ���ͱ�ʾӦ��, �������߲������� */
        SDA=1;
        delay_us(1);
        
         /* ��һ��ʱ��һ���, �ͷ���, û��Ӧ�� */
        SCL=1;
        delay_us(1);
        SCL=0;
}

/**
 * @brief I2C ����һ���ֽ�
 * @param data ���͵�����
 */

void I2C_SentByte(u8 data)
{
        u8 i;
        //I2C_set_sda_output();
        
        /* ���е����ݵ������ SDA ���϶����� SCL �ĵ͵�ƽ�ڼ� */
        SCL=0;
        
        for(i=0;i<8;i++)
        {
                /* д�����ݵ����λ */
                SDA = (data & 0x80)>>7;
                 /* �����������λ, ��������һ��, �ε�λ������µ����λ */
                data <<= 1;
                delay_us(1);
                 /* �� SCL ��������(���߸ߵ�ƽ�ڼ�), ���ݱ��ӻ����ն�ȡ */
                SCL=1;
                delay_us(1);
                SCL=0;
        }
        /* �⺯��������ʱ���ǲ��� SCL = 0 */
        /* һ������������ͻ��� WaitResponse ������ */
        /* WaitResponse ��ʱ�����ֱ�Ӵ� SCL = 0 ��ʼ��, �����Ͳ�������һ�� SCL ������ */
}

/**
 * @brief I2C ��һ���ֽ�
 * @note ���ݲ�������Ҫ��ҪӦ��, ���ֻҪ����4���ֽ�, ǰ3��Ӧ��, ���һ�β�Ӧ����Զ������ˡ�
 */
u8 I2C_ReadByte(void)
{
        u8 i, data = 0;
        //I2C_set_sda_input();
        for(i = 0; i < 8; i++)
        {
                SCL=0;
                delay_us(1);
                SCL=1;
                data <<= 1;
                if(READSDA)
                {
                        data++;
                }
                delay_us(1);
        }
        return data;
}









