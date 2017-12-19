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

/******************************* 定义 XPT2046 全局变量 ***************************/
//默认触摸参数，不同的屏幕稍有差异，可重新调用触摸校准函数获取
strType_LCD_TouchPara strLCD_TouchPara[] = { 	
-0.006464,   -0.073259,  280.358032,    0.074878,    0.002052,   -6.545977,//扫描方式0
0.086314,    0.001891,  -12.836658,   -0.003722,   -0.065799,  254.715714,//扫描方式1
0.002782,    0.061522,  -11.595689,    0.083393,    0.005159,  -15.650089,//扫描方式2
0.089743,   -0.000289,  -20.612209,   -0.001374,    0.064451,  -16.054003,//扫描方式3
0.000767,   -0.068258,  250.891769,   -0.085559,   -0.000195,  334.747650,//扫描方式4
-0.084744,    0.000047,  323.163147,   -0.002109,   -0.066371,  260.985809,//扫描方式5
-0.001848,    0.066984,  -12.807136,   -0.084858,   -0.000805,  333.395386,//扫描方式6
-0.085470,   -0.000876,  334.023163,   -0.003390,    0.064725,   -6.211169,//扫描方式7
}; 

/************************静态函数的声明************************/
static void LCD_ReadAdc_XY ( int16_t * sX_Ad, int16_t * sY_Ad )  ;
static uint16_t LCD_ReadAdc ( uint8_t ucChannel );
static uint8_t LCD_ReadAdc_Smooth_XY ( strType_LCD_Coordinate * pScreenCoordinate );
static uint8_t LCD_Calculate_CalibrationFactor ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_Coordinate * pScreenSample, strType_LCD_Calibration * pCalibrationFactor );
static void LCD_DrawCross ( uint16_t usX, uint16_t usY );



volatile uint8_t LCD_TouchFlag = 0;


void TOUCH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 打开SPI GPIO的时钟
	RCC_APB2PeriphClockCmd(Touch_SPI_GPIO_CLK, ENABLE);	

	// 将CS CLK MOSI 的GPIO配置为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = Touch_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Touch_SPI_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Touch_SPI_CLK_PIN;
	GPIO_Init(Touch_SPI_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Touch_SPI_MOSI_PIN;
	GPIO_Init(Touch_SPI_MOSI_PORT, &GPIO_InitStructure);		

  // 将PENIRQ MISO 的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = Touch_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Touch_SPI_MISO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Touch_PENIRQ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Touch_PENIRQ_GPIO_PORT, &GPIO_InitStructure);
		
}



/**
  * @brief  用于 XPT2046 的简单微秒级延时函数
  * @param  nCount ：延时计数值，单位为微妙
  * @retval 无
  */	
static void DelayUS ( __IO uint32_t ulCount )
{
	uint32_t i;


	for ( i = 0; i < ulCount; i ++ )
	{
		uint8_t uc = 12;     //设置值为12，大约延1微秒  
	      
		while ( uc -- );     //延1微秒	

	}
	
}


//发送命令
void Touch_SendCMD(uint8_t cmd)	
{
	uint8_t i;
	
	/*设置引脚的初始状态*/
	Touch_CS_DISABLE();
	Touch_CLK_LOW();
	Touch_MOSI_0();
	
	/*片选选中*/
	DelayUS(10);
	Touch_CS_ENABLE();
	
	/*产生8个时钟，发送数据*/
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
	
//	/*片选取消*/
//	XPT2046_CS_ENABLE();
	
}





//读取数据
uint16_t Touch_ReceiveData(void)	
{
	uint8_t i;
	
	uint16_t receive_temp=0;
	
//	/*设置引脚的初始状态*/
//	XPT2046_CS_ENABLE();
//	XPT2046_CLK_LOW();
//	XPT2046_MOSI_0();
//	
//	/*片选选中*/
//	XPT2046_DelayUS(10);
//	XPT2046_CS_DISABLE();
	
	Touch_CLK_HIGH();		
	DelayUS(5);
	Touch_CLK_LOW();
	DelayUS(5);	
	
	/*产生12个时钟，读取数据*/
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
	
	/*片选取消*/
	Touch_CS_DISABLE();
	
	return receive_temp;
	
}





//触摸检测状态机
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
  * @brief  对 XPT2046 选择一个模拟通道后，启动ADC，并返回ADC采样结果
  * @param  ucChannel
  *   该参数为以下值之一：
  *     @arg 0x90 :通道Y+的选择控制字
  *     @arg 0xd0 :通道X+的选择控制字
  * @retval 该通道的ADC采样结果
  */
