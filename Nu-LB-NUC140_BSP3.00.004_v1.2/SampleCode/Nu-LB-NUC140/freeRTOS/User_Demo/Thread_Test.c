/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*
	This file create two tasks, in order to test muti-tasks of FreeRTOS.
*/

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "NUC1xx_Test.h"

#define THREAD_TEST_STACK_SIZE	configMINIMAL_STACK_SIZE	
#define THREAD_TEST_PRIORITY	( tskIDLE_PRIORITY + 1 )

void Thread_Task1( void *pvParameters );
void Thread_Task2( void *pvParameters );

void vStartThreadTasks( void )
{
	xTaskCreate( Thread_Task1, ( signed char * ) "Th_Task1", THREAD_TEST_STACK_SIZE, NULL, THREAD_TEST_PRIORITY, ( xTaskHandle * ) NULL );
	xTaskCreate( Thread_Task2, ( signed char * ) "Th_Task2", THREAD_TEST_STACK_SIZE, NULL, THREAD_TEST_PRIORITY, ( xTaskHandle * ) NULL );
}

void Thread_Task1( void *pvParameters )
{
portTickType xLastTime1;
	
	/* Get current system TickCount. */
	xLastTime1 = xTaskGetTickCount();
	
	for( ;; )
	{
		/* Run a time every 400 milliseconds. */
		vTaskDelayUntil( &xLastTime1, 400 );
		printf("Task1 called-\n");
	}	
}

void Thread_Task2( void *pvParameters )
{
portTickType xLastTime2;
	
	/* Get current system TickCount. */
	xLastTime2 = xTaskGetTickCount();	

	for( ;; )
	{
		/* Run a time every 800 milliseconds. */
		vTaskDelayUntil( &xLastTime2, 800 );
		printf("Task2 called--\n");
	}
}
