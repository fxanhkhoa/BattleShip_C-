/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvUART.h"
#include "DrvGPIO.h"
#include "NUC1xx.h"
#include "NUC1xx_Test.h"

#include "FreeRTOS.h"
#include "task.h"

#define RXBUFSIZE 64
#define UARTStack_Size	configMINIMAL_STACK_SIZE	

static int32_t vUART_Test(void);
static portTASK_FUNCTION_PROTO( vUARTTask, pvParameters );

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t comRbuf[RXBUFSIZE];
volatile uint16_t comRbytes = 0;		/* Available receiving bytes */
volatile uint16_t comRhead 	= 0;
volatile uint16_t comRtail 	= 0;
volatile int32_t g_bWait 	= TRUE;

/*---------------------------------------------------------------------------------------------------------*/
/* UART Interrupt Handler                                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
void UART02_IRQHandler(void)
{
 	uint8_t bInChar[1]={0xFF};
	uint32_t u32IntStatus;
	
	u32IntStatus = inpw(&UART0->ISR);
	
	if(u32IntStatus & RDAIE)
	{
		printf("\nInput:");
		
		/* Get all the input characters */
		while(UART0->ISR.RDA_IF==1) 
		{
		    /* Check RX empty */
			while (UART0->FSR.RX_EMPTY == 1);
			bInChar[0] = UART0->DATA;					  

			printf("%c ", bInChar[0]);
			
			if(bInChar[0] == '0')	
			{	
				g_bWait = FALSE;
			}
		
			/* Check if buffer full */
			if(comRbytes < RXBUFSIZE)
			{
				/* Enqueue the character */
				comRbuf[comRtail] = bInChar[0];
				comRtail = (comRtail == (RXBUFSIZE-1)) ? 0 : (comRtail+1);
				comRbytes++;
			}			
		}
		printf("\nTransmission Test:");
	}
	else if(u32IntStatus & THREIE)
	{   
		   
        uint16_t tmp;
        tmp = comRtail;
		if(comRhead != tmp)
		{
			bInChar[0] = comRbuf[comRhead];
			/* Wait Tx empty */
			while (UART0->FSR.TX_EMPTY != 1);
			UART0->DATA = bInChar[0];						
			comRhead = (comRhead == (RXBUFSIZE-1)) ? 0 : (comRhead+1);
			comRbytes--;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* UART Test Sample 				                                                                       */
/* Test Item					                                                                           */
/* It sends the received data to HyperTerminal.														       */
/*---------------------------------------------------------------------------------------------------------*/
#if 0
int32_t main()
#else
int32_t vUART_Test(void)
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

	printf("\nUART Sample Demo. (Press '0' to exit)\n");

	/* Enable Interrupt */
	UART0->IER.RDA_IEN = 1;	
	UART0->IER.THRE_IEN = 1;
	UART0->IER.RLS_IEN = 1;
	NVIC_EnableIRQ(UART0_IRQn);

	while(g_bWait);	
		
	/* Disable Interrupt */
	UART0->IER.RDA_IEN = 0;	
	UART0->IER.THRE_IEN = 0;
	UART0->IER.RLS_IEN = 0;

	/* Disable I & F bit */ 
	NVIC_DisableIRQ (UART0_IRQn);		
	
	printf("\nUART Sample Demo End.\n");
	
	return TRUE;
}	
#endif

void vStartUARTTasks( unsigned portBASE_TYPE uxPriority )
{
	xTaskCreate( vUARTTask, ( signed char * ) "UART", UARTStack_Size, NULL, uxPriority, ( xTaskHandle * ) NULL );
}

static portTASK_FUNCTION( vUARTTask, pvParameters )
{
	/* The parameters are not used. */
	( void ) pvParameters;

	vUART_Test();
	for(;;);
}





















































































































