/******************************************************************************
* PROJECT CODE：四元数法测量三维姿态*
* CREATE DATE ：2015.8.12  *
* CREATED BY  ：BoX 	  *	  
* FUNCTION    ：MPU6050四元数算法 *
* MODIFY DATE ：2015.8.12*
* DOCUMENT    ： 	     *
* OTHERS      ：四元数算法*
*******************************************************************************/
#include "ahrs.h"
/*------------------------------------------
								AHRS参数
------------------------------------------*/
#define USE_SENSOR_MAG   (0) /* 如果用到的九轴（加磁场计） */
#define Kp 							  15.0f
#define Ki 							  0.02f
/*------------------------------------------
								全局变量
------------------------------------------*/
//MPU6050_AxisTypeDef    Axis;
/* 四元数结构体	*/
AHRS_QuaternionTypeDef  QuadNumber = {1, 0, 0, 0};
/* 欧拉角结构体 */
AHRS_EulerAngleTypeDef  EulerAngle = {0};
/*------------------------------------------
 函数功能: 欧拉角转四元数
 函数参数: - Pointer of struct AHRS_QuaternionTypeDef
		   - Pointer of struct AHRS_EulerAngleTypeDef
------------------------------------------*/
void Euler_ToQuaternion(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE)
{
  float halfP = pE->Pitch / 2.0f;
  float halfR = pE->Roll  / 2.0f;
  float halfY = pE->Yaw   / 2.0f;

#ifndef USE_ARM_MATH_LIB
  float sinP = sinf(halfP);
  float cosP = cosf(halfP);
  float sinR = sinf(halfR);
  float cosR = cosf(halfR);
  float sinY = sinf(halfY);
  float cosY = cosf(halfY);
#else
  float sinP = arm_sin_f32(halfP);
  float cosP = arm_cos_f32(halfP);
  float sinR = arm_sin_f32(halfR);
  float cosR = arm_cos_f32(halfR);
  float sinY = arm_sin_f32(halfY);
  float cosY = arm_cos_f32(halfY);
#endif
  pQ->q0 = cosY*cosR*cosP + sinY*sinR*sinP;
  pQ->q1 = cosY*cosR*sinP - sinY*sinR*cosP;
  pQ->q2 = cosY*sinR*cosP + sinY*cosR*sinP;
  pQ->q3 = sinY*cosR*cosP - cosY*sinR*sinP;
}
/*------------------------------------------
 函数功能: 四元数转欧拉角
 函数参数: - Pointer of struct AHRS_QuaternionTypeDef
		   - Pointer of struct AHRS_EulerAngleTypeDef
------------------------------------------*/
void Quaternion_ToEuler(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE)
{
  float NumQ_T11 = pQ->q0*pQ->q0 + pQ->q1*pQ->q1 - pQ->q2*pQ->q2 - pQ->q3*pQ->q3;
  float NumQ_T12 = 2.0f * (pQ->q0*pQ->q3 + pQ->q1*pQ->q2);
  float NumQ_T13 = 2.0f * (pQ->q1*pQ->q3 - pQ->q0*pQ->q2);
  float NumQ_T23 = 2.0f * (pQ->q0*pQ->q1 + pQ->q2*pQ->q3);
  float NumQ_T33 = pQ->q0*pQ->q0 - pQ->q1*pQ->q1 - pQ->q2*pQ->q2 + pQ->q3*pQ->q3;

  pE->Pitch = -asinf(NumQ_T13);
  pE->Roll  = atan2f(NumQ_T23, NumQ_T33);
  pE->Yaw   = atan2f(NumQ_T12, NumQ_T11);
}
/*------------------------------------------
 函数功能: 四元数乘法（矩阵乘法）
 函数参数: - Pointer of struct AHRS_QuaternionTypeDef
					 - Pointer of struct AHRS_QuaternionTypeDef
------------------------------------------*/
AHRS_QuaternionTypeDef Quaternion_Multiply(AHRS_QuaternionTypeDef NowQ , AHRS_QuaternionTypeDef OldQ)
{
  AHRS_QuaternionTypeDef NewQ;

  NewQ.q0 = NowQ.q0*OldQ.q0 - NowQ.q1*OldQ.q1 - NowQ.q2*OldQ.q2 - NowQ.q3*OldQ.q3;
  NewQ.q1 = NowQ.q0*OldQ.q1 + NowQ.q1*OldQ.q0 + NowQ.q2*OldQ.q3 - NowQ.q3*OldQ.q2;
  NewQ.q2 = NowQ.q0*OldQ.q2 - NowQ.q1*OldQ.q3 + NowQ.q2*OldQ.q0 + NowQ.q3*OldQ.q1;
  NewQ.q3 = NowQ.q0*OldQ.q3 + NowQ.q1*OldQ.q2 - NowQ.q2*OldQ.q1 + NowQ.q3*OldQ.q0;

  Quaternion_Normalize(&NewQ);

  return NewQ;
}
/*------------------------------------------
 函数功能: 四元数归一化
 函数参数: - Pointer of struct AHRS_QuaternionTypeDef
------------------------------------------*/
void Quaternion_Normalize(AHRS_QuaternionTypeDef *pQ)
{
  float Normalize = 0.0f;

  Normalize = invSqrtf(squa(pQ->q0) + squa(pQ->q1) + squa(pQ->q2) + squa(pQ->q3));

  pQ->q0 = pQ->q0 * Normalize;
  pQ->q1 = pQ->q1 * Normalize;
  pQ->q2 = pQ->q2 * Normalize;
  pQ->q3 = pQ->q3 * Normalize;
}
/*------------------------------------------
 函数功能: 一阶龙格库塔法更新四元数
 函数参数: - Pointer of struct AHRS_QuaternionTypeDef
					 - GyrX (弧度)
					 - GyrY (弧度)
				   - GyrZ (弧度)
					 - helfTimes (采样周期的一半,单位秒)
------------------------------------------*/
void RungeKutta(AHRS_QuaternionTypeDef *pQ, float GyrX, float GyrY, float GyrZ, float helfTimes)
{
  pQ->q0 = pQ->q0 + (-pQ->q1*GyrX - pQ->q2*GyrY - pQ->q3*GyrZ) * helfTimes;
  pQ->q1 = pQ->q1 + ( pQ->q0*GyrX - pQ->q3*GyrY + pQ->q2*GyrZ) * helfTimes;
  pQ->q2 = pQ->q2 + ( pQ->q3*GyrX + pQ->q0*GyrY - pQ->q1*GyrZ) * helfTimes;
  pQ->q3 = pQ->q3 + (-pQ->q2*GyrX + pQ->q1*GyrY + pQ->q0*GyrZ) * helfTimes;
}
/*------------------------------------------
 函数功能: 初始化AHRS结构体
 函数参数: - Pointer of struct AHRS_QuaternionTypeDef
					 - Pointer of struct AHRS_EulerAngleTypeDef
------------------------------------------*/
void AHRS_Init(AHRS_QuaternionTypeDef *pQ, AHRS_EulerAngleTypeDef *pE)
{
  pQ->q0 		= 1.0f;
  pQ->q1 		= 0.0f;
  pQ->q2 		= 0.0f;
  pQ->q3    = 0.0f;
  pE->Pitch = 0.0f;
  pE->Roll  = 0.0f;
  pE->Yaw   = 0.0f;
}
/*------------------------------------------
 函数功能: 更新AHRS
 函数参数: 无
------------------------------------------*/
//void AHRS_Update(void)
//{
//#if USE_SENSOR_MAG == 1
//	float tempX = 0, tempY = 0;
//  float hx, hy, hz;
//  float wx, wy, wz;
//  float bx, bz;
//	float MegX, MegY, MegZ;
//	float Mq11, Mq12, Mq13, Mq21, Mq22, Mq23, Mq31, Mq32, Mq33;
//#endif
//	
//  float Normalize;
//  float gx, gy, gz;

