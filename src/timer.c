#include "timer.h"

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

u8 TIM4CH1_CAPTURE_STA = 0;	//ÍšµÀ1ÊäÈë²¶»ñ±êÖŸ£¬žßÁœÎ»×ö²¶»ñ±êÖŸ£¬µÍ6Î»×öÒç³ö±êÖŸ		
u16 TIM4CH1_CAPTURE_UPVAL;
u16 TIM4CH1_CAPTURE_DOWNVAL;

u8 TIM4CH2_CAPTURE_STA = 0;	//ÍšµÀ2ÊäÈë²¶»ñ±êÖŸ£¬žßÁœÎ»×ö²¶»ñ±êÖŸ£¬µÍ6Î»×öÒç³ö±êÖŸ		
u16 TIM4CH2_CAPTURE_UPVAL;
u16 TIM4CH2_CAPTURE_DOWNVAL;

u8 TIM4CH3_CAPTURE_STA = 0;	//ÍšµÀ3ÊäÈë²¶»ñ±êÖŸ£¬žßÁœÎ»×ö²¶»ñ±êÖŸ£¬µÍ6Î»×öÒç³ö±êÖŸ		
u16 TIM4CH3_CAPTURE_UPVAL;
u16 TIM4CH3_CAPTURE_DOWNVAL;

u8 TIM4CH4_CAPTURE_STA = 0;	//ÍšµÀ1ÊäÈë²¶»ñ±êÖŸ£¬žßÁœÎ»×ö²¶»ñ±êÖŸ£¬µÍ6Î»×öÒç³ö±êÖŸ		
u16 TIM4CH4_CAPTURE_UPVAL;
u16 TIM4CH4_CAPTURE_DOWNVAL;

u32 tempup1 = 0;	//²¶»ñ×ÜžßµçÆœµÄÊ±Œä
u32 tempup2 = 0;	//²¶»ñ×ÜžßµçÆœµÄÊ±Œä
u32 tempup3 = 0;	//²¶»ñ×ÜžßµçÆœµÄÊ±Œä
u32 tempup4 = 0;	//²¶»ñ×ÜžßµçÆœµÄÊ±Œä
u32 tim4_T1;
u32 tim4_T2;
u32 tim4_T3;
u32 tim4_T4;

int pwmout1, pwmout2, pwmout3, pwmout4; 				//Êä³öÕŒ¿Õ±È

//#define Moto_PwmMax 2100

int16_t MOTO1_PWM = 0;
int16_t MOTO2_PWM = 0;
int16_t MOTO3_PWM = 0;
int16_t MOTO4_PWM = 0;

void Moto_PwmRflash(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
{		
	//if(MOTO1_PWM>Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	//if(MOTO2_PWM>Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	//if(MOTO3_PWM>Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	//if(MOTO4_PWM>Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
	//if(MOTO1_PWM<900)	MOTO1_PWM = 900;
	//if(MOTO2_PWM<900)	MOTO2_PWM = 900;
	//if(MOTO3_PWM<900)	MOTO3_PWM = 900;
	//if(MOTO4_PWM<900)	MOTO4_PWM = 900;
	
	TIM2->CCR1 = MOTO1_PWM;
	TIM2->CCR2 = MOTO2_PWM;
	TIM2->CCR3 = MOTO3_PWM;
	TIM2->CCR4 = MOTO4_PWM;
}

/*
 * 函数名：TIM5_PWM_OUTPUT
 * 描述  ：TIM5输出可控PWM信号
 * 输入  ：DR1
 * 输出  ：无
 * 调用	 ：外部调用		   
 */ 

void TIM2_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4)
{
	TIM_SetCompare1(TIM2,DR1);
	TIM_SetCompare2(TIM2,DR2);
	TIM_SetCompare3(TIM2,DR3);
	TIM_SetCompare4(TIM2,DR4);
}

