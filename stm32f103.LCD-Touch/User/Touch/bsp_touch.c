#include "bsp_touch.h"
#include <stdio.h> 
#include <string.h>
#include "bsp_lcd.h"
#include "fonts.h"
#include "bsp_spi.h"
#include "bsp_led.h"
#include "bsp_usart.h" 


extern uint16_t LCD_X_LENGTH;
extern uint16_t LCD_Y_LENGTH;
extern uint8_t LCD_SCAN_MODE;

/******************************* ���� XPT2046 ȫ�ֱ��� ***************************/
//Ĭ�ϴ�����������ͬ����Ļ���в��죬�����µ��ô���У׼������ȡ
strType_LCD_TouchPara strLCD_TouchPara[] = { 	
-0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,//ɨ�跽ʽ0
0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,//ɨ�跽ʽ1
0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,//ɨ�跽ʽ2
0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,//ɨ�跽ʽ3
0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,//ɨ�跽ʽ4
-0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,//ɨ�跽ʽ5
-0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,//ɨ�跽ʽ6
-0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,//ɨ�跽ʽ7
}; 

/************************��̬����������************************/
static void LCD_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad )  ;
static uint16_t LCD_ReadAdc ( uint8_t ucChannel );
static uint8_t LCD_ReadAdc_Smooth_XY ( strType_LCD_Coordinate * pScreenCoordinate );
static uint8_t LCD_Calculate_CalibrationFactor ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_Coordinate * pScreenSample, strType_LCD_Calibration * pCalibrationFactor );
static void LCD_DrawCross ( uint16_t usX, uint16_t usY );



volatile uint8_t LCD_TouchFlag = 0;


