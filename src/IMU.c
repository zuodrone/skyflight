
#include "IMU.h"
#include "mpu6050.h"
#include "math.h"

/*-------------------------------------------------------------------------------------------------------------------*/
char databuffer[15];       //接收数据缓存区
SENSOR_DATA Gyrobuf;
SENSOR_DATA Accbuf;

SENSOR_DATA Gyrooffset;
SENSOR_DATA	Accoffset;

IMU_DATA GyroFinal;
IMU_DATA AccFinal;

S_FLOAT_ANGLE Q_ANGLE;					//输出姿态角结构体

float   halfT=0.002;;                    //采样时间的一半，我们的应该是2ms

float Accel_x;	     //XÖáŒÓËÙ¶ÈÖµÔÝŽæ
float Accel_y;	     //YÖáŒÓËÙ¶ÈÖµÔÝŽæ
float Accel_z;	     //ZÖáŒÓËÙ¶ÈÖµÔÝŽæ

float Gyro_x;		 //XÖáÍÓÂÝÒÇÊýŸÝÔÝŽæ
float Gyro_y;        //YÖáÍÓÂÝÒÇÊýŸÝÔÝŽæ
float Gyro_z;		 //ZÖáÍÓÂÝÒÇÊýŸÝÔÝŽæ

/*-------------------------------------------------------------------------------------------------------------------*/					

void Get_MPU6050data()
{
	Accbuf.X = GetData(ACCEL_XOUT_H); //xÖáŒÓËÙ¶ÈÖµÔÝŽæ
	Accbuf.Y = GetData(ACCEL_YOUT_H); //yÖáŒÓËÙ¶ÈÖµÔÝŽæ
	Accbuf.Z = GetData(ACCEL_ZOUT_H); //zÖáŒÓËÙ¶ÈÖµÔÝŽæ
	Gyrobuf.X  = GetData(GYRO_XOUT_H);  //xÖáÍÓÂÝÒÇÖµÔÝŽæ
	Gyrobuf.Y  = GetData(GYRO_YOUT_H);  //yÖáÍÓÂÝÒÇÖµÔÝŽæ
	Gyrobuf.Z  = GetData(GYRO_ZOUT_H);  //zÖáÍÓÂÝÒÇÖµÔÝŽæ
}

/*-----------------------------------加速度计基准校正(error)----------------------------------------*/
void Acc_Correct(void)
{
	unsigned char i=0;
	unsigned char numAcc=200;

	int Angleaccx=0;
	int Angleaccy=0;
	int Angleaccz=0;							  //加速度计校正中间变量

	for(i=0;i<numAcc;i++)
	{		
		Get_MPU6050data();
		Angleaccx+=Accbuf.X;
		Angleaccy+=Accbuf.Y;
		Angleaccz+=Accbuf.Z;
		delay_ms(2);
	}	

	Accoffset.X= Angleaccx/numAcc;					   
	Accoffset.Y= Angleaccy/numAcc;
	Accoffset.Z= Angleaccy/numAcc;				   //得到加速度计基准
		
}


