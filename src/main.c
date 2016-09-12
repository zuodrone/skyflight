/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/
#include <stdio.h>

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "mpu6050.h"
#include "kalman.h"
#include "IMU.h"
#include "control.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include <CoOS.h>

unsigned char TX_DATA[4];  	 //显示据缓存区

#if 0
# 做四轴飞行器前期的数据采集工作结束了。
# PB6~PB9（TIMER4）接遥控器接收机的四个通道。
# PA0~PA3（TIMER2）接四个无刷电机电调。
# PB10~PB11（IIC2）接MPU6050。
#endif

#ifdef USE_CCRTOS
OS_STK taskA_stk[128];  //建立一个任务栈A
OS_FlagID FlagID1;  //定义事件标记用于串口输出TASK1
OS_FlagID FlagID2;  //定义事件标记用于串口输出TASK2
OS_MutexID mutex_dev;  //建立一个互斥体，例如：串口同一时间只能一个使用所以要使用互斥

void TaskA(void* pData)
{
 for(;;)
 {
  CoWaitForSingleFlag(FlagID1,0);  //等待状态标志
  CoEnterMutexSection(mutex_dev);  //进入指定的互斥区域 
  //GPIO_SetBits(GPIOB,GPIO_Pin_5);
  //GPIO_ResetBits(GPIOE,GPIO_Pin_5);
  //printf("TASK1 RUNNING...\n");
  CoLeaveMutexSection(mutex_dev);  //离开指定的互斥区域 
 }
}
#endif

int main(int argc, char *argv[])
{
	int i;
	SystemInit(); 				//初始化系统设置时钟为72Mhz
	delay_init();               //延时函数初始化	  
	NVIC_Configuration();       //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);            //串口初始化为9600
	TIM4_Cap_Init(0xffff,72-1); //PWM捕获初始化,以1Mhz的频率计数
	Moto_Init();                //初始化4路PWM输出

	for(i=0;i<10000;i++)								//用以初始化电调航程的时间
	{
	 	TIM2_PWM_OUTPUT(pwmout2,pwmout2,pwmout2,pwmout2);
		delay_ms(1);
	}

	delay_ms(100);
	I2C_MPU6050_Init();         //IIC初始化
	delay_ms(100);
	InitMPU6050();              //MPU6050初始化

	Acc_Correct();
	Gyro_Correct();

	PID_controllerInit();
	controlmiddleinit(pwmout1,pwmout2,pwmout3,pwmout4);	 //	  pwmout1:横滚	  pwmout2:油门	 //	  pwmout3:俯仰	  pwmout4:航向
	delay_ms(500);
	delay_ms(500);

#ifdef USE_CCRTOS	
	CoInitOS ();  //操作系统初始化
	
	CoCreateTask (TaskA,0,0,&taskA_stk[128-1],128); //创建任务A
	
	mutex_dev = CoCreateMutex(); //创建一个互斥体
	FlagID1 = CoCreateFlag(1,0);  //自动重置事件标记，不可通知状态
	FlagID2 = CoCreateFlag(1,0);  //自动重置事件标记，不可通知状态
	
	CoStartOS ();  //启动操作系统开始多线程
#endif	
	while(1)
	{
		//USART_SendData(USART1, '%d1111111111111111\n', 2);
		USART_SendData(USART1, "C1:%d\tC2:%d\tC3:%d\tC4:%d\t",pwmout1,pwmout2,pwmout3,pwmout4);//输出遥控器4路脉宽时间us
		
		IMUdataprepare();
		IMUupdate(GyroFinal.X,GyroFinal.Y,GyroFinal.Z,AccFinal.X,AccFinal.Y,AccFinal.Z);

		Getdesireddata(pwmout1,pwmout2,pwmout3,pwmout4);
		PID_CAL();
		TIM2_PWM_OUTPUT(MOTOR1,MOTOR2,MOTOR3,MOTOR4);		//MOTOR1    REAR_R  后右电机
																				//MOTOR2	FRONT_R 前右电机
																				//MOTOR3	REAR_L  后左电机  
																				//MOTOR4	FRONT_L 前左电机
#if 0
		//如果接收的数据正常
		if( (pwmout1>800) && (pwmout1<2200) && (pwmout2>800)  && (pwmout2<2200)&& (pwmout3>800) && (pwmout3<2200)  && (pwmout4>800) && (pwmout4<2200) )
		{
			Moto_PwmRflash(pwmout1,pwmout2,pwmout3,pwmout4);
		}
		else //接收机接收的数据不正常,或者遥控器电源没开
		{
			Moto_PwmRflash(900,900,900,900);
		}
#endif
		//Angle_Calcu();
		//USART_SendData(USART1, "\tX:%f\tY:%f\r\n",Angle_X_Final,Angle_Y_Final);//输出MPU6050得到的角度值

	}
	return 0;
}