void TOUCH_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

        // ��SPI GPIO��ʱ��
        RCC_APB2PeriphClockCmd(Touch_SPI_GPIO_CLK, ENABLE);	

        // ��CS CLK MOSI ��GPIO����Ϊ�������ģʽ
        GPIO_InitStructure.GPIO_Pin = Touch_SPI_CS_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(Touch_SPI_CS_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = Touch_SPI_CLK_PIN;
        GPIO_Init(Touch_SPI_CLK_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = Touch_SPI_MOSI_PIN;
        GPIO_Init(Touch_SPI_MOSI_PORT, &GPIO_InitStructure);		

        // ��PENIRQ MISO ��GPIO����Ϊ��������ģʽ
        GPIO_InitStructure.GPIO_Pin = Touch_SPI_MISO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(Touch_SPI_MISO_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = Touch_PENIRQ_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(Touch_PENIRQ_GPIO_PORT, &GPIO_InitStructure);
                
}



/**
  * @brief  ���� XPT2046 �ļ�΢�뼶��ʱ����
  * @param  nCount ����ʱ����ֵ����λΪ΢��
  * @retval ��
  */    
static void DelayUS ( __IO uint32_t ulCount )
{
        uint32_t i;


        for ( i = 0; i < ulCount; i ++ )
        {
                uint8_t uc = 12;     //����ֵΪ12����Լ��1΢��  
              
                while ( uc -- );     //��1΢��	

        }

}


//��������
void Touch_SendCMD(uint8_t cmd) 
{
        uint8_t i;

        /*�������ŵĳ�ʼ״̬*/
        Touch_CS_DISABLE();
        Touch_CLK_LOW();
        Touch_MOSI_0();

        /*Ƭѡѡ��*/
        DelayUS(10);
        Touch_CS_ENABLE();

        /*����8��ʱ�ӣ���������*/
        for(i=0;i<8;i++)
        {
                if( (cmd & (0x80>>i)) == 0 )
                {
                        Touch_MOSI_0();
                }
                else
                {
                        Touch_MOSI_1(); 
                }       
                
                Touch_CLK_HIGH();       
                DelayUS(5);
                        
                Touch_CLK_LOW();
                DelayUS(5);     
        }       

        //      /*Ƭѡȡ��*/
        //      XPT2046_CS_ENABLE();

}





//��ȡ����
uint16_t Touch_ReceiveData(void)        
{
        uint8_t i;

        uint16_t receive_temp=0;

//      /*�������ŵĳ�ʼ״̬*/
//      XPT2046_CS_ENABLE();
//      XPT2046_CLK_LOW();
//      XPT2046_MOSI_0();
//      
//      /*Ƭѡѡ��*/
//      XPT2046_DelayUS(10);
//      XPT2046_CS_DISABLE();

        Touch_CLK_HIGH();       
        DelayUS(5);
        Touch_CLK_LOW();
        DelayUS(5);     

        /*����12��ʱ�ӣ���ȡ����*/
        for(i=0;i < 12;i++)
        {       
                receive_temp = receive_temp << 1;

                Touch_CLK_HIGH();       
                DelayUS(5);
                
                
                if( Touch_MISO() == Bit_SET)
                {
                        receive_temp |= 0x01;
                }
                else
                {
                        receive_temp |= 0x00;   
                }       

                        
                Touch_CLK_LOW();
                DelayUS(5);     
                

        }       

        /*Ƭѡȡ��*/
        Touch_CS_DISABLE();

        return receive_temp;

}





//�������״̬��
uint8_t touch_detect(void)
{
        static Touch_State touch_state = TOUCH_STATE_RELEASE; 
        static uint8_t i;
        uint8_t result;
        
        switch(touch_state)
        {
                case TOUCH_STATE_RELEASE:
                        if(Touch_PENIRQ_Read() == Touch_PENIRQ_ActiveLevel)
                        {
                                        touch_state = TOUCH_STATE_WAITING;
                                        result = TOUCH_NOT_PRESSED;
                        }
                        else
                        {       
                                        touch_state = TOUCH_STATE_RELEASE;
                                        result = TOUCH_NOT_PRESSED;
                        }       
                break;
                
                case TOUCH_STATE_WAITING:
                if(Touch_PENIRQ_Read() == Touch_PENIRQ_ActiveLevel)
                {
                        i++;
                        
                        if(i>10)
                        {
                                touch_state = TOUCH_STATE_PRESSED;
                                result = TOUCH_PRESSED;
                        }       
                        else
                        {
                                touch_state = TOUCH_STATE_WAITING;
                                result = TOUCH_NOT_PRESSED;     
                        }       
                                
                }
                else
                {       
                        i = 0;
                        touch_state = TOUCH_STATE_RELEASE;
                        result = TOUCH_NOT_PRESSED;
                }       
                
                        break;
                
                case TOUCH_STATE_PRESSED:       
                        if(Touch_PENIRQ_Read() == Touch_PENIRQ_ActiveLevel)
                        {
                                        touch_state = TOUCH_STATE_PRESSED;
                                        result = TOUCH_PRESSED;
                        }
                        else
                        {       
                                        touch_state = TOUCH_STATE_RELEASE;
                                        result = TOUCH_NOT_PRESSED;
                        }       
                        
                        break;
        
        }
        
        return result;
}

/**
  * @brief  �� XPT2046 ѡ��һ��ģ��ͨ��������ADC��������ADC�������
  * @param  ucChannel
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0x90 :ͨ��Y+��ѡ�������
  *     @arg 0xd0 :ͨ��X+��ѡ�������
  * @retval ��ͨ����ADC�������
  */
static uint16_t LCD_ReadAdc ( uint8_t ucChannel )
{
        Touch_SendCMD ( ucChannel );

  return        Touch_ReceiveData ();
        
}

/**
  * @brief  ��ȡ LCD ��Xͨ����Yͨ����ADֵ��12 bit�������4096��
  * @param  sX_Ad �����Xͨ��ADֵ�ĵ�ַ
  * @param  sY_Ad �����Yͨ��ADֵ�ĵ�ַ
  * @retval ��
  */
static void   LCD_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad )  
{ 
        int16_t sX_Ad_Temp, sY_Ad_Temp; 

        sX_Ad_Temp = LCD_ReadAdc ( LCD_CHANNEL_X );

        DelayUS ( 1 ); 

        sY_Ad_Temp = LCD_ReadAdc ( LCD_CHANNEL_Y ); 

        * sX_Ad = sX_Ad_Temp; 
        * sY_Ad = sY_Ad_Temp; 	
}

/**
  * @brief  �ڴ��� LCD ��Ļʱ��ȡһ�������ADֵ�����Ը���������˲�
  * @param  ��
  * @retval �˲�֮�������ADֵ
  */

static uint8_t LCD_ReadAdc_Smooth_XY ( strType_LCD_Coordinate * pScreenCoordinate )
{
        uint8_t ucCount = 0, i;

        int16_t sAD_X, sAD_Y;
        int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //����X��Y���ж�β���

        //�洢�����е���Сֵ�����ֵ
        int32_t lX_Min, lX_Max, lY_Min, lY_Max;


        /* ѭ������10�� */ 
        do      
        {       
                LCD_ReadAdc_XY ( & sAD_X, & sAD_Y );  
                
                sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
                sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
                
                ucCount ++;  
                
        }       while ( ( Touch_PENIRQ_Read() == Touch_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );//�û����������ʱ��TP_INT_IN�ź�Ϊ�� ���� ucCount<10


        /*������ʵ���*/
        if ( Touch_PENIRQ_Read() != Touch_PENIRQ_ActiveLevel )
                LCD_TouchFlag = 0;      //�жϱ�־��λ


        /*����ɹ�����10������*/
        if ( ucCount ==10 )     
        {
                lX_Max = lX_Min = sBufferArray [ 0 ] [ 0 ];
                lY_Max = lY_Min = sBufferArray [ 1 ] [ 0 ];       
                
                for ( i = 1; i < 10; i ++ )
                {
                        if ( sBufferArray[ 0 ] [ i ] < lX_Min )
                                lX_Min = sBufferArray [ 0 ] [ i ];
                        
                        else if ( sBufferArray [ 0 ] [ i ] > lX_Max )
                                lX_Max = sBufferArray [ 0 ] [ i ];

                }
                
                for ( i = 1; i < 10; i ++ )
                {
                        if ( sBufferArray [ 1 ] [ i ] < lY_Min )
                                lY_Min = sBufferArray [ 1 ] [ i ];
                        
                        else if ( sBufferArray [ 1 ] [ i ] > lY_Max )
                                lY_Max = sBufferArray [ 1 ] [ i ];

                }
                
                
                /*ȥ����Сֵ�����ֵ֮����ƽ��ֵ*/
                pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
                                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;
                
                pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
                                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
                
                
                return 1;       
                
        }       
        return 0;       
}

/**
  * @brief  ���� LCD ��������У��ϵ����ע�⣺ֻ����LCD�ʹ�����������Ƕȷǳ�Сʱ,�����������湫ʽ��
  * @param  pDisplayCoordinate ����Ļ��Ϊ��ʾ����֪����
  * @param  pstrScreenSample ������֪����㴥��ʱ LCD ����������
  * @param  pCalibrationFactor ��������Ϊ�趨����Ͳ�����������������������Ļ����У��ϵ��
  * @retval ����״̬
  *   	    �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :����ɹ�
  *     @arg 0 :����ʧ��
  */
static uint8_t LCD_Calculate_CalibrationFactor ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_Coordinate * pScreenSample, strType_LCD_Calibration * pCalibrationFactor )
{
        uint8_t ucRet = 1;


        /* K�� ( X0��X2 )  ( Y1��Y2 )�� ( X1��X2 )  ( Y0��Y2 ) */
        pCalibrationFactor -> Divider =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
                                                                                         ( ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;


        if (  pCalibrationFactor -> Divider == 0  )
                ucRet = 0;

        else
        {
                /* A�� (  ( XD0��XD2 )  ( Y1��Y2 )�� ( XD1��XD2 )  ( Y0��Y2 ) )��K	*/
                pCalibrationFactor -> An =  ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
                                                                                ( ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) );
                
                /* B�� (  ( X0��X2 )  ( XD1��XD2 )�� ( XD0��XD2 )  ( X1��X2 ) )��K	*/
                pCalibrationFactor -> Bn =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) ) - 
                                                                                ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) );
                
                /* C�� ( Y0 ( X2XD1��X1XD2 )+Y1 ( X0XD2��X2XD0 )+Y2 ( X1XD0��X0XD1 ) )��K */
                pCalibrationFactor -> Cn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .x - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .x ) * pScreenSample [ 0 ] .y +
                                                                                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .x - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .x ) * pScreenSample [ 1 ] .y +
                                                                                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .x - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .x ) * pScreenSample [ 2 ] .y ;
                
                /* D�� (  ( YD0��YD2 )  ( Y1��Y2 )�� ( YD1��YD2 )  ( Y0��Y2 ) )��K	*/
                pCalibrationFactor -> Dn =  ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
                                                                                ( ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
                
                /* E�� (  ( X0��X2 )  ( YD1��YD2 )�� ( YD0��YD2 )  ( X1��X2 ) )��K	*/
                pCalibrationFactor -> En =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) ) - 
                                                                                ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) ) ;
                
                
                /* F�� ( Y0 ( X2YD1��X1YD2 )+Y1 ( X0YD2��X2YD0 )+Y2 ( X1YD0��X0YD1 ) )��K */
                pCalibrationFactor -> Fn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .y - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .y ) * pScreenSample [ 0 ] .y +
                                                                                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .y - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .y ) * pScreenSample [ 1 ] .y +
                                                                                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .y - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .y ) * pScreenSample [ 2 ] .y;
                        
        }


        return ucRet;


}


