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

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "NUC1xx_Test.h"

typedef struct AMessage
{
    portCHAR ucMessageID;
    portCHAR ucData[ 5 ];
}msg_t;

#define MSG_COUNT	5

static msg_t msg[MSG_COUNT];
portCHAR index = 0;

#define QUEUE_TEST_STACK_SIZE		configMINIMAL_STACK_SIZE	
#define QUEUE_CONSUMER_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define QUEUE_PRODUCER_PRIORITY		( tskIDLE_PRIORITY + 1 )

void Queue_Task1( void *pvParameters );
void Queue_Task2( void *pvParameters );

xQueueHandle xQueue;

void vStartQueueTasks( void )
{
int i = 0;

	/* Create the queue used by consumer and producer tasks. */
	xQueue = xQueueCreate( MSG_COUNT, sizeof( msg_t ) );

	for( index = 0; index < MSG_COUNT; index++ )
	{
		msg[index].ucMessageID = index;
		for( i = 0; i < 5; i++ )
		{
			msg[index].ucData[i] = index*5 + i;
			printf( "input data[%d] = %d\n", index*5+i, msg[index].ucData[i] );
		}
	}
	
	xTaskCreate( Queue_Task1, ( signed char * ) "Q_Task1", QUEUE_TEST_STACK_SIZE, ( void * ) NULL, QUEUE_CONSUMER_PRIORITY, ( xTaskHandle * ) NULL );
	xTaskCreate( Queue_Task2, ( signed char * ) "Q_Task2", QUEUE_TEST_STACK_SIZE, ( void * ) NULL, QUEUE_PRODUCER_PRIORITY, ( xTaskHandle * ) NULL );
}

/* Consumer */
void Queue_Task1( void *pvParameters )
{
msg_t message;
int i = 0;
	for( ;; )
	{
		if( xQueueReceive( xQueue, &message, 10 ) == pdPASS )
		{
			for( i = 0; i < 5; i++ )
			{
				printf( "getdata[%d] = %d\n", 5*message.ucMessageID+i, message.ucData[i] );
			}
		}
		else
		{
			printf( "Get queue timeout!\n" );	
		}
	}	
}

/* Producer */
void Queue_Task2( void *pvParameters )
{
	index = 0;
	for( ;; )
	{
		if( xQueueSend( xQueue, ( void * ) &msg[index], 10 ) != pdPASS )
		{
			printf( "Fill queue error\n" );	
		}
		else
		{			
			index++;
			if( index > 4 )
			{
				index = 0;
			}
		}
	}
}
