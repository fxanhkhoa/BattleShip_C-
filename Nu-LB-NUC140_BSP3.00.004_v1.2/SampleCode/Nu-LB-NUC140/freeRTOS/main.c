/*
    FreeRTOS V6.0.5 - Copyright (C) 2010 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks.
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" task -  This only executes every five seconds but has the highest
 * priority so is guaranteed to get processor time.  Its main function is to
 * check that all the standard demo tasks are still operational.  Should any
 * unexpected behaviour within a demo task be discovered the 'check' task will
 * write an error to the Hyper-terminal.  If all the demo tasks are executing 
 * with their expected behaviour then the check task writes PASSto the 
 * Hyper-terminal, as described above.
 *
 */

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* NUC1xx driver includes. */
#include "UART.h"
#include "GPIO.h"
#include "SYS.h"

/* Demo app includes. */
#include "NUC1xx_Test.h"
#include "App_Cfg.h"

#if BLOCK_Q_TEST
#include "BlockQ.h"
#endif

#if SUICID_TEST
#include "death.h"
#endif

#if MATH_TEST
#include "integer.h"
#endif

#if BLOCK_TM_TEST
#include "blocktim.h"
#endif

#if SEM_TEST
#include "semtest.h"
#endif

#if POLL_Q_TEST
#include "PollQ.h"
#endif

#if LED_TEST
#include "flash.h"
#include "partest.h"
#endif

#if REMUTEX_TEST
#include "recmutex.h"
#endif

#if COUNTSEM_TEST
#include "countsem.h"
#endif

#if GENQ_TEST
#include "GenQTest.h"
#endif

#if DYNAMIC_TEST
#include "dynamic.h"
#endif

#if QPEEK_TEST
#include "QPeek.h"
#endif

#if ALT_QUEUE_TEST
#include "AltQTest.h"
#endif

#if ALT_BLOCKQ_TEST
#include "AltBlckQ.h"
#endif

#if ALT_BLOCK_TEST
#include "AltBlock.h"
#endif

#if ALT_POLLQ_TEST
#include "AltPollQ.h"
#endif

#if MATH_F_TEST
#include "flop.h"
#endif

#if INTQ_TEST
#include "IntQueue.h"
#endif

/* Task priorities. */
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainINTEGER_TASK_PRIORITY           ( tskIDLE_PRIORITY )
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )
#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainUART_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainGENQ_TASK_PRIORITY				( tskIDLE_PRIORITY )

#define mainCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE )

/* The time between cycles of the 'check' task. */
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )

/*-----------------------------------------------------------*/

/*
 * Configure the clocks, GPIO and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );

static void NUC1xx_UART_Init(void);

/*
 * Checks the status of all the demo tasks then prints a message to the
 * display.  The message will be either PASS - and include in brackets the
 * maximum measured jitter time (as described at the to of the file), or a
 * message that describes which of the standard demo tasks an error has been
 * discovered in.
 *
 * Messages are written directly to the terminal.
 */
#if CHECK_TEST
static void vCheckTask( void *pvParameters );
#endif