/**
  * @brief  �� LCD ����ʾУ������ʱ��Ҫ��ʮ��
  * @param  usX �����ض�ɨ�跽����ʮ�ֽ�����X����
  * @param  usY �����ض�ɨ�跽����ʮ�ֽ�����Y����
  * @retval ��
  */
static void LCD_DrawCross ( uint16_t usX, uint16_t usY )
{
        LCD_DrawLine(usX-10,usY,usX+10,usY);
        LCD_DrawLine(usX, usY - 10, usX, usY+10);	
}

/**
  * @brief  LCD ������У׼
  * @param        LCD_Mode��ָ��ҪУ������Һ��ɨ��ģʽ�Ĳ���
  * @note  ���������ú���Һ��ģʽ����ΪLCD_Mode
  * @retval У׼���
  *   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :У׼�ɹ�
  *     @arg 0 :У׼ʧ��
  */
uint8_t LCD_Touch_Calibrate ( uint8_t LCD_Mode ) 
{

                uint8_t i;
                
                char cStr [ 100 ];
                
                uint16_t usTest_x = 0, usTest_y = 0, usGap_x = 0, usGap_y = 0;
                
          char * pStr = 0;

        strType_LCD_Coordinate strCrossCoordinate[4], strScreenSample[4];
          
          strType_LCD_Calibration CalibrationFactor;
                
                LCD_SetFont(&Font8x16);
                LCD_SetColors(BLUE,BLACK);

                //����ɨ�跽�򣬺���
                LCD_GramScan ( LCD_Mode );
                
                
                /* �趨��ʮ���ֽ��������� */ 
                strCrossCoordinate [0].x = LCD_X_LENGTH >> 2;
                strCrossCoordinate[0].y = LCD_Y_LENGTH >> 2;
                
                strCrossCoordinate[1].x = strCrossCoordinate[0].x;
                strCrossCoordinate[1].y = ( LCD_Y_LENGTH * 3 ) >> 2;
                
                strCrossCoordinate[2].x = ( LCD_X_LENGTH * 3 ) >> 2;
                strCrossCoordinate[2].y = strCrossCoordinate[1].y;
                
                strCrossCoordinate[3].x = strCrossCoordinate[2].x;
                strCrossCoordinate[3].y = strCrossCoordinate[0].y;      
                
                
                for ( i = 0; i < 4; i ++ )
                { 
                        LCD_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH );       
                        
                        pStr = "Touch Calibrate ......";        
                        //����ո񣬾�����ʾ
                        sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
                        LCD_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );       
                
                        //����ո񣬾�����ʾ
                        sprintf ( cStr, "%*c%d",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width)) -1)/2,' ',i + 1 );
                        LCD_DispStringLine_EN (( LCD_Y_LENGTH >> 1 ) - (((sFONT *)LCD_GetFont())->Height), cStr ); 
                
                        DelayUS ( 300000 );     //�ʵ�����ʱ���б�Ҫ
                        
                        LCD_DrawCross ( strCrossCoordinate[i] .x, strCrossCoordinate[i].y );  //��ʾУ���õġ�ʮ����

                        while ( ! LCD_ReadAdc_Smooth_XY ( & strScreenSample [i] ) );               //��ȡLCD���ݵ�����pCoordinate����ptrΪ��ʱ��ʾû�д��㱻����

                }
                
                
                LCD_Calculate_CalibrationFactor ( strCrossCoordinate, strScreenSample, & CalibrationFactor ) ;  	 //��ԭʼ��������� ԭʼ�����������ת��ϵ��
                
                if ( CalibrationFactor.Divider == 0 ) goto Failure;
                
                        
                usTest_x = ( ( CalibrationFactor.An * strScreenSample[3].x ) + ( CalibrationFactor.Bn * strScreenSample[3].y ) + CalibrationFactor.Cn ) / CalibrationFactor.Divider;		//ȡһ�������Xֵ	 
                usTest_y = ( ( CalibrationFactor.Dn * strScreenSample[3].x ) + ( CalibrationFactor.En * strScreenSample[3].y ) + CalibrationFactor.Fn ) / CalibrationFactor.Divider;    //ȡһ�������Yֵ
                
                usGap_x = ( usTest_x > strCrossCoordinate[3].x ) ? ( usTest_x - strCrossCoordinate[3].x ) : ( strCrossCoordinate[3].x - usTest_x );   //ʵ��X�������������ľ��Բ�
                usGap_y = ( usTest_y > strCrossCoordinate[3].y ) ? ( usTest_y - strCrossCoordinate[3].y ) : ( strCrossCoordinate[3].y - usTest_y );   //ʵ��Y�������������ľ��Բ�
                
        if ( ( usGap_x > 15 ) || ( usGap_y > 15 ) ) goto Failure;       //����ͨ���޸�������ֵ�Ĵ�С����������    
                

        /* У׼ϵ��Ϊȫ�ֱ��� */ 
                strLCD_TouchPara[LCD_Mode].dX_X = ( CalibrationFactor.An * 1.0 ) / CalibrationFactor.Divider;
                strLCD_TouchPara[LCD_Mode].dX_Y = ( CalibrationFactor.Bn * 1.0 ) / CalibrationFactor.Divider;
                strLCD_TouchPara[LCD_Mode].dX   = ( CalibrationFactor.Cn * 1.0 ) / CalibrationFactor.Divider;
                
                strLCD_TouchPara[LCD_Mode].dY_X = ( CalibrationFactor.Dn * 1.0 ) / CalibrationFactor.Divider;
                strLCD_TouchPara[LCD_Mode].dY_Y = ( CalibrationFactor.En * 1.0 ) / CalibrationFactor.Divider;
                strLCD_TouchPara[LCD_Mode].dY   = ( CalibrationFactor.Fn * 1.0 ) / CalibrationFactor.Divider;
                        
        LCD_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH );

        LCD_SetTextColor(GREEN);

        pStr = "Calibrate Succed";
        //����ո񣬾�����ʾ     
        sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
        LCD_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );       
        DelayUS ( 1000000 );

        return 1;    


        Failure:

        LCD_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH ); 

        LCD_SetTextColor(RED);

        pStr = "Calibrate fail";        
        //����ո񣬾�����ʾ     
        sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
        LCD_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );       

        pStr = "try again";
        //����ո񣬾�����ʾ     
        sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
        LCD_DispStringLine_EN ( ( LCD_Y_LENGTH >> 1 ) + (((sFONT *)LCD_GetFont())->Height), cStr );				

        DelayUS ( 1000000 );    

        return 0; 
                
                
}
/**
  * @brief  ��FLASH�л�ȡ �� ����У������������У�����д�뵽SPI FLASH�У�
  * @note       ��FLASH�д�δд�������������
  *       �ᴥ��У������У��LCD_Modeָ��ģʽ�Ĵ�����������ʱ����ģʽд��Ĭ��ֵ
  *
  *     ��FLASH�����д����������Ҳ�ǿ������У��
	*						��ֱ��ʹ��FLASH��Ĵ�������ֵ
  *
	*					ÿ��У��ʱֻ�����ָ����LCD_Modeģʽ�Ĵ�������������ģʽ�Ĳ���
  * @note  ���������ú���Һ��ģʽ����ΪLCD_Mode
  *
	* @param  LCD_Mode:ҪУ������������Һ��ģʽ
	* @param  forceCal:�Ƿ�ǿ������У������������Ϊ����ֵ��
	*		@arg 1��ǿ������У��
	*		@arg 0��ֻ�е�FLASH�в����ڴ���������־ʱ������У��
  * @retval ��
  */