static uint16_t LCD_ReadAdc ( uint8_t ucChannel )
{
	Touch_SendCMD ( ucChannel );

  return 	Touch_ReceiveData ();
	
}

/**
  * @brief  读取 LCD 的X通道和Y通道的AD值（12 bit，最大是4096）
  * @param  sX_Ad ：存放X通道AD值的地址
  * @param  sY_Ad ：存放Y通道AD值的地址
  * @retval 无
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
  * @brief  在触摸 LCD 屏幕时获取一组坐标的AD值，并对该坐标进行滤波
  * @param  无
  * @retval 滤波之后的坐标AD值
  */

static uint8_t LCD_ReadAdc_Smooth_XY ( strType_LCD_Coordinate * pScreenCoordinate )
{
	uint8_t ucCount = 0, i;
	
	int16_t sAD_X, sAD_Y;
	int16_t sBufferArray [ 2 ] [ 10 ] = { { 0 },{ 0 } };  //坐标X和Y进行多次采样
	
	//存储采样中的最小值、最大值
	int32_t lX_Min, lX_Max, lY_Min, lY_Max;


	/* 循环采样10次 */ 
	do					       				
	{		  
		LCD_ReadAdc_XY ( & sAD_X, & sAD_Y );  
		
		sBufferArray [ 0 ] [ ucCount ] = sAD_X;  
		sBufferArray [ 1 ] [ ucCount ] = sAD_Y;
		
		ucCount ++;  
		
	}	while ( ( Touch_PENIRQ_Read() == Touch_PENIRQ_ActiveLevel ) && ( ucCount < 10 ) );//用户点击触摸屏时即TP_INT_IN信号为低 并且 ucCount<10
	
	
	/*如果触笔弹起*/
	if ( Touch_PENIRQ_Read() != Touch_PENIRQ_ActiveLevel )
		LCD_TouchFlag = 0;			//中断标志复位

	
	/*如果成功采样10个样本*/
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
		
		
		/*去除最小值和最大值之后求平均值*/
		pScreenCoordinate ->x =  ( sBufferArray [ 0 ] [ 0 ] + sBufferArray [ 0 ] [ 1 ] + sBufferArray [ 0 ] [ 2 ] + sBufferArray [ 0 ] [ 3 ] + sBufferArray [ 0 ] [ 4 ] + 
		                           sBufferArray [ 0 ] [ 5 ] + sBufferArray [ 0 ] [ 6 ] + sBufferArray [ 0 ] [ 7 ] + sBufferArray [ 0 ] [ 8 ] + sBufferArray [ 0 ] [ 9 ] - lX_Min-lX_Max ) >> 3;
		
		pScreenCoordinate ->y =  ( sBufferArray [ 1 ] [ 0 ] + sBufferArray [ 1 ] [ 1 ] + sBufferArray [ 1 ] [ 2 ] + sBufferArray [ 1 ] [ 3 ] + sBufferArray [ 1 ] [ 4 ] + 
		                           sBufferArray [ 1 ] [ 5 ] + sBufferArray [ 1 ] [ 6 ] + sBufferArray [ 1 ] [ 7 ] + sBufferArray [ 1 ] [ 8 ] + sBufferArray [ 1 ] [ 9 ] - lY_Min-lY_Max ) >> 3; 
		
		
		return 1;		
		
	}   	
	return 0;    	
}

/**
  * @brief  计算 LCD 触摸坐标校正系数（注意：只有在LCD和触摸屏间的误差角度非常小时,才能运用下面公式）
  * @param  pDisplayCoordinate ：屏幕人为显示的已知坐标
  * @param  pstrScreenSample ：对已知坐标点触摸时 LCD 产生的坐标
  * @param  pCalibrationFactor ：根据人为设定坐标和采样回来的坐标计算出来的屏幕触摸校正系数
  * @retval 计算状态
  *   	    该返回值为以下值之一：
  *     @arg 1 :计算成功
  *     @arg 0 :计算失败
  */
