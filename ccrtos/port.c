/**
 *******************************************************************************
 * @file       prot.c
 * @version   V1.1.4    
 * @date      2011.04.20
 * @brief      Compiler adapter for CooCox CoOS kernel.	
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2010 CooCox </center></h2>
 *******************************************************************************
 */ 

/*---------------------------- Include ---------------------------------------*/
#include <coocox.h>


/**
 ******************************************************************************
 * @brief      Plus a byte integers and Saved into memory cell
 * @param[in]  data    byte integers.	 
 * @param[out] None  
 * @retval     Returns Original value.		 
 *
 * @par Description
 * @details    This function is called to Plus a byte integers 
 *             and Saved into memory cell.
 ******************************************************************************
 */

U8 Inc8 (volatile U8 *data) {
	register U8  result = 0;

	__asm volatile
	(
		" PUSH    {R1}     \n"
		" CPSID   I        \n"
		" LDRB    R1,[%1]  \n"
		" ADD     R1,#1    \n"
		" STRB    R1,[%1]  \n"
		" CPSIE   I        \n"
		" SUB     R1,#0x1  \n"
		" MOVS    %0,R1    \n"
		" POP     {R1}     \n"
		:"=r"(result)
		:"r"(data)
	);
	return (result);

}


/**
 ******************************************************************************
 * @brief      Decrease a byte integers and Saved into memory cell
 * @param[in]  data    byte integers.	 
 * @param[out] None  
 * @retval     Returns Original value.		 
 *
 * @par Description
 * @details    This function is called to Decrease a byte integers 
 *             and Saved into memory cell.
 ******************************************************************************
 */
U8 Dec8 (volatile U8 *data) {
	register U8  result = 0;
	__asm__ volatile
	(
		" PUSH    {R1}     \n"
		" CPSID   I        \n"
		" LDRB    R1,[%1]  \n"
		" SUB     R1,#1    \n"
		" STRB    R1,[%1]  \n"
		" CPSIE   I        \n"
		" MOVS    %0,R1    \n"
		" POP     {R1}     \n"
		:"=r"(result)
		:"r"(data)
	);
	return (result);
}

/**
 ******************************************************************************
 * @brief      Set environment	for Coocox OS running
 * @param[in]  pstk    stack pointer	 
 * @param[out] None  
 * @retval     None.		 
 *
 * @par Description
 * @details    This function is called to Set environment
 *              for Coocox OS running.
 ******************************************************************************
 */
void SetEnvironment(OS_STK *pstk)
{
	__asm__ volatile
	(
		" SUB    R0,#28 \n"
		" MSR    PSP,R0 \n"
		" BX      LR               \n"
	);
}

/**
 ******************************************************************************
 * @brief      ENABLE Interrupt
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *
 * @par Description
 * @details    This function is called to ENABLE Interrupt.
 ******************************************************************************
 */
void IRQ_ENABLE_RESTORE(void) {
	__asm__ volatile
	(
		" CPSIE   I        \n"
	);
	return;
}

/**
 ******************************************************************************
 * @brief      Close Interrupt
 * @param[in]  None
 * @param[out] None
 * @retval     None
 *
 * @par Description
 * @details    This function is called to close Interrupt.
 ******************************************************************************
 */
void IRQ_DISABLE_SAVE(void) {
	__asm__ volatile
	(
		" CPSID   I        \n"
	);
	return;
}

/**
 ******************************************************************************
 * @brief      Do ready work to Switch Context for task change
 * @param[in]  None
 * @param[out] None
 * @retval     None.
 *
 * @par Description
 * @details    This function is called to Do ready work to
 *              Switch Context for task change
 ******************************************************************************
 */
#if 1
U32 NVIC_INT_CTRL  = 0xE000ED04;            // Interrupt control state register
U32 NVIC_PENDSVSET = 0x10000000;            // Value to trigger PendSV exception
void SwitchContext(void) {
	__asm__ volatile
	(
		" LDR     R3,=NVIC_INT_CTRL  \n"
		" LDR     R3,[R3]            \n"
		" LDR     R2,=NVIC_PENDSVSET \n"
		" LDR     R1,[R2]            \n"
		" STR     R1, [R3]           \n"
		" BX      LR               \n"
	);
}
#else
 void SwitchContext(void)
{
	__asm__ volatile
	(
    "LDR     R0, =0xE000ED04	\n"
    "LDR     R1, =0x10000000	\n"
    "STR     R1, [R0]			\n"
    "BX      LR					\n");
}
#endif