void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal)
{
	uint8_t para_flag=0;
	
	//��ʼ��FLASH
	SPI_FLASH_Init();
	
	//��ȡ����������־
	SPI_FLASH_BufferRead(&para_flag,FLASH_TOUCH_PARA_FLAG_ADDR,1);

	//�������ڱ�־��florceCal=1ʱ������У������
	if(para_flag != FLASH_TOUCH_PARA_FLAG_VALUE | forceCal ==1)
	{ 		
		//����־���ڣ�˵��ԭ��FLASH���д���������
		//�ȶ�������LCDģʽ�Ĳ���ֵ���Ա��Ժ�ǿ�Ƹ���ʱֻ����ָ��LCDģʽ�Ĳ���,����ģʽ�Ĳ���
		if(  para_flag == FLASH_TOUCH_PARA_FLAG_VALUE && forceCal == 1)
		{
			SPI_FLASH_BufferRead((uint8_t *)&strLCD_TouchPara,FLASH_TOUCH_PARA_ADDR,4*6*8);	
		}
		
		//�ȴ�������У�����,����ָ��LCDģʽ�Ĵ�������ֵ
		while( ! LCD_Touch_Calibrate (LCD_Mode) );     

		//��������
		SPI_FLASH_SectorErase(0);
		
		//���ô���������־
		para_flag = FLASH_TOUCH_PARA_FLAG_VALUE;
		//д�봥��������־
		SPI_FLASH_BufferWrite(&para_flag,FLASH_TOUCH_PARA_FLAG_ADDR,1);
		//д�����µĴ�������
		SPI_FLASH_BufferWrite((uint8_t *)&strLCD_TouchPara,FLASH_TOUCH_PARA_ADDR,4*6*8);
 
	}
	else	//����־�����Ҳ�ǿ��У������ֱ�Ӵ�FLASH�ж�ȡ
	{
		SPI_FLASH_BufferRead((uint8_t *)&strLCD_TouchPara,FLASH_TOUCH_PARA_ADDR,4*6*8);	
	}
}
/**
  * @brief  ��ȡ LCD �����㣨У׼�󣩵�����
  * @param  pDisplayCoordinate ����ָ���Ż�ȡ���Ĵ���������
  * @param  pTouchPara������У׼ϵ��
  * @retval ��ȡ���
  *   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :��ȡ�ɹ�
  *     @arg 0 :��ȡʧ��
  */