static uint8_t LCD_Calculate_CalibrationFactor ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_Coordinate * pScreenSample, strType_LCD_Calibration * pCalibrationFactor )
{
	uint8_t ucRet = 1;

	
	/* K＝ ( X0－X2 )  ( Y1－Y2 )－ ( X1－X2 )  ( Y0－Y2 ) */
	pCalibrationFactor -> Divider =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
									                 ( ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
	
	
	if (  pCalibrationFactor -> Divider == 0  )
		ucRet = 0;

	else
	{
		/* A＝ (  ( XD0－XD2 )  ( Y1－Y2 )－ ( XD1－XD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor -> An =  ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) );
		
		/* B＝ (  ( X0－X2 )  ( XD1－XD2 )－ ( XD0－XD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor -> Bn =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .x - pDisplayCoordinate [ 2 ] .x ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .x - pDisplayCoordinate [ 2 ] .x ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) );
		
		/* C＝ ( Y0 ( X2XD1－X1XD2 )+Y1 ( X0XD2－X2XD0 )+Y2 ( X1XD0－X0XD1 ) )／K */
		pCalibrationFactor -> Cn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .x - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .x ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .x - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .x ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .x - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .x ) * pScreenSample [ 2 ] .y ;
		
		/* D＝ (  ( YD0－YD2 )  ( Y1－Y2 )－ ( YD1－YD2 )  ( Y0－Y2 ) )／K	*/
		pCalibrationFactor -> Dn =  ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .y - pScreenSample [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 0 ] .y - pScreenSample [ 2 ] .y ) ) ;
		
		/* E＝ (  ( X0－X2 )  ( YD1－YD2 )－ ( YD0－YD2 )  ( X1－X2 ) )／K	*/
		pCalibrationFactor -> En =  ( ( pScreenSample [ 0 ] .x - pScreenSample [ 2 ] .x ) *  ( pDisplayCoordinate [ 1 ] .y - pDisplayCoordinate [ 2 ] .y ) ) - 
								                ( ( pDisplayCoordinate [ 0 ] .y - pDisplayCoordinate [ 2 ] .y ) *  ( pScreenSample [ 1 ] .x - pScreenSample [ 2 ] .x ) ) ;
		
		
		/* F＝ ( Y0 ( X2YD1－X1YD2 )+Y1 ( X0YD2－X2YD0 )+Y2 ( X1YD0－X0YD1 ) )／K */
		pCalibrationFactor -> Fn =  ( pScreenSample [ 2 ] .x * pDisplayCoordinate [ 1 ] .y - pScreenSample [ 1 ] .x * pDisplayCoordinate [ 2 ] .y ) * pScreenSample [ 0 ] .y +
								                ( pScreenSample [ 0 ] .x * pDisplayCoordinate [ 2 ] .y - pScreenSample [ 2 ] .x * pDisplayCoordinate [ 0 ] .y ) * pScreenSample [ 1 ] .y +
								                ( pScreenSample [ 1 ] .x * pDisplayCoordinate [ 0 ] .y - pScreenSample [ 0 ] .x * pDisplayCoordinate [ 1 ] .y ) * pScreenSample [ 2 ] .y;
			
	}
	
	
	return ucRet;
	
	
}


/**
  * @brief  在 LCD 上显示校正触摸时需要的十字
  * @param  usX ：在特定扫描方向下十字交叉点的X坐标
  * @param  usY ：在特定扫描方向下十字交叉点的Y坐标
  * @retval 无
  */
static void LCD_DrawCross ( uint16_t usX, uint16_t usY )
{
	LCD_DrawLine(usX-10,usY,usX+10,usY);
	LCD_DrawLine(usX, usY - 10, usX, usY+10);	
}

