/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvUSB.h"

#define DEBUG 1

extern int32_t HID_MainProcess(void);

void Delay(uint32_t delayCnt)
{
    while(delayCnt--)
    {
        __NOP();
        __NOP();
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
	extern uint32_t SystemFrequency;
#ifdef DEBUG
 	STR_UART_T sParam;
#endif
   UNLOCKREG();
 #ifdef DEBUG
	/* Set UART Pin */
	DrvGPIO_InitFunction(FUNC_UART0);
	
	/* UART Setting */
    sParam.u32BaudRate 		= 115200;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
	DrvUART_Open(UART_PORT0,&sParam);
#endif

 
    SYSCLK->PWRCON.XTL12M_EN = 1;
    
    /* Enable PLL */
    DrvSYS_SetPLLPowerDown(0);
	Delay(1000);

	/* Switch to PLL clock */
	DrvSYS_SetHCLKSource(2);    

    Delay(100);

    /* The PLL must be 48MHz x N times when using USB */
    SystemFrequency = 48000000;

#ifdef DEBUG    
    printf("NUC100 USB HID\n");
#endif
	
    /* Execute HID process */
	HID_MainProcess();

}




