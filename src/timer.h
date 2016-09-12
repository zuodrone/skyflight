#ifndef _TIMER_HH
#define _TIMER_HH

#include "sys.h"

extern u32 tempup1;	//捕获总高电平的时间
extern u32 tempup2;	//捕获总高电平的时间
extern u32 tempup3;	//捕获总高电平的时间
extern u32 tempup4;	//捕获总高电平的时间

extern int pwmout1,pwmout2,pwmout3,pwmout4;

void Moto_Init(void);
void Moto_PwmRflash(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM);
void TIM2_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4);

void TIM4_Cap_Init(u16 arr, u16 psc);

#endif