//  float ErrX, ErrY, ErrZ;
//  float AccX, AccY, AccZ;
//  float GyrX, GyrY, GyrZ;

//  float Mq13,Mq23,Mq33;

//  static float AngZ_Temp = 0.0f;
//  static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

//#if USE_SENSOR_MAG == 1
//	Mq11 =  squa(QuadNumber.q0) + squa(QuadNumber.q1) - squa(QuadNumber.q2) - squa(QuadNumber.q3);
//	Mq12 = 2.0f*(QuadNumber.q1*QuadNumber.q2 + QuadNumber.q0*QuadNumber.q3);
//#endif
//  
//	Mq13 = 2.0f*(QuadNumber.q1 * QuadNumber.q3 - QuadNumber.q0 * QuadNumber.q2);

//#if USE_SENSOR_MAG == 1
//	Mq21 = 2.0f*(QuadNumber.q1*QuadNumber.q2 - QuadNumber.q0*QuadNumber.q3);
//	Mq22 = squa(QuadNumber.q0) - squa(QuadNumber.q1) + squa(QuadNumber.q2) - squa(QuadNumber.q3);
//#endif 

//  Mq23 = 2.0f*(QuadNumber.q0 * QuadNumber.q1 + QuadNumber.q2 * QuadNumber.q3);