/*-----------------------------------------------------------*/
int main( void )
{
#ifdef DEBUG
  debug();
#endif

	/* Configure realitive hardware platform. */
	prvSetupHardware();
	
	/* Start the standard demo tasks. */
#if LED_TEST
	vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
#endif
#if POLL_Q_TEST
	vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
#endif
#if BLOCK_Q_TEST
	vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
#endif
#if SEM_TEST
	vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
#endif
#if MATH_TEST
	vStartIntegerMathTasks( mainINTEGER_TASK_PRIORITY );
#endif
#if BLOCK_TM_TEST
	vCreateBlockTimeTasks();
#endif
#if REMUTEX_TEST
	vStartRecursiveMutexTasks();
#endif
#if COUNTSEM_TEST
	vStartCountingSemaphoreTasks();
#endif
#if GENQ_TEST
	vStartGenericQueueTasks( mainGENQ_TASK_PRIORITY );
#endif
#if DYNAMIC_TEST
	vStartDynamicPriorityTasks();
#endif
#if QPEEK_TEST
	vStartQueuePeekTasks();
#endif
#if ALT_QUEUE_TEST
	vStartAltGenericQueueTasks( tskIDLE_PRIORITY );
#endif
#if ALT_BLOCKQ_TEST
	vStartAltBlockingQueueTasks( tskIDLE_PRIORITY + 2 );
#endif
#if ALT_BLOCK_TEST
	vCreateAltBlockTimeTasks();
#endif
#if ALT_POLLQ_TEST
	vStartAltPolledQueueTasks( tskIDLE_PRIORITY + 2 );
#endif
#if MATH_F_TEST
	vStartMathTasks( tskIDLE_PRIORITY );
#endif
#if INTQ_TEST
	vStartInterruptQueueTasks();
#endif

	/* Start user demo tasks. */
#if UART_TEST
	vStartUARTTasks( mainUART_TASK_PRIORITY );
#endif
#if THREAD_TEST
	vStartThreadTasks();
#endif
#if QUEUE_TEST
	vStartQueueTasks();
#endif

	/* Start the tasks defined within this file/specific to this demo. */
#if CHECK_TEST
    xTaskCreate( vCheckTask, ( signed portCHAR * ) "Check", mainCHECK_TASK_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );	
#endif

	/* The suicide tasks must be created last as they need to know how many
	tasks were running prior to their creation in order to ascertain whether
	or not the correct/expected number of tasks are running at any given time. */
#if SUICID_TEST
    vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );
#endif	
	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was not enough heap space to create the
	idle task. */
	return 0;
}
/*-----------------------------------------------------------*/
#if CHECK_TEST
static void vCheckTask( void *pvParameters )
{
portTickType xLastExecutionTime;
static unsigned portBASE_TYPE times = 0;

	printf( "Check begin...\n" );

	xLastExecutionTime = xTaskGetTickCount();

    for( ;; )
	{
		/* Perform this check every mainCHECK_DELAY milliseconds. */
		vTaskDelayUntil( &xLastExecutionTime, mainCHECK_DELAY );

		/* Has an error been found in any task? */
	#if BLOCK_Q_TEST
        if( xAreBlockingQueuesStillRunning() != pdTRUE )
		{
			printf( "ERROR IN BLOCK Q\n" );
		}
	#endif
	#if BLOCK_TM_TEST
		else if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
		{
			printf("ERROR IN BLOCK TIME\n");
		}
    #endif
	#if SEM_TEST
		else if( xAreSemaphoreTasksStillRunning() != pdTRUE )
        {
			printf( "ERROR IN SEMAPHORE\n" );
        }
	#endif
	#if POLL_Q_TEST
        else if( xArePollingQueuesStillRunning() != pdTRUE )
        {
			printf( "ERROR IN POLL Q\n" );
        }
	#endif
	#if SUICID_TEST
        else if( xIsCreateTaskStillRunning() != pdTRUE )
        {
			printf( "ERROR IN CREATE\n" );
        }
	#endif
	#if MATH_TEST
        else if( xAreIntegerMathsTaskStillRunning() != pdTRUE )
        {
			printf( "ERROR IN MATH\n" );
        }
	#endif
	#if REMUTEX_TEST	
		else if( xAreRecursiveMutexTasksStillRunning() != pdTRUE )
        {
			printf( "ERROR IN REMUTEX\n" );
        }
	#endif
	#if COUNTSEM_TEST	
		else if( xAreCountingSemaphoreTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN COUNTSEM\n" );
		}
	#endif
	#if GENQ_TEST	
		else if( xAreGenericQueueTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN GENQUEUE\n" );
		}	
	#endif
	#if DYNAMIC_TEST	
		else if( xAreDynamicPriorityTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN DYNAMIC TEST\n" );
		}	
	#endif
	#if QPEEK_TEST	
		else if( xAreQueuePeekTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN Q_PEEK\n" );
		}	
	#endif
	#if ALT_QUEUE_TEST	
		else if( xAreAltGenericQueueTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN ALT_QUEUE\n" );
		}	
	#endif
	#if ALT_BLOCKQ_TEST	
		else if( xAreAltBlockingQueuesStillRunning() != pdTRUE )
		{
			printf( "ERROR IN ALT_BLOCKQ\n" );
		}	
	#endif
	#if ALT_BLOCK_TEST	
		else if( xAreAltBlockTimeTestTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN ALT_BLOCK\n" );
		}	
	#endif
	#if ALT_POLLQ_TEST	
		else if( xAreAltPollingQueuesStillRunning() != pdTRUE )
		{
			printf( "ERROR IN ALT_POLLQ\n" );
		}	
	#endif
	#if MATH_F_TEST	
		else if( xAreMathsTaskStillRunning() != pdTRUE )
		{
			printf( "ERROR IN FLOP\n" );
		}	
	#endif
	#if INTQ_TEST	
		else if( xAreIntQueueTasksStillRunning() != pdTRUE )
		{
			printf( "ERROR IN INTQ\n" );
		}	
	#endif
		else
		{
			printf( "ALL Test Pass %d times! \n",  ++times );
		}
	}
}
#endif
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	UNLOCKREG();

	/* Enable external 12M Crystal oscillation. */
	SYSCLK->PWRCON.XTL12M_EN = 1;

	/* Enable the 64 clock cycle delay. */
	SYSCLK->PWRCON.WU_DLY = 1;

	/* Enable PLL. */
	SYSCLK->PLLCON.PD = 0;
	SYSCLK->PLLCON.OE = 0;

	/* Select PLL output as system clock source. */
	SYSCLK->CLKSEL0.HCLK_S = 2;

	/* Configure SysTick clock source. */
	SYSCLK->CLKSEL0.STCLK_S = 3;

	/* Initialize NUC1xx MUC UART0. */
	NUC1xx_UART_Init();
}
/*-----------------------------------------------------------*/

