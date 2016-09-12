/**
 *******************************************************************************
 * @file       OsConfig.h
 * @version   V1.1.4    
 * @date      2011.04.20
 * @brief      This file use by user to configuration CooCox CoOS.
 * @note       Ensure you have knew every item before modify this file. 
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */ 


#ifndef  _CONFIG_H
#define  _CONFIG_H


/*
		选择芯片类型 cortex-m3(1),cortex-m0(2) 如果是 cortex-m3 需要改为 1   
*/
#define CFG_CHIP_TYPE           (1)

/*
		定义分配的最低优先级
*/
#define CFG_LOWEST_PRIO         (64)

/*
	 这个指示用户最多能创建多少个任务,定义到恰到好处最节省空间
*/			
#define CFG_MAX_USER_TASKS      (3)

/*
		空闲任务堆栈大小
*/	
#define CFG_IDLE_STACK_SIZE     (25)

/* 
    系统时钟频率(Hz). 	                 	         
*/    
#define CFG_CPU_FREQ            (72000000)  


/* 
		这个是系统节拍周期，我们设置为100，表示10ms、100Hz的节拍时钟	 (Hz).	
*/
#define CFG_SYSTICK_FREQ        (100) 		

/*
		在ISR中调用系统API的最大数                         
*/
#define CFG_MAX_SERVICE_REQUEST (5)

/*
		调度算法选择
	  * 1 -- 二进制调度算法
	  * 0 -- 顺序列表调度
*/
#if (CFG_MAX_USER_TASKS) <15
#define CFG_ORDER_LIST_SCHEDULE_EN  (1)
#else 
#define CFG_ORDER_LIST_SCHEDULE_EN  (0)
#endif	


/*

	 * 时间片轮询开关
	 * 1 -- 开
	 * 0 -- 关

*/
#define CFG_ROBIN_EN            (1)         

/* 
    默认任务片	                          
*/
#if	CFG_ROBIN_EN > 0
#define CFG_TIME_SLICE          (10)	  		
#endif


/*----------------------- Schedule model Config -----------------------------*/
/*!< 
* 调度模式配置

 * 1 -- 使能     0 -- 关闭

 * 使能或关闭所有的等待函数，包括信号量、邮箱、队列、事件标志、互斥量、延时模块

 * Schedule model Config

 * Enable(1) or disable(0) all waiting function.

 * Include sem,mailbox,queue,flag,mutex,delay modules.          

      
*/
#define CFG_TASK_WAITTING_EN     (1)

/*!< 
 * 同步或静止调度模式

 * 如果是一个不变化的调度，当系统运行以后不能创建任务也不能更改任务的优先级。在同步

 * 调度模式中，以上功能都可以有。当任务结束时，如果系统是在静止调度模式，则系统不会

 * 删除任务源，所以你可以重新激活该任务。但是在同步模式下，任务源被彻底删除。如果是

 * 在静止任务调度模式下，互斥模块是不能用的，因为在这种模式下不支持更改任务的优先级  

 * Dynamic task scheduling(1) or Static Task Scheduling(0) model.

 * If in Static Task Scheduling model(0),cannot creat task and change task 

 * priority after coocox os start running. In Dynamic Task Scheduling model(1),

 *  all these can. When task terminated, if in Static Task Scheduling model(0), 

 * CoOS do not recovered task resources, and you can activate it again.

 * But in Dynamic Task Scheduling model(1),task resources will be recovered.

 * If in Static Task Scheduling model(0),mutex module can not be used, as this 

 * model don't support to change task priority while CoOS running. 

*/
#define CFG_TASK_SCHEDULE_EN     (1)


/*---------------------- Task Management Config -----------------------------*/
/*!< 
Enable(1) or disable(0) CoSetPriority() API.		      
*/
#if CFG_TASK_SCHEDULE_EN >0
#define CFG_PRIORITY_SET_EN       (1)	
#endif

/*!< 
Enable(1) or disable(0) CoAwakeTask() and CoSuspendTask() API.		      
*/
#define CFG_TASK_SUSPEND_EN       (1)


/*---------------------- Debug Management Config ----------------------------*/
/*!< 
Enable(1) or disable(0) parameter checkout .		  
*/
#define CFG_PAR_CHECKOUT_EN     (1)	