/*-----------------------------------陀螺仪基准校正----------------------------------------*/
void Gyro_Correct(void)
{
	unsigned char i=0;
	unsigned char numGyro=200;

	int Gyrox=0;
	int Gyroy=0;
	int Gyroz=0;							  //陀螺仪校正中间变量

	for(i=0;i<numGyro;i++)
	{
		Get_MPU6050data();
		Gyrox+=Gyrobuf.X;
		Gyroy+=Gyrobuf.Y;
		Gyroz+=Gyrobuf.Z;
		delay_ms(2);
	}	

	Gyrooffset.X= Gyrox/numGyro;					   
	Gyrooffset.Y= Gyroy/numGyro;
	Gyrooffset.Z= Gyroz/numGyro;
}
/*-----------------------------------MPU6050数据初处理---------------------------------------*/
void IMUdataprepare(void)
{  
	Get_MPU6050data();
	GyroFinal.X=(Gyrobuf.X-Gyrooffset.X)*0.061*0.0174;
	GyroFinal.Y=(Gyrobuf.Y-Gyrooffset.Y)*0.061*0.0174;
	GyroFinal.Z=(Gyrobuf.Z-Gyrooffset.Z)*0.061*0.0174;		//读出值减去基准值乘以单位，计算陀螺仪角速度
	// 1/16.4=0.061
	//此处的0.0174是3.14/180,将度每秒变为弧度每秒。
/*							
	AccFinal.X=(float)(Accbuf.X)/163.84;		
	AccFinal.Y=(float)(Accbuf.Y)/163.84;		
	AccFinal.Z=(float)(Accbuf.Z)/163.84;	*/	
	
	AccFinal.X=(float)((Accbuf.X-Accoffset.X)*0.244)*0.0098;		
	AccFinal.Y=(float)((Accbuf.Y-Accoffset.Y)*0.244)*0.0098;		
	AccFinal.Z=(float)((Accbuf.Z-Accoffset.Z)*0.244)*0.0098;	
		
/*	AccFinal.X=(float)((Accbuf.X)*0.244)*0.0098;		
	AccFinal.Y=(float)((Accbuf.Y)*0.244)*0.0098;		
	AccFinal.Z=(float)((Accbuf.Z)*0.244)*0.0098;			*/	  
	//+-8g,4096lsb/g--0.244mg/lsb
	//此处0.0098是：(9.8m/s^2)/1000,乘以mg得m/s^2
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
  static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;   
//  static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

  float delta_2=0;
//  float delta_4=0;

  const static float FACTOR = 0.002;

  float norm=0.0f;
  //float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  //float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  //float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;

  norm = sqrt(ax*ax + ay*ay +az*az);       // 测量正常化 把加速度计的三维向量转成单位向量。
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
    
  vx = 2*(q1q3 - q0q2);						// 估计方向的重力						
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
//这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
//根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
//所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。


  // Error is sum of cross product between estimated direction and measured direction of field vectors
  ex = (ay*vz - az*vy);                           					
  ey = (az*vx - ax*vz);
  ez = (ax*vy - ay*vx);
//axyz是机体坐标参照系上，加速度计测出来的重力向量，也就是实际测出来的重力向量。
//axyz是测量得到的重力向量，vxyz是陀螺积分后的姿态来推算出的重力向量，它们都是机体坐标参照系上的重力向量。
//那它们之间的误差向量，就是陀螺积分后的姿态和加计测出来的姿态之间的误差。
//向量间的误差，可以用向量叉积（也叫向量外积、叉乘）来表示，exyz就是两个重力向量的叉积。
//这个叉积向量仍旧是位于机体坐标系上的，而陀螺积分误差也是在机体坐标系，而且叉积的大小与陀螺积分误差成正比，正好拿来纠正陀螺。（你可以自己拿东西想象一下）由于陀螺是对机体直接积分，所以对陀螺的纠正量会直接体现在对机体坐标系的纠正。

/*  
  exInt = exInt + ex * Ki ;					//计算和应用积分反馈			 
  eyInt = eyInt + ey * Ki ;
  ezInt = ezInt + ez * Ki ;


  gx = gx + Kp*ex + exInt;					//校正陀螺仪测量值	   用叉积误差来做PI修正陀螺零偏							
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;		  */ 

//  halfT=GET_NOWTIME();			  				//两次计算的时间间隔，单位秒
  halfT=0.002;
  gx = gx + ex*FACTOR/halfT; 					//校正陀螺仪测量值	   用叉积误差来做PI修正陀螺零偏							
  gy = gy + ey*FACTOR/halfT; 
  gz = gz + ez*FACTOR/halfT;	 

/* 
  delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);	
  delta_4=delta_2*delta_2;

  q0 = (1-delta_2/8+delta_4/384)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT*(0.5-delta_2/48);			// 整合四元数率	 四元数微分方程	四元数更新算法，四阶毕卡法
  q1 = (1-delta_2/8+delta_4/384)*q1 + (q0*gx + q2*gz - q3*gy)*halfT*(0.5-delta_2/48);
  q2 = (1-delta_2/8+delta_4/384)*q2 + (q0*gy - q1*gz + q3*gx)*halfT*(0.5-delta_2/48);
  q3 = (1-delta_2/8+delta_4/384)*q3 + (q0*gz + q1*gy - q2*gx)*halfT*(0.5-delta_2/48);	*/		 

 
  delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);	
 
  q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;			// 整合四元数率	 四元数微分方程	四元数更新算法，二阶毕卡法
  q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;			 
/* 												   
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;							// 整合四元数率	 四元数微分方程	四元数更新算法，一阶龙库法
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;			   	   */
										   
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);		// 正常化四元
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  //转换为欧拉角
  Q_ANGLE.Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 					// pitch
  Q_ANGLE.Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 	// roll
  Q_ANGLE.Yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1)* 57.3; // yaw
}

void AGMIMUupdate(float gx, float gy, float gz, float ax, float ay, float az,float mx, float my, float mz)
{
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez,halfT;
  static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

  // 先把这些用得到的值算好
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;   
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
 
  float delta_2=0;

  const static float FACTOR = 0.002;

  norm = sqrt(ax*ax + ay*ay + az*az);       
  ax = ax / norm;
  ay = ay / norm;
  az = az / norm;

  norm = sqrt(mx*mx + my*my + mz*mz);          
  mx = mx / norm;
  my = my / norm;
  mz = mz / norm;

  // compute reference direction of flux
  hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
  hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
  hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz;     
  
  // estimated direction of gravity and flux (v and w)
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
//  wx = (float)(2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2));
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
//  wz = (float)(2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2));  
  
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);


 // halfT=GET_NOWTIME();			  //两次计算的时间间隔，单位秒

  gx = gx + ex*FACTOR/halfT; 					//校正陀螺仪测量值	   用叉积误差来做PI修正陀螺零偏							
  gy = gy + ey*FACTOR/halfT; 
  gz = gz + ez*FACTOR/halfT;	 
	  
  delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);	
 
  q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;			// 整合四元数率	 四元数微分方程	四元数更新算法，二阶毕卡法
  q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;			 
/* 												   
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;			// 整合四元数率	 四元数微分方程	四元数更新算法，一阶龙库法
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;			   	   */
										   
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);		// 正常化四元
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  //转换为欧拉角
  Q_ANGLE.Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 					// pitch
  Q_ANGLE.Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 	// roll
  Q_ANGLE.Yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1)* 57.3; // yaw
}