/**
  * @brief  LCD 触摸屏校准
	* @param	LCD_Mode：指定要校正哪种液晶扫描模式的参数
  * @note  本函数调用后会把液晶模式设置为LCD_Mode
  * @retval 校准结果
	*   该返回值为以下值之一：
  *     @arg 1 :校准成功
  *     @arg 0 :校准失败
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
	
		//设置扫描方向，横屏
		LCD_GramScan ( LCD_Mode );
		
		
		/* 设定“十”字交叉点的坐标 */ 
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
			//插入空格，居中显示
			sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
			LCD_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );			
		
			//插入空格，居中显示
			sprintf ( cStr, "%*c%d",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width)) -1)/2,' ',i + 1 );
			LCD_DispStringLine_EN (( LCD_Y_LENGTH >> 1 ) - (((sFONT *)LCD_GetFont())->Height), cStr ); 
		
			DelayUS ( 300000 );		                     //适当的延时很有必要
			
			LCD_DrawCross ( strCrossCoordinate[i] .x, strCrossCoordinate[i].y );  //显示校正用的“十”字

			while ( ! LCD_ReadAdc_Smooth_XY ( & strScreenSample [i] ) );               //读取LCD数据到变量pCoordinate，当ptr为空时表示没有触点被按下

		}
		
		
		LCD_Calculate_CalibrationFactor ( strCrossCoordinate, strScreenSample, & CalibrationFactor ) ;  	 //用原始参数计算出 原始参数与坐标的转换系数
		
		if ( CalibrationFactor.Divider == 0 ) goto Failure;
		
			
		usTest_x = ( ( CalibrationFactor.An * strScreenSample[3].x ) + ( CalibrationFactor.Bn * strScreenSample[3].y ) + CalibrationFactor.Cn ) / CalibrationFactor.Divider;		//取一个点计算X值	 
		usTest_y = ( ( CalibrationFactor.Dn * strScreenSample[3].x ) + ( CalibrationFactor.En * strScreenSample[3].y ) + CalibrationFactor.Fn ) / CalibrationFactor.Divider;    //取一个点计算Y值
		
		usGap_x = ( usTest_x > strCrossCoordinate[3].x ) ? ( usTest_x - strCrossCoordinate[3].x ) : ( strCrossCoordinate[3].x - usTest_x );   //实际X坐标与计算坐标的绝对差
		usGap_y = ( usTest_y > strCrossCoordinate[3].y ) ? ( usTest_y - strCrossCoordinate[3].y ) : ( strCrossCoordinate[3].y - usTest_y );   //实际Y坐标与计算坐标的绝对差
		
    if ( ( usGap_x > 15 ) || ( usGap_y > 15 ) ) goto Failure;       //可以通过修改这两个值的大小来调整精度    
		

    /* 校准系数为全局变量 */ 
		strLCD_TouchPara[LCD_Mode].dX_X = ( CalibrationFactor.An * 1.0 ) / CalibrationFactor.Divider;
		strLCD_TouchPara[LCD_Mode].dX_Y = ( CalibrationFactor.Bn * 1.0 ) / CalibrationFactor.Divider;
		strLCD_TouchPara[LCD_Mode].dX   = ( CalibrationFactor.Cn * 1.0 ) / CalibrationFactor.Divider;
		
		strLCD_TouchPara[LCD_Mode].dY_X = ( CalibrationFactor.Dn * 1.0 ) / CalibrationFactor.Divider;
		strLCD_TouchPara[LCD_Mode].dY_Y = ( CalibrationFactor.En * 1.0 ) / CalibrationFactor.Divider;
		strLCD_TouchPara[LCD_Mode].dY   = ( CalibrationFactor.Fn * 1.0 ) / CalibrationFactor.Divider;
			
	LCD_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH );
	
	LCD_SetTextColor(GREEN);
	
	pStr = "Calibrate Succed";
	//插入空格，居中显示	
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
	LCD_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );	
	DelayUS ( 1000000 );

	return 1;    
	

Failure:
	
	LCD_Clear ( 0, 0, LCD_X_LENGTH, LCD_Y_LENGTH ); 
	
	LCD_SetTextColor(RED);
	
	pStr = "Calibrate fail";	
	//插入空格，居中显示	
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
	LCD_DispStringLine_EN (LCD_Y_LENGTH >> 1, cStr );	

	pStr = "try again";
	//插入空格，居中显示		
	sprintf(cStr,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr)	;	
	LCD_DispStringLine_EN ( ( LCD_Y_LENGTH >> 1 ) + (((sFONT *)LCD_GetFont())->Height), cStr );				

	DelayUS ( 1000000 );		
	
	return 0; 
		
		
}
/**
  * @brief  从FLASH中获取 或 重新校正触摸参数（校正后会写入到SPI FLASH中）
  * @note		若FLASH中从未写入过触摸参数，
	*						会触发校正程序校正LCD_Mode指定模式的触摸参数，此时其它模式写入默认值
  *
	*					若FLASH中已有触摸参数，且不强制重新校正
	*						会直接使用FLASH里的触摸参数值
  *
	*					每次校正时只会更新指定的LCD_Mode模式的触摸参数，其它模式的不变
  * @note  本函数调用后会把液晶模式设置为LCD_Mode
  *
	* @param  LCD_Mode:要校正触摸参数的液晶模式
	* @param  forceCal:是否强制重新校正参数，可以为以下值：
	*		@arg 1：强制重新校正
	*		@arg 0：只有当FLASH中不存在触摸参数标志时才重新校正
  * @retval 无
  */

void Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal)
{
	uint8_t para_flag=0;
	
	//初始化FLASH
	SPI_FLASH_Init();
	
	//读取触摸参数标志
	SPI_FLASH_BufferRead(&para_flag,FLASH_TOUCH_PARA_FLAG_ADDR,1);

	//若不存在标志或florceCal=1时，重新校正参数
	if(para_flag != FLASH_TOUCH_PARA_FLAG_VALUE | forceCal ==1)
	{ 		
		//若标志存在，说明原本FLASH内有触摸参数，
		//先读回所有LCD模式的参数值，以便稍后强制更新时只更新指定LCD模式的参数,其它模式的不变
		if(  para_flag == FLASH_TOUCH_PARA_FLAG_VALUE && forceCal == 1)
		{
			SPI_FLASH_BufferRead((uint8_t *)&strLCD_TouchPara,FLASH_TOUCH_PARA_ADDR,4*6*8);	
		}
		
		//等待触摸屏校正完毕,更新指定LCD模式的触摸参数值
		while( ! LCD_Touch_Calibrate (LCD_Mode) );     

		//擦除扇区
		SPI_FLASH_SectorErase(0);
		
		//设置触摸参数标志
		para_flag = FLASH_TOUCH_PARA_FLAG_VALUE;
		//写入触摸参数标志
		SPI_FLASH_BufferWrite(&para_flag,FLASH_TOUCH_PARA_FLAG_ADDR,1);
		//写入最新的触摸参数
		SPI_FLASH_BufferWrite((uint8_t *)&strLCD_TouchPara,FLASH_TOUCH_PARA_ADDR,4*6*8);
 
	}
	else	//若标志存在且不强制校正，则直接从FLASH中读取
	{
		SPI_FLASH_BufferRead((uint8_t *)&strLCD_TouchPara,FLASH_TOUCH_PARA_ADDR,4*6*8);	
	}
}
/**
  * @brief  获取 LCD 触摸点（校准后）的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @param  pTouchPara：坐标校准系数
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t LCD_Get_TouchedPoint ( strType_LCD_Coordinate * pDisplayCoordinate, strType_LCD_TouchPara * pTouchPara )
{
	uint8_t ucRet = 1;           //若正常，则返回0

	strType_LCD_Coordinate strScreenCoordinate; 
	
	if ( LCD_ReadAdc_Smooth_XY ( & strScreenCoordinate ) )
	{    
		pDisplayCoordinate ->x = ( ( pTouchPara[LCD_SCAN_MODE].dX_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dX_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dX );        
		pDisplayCoordinate ->y = ( ( pTouchPara[LCD_SCAN_MODE].dY_X * strScreenCoordinate.x ) + ( pTouchPara[LCD_SCAN_MODE].dY_Y * strScreenCoordinate.y ) + pTouchPara[LCD_SCAN_MODE].dY );

	}

	else ucRet = 0;            //如果获取的触点信息有误，则返回0
	
	return ucRet;
} 

/**
  * @brief   触摸屏被按下的时候会调用本函数
  * @param  touch包含触摸坐标的结构体
  * @note  请在本函数中编写自己的触摸按下处理应用
  * @retval 无
  */
void LCD_TouchDown(strType_LCD_Coordinate * touch)
{
        //若为负值表示之前已处理过
        //      if(touch->pre_x == -1 && touch->pre_x == -1)
        //      return;

        /***在此处编写自己的触摸按下处理应用***/

        LCD_SetColors(RED,WHITE);
        LCD_Clear(30,101,58,48);
        LCD_DispString_CH(43,117,"红灯");

        LCD_SetColors(GREEN,WHITE);
        LCD_Clear(91,101,58,48);
        LCD_DispString_CH(108,117,"绿灯");

        LCD_SetColors(BLUE,WHITE);
        LCD_Clear(152,101,58,48);
        LCD_DispString_CH(169,117,"蓝灯");
        LCD_SetColors(WHITE,MAGENTA);
        LCD_Clear(151,151,60,50);
        LCD_DispString_CH(159,168,"流水灯");
        Water_Lape();

        LCD_SetColors(MAGENTA,WHITE);
        LCD_Clear(152,152,58,48);
        LCD_DispString_CH(159,168,"流水灯");

        /***在上面编写自己的触摸按下处理应用***/	
}

/**
  * @brief   触摸屏释放的时候会调用本函数
  * @param  touch包含触摸坐标的结构体
  * @note  请在本函数中编写自己的触摸释放处理应用
  * @retval 无
  */