void Tim2_init(void)//PWMÆµÂÊ50Hz
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//Ê¹ÄÜ¶šÊ±Æ÷2Ê±ÖÓ
	PrescalerValue = (uint16_t) 71;//ŒÆËã·ÖÆµÏµÊý
	TIM_TimeBaseStructure.TIM_Period = 20000;	//ÉèÖÃÔÚÏÂÒ»žöžüÐÂÊÂŒþ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØŒÄŽæÆ÷ÖÜÆÚµÄÖµ
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//ÉèÖÃÊ±ÖÓ·Öžî	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//ŒÆÊýÆ÷ÏòÉÏŒÇÊý
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//¶šÊ±Æ÷2³õÊŒ»¯
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM_CNT<TIMX_CCRXÊ±£¬Êä³öÎªÓÐÐ§µçÆœ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//±ÈœÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_Pulse = 0;//³õÊŒÕŒ¿Õ±ÈÎª0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//ÉèÖÃÓÐÐ§µçÆœÎªžßµçÆœ£¬œáºÏPWMÄ£Êœ£¬ÉÏÃæÓÃµÄÊÇPWM1
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//³õÊŒ»¯TIM_OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ê¹ÄÜÔ€×°ÔØ
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);//Ê¹ÄÜ¶šÊ±Æ÷2
}


void Moto_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //GPIOAÊ±ÖÓÊ¹ÄÜ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IOËÙÂÊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //žŽÓÃ¹ŠÄÜÊä³ö
	GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊŒ»¯
	Tim2_init();//¶šÊ±Æ÷2PWM³õÊŒ»¯	
}

//¶šÊ±Æ÷4ÍšµÀ1ÊäÈë²¶»ñÅäÖÃ

TIM_ICInitTypeDef TIM4_ICInitStructure;

void TIM4_Cap_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//Ê¹ÄÜTIM4Ê±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //Ê¹ÄÜGPIOBÊ±ÖÓ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8
			| GPIO_Pin_9;  //PB6,7,8,9 Çå³ýÖ®Ç°ÉèÖÃ  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6,7,8,9 ÊäÈë 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);//PB6,7,8,9  ÏÂÀ­

	//³õÊŒ»¯¶šÊ±Æ÷4 TIM4	 
	TIM_TimeBaseStructure.TIM_Period = arr; //Éè¶šŒÆÊýÆ÷×Ô¶¯ÖØ×°Öµ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ô€·ÖÆµÆ÷ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Öžî:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏŒÆÊýÄ£Êœ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //žùŸÝTIM_TimeBaseInitStructÖÐÖž¶šµÄ²ÎÊý³õÊŒ»¯TIMxµÄÊ±Œä»ùÊýµ¥Î»

	//³õÊŒ»¯TIM4ÊäÈë²¶»ñ²ÎÊý ÍšµÀ1
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ÅäÖÃÊäÈëÂË²šÆ÷ ²»ÂË²š
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	//³õÊŒ»¯TIM4ÊäÈë²¶»ñ²ÎÊý ÍšµÀ2
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ÅäÖÃÊäÈëÂË²šÆ÷ ²»ÂË²š
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	//³õÊŒ»¯TIM4ÊäÈë²¶»ñ²ÎÊý ÍšµÀ3
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ÅäÖÃÊäÈëÂË²šÆ÷ ²»ÂË²š
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	//³õÊŒ»¯TIM4ÊäÈë²¶»ñ²ÎÊý ÍšµÀ4
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Ó³ÉäµœTI1ÉÏ
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ÅäÖÃÊäÈëÂË²šÆ÷ ²»ÂË²š
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);

	//ÖÐ¶Ï·Ö×é³õÊŒ»¯
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕŒÓÅÏÈŒ¶1Œ¶
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //ŽÓÓÅÏÈŒ¶0Œ¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍšµÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);   //žùŸÝNVIC_InitStructÖÐÖž¶šµÄ²ÎÊý³õÊŒ»¯ÍâÉèNVICŒÄŽæÆ÷ 

	TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4,
			ENABLE);   //²»ÔÊÐížüÐÂÖÐ¶Ï£¬ÔÊÐíCC1IE,CC2IE,CC3IE,CC4IE²¶»ñÖÐ¶Ï	

	TIM_Cmd(TIM4, ENABLE); 		//Ê¹ÄÜ¶šÊ±Æ÷4

}

