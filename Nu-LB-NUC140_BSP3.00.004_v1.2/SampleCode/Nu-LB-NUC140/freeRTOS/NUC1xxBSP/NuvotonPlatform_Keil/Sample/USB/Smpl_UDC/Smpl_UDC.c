/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvGPIO.h"
#include "DrvSYS.h"
#include "DrvUART.h"
#include "DrvUSB.h"
#include "UDC.h"


void Delay(uint32_t delayCnt)
{
    while(delayCnt--)
    {
        __NOP();
        __NOP();
    }
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int32_t main(void)
{
    STR_UART_T sParam;
	extern uint32_t SystemFrequency;

    UNLOCKREG();
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


    SYSCLK->PWRCON.XTL12M_EN = 1;

    /* Enable PLL */
    DrvSYS_SetPLLPowerDown(0);
    Delay(1000);

    DrvSYS_SetHCLKSource(2);
    
    Delay(100);

    /* The PLL must be 48MHz x N times when using USB */
    SystemFrequency = 48000000;
    
    printf("NUC100 USB MassStorage Start!\n");
        
	/* Initialize USB Device function */
    udcInit();
    
    /* Initialize mass storage device */
    udcFlashInit();  
    
    /* Start USB Mass Storage */
	udcMassBulk();
	
	return 0;
}