void LCD_TouchUp(strType_LCD_Coordinate * touch) 
{
	//若为负值表示之前已处理过
//	if(touch->pre_x == -1 && touch->pre_x == -1)
//		return;
		
	/***在此处编写自己的触摸释放处理应用***/
	
	/***在上面编写自己的触摸释放处理应用***/
}

/**
	* @brief   检测到触摸中断时调用的处理函数,通过它调用tp_down 和tp_up汇报触摸点
*	@note 	 本函数需要在while循环里被调用，也可使用定时器定时调用
*			例如，可以每隔5ms调用一次，消抖阈值宏DURIATION_TIME可设置为2，这样每秒最多可以检测100个点。
*						可在XPT2046_TouchDown及XPT2046_TouchUp函数中编写自己的触摸应用
	* @param   none
	* @retval  none
	*/
void LCD_TouchEvenHandler(void )
{
	  static strType_LCD_Coordinate cinfo={-1,-1,-1,-1};
	
	  static uint8_t flag_R=0,flag_G=0,flag_B=0,flag_E=0;
	  
		if(touch_detect() == TOUCH_PRESSED)
		{
			//获取触摸坐标
			LCD_Get_TouchedPoint(&cinfo,strLCD_TouchPara);
			
			//输出调试信息到串口
			printf("x=%d,y=%d",cinfo.x,cinfo.y);
			
			
			if(cinfo.x>29 && cinfo.x<89 && cinfo.y>100 && cinfo.y<150)
			{
				
				flag_R=~flag_R;
				
				if (flag_R)
				{
					LCD_SetColors(WHITE,RED);
					LCD_Clear(29,100,60,50);
					LCD_DispString_CH(43,117,"红灯");
				}
				else
				{
					LCD_SetColors(RED,WHITE);
					LCD_Clear(30,101,58,48);
					LCD_DispString_CH(43,117,"红灯");
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
					LCD_DispString_CH(108,117,"绿灯");
				}
				else
				{
					LCD_SetColors(GREEN,WHITE);
					LCD_Clear(91,101,58,48);
					LCD_DispString_CH(108,117,"绿灯");
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
					LCD_DispString_CH(169,117,"蓝灯");
				}
				else
				{
					LCD_SetColors(BLUE,WHITE);
					LCD_Clear(152,101,58,48);
					LCD_DispString_CH(169,117,"蓝灯");
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
					LCD_DispString_CH(35,167,"蜂鸣器");
					BEEP(ON);
				}
				else
				{
					LCD_SetColors(ORANGE,WHITE);
					LCD_Clear(30,151,58,48);
					LCD_DispString_CH(35,167,"蜂鸣器");
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
				LCD_DisplayStringEx(56,40,32,32,(uint8_t *)"秉火物联",0);
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
			
			
			
			
//			//调用触摸被按下时的处理函数，可在该函数编写自己的触摸按下处理过程

			
			while(touch_detect() == TOUCH_PRESSED);
			
		}
		else 
		{
//			LED_BLUE;
			
			
//			//调用触摸被释放时的处理函数，可在该函数编写自己的触摸释放处理过程
//			LCD_TouchUp(&cinfo); 
//			
//			/*触笔释放，把 xy 重置为负*/
//			cinfo.x = -1;
//			cinfo.y = -1; 
//			cinfo.pre_x = -1;
//			cinfo.pre_y = -1;
		}

}

void Screen_Init(void)
{
	
	LCD_SetColors(RED,WHITE);
	LCD_Clear(0,0,240,320);
	LCD_SetTextColor(RED);
	LCD_DrawRectangle(29,100,60,50,0);
	LCD_DispString_CH(43,117,"红灯");
	
	LCD_SetTextColor(GREEN);
	LCD_DrawRectangle(90,100,60,50,0);
	LCD_DispString_CH(108,117,"绿灯");
	
	LCD_SetTextColor(BLUE);
	LCD_DrawRectangle(151,100,60,50,0);
	LCD_DispString_CH(169,117,"蓝灯");
	
	LCD_SetTextColor(ORANGE);
	LCD_DrawRectangle(29,151,60,50,0);
	LCD_DispString_CH(35,167,"蜂鸣器");
	
	LCD_SetTextColor(BLACK);
	LCD_DrawRectangle(90,151,60,50,0);
	LCD_DispString_CH(108,167,"复位");
	
	LCD_SetTextColor(MAGENTA);
	LCD_DrawRectangle(151,151,60,50,0);
	LCD_DispString_CH(161,167,"流水灯");
	
}



