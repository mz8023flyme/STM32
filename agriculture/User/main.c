#include "main.h"

const uint32_t Administrator_ID = 0xD92717A3;           //����Ա����
const uint32_t Initial=0x00000000;                      //Ĭ��ûˢ�����ķ���ֵ
const u8  Open_Door=  10;
const u8  Close_Door=  20;

u8 Temp_flag = 1;                      //�Ƿ����Զ�����ϵͳ
u8 Show_flag = 1;
u8 Show_num  = 1;
u8 BEEP_Flag = 0 ;
u8 Irrigation_flag=0;   //����־
u8 Exhaust_flag=0;      //�ŷ��־
u8 Administ_Flag=0;            //����Ա��־�����ڿ�ʼ��ʱ�����������ǹ���Ա�Ž���¼��ģʽ
u8 Administ_Entering=0;        //����ȷ���Ƿ����¼��ģʽ�ı�־
float Illumination=0;                   //��ǿֵ
u8 anomaly = 0;           //�쳣���� 0δ���� 1δ֪��ˢ��  2�¶ȹ���
char temp[20];                  //װ���ַ���������Ļ��ʾ
char Humi[20];
char lighting[20];
char Blank[16]="               ";   //15���ո� ������
u32 ID_Card[256];                       //���ڴ洢¼��Ŀ���
u8 Step=0;                      //¼���Ĳ���
u8 Mode;
u8 Door_Flag=2;         //0Ĭ��  ��������   1����  2����


int main(void)
{
        System_Init();                  //ϵͳ��ʼ��
        printf("[%s][%d]\r\n", __func__, __LINE__);
        
        /*************************�ֲ������Ķ���**************************/
        char cCard_str[20];
        u8 Entering_Flag=1;
        uint32_t New_Card =0x22222222;      //�µĿ��ţ�������ʱ�洢��Ҫ¼��Ŀ���
        uint32_t New_Card1=0x88888888;      //�µĿ��ţ�������ʱ�洢��Ҫ¼��Ŀ���
        uint32_t New_Card2=0x66666666;      //�µĿ��ţ�������ʱ�洢��Ҫ¼��Ŀ���
//              The_System_First_Run();                //��������
        uint32_t *p_Pos= (uint32_t *)(Pos_Address);
        uint32_t  Card_Pos=*p_Pos;
        uint32_t *p_Card;
        u8 i;           //���ڼ���Կ�׿��ѭ�������Լ�������ѭ������

        u8 Temperature_Max=30.00;          //����Ԥ������ֵ
        
        //�� flash ����� ID �ż��ص��ڴ�����
        printf("\r\n������%d������\r\n",*p_Pos);
        for(i=0;i<Card_Pos;i++)
        {
                p_Card = (uint32_t *)(Card_Address+i*4);
                ID_Card[i]=*p_Card;
                printf("IDΪ%8X\r\n",ID_Card[i]);
        }
        
        Mode = General_Mode;
        while(1)
        {
//                TemDown_ON;
//                delay_ms(2000);
//                
//                TemDown_OFF;
//                delay_ms(2300);
//                printf("Show_flag=%d\n",Show_flag);
//                printf("ռ�ձ�%d\n",TIM3->CCR4);
                updata();
                if(Temp_int>Temperature_Max)
                {
                        Mode=Abnormal_Mode;
                        anomaly=2;
                }
//                else          //ģʽ��������ı�
//                {
//                        Mode=General_Mode;
//                }
                New_Card=RFID_Number();
                //���ڴ����ѯ�Ƿ����������
                for(i=0;i<Card_Pos;i++)
                {
                        if(New_Card==ID_Card[i])
                        {
                                Door_Flag=1;
                                Door_Time=0;
                                if(Mode!=Entering_Mode)
                                {
                                        BEEP_OFF;
                                        Mode=General_Mode;
                                        BEEP_Flag=0;
                                }
                                break;
                        }
                        if(i==Card_Pos-1 && New_Card!=Initial && Mode!=Entering_Mode)
                        { 
                                //��ѯ��δ�ҵ���ȷ��Ϊδ֪��ˢ���򱨾�
                                Mode=Abnormal_Mode;
                                Door_Flag=2;
                                Door_Time=0;
                                BEEP_Flag=1;
                                BEEP_Time=0;
                                anomaly=1;
                        }
                }
                //��ͨģʽ�µĹ���Ա���Ų��ҿ�ʼ��ʱ
                
                if( (New_Card == Administrator_ID) && (Mode==General_Mode) && Administ_Entering==0 )
                {
                        Administ_Flag=1;             //��־λ��1�ö�ʱ����ʼ��ʱ
                        
                }
                //5��ʱ�䵽   ���Ҳ��迨��ģʽ����ȷ��ô������¼��ģʽ
                if( (Administ_Entering!=0) && Step==0 &&  (Administrator_ID == New_Card)  && (Mode==General_Mode) )
                {
                        Administ_Entering=0;
                        Administ_Time=0;                 //����¼��ģʽ֮��Administ_Entering�ر�
                        Step=1;                          //ͬʱ��ʱ����  �������߹���
                        Mode=Entering_Mode;
                        delay_ms(1000);
                        printf("��ʼ¼���µĿ���\r\n");
                        OLED_ShowStr(0,0,Blank,2);         //16���ո�
                        OLED_ShowStr(0,1,Blank,2);
                        OLED_ShowStr(0,2,"    ",2);
                        OLED_ShowCN(32,2,4,begin_card1);
                        OLED_ShowStr(97,2,"    ",2);
                        OLED_ShowStr(0,4,"    ",2);
                        OLED_ShowCN(32,4,4,begin_card2);
                        OLED_ShowStr(97,4,"    ",2);
                        OLED_ShowStr(0,6,Blank,2);
                        OLED_ShowStr(0,7,Blank,2);
                }
                switch(Mode)
                {
                        case Abnormal_Mode:             //�쳣ģʽ
                        
                        if(BEEP_Flag!=0)        //��������30���ֹͣ
                        {                       
                                BEEP_ON;
                                        //��Ļ������ʾ�쳣ԭ��ֱ������Ϊֹ
                                TIM3->CCR3=Close_Door;    //0�����
                                OLED_ShowStr(0,0,Blank,2);         //16���ո�
                                OLED_ShowStr(0,1,Blank,2);
                                OLED_ShowStr(0,5,Blank,2);
                                OLED_ShowStr(0,6,Blank,2);
                                OLED_ShowStr(0,7,Blank,2);
                                switch(anomaly)
                                {
                                        case 0  :       // δ����
                                                OLED_ShowStr(0,3,"    ",2);
                                                OLED_ShowStr(97,3,"   ",2);
                                                OLED_ShowCN(32,3,4,Nodoor);
                                                break;
                                        case 1  :       // δ֪��ˢ��
                                                OLED_ShowStr(0,3,"    ",2);
                                                OLED_ShowStr(97,3,"   ",2);
                                                OLED_ShowCN(32,3,2,NFC);
                                                OLED_ShowCN(64,3,2,fail);
                                                break;
                                        case 2  :       // ����Ԥ��
                                                OLED_ShowStr(0,3,"    ",2);
                                                OLED_ShowStr(97,3,"   ",2);
                                                OLED_ShowCN(32,3,4,High_Temperature);
                                                break;
                                        default :
                                                break;
                                }
                        }
                        else
                        {
                                Mode=General_Mode;
                                BEEP_OFF;
                        }
                        break ; 
                        
                        case Entering_Mode:             //¼��ģʽ
                                        
                                        if(Step==1 && New_Card!=Administrator_ID && New_Card!=Initial  )
                                        {
                                                New_Card1=New_Card;
                                                Step=2;
                                                printf("New_Card1:%08X  ",New_Card1);
                                                __ASM("CPSID I");        //���ж�
                                                sprintf(cCard_str,"    %08X    ",New_Card1);
                                                __ASM("CPSIE I"); //���ж�
                                                OLED_ShowCN(32,1,2,NFC);
                                                OLED_ShowCN(64,1,2,successful);
                                                OLED_ShowStr(0,3,cCard_str,2);
                                                OLED_ShowStr(0,5,"     ",2);
                                                OLED_ShowStr(89,5,"    ",2);
                                                OLED_ShowCN(40,5,3,please);
                                                
                                        }
                                        if(New_Card==Administrator_ID && Step==2)
                                        {
                                                Step=3;
                                                OLED_ShowStr(0,0,Blank,2);
                                                OLED_ShowStr(0,1,Blank,2);
                                                OLED_ShowStr(0,2,Blank,2);
                                                OLED_ShowStr(0,5,Blank,2);
                                                OLED_ShowStr(0,6,Blank,2);
                                                OLED_ShowCN(24,3,5,pleaseagain);
                                                
                                        }
                                        if( Step==3 && New_Card!=Administrator_ID && New_Card!=Initial )
                                        {
                                                New_Card2=New_Card;
                                                Step=4;
                                        }

                                        if( Step==4 && New_Card1==New_Card2 )
                                        {
                                               
                                                for (i=0;i<Card_Pos;i++)
                                                {
                                                        if(New_Card2==ID_Card[i])
                                                        {
                                                                printf("�˿���¼��\r\n");
                                                                OLED_ShowCN(24,3,5,card_already);
                                                                delay_ms(500);
                                                                Entering_Flag=0;
                                                                break;
                                                        }
                                                }
                                                if(Entering_Flag!=0)
                                                {
                                                        ID_Card[Card_Pos]=New_Card2;
                                                        FLASH_Unlock();
                                                        FLASH_ProgramWord(Card_Address+Card_Pos*4,New_Card2);
                                                        FLASH_ErasePage(Pos_Address);
                                                        FLASH_ProgramWord(Pos_Address,Card_Pos+1);
                                                        FLASH_Lock();
                                                        Card_Pos=Card_Pos+1;
                                                        printf("¼��ɹ�\r\n");
                                                        printf("�µĿ���Ϊ��%08X\r\n",New_Card2);
//                                                        OLED_ShowStr(0,3,Blank,2);
                                                        OLED_ShowStr(0,2,"    ",2);
                                                        OLED_ShowStr(0,97,"   ",2);
                                                        OLED_ShowCN(32,2,2,enter);
                                                        OLED_ShowCN(64,2,2,successful);
                                                        OLED_ShowStr(0,4,cCard_str,2);
                                                        delay_ms(500);
                                                }
                                                Entering_Flag=1;
                                                Step=0;
                                                Mode= General_Mode;
                                        }
                                        if(New_Card1!=New_Card2 && Step==4)
                                        {
                                                printf("���ſ���һ��\r\n");
                                                OLED_ShowCN(16,3,6,inconformity);
                                                delay_ms(500);
                                                Step=0;
                                                Mode= General_Mode;
                                        }
                        break ;
                        
                        case General_Mode:              //��ͨģʽ
                                
                                if(Temp_flag!=0)
                                {
                                        Temperature_System();   //���ú���ϵͳִ�к���
                                }
                                
                                switch(Door_Flag)   //0Ĭ��  ��������   1����  2����
                                {
                                        case 1:
                                                TIM3->CCR3=Open_Door;    //90�㿪��
                                                break;
                                        case 2:
                                                TIM3->CCR3=Close_Door;    //0�����
                                                delay_ms(1000);  //�������λ��һ����ʱ��
                                                Door_Flag=0;
                                                break;
                                        default:
                                                TIM3->CCR3=0;    //��ֹ����
                                                break;
                                }
                                //����   �����ǿС��һ����ʱ��Ϳ���
                                if( Illumination < light_Min)
                                {
                                        LED_ON;
                                }
                                if( Illumination > light_Max)
                                {
                                        LED_OFF;
                                }
                                
                                
                                if(Show_flag!=0)//��ʾ������������
                                {
//                                        __ASM("CPSID I");        //���ж�
                                        sprintf(temp,"%d.%d     ",Temp_int,Temp_deci);
                                        //DHT11��ʪ��
                                        sprintf(Humi,"%d.%d    ",Humi_int,Humi_deci);
//                                        //������ʪ��
//                                        sprintf(Humi,"%.1f     ",Soil_Humidity);  //4���ո�����
                                        sprintf(lighting,"%4.2f    ",Illumination);
//                                        __ASM("CPSIE I"); //���ж�
                                        OLED_ShowStr(0,0,"   ",2);     //4���ո�
                                        OLED_ShowStr(97,0,"  ",2);     //3���ո�
                                        OLED_ShowCN(32,0,4,title);      //��ʾ����----�ǻ�ũҵ
                                        OLED_ShowCN(0,2,3,temp_code);   //��ʾ�¶�
                                        OLED_ShowCN(0,4,3,Humi_code);   //��ʾʪ��
                                        OLED_ShowCN(0,6,3,light);       //��ʾ��ǿ
                                        OLED_ShowStr(49,2,temp,2);
                                        OLED_ShowStr(49,4,Humi,2);
                                        OLED_ShowStr(49,6,lighting,2);
//                                        __ASM("CPSIE I"); //���ж�
                                       
                                }
                                else
                                {
//                                        __ASM("CPSID I");        //���ж�
                                        OLED_ShowStr(0,0,"    ",2);     //4���ո�
                                        OLED_ShowStr(97,0,"   ",2);     //3���ո�
                                        OLED_ShowCN(32,0,4,title);      //��ʾ����----�ǻ�ũҵ
                                        OLED_ShowCN(0,2,2,Air_Temp);
                                        OLED_ShowCN(0,4,2,Irrigation);
                                        OLED_ShowCN(0,6,2,Exhaust);
                                        OLED_ShowCN(33,2,3,System);
                                        OLED_ShowCN(33,4,3,System);
                                        OLED_ShowCN(33,6,3,System);
                                        if(Temp_flag!=0)
                                        {
                                                OLED_ShowCN(81,2,2,Open);
                                        }
                                        else
                                        {
                                                OLED_ShowCN(81,2,2,Close);
                                        }
                                        if(Irrigation_flag!=0)
                                        {
                                                OLED_ShowCN(81,4,2,Open);
                                        }
                                        else
                                        {
                                                OLED_ShowCN(81,4,2,Close);
                                        }
                                        if(Exhaust_flag!=0)
                                        {
                                                OLED_ShowCN(81,6,2,Open);
                                        }
                                        else
                                        {
                                                OLED_ShowCN(81,6,2,Close);
                                        }
//                                        __ASM("CPSIE I"); //���ж�
                                      
                                }
                               
                        break ;
                        default :
                                break;
                }
                
        }
        
}