//#if USE_SENSOR_MAG == 1
//	Mq31 = 2.0f*(QuadNumber.q0*QuadNumber.q2 + QuadNumber.q1*QuadNumber.q3);
//	Mq32 = 2.0f*(QuadNumber.q2*QuadNumber.q3 - QuadNumber.q0*QuadNumber.q1);
//#endif

//	Mq33 = squa(QuadNumber.q0) - squa(QuadNumber.q1) - squa(QuadNumber.q2) + squa(QuadNumber.q3);

//  Normalize = invSqrtf(squa(Axis.AccX) + squa(Axis.AccY) + squa(Axis.AccZ)); 
//	
//  AccX = Axis.AccX * Normalize;
//  AccY = Axis.AccY * Normalize;
//  AccZ = Axis.AccZ * Normalize;

//#if USE_SENSOR_MAG == 1
//	Normalize = invSqrtf(squa(Meg.TrueX) + squa(Meg.TrueY) + squa(Meg.TrueZ));
//	MegX = Meg.TrueX*Normalize;
//	MegY = Meg.TrueY*Normalize;
//	MegZ = Meg.TrueZ*Normalize;
//#endif

//  gx = Mq13;
//  gy = Mq23;
//  gz = Mq33;

//#if USE_SENSOR_MAG == 1
//	hx = MegX*Mq11 + MegY*Mq21 + MegZ*Mq31;
//	hy = MegX*Mq12 + MegY*Mq22 + MegZ*Mq32;
//	hz = MegX*Mq13 + MegY*Mq23 + MegZ*Mq33;

//	bx = sqrtf(squa(hx) + squa(hy));
//	bz = hz;

//	wx = bx*Mq11 + bz*Mq13;
//	wy = bx*Mq21 + bz*Mq23;
//	wz = bx*Mq31 + bz*Mq33;
//#endif

//#if USE_SENSOR_MAG == 1
//	ErrX = (AccY*gz - AccZ*gy) + (MegY*wz - MegZ*wy);
//  ErrY = (AccZ*gx - AccX*gz) + (MegZ*wx - MegX*wz);
//  ErrZ = (AccX*gy - AccY*gx) + (MegX*wy - MegY*wx);
//#endif

//  ErrX = (AccY*gz - AccZ*gy);
//  ErrY = (AccZ*gx - AccX*gz);
//  ErrZ = (AccX*gy - AccY*gx);

//  exInt = exInt + ErrX*Ki;
//  eyInt = eyInt + ErrY*Ki;
//  ezInt = ezInt + ErrZ*Ki;

//  GyrX = toRad(Axis.GyroX);
//  GyrY = toRad(Axis.GyroY);
//  GyrZ = toRad(Axis.GyroZ);

//  GyrX = GyrX + Kp*ErrX + exInt;
//  GyrY = GyrY + Kp*ErrY + eyInt;
//  GyrZ = GyrZ + Kp*ErrZ + ezInt;

