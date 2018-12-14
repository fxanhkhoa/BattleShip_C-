/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"
#include "NUC1xx.h"

#define RXBUFSIZE 64

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint8_t comRbuf[RXBUFSIZE];
volatile uint16_t comRbytes = 0;		/* Available receiving bytes */
volatile uint16_t comRhead 	= 0;
volatile uint16_t comRtail 	= 0;
volatile int32_t g_bWait 	= TRUE;

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void ACMP_IRQHandler(void);


/*---------------------------------------------------------------------------------------------------------*/
/* ACMP Callback function                                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t acmpIntCnt = 0;
void ACMP_IRQHandler(void)
{
    acmpIntCnt++;
    if(ACMP->CMPSR.CMPF0)
    {
        if(ACMP->CMPSR.CO0 == 1)
            printf("CP0 > CN0 (%d)\n", acmpIntCnt);
        else
            printf("CP0 <= CN0 (%d)\n", acmpIntCnt);
        
        ACMP->CMPSR.CMPF0 = 1;
    }

    if(ACMP->CMPSR.CMPF1)
    {
        if(ACMP->CMPSR.CO1 == 1)
            printf("CP1 > CN1 (%d)\n", acmpIntCnt);
        else
            printf("CP1 <= CN1 (%d)\n", acmpIntCnt);
        ACMP->CMPSR.CMPF1 = 1;
    }
}

void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}



/*---------------------------------------------------------------------------------------------------------*/
/* UART Test Sample 				                                                                       */
/* Test Item					                                                                           */
/* It sends the received data to HyperTerminal.														       */
/*---------------------------------------------------------------------------------------------------------*/

int32_t main()
{
	STR_UART_T sParam;

	UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
    /* Waiting for 12M Xtal stalble */
    SysTimerDelay(5000);
    
    SystemFrequency = 12000000;
    
	/* Set UART Pin */
	DrvGPIO_InitFunction(FUNC_UART0);
	
	/* UART Setting */
    sParam.u32BaudRate 		= 115200;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;


    DrvUART_Open(UART_PORT0,&sParam);


    printf("+----------------------------------------------------------------+\n");
    printf("|                     ACMP Sample Code                           |\n");           
    printf("+----------------------------------------------------------------+\n");
    printf("  PC.6 & PC.14 are inputs and used to compare with 1.2v.\n");
    printf("\n");

    DrvGPIO_InitFunction(FUNC_ACMP0);
    DrvGPIO_InitFunction(FUNC_ACMP1);

    /* Enable ACMP clock source */
    SYSCLK->APBCLK.ACMP_EN = 1;

    /* Set relative GPIO to be input */
    GPIOC->PMD.PMD6 = 0;
    GPIOC->PMD.PMD7 = 0;
    GPIOC->PMD.PMD14 = 0;
    GPIOC->PMD.PMD15 = 0;


    /* Configure ACMP0 */
    ACMP->CMPCR[0].CMPEN = 1;
    ACMP->CMPCR[0].CMPIE = 1;
    ACMP->CMPCR[0].CMPCN = 1;
    ACMP->CMPCR[0].CMP_HYSEN = 0;

    /* Configure ACMP1 */
    ACMP->CMPCR[1].CMPEN = 1;
    ACMP->CMPCR[1].CMPIE = 1;
    ACMP->CMPCR[1].CMPCN = 1;
    ACMP->CMPCR[1].CMP_HYSEN = 0;

    NVIC_EnableIRQ(ACMP_IRQn);

    while(1);
}	
























































































