//¶šÊ±Æ÷4ÖÐ¶Ï·þÎñ³ÌÐò
void TIM4_IRQHandler(void)
{
	if ((TIM4CH1_CAPTURE_STA & 0X80) == 0) 		//»¹ÎŽ³É¹Š²¶»ñ	
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) 		//²¶»ñ1·¢Éú²¶»ñÊÂŒþ
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1); 		//Çå³ýÖÐ¶Ï±êÖŸÎ»
			if (TIM4CH1_CAPTURE_STA & 0X40)		//²¶»ñµœÒ»žöÏÂœµÑØ
			{
				TIM4CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM4);//ŒÇÂŒÏÂŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
				if (TIM4CH1_CAPTURE_DOWNVAL < TIM4CH1_CAPTURE_UPVAL)
				{
					tim4_T1 = 65535;
				}
				else
					tim4_T1 = 0;
				tempup1 = TIM4CH1_CAPTURE_DOWNVAL - TIM4CH1_CAPTURE_UPVAL
						+ tim4_T1;		//µÃµœ×ÜµÄžßµçÆœµÄÊ±Œä
				pwmout1 = tempup1;		//×ÜµÄžßµçÆœµÄÊ±Œä
				TIM4CH1_CAPTURE_STA = 0;		//²¶»ñ±êÖŸÎ»ÇåÁã
				TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Rising); //ÉèÖÃÎªÉÏÉýÑØ²¶»ñ		  
			}
			else //·¢Éú²¶»ñÊ±Œäµ«²»ÊÇÏÂœµÑØ£¬µÚÒ»ŽÎ²¶»ñµœÉÏÉýÑØ£¬ŒÇÂŒŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
			{
				TIM4CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM4);		//»ñÈ¡ÉÏÉýÑØÊýŸÝ
				TIM4CH1_CAPTURE_STA |= 0X40;		//±êŒÇÒÑ²¶»ñµœÉÏÉýÑØ
				TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Falling);//ÉèÖÃÎªÏÂœµÑØ²¶»ñ
			}
		}
	}

	if ((TIM4CH2_CAPTURE_STA & 0X80) == 0)		//»¹ÎŽ³É¹Š²¶»ñ	
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)		//²¶»ñ2·¢Éú²¶»ñÊÂŒþ
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);		//Çå³ýÖÐ¶Ï±êÖŸÎ»
			if (TIM4CH2_CAPTURE_STA & 0X40)		//²¶»ñµœÒ»žöÏÂœµÑØ
			{
				TIM4CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM4);//ŒÇÂŒÏÂŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
				if (TIM4CH2_CAPTURE_DOWNVAL < TIM4CH2_CAPTURE_UPVAL)
				{
					tim4_T2 = 65535;
				}
				else
					tim4_T2 = 0;
				tempup2 = TIM4CH2_CAPTURE_DOWNVAL - TIM4CH2_CAPTURE_UPVAL
						+ tim4_T2;		//µÃµœ×ÜµÄžßµçÆœµÄÊ±Œä
				pwmout2 = tempup2;		//×ÜµÄžßµçÆœµÄÊ±Œä
				TIM4CH2_CAPTURE_STA = 0;		//²¶»ñ±êÖŸÎ»ÇåÁã
				TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Rising); //ÉèÖÃÎªÉÏÉýÑØ²¶»ñ		  
			}
			else //·¢Éú²¶»ñÊ±Œäµ«²»ÊÇÏÂœµÑØ£¬µÚÒ»ŽÎ²¶»ñµœÉÏÉýÑØ£¬ŒÇÂŒŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
			{
				TIM4CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM4);		//»ñÈ¡ÉÏÉýÑØÊýŸÝ
				TIM4CH2_CAPTURE_STA |= 0X40;		//±êŒÇÒÑ²¶»ñµœÉÏÉýÑØ
				TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Falling);//ÉèÖÃÎªÏÂœµÑØ²¶»ñ
			}
		}
	}

	if ((TIM4CH3_CAPTURE_STA & 0X80) == 0)		//»¹ÎŽ³É¹Š²¶»ñ	
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)		//²¶»ñ3·¢Éú²¶»ñÊÂŒþ
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);		//Çå³ýÖÐ¶Ï±êÖŸÎ»
			if (TIM4CH3_CAPTURE_STA & 0X40)		//²¶»ñµœÒ»žöÏÂœµÑØ
			{
				TIM4CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM4);//ŒÇÂŒÏÂŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
				if (TIM4CH3_CAPTURE_DOWNVAL < TIM4CH3_CAPTURE_UPVAL)
				{
					tim4_T3 = 65535;
				}
				else
					tim4_T3 = 0;
				tempup3 = TIM4CH3_CAPTURE_DOWNVAL - TIM4CH3_CAPTURE_UPVAL
						+ tim4_T3;		//µÃµœ×ÜµÄžßµçÆœµÄÊ±Œä
				pwmout3 = tempup3;		//×ÜµÄžßµçÆœµÄÊ±Œä
				TIM4CH3_CAPTURE_STA = 0;		//²¶»ñ±êÖŸÎ»ÇåÁã
				TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising); //ÉèÖÃÎªÉÏÉýÑØ²¶»ñ		  
			}
			else //·¢Éú²¶»ñÊ±Œäµ«²»ÊÇÏÂœµÑØ£¬µÚÒ»ŽÎ²¶»ñµœÉÏÉýÑØ£¬ŒÇÂŒŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
			{
				TIM4CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM4);		//»ñÈ¡ÉÏÉýÑØÊýŸÝ
				TIM4CH3_CAPTURE_STA |= 0X40;		//±êŒÇÒÑ²¶»ñµœÉÏÉýÑØ
				TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);//ÉèÖÃÎªÏÂœµÑØ²¶»ñ
			}
		}
	}

	if ((TIM4CH4_CAPTURE_STA & 0X80) == 0)		//»¹ÎŽ³É¹Š²¶»ñ	
	{
		if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET)		//²¶»ñ4·¢Éú²¶»ñÊÂŒþ
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);		//Çå³ýÖÐ¶Ï±êÖŸÎ»
			if (TIM4CH4_CAPTURE_STA & 0X40)		//²¶»ñµœÒ»žöÏÂœµÑØ
			{
				TIM4CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM4);//ŒÇÂŒÏÂŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
				if (TIM4CH4_CAPTURE_DOWNVAL < TIM4CH4_CAPTURE_UPVAL)
				{
					tim4_T4 = 65535;
				}
				else
					tim4_T4 = 0;
				tempup4 = TIM4CH4_CAPTURE_DOWNVAL - TIM4CH4_CAPTURE_UPVAL
						+ tim4_T4;		//µÃµœ×ÜµÄžßµçÆœµÄÊ±Œä
				pwmout4 = tempup4;		//×ÜµÄžßµçÆœµÄÊ±Œä
				TIM4CH4_CAPTURE_STA = 0;		//²¶»ñ±êÖŸÎ»ÇåÁã
				TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Rising); //ÉèÖÃÎªÉÏÉýÑØ²¶»ñ		  
			}
			else //·¢Éú²¶»ñÊ±Œäµ«²»ÊÇÏÂœµÑØ£¬µÚÒ»ŽÎ²¶»ñµœÉÏÉýÑØ£¬ŒÇÂŒŽËÊ±µÄ¶šÊ±Æ÷ŒÆÊýÖµ
			{
				TIM4CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM4);		//»ñÈ¡ÉÏÉýÑØÊýŸÝ
				TIM4CH4_CAPTURE_STA |= 0X40;		//±êŒÇÒÑ²¶»ñµœÉÏÉýÑØ
				TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Falling);//ÉèÖÃÎªÏÂœµÑØ²¶»ñ
			}
		}
	}
}