//  RungeKutta(&QuadNumber, GyrX, GyrY, GyrZ, MPU6050_SAMPLERATE_HALFPRI);
//  Quaternion_Normalize(&QuadNumber);
//  Quaternion_ToEuler(&QuadNumber, &EulerAngle);

//#if USE_SENSOR_MAG == 1
//  tempX    = ( Mag.X*arm_cos_f32(Mag.EllipseSita)+Mag.Y*arm_sin_f32(Mag.EllipseSita))/Mag.EllipseB;
//  tempY    = (-Mag.X*arm_sin_f32(Mag.EllipseSita)+Mag.Y*arm_cos_f32(Mag.EllipseSita))/Mag.EllipseA;
//  EulerAngle.Yaw = atan2f(tempX, tempY);
//#endif

//  EulerAngle.Pitch = toDeg(EulerAngle.Pitch);
//  EulerAngle.Roll  = toDeg(EulerAngle.Roll);
//  EulerAngle.Yaw   = toDeg(EulerAngle.Yaw) + 180.0f;

//   /* 互补滤波 */
//  #define CF_A 0.9f
//  #define CF_B 0.1f
//  AngZ_Temp = AngZ_Temp + GyrZ * MPU6050_SAMPLERATE_PRI;
//  AngZ_Temp = CF_A * AngZ_Temp + CF_B * EulerAngle.Yaw;
//	
//  if(AngZ_Temp>360.0f)
//    EulerAngle.Yaw = AngZ_Temp - 360.0f;
//  else if(AngZ_Temp<0.0f)
//    EulerAngle.Yaw = AngZ_Temp + 360.0f;
//  else
//    EulerAngle.Yaw = AngZ_Temp;
//}
/*------------------------------------------
 函数功能: 卡尔曼滤波1
 函数参数: 无
------------------------------------------*/
float Kalman_Filter1(float Accel,float Gyro)		
{
	static float Gyro_y;   //Y轴陀螺仪数据暂存
	static float angle = 0.0;
	static float Q_bias = 0.0;
	static float angle_err = 0.0;
	static float Q_angle = 0.1;  //角度数据置信度
	static float Q_gyro = 0.1;   //角速度数据置信度
	static float R_angle = 0.5;
	static float dt = 0.005;	   //dt为滤波器采样时间(秒)
	static char  C_0 = 1;
	static float PCt_0=0, PCt_1=0, E=0;
	static float K_0=0, K_1=0, t_0=0, t_1=0;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };

	angle += (Gyro - Q_bias) * dt; //先验估计
	angle_err = Accel - angle;	//zk-先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;	

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP [0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle += K_0 * angle_err;	 //最优角度
	Q_bias += K_1 * angle_err;	 
	Gyro_y = Gyro - Q_bias;	 //最优角速度

	return (angle);				 //最终角度			 
}

/*------------------------------------------
 函数功能: 卡尔曼滤波2
 函数参数: 无
------------------------------------------*/
float Kalman_Filter2(float Accel,float Gyro)		
{
	static float Gyro_y;   //Y轴陀螺仪数据暂存
	static float angle = 0.0;
	static float Q_bias = 0.0;
	static float angle_err = 0.0;
	static float Q_angle = 0.1;  //角度数据置信度
	static float Q_gyro = 0.1;   //角速度数据置信度
	static float R_angle = 0.5;
	static float dt = 0.005;	   //dt为滤波器采样时间(秒)
	static char  C_0 = 1;
	static float PCt_0=0, PCt_1=0, E=0;
	static float K_0=0, K_1=0, t_0=0, t_1=0;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };

	angle += (Gyro - Q_bias) * dt; //先验估计
	angle_err = Accel - angle;	//zk-先验估计

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分
	Pdot[1]=- PP[1][1];
	Pdot[2]=- PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;	

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle += K_0 * angle_err;	 //最优角度
	Q_bias += K_1 * angle_err;	 
	Gyro_y = Gyro - Q_bias;	 //最优角速度

	return (angle);				 //最终角度			 
}


