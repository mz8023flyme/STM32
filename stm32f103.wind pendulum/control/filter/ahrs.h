/*-----------------------------------------------------------------------------------
										STM32F401+MPU6050+AHRS??·¨êμ??
	
	?ò?é: í¨1y???aêy???ó????°??ó?ù?è??oííó?Yò?μ?êy?Y??DDèúo?????3??·à-??.
			 
	×÷??: BoX
	°?±?: V1.0
	??ì¨: STM32F401RE Nucleo Board
	è??ú: 2014?ê7??6è?
------------------------------------------------------------------------------------*/
#ifndef __AHRS__H__
#define __AHRS__H__

#include "stm32f10x.h"
#include "math.h"
/* include "arm_math.h" */

/*------------------------------------------
						oê?¨ò?êy?§????
------------------------------------------*/
#define invSqrtf( iSq ) (1.0f/sqrtf((float)iSq))
#define squa( Sq )      (((float)Sq)*((float)Sq))
#define toRad( Math_D )	((float)(Math_D)*0.0174532925f)
#define toDeg( Math_R )	((float)(Math_R)*57.2957795f)
	
/* 采样率500Hz,利用中断定时来保证,当动态范围大的时候可适当提高采样率 */
#define MPU6050_SAMPLERATE_FEQ  	 ((uint16_t)200) // 200Hz
#define MPU6050_SAMPLERATE_PRI  	 ((float)0.005f) // 5ms
#define MPU6050_SAMPLERATE_HALFPRI ((float)0.0025f)   //2.5ms
/*------------------------------------------
							欧拉角
------------------------------------------*/
typedef volatile struct 
{
  float Pitch;
  float Roll;
  float Yaw;
}AHRS_EulerAngleTypeDef;
/*------------------------------------------
							四元数
------------------------------------------*/
typedef volatile struct 
{
  float q0;
  float q1;
  float q2;
  float q3;
}AHRS_QuaternionTypeDef;
/*------------------------------------------
							函数定义
------------------------------------------*/
void Euler_ToQuaternion(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE);
void Quaternion_ToEuler(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE);
void Quaternion_Normalize(AHRS_QuaternionTypeDef *pQ);
void RungeKutta(AHRS_QuaternionTypeDef *pQ, float GyrX, float GyrY, float GyrZ, float helfTimes);
void AHRS_Init(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE);
void AHRS_Update(void);
AHRS_QuaternionTypeDef Quaternion_Multiply(AHRS_QuaternionTypeDef NowQ , AHRS_QuaternionTypeDef OldQ);
float Kalman_Filter1(float Accel,float Gyro);
float Kalman_Filter2(float Accel,float Gyro);
#endif