void  Temperature_System (void)
{
        if(Temp_flag!=0)
        {
                if( Temperature > temp_max )
                {
                                 //�ر�̫���Ʋ�����ˮ�����н���
//                        TemDown_ON;
                }
                else if ( Temperature < ((temp_max+temp_min)/2)  )
                {
//                        TemDown_OFF;
                }
                else if ( Temperature > ((temp_max+temp_min)/2)  )
                {
                        LED_OFF ;
                }
                else if ( Temperature < temp_min )
                {
                              //�ر�ˮ��������̫���ƽ�������
                        LED_ON;
                }
        }
}
void System_Init(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();     //usart1
        DHT11_Init();
        USART_Config();    //usart2
        LED_GPIO_Config();
        BASIC_TIM_Init();
        GENERAL_TIM_Init();
        I2C_init();
        OLED_Init();
        TSL2561_Init();
        spi_Init();
        ADC1_Init();
        RC522_Init();
}

uint32_t RFID_Number ( void )
{
        u8 Array_ID [ 4 ];                  //�Ⱥ���IC�������ͺ�UID(IC�����к�)
        u8 Status,i;
        u32 Card_Number=Initial;
        if ( ( Status = PcdRequest ( PICC_REQALL, Array_ID ) ) != MI_OK )       //Ѱ��
        {
                Status = PcdRequest ( PICC_REQALL, Array_ID );
        }
        if(Status==MI_OK)
        {
                if ( PcdAnticoll ( Array_ID ) == MI_OK )
                {
                        for(i=0;i<4;i++)
                        Card_Number=(Card_Number<<8)|Array_ID[i];
                        printf("card:%08X\r\n",Card_Number);
                }
        }
        return Card_Number;
}

