/*
	FreeRTOS.org V5.2.0 - Copyright (C) 2003-2009 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License (version 2) as published
	by the Free Software Foundation and modified by the FreeRTOS exception.

	FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA  02111-1307  USA.

	A special exception to the GPL is included to allow you to distribute a
	combined work that includes FreeRTOS.org without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details.


	***************************************************************************
	*                                                                         *
	* Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
	*                                                                         *
	* This is a concise, step by step, 'hands on' guide that describes both   *
	* general multitasking concepts and FreeRTOS specifics. It presents and   *
	* explains numerous examples that are written using the FreeRTOS API.     *
	* Full source code for all the examples is provided in an accompanying    *
	* .zip file.                                                              *
	*                                                                         *
	***************************************************************************

	1 tab == 4 spaces!

	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/

#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"

/* Demo includes. */
#include "IntQueueTimer.h"
#include "IntQueue.h"

/* Library includes. */
#include "DrvTimer.h"
#include "NUC1xx.h"

#define tmrTIMER_2_FREQUENCY	( 2000UL )
#define tmrTIMER_3_FREQUENCY	( 2001UL )

void vInitialiseTimerForIntQueueTest( void )
{
unsigned long ulFrequency;

	/* Select HCLK as Timer2 clock source */  
	SYSCLK->CLKSEL1.TMR2_S = 2; 
	/* Select HCLK as Timer3 clock source */  
	SYSCLK->CLKSEL1.TMR3_S = 2; 

	/* Enable Timer2 clock source */
	SYSCLK->APBCLK.TMR2_EN = 1;	
	/* Enable Timer3 clock source */
	SYSCLK->APBCLK.TMR3_EN = 1;	

	/* Reset IP TMR2 */
	SYS->IPRSTC2.TMR2_RST = 1;
	SYS->IPRSTC2.TMR2_RST = 0;
	/* Reset IP TMR3 */
	SYS->IPRSTC2.TMR3_RST = 1;
	SYS->IPRSTC2.TMR3_RST = 0;

	/* Set timer2 Operation mode period mode */
	TIMER2->TCSR.MODE = 1;
	/* Set timer3 Operation mode period mode */
	TIMER3->TCSR.MODE = 1;	

	/* Set the timer interrupts to be above the kernel.  The interrupts are
	 assigned different priorities so they nest with each other. */
	NVIC_SetPriority( TMR2_IRQn, 2 );
	NVIC_SetPriority( TMR3_IRQn, 1 );

	/* Ensure interrupts do not start until the scheduler is running. */
	portDISABLE_INTERRUPTS();

	/* The rate at which the timers will interrupt. */
	ulFrequency = configCPU_CLOCK_HZ / tmrTIMER_2_FREQUENCY;
	/* Set Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER2->TCSR.PRESCALE = 0;
	TIMER2->TCMPR = ulFrequency;
	/* Enable timer2 interrupt */		
	TIMER2->TCSR.IE = 1 ;
	NVIC_EnableIRQ(TMR2_IRQn); 
	
	/* The rate at which the timers will interrupt. */
	ulFrequency = configCPU_CLOCK_HZ / tmrTIMER_3_FREQUENCY;   
	/* Set Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER3->TCSR.PRESCALE = 0;
	TIMER3->TCMPR = ulFrequency;
	/* Enable timer3 interrupt */		
	TIMER3->TCSR.IE = 1 ;
	NVIC_EnableIRQ(TMR3_IRQn);

	/* Reset timer2 counter */
	TIMER2->TCSR.CRST = 1;	
	/* Reset timer3 counter */
	TIMER3->TCSR.CRST = 1;

	/* Enable Timer2 */					
	TIMER2->TCSR.CEN = 1;		
	/* Enable Timer3 */					
	TIMER3->TCSR.CEN = 1;		
}
/*-----------------------------------------------------------*/

void TMR2_IRQHandler(void)
{
	TIMER2->TISR.TIF =1;
	portEND_SWITCHING_ISR( xFirstTimerHandler() );  
}

void TMR3_IRQHandler(void)
{
	TIMER3->TISR.TIF =1;
	portEND_SWITCHING_ISR( xSecondTimerHandler() );  
}