/*!< 
Enable(1) or disable(0) stack overflow checkout .  	  
*/		
#define CFG_STK_CHECKOUT_EN     (1)		



/*---------------------- Memory Management Config ----------------------------*/
/*!< 
Enable(1) or disable(0) memory management.	      
*/
#define CFG_MM_EN               (1)   

/*!< 
Max number of memory.(must be less than 32).      
*/ 
#if CFG_MM_EN >0
#define CFG_MAX_MM              (2)
#endif 



/*-------------------- Kernel heap Management Config -------------------------*/
/*!< 
Enable(1) or disable(0) kernel heap management.	      
*/
#define CFG_KHEAP_EN            (1)  		

/*!< 
Kernel heap size(word).      
*/ 
#if CFG_KHEAP_EN >0
#define KHEAP_SIZE              (50)
#endif   


		
/*---------------------- Time Management Config -----------------------------*/
/*!< 
Enable(1) or disable(0) TimeDelay() API.		      
*/
#if CFG_TASK_WAITTING_EN >0
#define CFG_TIME_DELAY_EN       (1)	
#endif


/*---------------------- Timer Management Config ----------------------------*/
/*!< 
Enable(1) or disable(0) timer management.		      
*/
#define CFG_TMR_EN              (1)		

/*!< 
Specify max number timer.(must be less than 32)      
*/	
#if CFG_TMR_EN >0
#define CFG_MAX_TMR             (2)			
#endif


/*---------------------- Event Management Config ----------------------------*/
/*!< 
Enable(1) or disable(0) events management,			 
events including semaphore,mailbox,queue.             
*/
#if CFG_TASK_WAITTING_EN > 0
#define  CFG_EVENT_EN           (1) 
#endif
	   	
#if CFG_EVENT_EN > 0
/*!< 
Event sort type.(1)FIFO (2)PRI (3)FIFO+PRI           
*/
#define CFG_EVENT_SORT          (3)		

/*!< 
Max number of event.(must be less than 255) 	      
Event = semaphore + mailbox + queue;			      
*/
#define CFG_MAX_EVENT           (20)

/*!< 
Enable(1) or disable(0) semaphore management.	      
*/
#define CFG_SEM_EN              (1)	

/*!< 
Enable(1) or disable(0) mailbox management.	      
*/		
#define CFG_MAILBOX_EN          (1)	

/*!< 
Enable(1) or disable(0) queue management.		      
*/		
#define CFG_QUEUE_EN            (1)	

/*!< 
Max number of queue.(less than CFG_MAX_EVENT).       
*/
#if	CFG_QUEUE_EN >0	
#define CFG_MAX_QUEUE           (2)
#endif   // CFG_QUEUE_EN
	
#endif   // CFG_EVENT_EN
	


/*----------------------- Flag Management Config ----------------------------*/
/*!< 
Enable(1) or disable(0) flag management.		     
Max number of flag is 32.					    	  
*/
#if CFG_TASK_WAITTING_EN > 0
#define  CFG_FLAG_EN           (1) 
#endif		


/*---------------------- Mutex Management Config ----------------------------*/
/*!< 
Enable(1) or disable(0) mutex management.	      
*/
#if CFG_TASK_WAITTING_EN > 0
#if CFG_TASK_SCHEDULE_EN > 0
#define  CFG_MUTEX_EN           (1) 
#endif
#endif

 
/*!< 
Max number of mutex.(must be less than 255).      
*/ 
#if CFG_MUTEX_EN >0
#define CFG_MAX_MUTEX           (10)			
#endif

/*---------------------- Utility Management Config --------------------------*/
/*!< 
Enable(1) or disable(0) utility management.    	  
*/
#define CFG_UTILITY_EN          (1)      
   
#if CFG_UTILITY_EN >0

/*!< 
Enable(1) or disable(0) TickToTime() utility	
*/
#define	CFG_TICK_TO_TIME_EN     (1)		
/*!< 
Enable(1) or disable(0) TimeToTick() utility	
*/	
#define	CFG_TIME_TO_TICK_EN     (1)		
#endif


#endif	 // _CONFIG_H