uint8_t LCD_Get_TouchedPoint ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           //���������򷵻�0

	strType_LCD_Coordinate strScreenCoordinate; 
	
	if ( LCD_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
	{    
		pDisplayCoordinate ->x = ( ( pTouchPara[LCD_SCAN_MODE].dX_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dX_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara[LCD_SCAN_MODE].dY_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dY_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dY );

	}

	else ucRet = 0;            //�����ȡ�Ĵ�����Ϣ�����򷵻�0
	
	return ucRet;
} 

/**
  * @brief   �����������µ�ʱ�����ñ�����
  * @param  touch������������Ľṹ��
  * @note  ���ڱ������б�д�Լ��Ĵ������´���Ӧ��
  * @retval ��
  */
void LCD_TouchDown(strType_LCD_Coordinate * touch)
{
        //��Ϊ��ֵ��ʾ֮ǰ�Ѵ����
        //      if(touch->pre_x == -1 && touch->pre_x == -1)
        //      return;

        /***�ڴ˴���д�Լ��Ĵ������´���Ӧ��***/

        LCD_SetColors(RED,WHITE);
        LCD_Clear(30,101,58,48);
        LCD_DispString_CH(43,117,"���");

        LCD_SetColors(GREEN,WHITE);
        LCD_Clear(91,101,58,48);
        LCD_DispString_CH(108,117,"�̵�");

        LCD_SetColors(BLUE,WHITE);
        LCD_Clear(152,101,58,48);
        LCD_DispString_CH(169,117,"����");
        LCD_SetColors(WHITE,MAGENTA);
        LCD_Clear(151,151,60,50);
        LCD_DispString_CH(159,168,"��ˮ��");
        Water_Lape();

        LCD_SetColors(MAGENTA,WHITE);
        LCD_Clear(152,152,58,48);
        LCD_DispString_CH(159,168,"��ˮ��");

        /***�������д�Լ��Ĵ������´���Ӧ��***/ 
}

/**
  * @brief   �������ͷŵ�ʱ�����ñ�����
  * @param  touch������������Ľṹ��
  * @note  ���ڱ������б�д�Լ��Ĵ����ͷŴ���Ӧ��
  * @retval ��
  */
void LCD_TouchUp(strType_LCD_Coordinate * touch) 
{
	//��Ϊ��ֵ��ʾ֮ǰ�Ѵ����
//	if(touch->pre_x == -1 && touch->pre_x == -1)
//		return;
		
	/***�ڴ˴���д�Լ��Ĵ����ͷŴ���Ӧ��***/
	
	/***�������д�Լ��Ĵ����ͷŴ���Ӧ��***/
}

/**
  * @brief   ��⵽�����ж�ʱ���õĴ�����,ͨ��������tp_down ��tp_up�㱨������
  * @note       ��������Ҫ��whileѭ���ﱻ���ã�Ҳ��ʹ�ö�ʱ����ʱ����
  *     ���磬����ÿ��5ms����һ�Σ�������ֵ��DURIATION_TIME������Ϊ2������ÿ�������Լ��100���㡣
  *     ����XPT2046_TouchDown��XPT2046_TouchUp�����б�д�Լ��Ĵ���Ӧ��
  * @param   none
  * @retval  none
  */
void LCD_TouchEvenHandler(void )
{
          static strType_LCD_Coordinate cinfo={-1,-1,-1,-1};

          static uint8_t flag_R=0,flag_G=0,flag_B=0,flag_E=0;
          
                if(touch_detect() == TOUCH_PRESSED)
                {
                        //��ȡ��������
                        LCD_Get_TouchedPoint(&cinfo,strLCD_TouchPara);
                        
                        //���������Ϣ������
                        printf("x=%d,y=%d",cinfo.x,cinfo.y);
                        
                        
                        if(cinfo.x>29 && cinfo.x<89 && cinfo.y>100 && cinfo.y<150)
                        {
                                
                                flag_R=~flag_R;
                                
                                if (flag_R)
                                {
                                        LCD_SetColors(WHITE,RED);
                                        LCD_Clear(29,100,60,50);
                                        LCD_DispString_CH(43,117,"���");
                                }
                                else
                                {
                                        LCD_SetColors(RED,WHITE);
                                        LCD_Clear(30,101,58,48);
                                        LCD_DispString_CH(43,117,"���");
                                }
                                LED_R_TOGGLE;
                                
                                
                        }
                        
                        else if (cinfo.x>90 && cinfo.x<140 && cinfo.y>100 && cinfo.y<150)
                        {
                                flag_G=~flag_G;
                                
                                if (flag_G)
                                {
                                        LCD_SetColors(WHITE,GREEN);
                                        LCD_Clear(90,100,60,50);
                                        LCD_DispString_CH(108,117,"�̵�");
                                }
                                else
                                {
                                        LCD_SetColors(GREEN,WHITE);
                                        LCD_Clear(91,101,58,48);
                                        LCD_DispString_CH(108,117,"�̵�");
                                }
                                LED_G_TOGGLE;
                                
                                        
                        }
                        else if (cinfo.x>151 && cinfo.x<211 && cinfo.y>100 && cinfo.y<150)
                        {
                                flag_B=~flag_B;
                                
                                if (flag_B)
                                {
                                        LCD_SetColors(WHITE,BLUE);
                                        LCD_Clear(151,100,60,50);
                                        LCD_DispString_CH(169,117,"����");
                                }
                                else
                                {
                                        LCD_SetColors(BLUE,WHITE);
                                        LCD_Clear(152,101,58,48);
                                        LCD_DispString_CH(169,117,"����");
                                }
                                LED_B_TOGGLE;
                                        
                        } 
                        
                        else if(cinfo.x>29 && cinfo.x<89 && cinfo.y>151 && cinfo.y<201)
                        {
                                flag_E=~flag_E;
                                
                                if (flag_E)
                                {
                                        LCD_SetColors(WHITE,ORANGE);
                                        LCD_Clear(29,150,60,50);
                                        LCD_DispString_CH(35,167,"������");
                                        BEEP(ON);
                                }
                                else
                                {
                                        LCD_SetColors(ORANGE,WHITE);
                                        LCD_Clear(30,151,58,48);
                                        LCD_DispString_CH(35,167,"������");
                                        BEEP(OFF);
                                }
                                
                        }
                        else if(cinfo.x>90 && cinfo.x<140 && cinfo.y>151 && cinfo.y<201)
                        {
                                flag_R=0;
                                flag_G=0;
                                flag_B=0;
                                flag_E=0;
                                Screen_Init();
                                LCD_SetColors(RED,WHITE);
                                LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"��������",0);
                                LED_OFF;
                                BEEP(OFF);
                        }
                        else if(cinfo.x>151 && cinfo.x<211 && cinfo.y>151 && cinfo.y<201)
                        {
                                flag_R=0;
                                flag_G=0;
                                flag_B=0;
                                LCD_TouchDown(&cinfo);
                        }
                        
                        
                        
                        
                //      //���ô���������ʱ�Ĵ����������ڸú�����д�Լ��Ĵ������´������

                        
                        while(touch_detect() == TOUCH_PRESSED);
                        
                }
                else 
                {
        //      LED_BLUE;
                        
                        
//      //���ô������ͷ�ʱ�Ĵ����������ڸú�����д�Լ��Ĵ����ͷŴ������
//      LCD_TouchUp(&cinfo); 
//      
//      /*�����ͷţ��� xy ����Ϊ��*/
//      cinfo.x = -1;
//      cinfo.y = -1; 
//      cinfo.pre_x = -1;
//      cinfo.pre_y = -1;
                }

}

void Screen_Init(void)
{

        LCD_SetColors(RED,WHITE);
        LCD_Clear(0,0,240,320);
        LCD_SetTextColor(RED);
        LCD_DrawRectangle(29,100,60,50,0);
        LCD_DispString_CH(43,117,"���");

        LCD_SetTextColor(GREEN);
        LCD_DrawRectangle(90,100,60,50,0);
        LCD_DispString_CH(108,117,"�̵�");

        LCD_SetTextColor(BLUE);
        LCD_DrawRectangle(151,100,60,50,0);
        LCD_DispString_CH(169,117,"����");

        LCD_SetTextColor(ORANGE);
        LCD_DrawRectangle(29,151,60,50,0);
        LCD_DispString_CH(35,167,"������");

        LCD_SetTextColor(BLACK);
        LCD_DrawRectangle(90,151,60,50,0);
        LCD_DispString_CH(108,167,"��λ");

        LCD_SetTextColor(MAGENTA);
        LCD_DrawRectangle(151,151,60,50,0);
        LCD_DispString_CH(161,167,"��ˮ��");

}



