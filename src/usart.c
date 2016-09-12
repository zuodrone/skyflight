#include "sys.h"
#include "usart.h"	

#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
  
////////////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀšÏÂÃæµÄÍ·ÎÄŒþŒŽ¿É.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//±Ÿ³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬ÎŽŸ­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍŸ
//ALIENTEK STM32¿ª·¢°å
//Ž®¿Ú1³õÊŒ»¯		   
//ÕýµãÔ­×Ó@ALIENTEK
//ŒŒÊõÂÛÌ³:www.openedv.com
//ÐÞžÄÈÕÆÚ:2012/8/18
//°æ±Ÿ£ºV1.5
//°æÈšËùÓÐ£¬µÁ°æ±ØŸ¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿ÆŒŒÓÐÏÞ¹«ËŸ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ÐÞžÄËµÃ÷ 
//Ö§³ÖÊÊÓŠ²»Í¬ÆµÂÊÏÂµÄŽ®¿Ú²šÌØÂÊÉèÖÃ.
//ŒÓÈëÁË¶ÔprintfµÄÖ§³Ö
//ÔöŒÓÁËŽ®¿ÚœÓÊÕÃüÁî¹ŠÄÜ.
//ÐÞÕýÁËprintfµÚÒ»žö×Ö·û¶ªÊ§µÄbug
//V1.4ÐÞžÄËµÃ÷
//1,ÐÞžÄŽ®¿Ú³õÊŒ»¯IOµÄbug
//2,ÐÞžÄÁËUSART_RX_STA,Ê¹µÃŽ®¿Ú×îŽóœÓÊÕ×ÖœÚÊýÎª2µÄ14ŽÎ·œ
//3,ÔöŒÓÁËUSART_REC_LEN,ÓÃÓÚ¶šÒåŽ®¿Ú×îŽóÔÊÐíœÓÊÕµÄ×ÖœÚÊý(²»ŽóÓÚ2µÄ14ŽÎ·œ)
//4,ÐÞžÄÁËEN_USART1_RXµÄÊ¹ÄÜ·œÊœ
//V1.5ÐÞžÄËµÃ÷
//1,ÔöŒÓÁË¶ÔUCOSIIµÄÖ§³Ö
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//ŒÓÈëÒÔÏÂŽúÂë,Ö§³Öprintfº¯Êý,¶ø²»ÐèÒªÑ¡Ôñuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//±ê×Œ¿âÐèÒªµÄÖ§³Öº¯Êý                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//¶šÒå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Êœ    
_sys_exit(int x) 
{ 
	x = x;
} 
//ÖØ¶šÒåfputcº¯Êý 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µœ·¢ËÍÍê±Ï   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*Ê¹ÓÃmicroLibµÄ·œ·š*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁËœÓÊÕ
//Ž®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄŽíÎó   	
u8 USART_RX_BUF[USART_REC_LEN];     //œÓÊÕ»º³å,×îŽóUSART_REC_LENžö×ÖœÚ.
//œÓÊÕ×ŽÌ¬
//bit15£¬	œÓÊÕÍê³É±êÖŸ
//bit14£¬	œÓÊÕµœ0x0d
//bit13~0£¬	œÓÊÕµœµÄÓÐÐ§×ÖœÚÊýÄ¿
u16 USART_RX_STA=0;       //œÓÊÕ×ŽÌ¬±êŒÇ	  
  
void uart_init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//žŽÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ž¡¿ÕÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ÅäÖÃ

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕŒÓÅÏÈŒ¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈŒ¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍšµÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//žùŸÝÖž¶šµÄ²ÎÊý³õÊŒ»¯VICŒÄŽæÆ÷
  
   //USART ³õÊŒ»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³€Îª8Î»ÊýŸÝžñÊœ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»žöÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅŒÐ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²ŒþÊýŸÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Êœ

    USART_Init(USART1, &USART_InitStructure); //³õÊŒ»¯Ž®¿Ú
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART1, ENABLE);                    //Ê¹ÄÜŽ®¿Ú 

}

void USART3_SendData(unsigned char ch)
{
	USART_SendData(USART1, (u8) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
}

char TX_DATA[10] = { 0 };
 //********串口发送数据***************************************
void Send_data(unsigned char axis)
{
  USART3_SendData(axis);
  USART3_SendData(':');
  Send_String(TX_DATA);
  USART3_SendData(' ');
  USART3_SendData(' ');
}
 
void Send_String(char *data)
 {
	 while(*data)
	 {
		 USART3_SendData(*data);
		 data++;
	 }
 }

void SendData(signed int A, signed int B, signed int C, signed int D)
{
  uint8_t check_byte;
  USART3_SendData(0x55);
  USART3_SendData(0x34);
  
  USART3_SendData(0x08);                                 //数据长度 8 byte
  
	//if(A<0)A=32768-A;
  USART3_SendData(A>>8);
  USART3_SendData(A);
	
	//if(B<0)B=32768-B;
  USART3_SendData(B>>8);
  USART3_SendData(B);
	
	//if(C<0)C=32768-C;
  USART3_SendData(C>>8);
  USART3_SendData(C);
	
	//if(D<0)D=32768-D;
  USART3_SendData(D>>8);
  USART3_SendData(D);
  
  check_byte  = (A>>8)^A^(B>>8)^B^(C>>8)^C^(D>>8)^D;                            //简单校验 异或所有数据
  
  USART3_SendData(check_byte);
}
 
void USART1_IRQHandler(void)                	//Ž®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//Èç¹ûÊ±ÖÓœÚÅÄÊý¶šÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //œÓÊÕÖÐ¶Ï(œÓÊÕµœµÄÊýŸÝ±ØÐëÊÇ0x0d 0x0aœáÎ²)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡œÓÊÕµœµÄÊýŸÝ
		
		if((USART_RX_STA&0x8000)==0)//œÓÊÕÎŽÍê³É
			{
			if(USART_RX_STA&0x4000)//œÓÊÕµœÁË0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//œÓÊÕŽíÎó,ÖØÐÂ¿ªÊŒ
				else USART_RX_STA|=0x8000;	//œÓÊÕÍê³ÉÁË 
				}
			else //»¹Ã»ÊÕµœ0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//œÓÊÕÊýŸÝŽíÎó,ÖØÐÂ¿ªÊŒœÓÊÕ	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//Èç¹ûÊ±ÖÓœÚÅÄÊý¶šÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
	OSIntExit();  											 
#endif
} 
#endif	