void The_System_First_Run( void )  //оƬ�״�����ʱ��Ҫ���������еĴ�����ִ�д˺�������flash ����д����Ӧ������
{
        uint32_t * P;
        FLASH_Unlock();                 //�˺���һ������ԭ��¼���Կ�׺Ž��ᱻɾ��   ����
        FLASH_ErasePage(Card_Address);
        FLASH_ErasePage(Pos_Address);
        FLASH_ProgramWord(Card_Address,Administrator_ID);
        FLASH_ProgramWord(Pos_Address,0x00000001);
        FLASH_Lock();
        P = (uint32_t *)(Card_Address);
        printf("\r\n����Ա����Ϊ:%8X",*P);
        P = (uint32_t *)(Pos_Address);
        printf("\r\n��ǰ��һ���յ�ַΪ:%d",*P);
}

void updata (void)
{
                DHT11_Data_TypeDef   DHT11_Data;
                Illumination=Read_Light();
                Soil_Humidity =(float) ADC_ConvertedValue/40.96;
                Soil_Humidity=100-Soil_Humidity;
                        if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
                {
                        Temp_int=DHT11_Data.temp_int;
                        Temp_deci=DHT11_Data.temp_deci;
                        Humi_int=DHT11_Data.humi_int;
                        Humi_deci=DHT11_Data.humi_deci;
                //��DHT11�ṹ��Ļ������������ȱ�������DHT11��������ǿ��ֵҲ�޷�����
//                sprintf ( DataStr,"{\"Temperature\":\"%d.%d\",\"Humidity\":\"%d.%d\",\"Illumination\":\"%0.2f\"}",\
//                DHT11_Data.temp_int,DHT11_Data.temp_deci,DHT11_Data.humi_int,DHT11_Data.humi_deci,Illumination);
                }
}







/*  json ��ʽ��װ��ʽ  */
//{"Temperature":"23.00","Humidity":"59.00","CO2":"21.6","PH":"4.6","Illumination":"6.5"}
//Message: {"method":"setMotor1","params":false}
//Message: {"method":"setCarPos","params":"73"}
//Message: {"method":"setHydrovalve1","params":false}
//                printf("[%s][%d]", __func__, __LINE__);
//setMotor1_true
//setHydrovalve1_false