void NUC1xx_UART_Init(void)
{
	/* Set UART Pin. */
	SYS->GPBMFP.UART0_RX = 1;
	SYS->GPBMFP.UART0_TX = 1;
	SYS->GPBMFP.UART0_nRTS = 1;
	SYS->GPBMFP.UART0_nCTS = 1;

	/* Reset IP */
	SYS->IPRSTC2.UART0_RST = 1;
	SYS->IPRSTC2.UART0_RST = 0;

	/* Enable UART clock */
	SYSCLK->APBCLK.UART0_EN = 1;

	/* Select UART clock source */
    SYSCLK->CLKSEL1.UART_S = 0;
    SYSCLK->PWRCON.XTL12M_EN = 1;
    
	/* Select UART clock source */
    SYSCLK->CLKSEL1.UART_S = 0;

	/* Tx FIFO Reset & Rx FIFO Reset & FIFO Mode Enable */
  	UART0->FCR.TFR = 1;
  	UART0->FCR.RFR = 1;

	/* Set Rx Trigger Level */
	UART0->FCR.RFITL = 0;  

	/* Set Parity & Data bits & Stop bits */
	UART0->LCR.SPE = 0;
	UART0->LCR.EPE = 0;
	UART0->LCR.PBE = 0;
	
	UART0->LCR.WLS = 3;
	UART0->LCR.NSB = 0;
	
	/* Set Time-Out */
	UART0->TOR = 0;

	/* Set baudrate 115200bps. */
	UART0->BAUD.DIVX_EN = 1;
	UART0->BAUD.DIVX1 = 1;
	UART0->BAUD.DIVX = 15;
	UART0->BAUD.DIV = 102;
}
