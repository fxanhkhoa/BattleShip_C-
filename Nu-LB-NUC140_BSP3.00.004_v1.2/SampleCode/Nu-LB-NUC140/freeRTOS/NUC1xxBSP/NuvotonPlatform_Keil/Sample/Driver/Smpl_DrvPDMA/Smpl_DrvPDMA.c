/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "Driver\DrvPDMA.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"
#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Define functions prototype                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define	UART_TEST_LENGTH			64
uint8_t SrcArray[UART_TEST_LENGTH];
uint8_t DestArray[UART_TEST_LENGTH];
int32_t IntCnt;
volatile int32_t IsTestOver;

/*---------------------------------------------------------------------------------------------------------*/
/* Clear buffer funcion                                                                              	   */
/*---------------------------------------------------------------------------------------------------------*/

void ClearBuf(uint32_t u32Addr, uint32_t u32Length,uint8_t u8Pattern)
{
	uint8_t* pu8Ptr;
	uint32_t i;
	
	pu8Ptr = (uint8_t *)u32Addr;
	
	for (i=0; i<u32Length; i++)
	{
		*pu8Ptr++ = u8Pattern;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Bulid Src Pattern function                                                                         	   */
/*---------------------------------------------------------------------------------------------------------*/

void BuildSrcPattern(uint32_t u32Addr, uint32_t u32Length)
{
    uint32_t i=0,j,loop;
    uint8_t* pAddr;
    
    pAddr = (uint8_t *)u32Addr;
    
    do {
        if (u32Length > 256)
	    	loop = 256;
	    else
	    	loop = u32Length;
	    	
	   	u32Length = u32Length - loop;    	

        for(j=0;j<loop;j++)
            *pAddr++ = (uint8_t)(j+i);
            
	    i++;        
	} while ((loop !=0) || (u32Length !=0));         
}

/*---------------------------------------------------------------------------------------------------------*/
/* PDMA Callback function                                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/

void PDMA0_Callback()
{
	extern int32_t IntCnt;
	printf("\tTransfer Done %02d!\r",++IntCnt);
	
	
	if(IntCnt<10)
	{
 		DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_1);		     
		DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_0);
		
	}
	else
	{
		IsTestOver = TRUE;
	}
}



/*---------------------------------------------------------------------------------------------------------*/
/* PDMA Sample Code: UART1 TX/RX PDMA                                                                  	   */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA_UART(void)
{
	STR_PDMA_T sPDMA;  
    uint32_t  UARTPort;
    volatile uint32_t i;
	BuildSrcPattern((uint32_t)SrcArray,UART_TEST_LENGTH);

   	UARTPort = UART1_BASE;    	
    i=UART_TEST_LENGTH;
    ClearBuf((uint32_t)DestArray, UART_TEST_LENGTH,0xFF);
    
	/* PDMA Init */
    DrvPDMA_Init();

	/* PDMA Setting */
	DrvPDMA_SetCHForAPBDevice(eDRVPDMA_CHANNEL_1,eDRVPDMA_UART1,eDRVPDMA_WRITE_APB);
	DrvPDMA_SetCHForAPBDevice(eDRVPDMA_CHANNEL_0,eDRVPDMA_UART1,eDRVPDMA_READ_APB);

	/* CH1 TX Setting */
	sPDMA.sSrcAddr.u32Addr 			= (uint32_t)SrcArray;
    sPDMA.sDestAddr.u32Addr 		= UARTPort;   
    sPDMA.u8TransWidth 				= eDRVPDMA_WIDTH_8BITS;
	sPDMA.u8Mode 					= eDRVPDMA_MODE_MEM2APB;
	sPDMA.sSrcAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_INCREMENTED; 
	sPDMA.sDestAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_FIXED;   
	sPDMA.i32ByteCnt                = UART_TEST_LENGTH;
	DrvPDMA_Open(eDRVPDMA_CHANNEL_1,&sPDMA);

 	/* CH0 RX Setting */
	sPDMA.sSrcAddr.u32Addr 			= UARTPort;
    sPDMA.sDestAddr.u32Addr 		= (uint32_t)DestArray;   
	sPDMA.u8Mode 					= eDRVPDMA_MODE_APB2MEM;
	sPDMA.sSrcAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_FIXED; 
	sPDMA.sDestAddr.eAddrDirection 	= eDRVPDMA_DIRECTION_INCREMENTED;   
	DrvPDMA_Open(eDRVPDMA_CHANNEL_0,&sPDMA);

	/* Enable INT */
    DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_0, eDRVPDMA_BLKD );
	DrvPDMA_EnableInt(eDRVPDMA_CHANNEL_1, eDRVPDMA_BLKD );
        
	/* Install Callback function */   
 	DrvPDMA_InstallCallBack(eDRVPDMA_CHANNEL_0,eDRVPDMA_BLKD,	(PFN_DRVPDMA_CALLBACK) PDMA0_Callback ); 
	
	/* Enable UART PDMA and Trigger PDMA specified Channel */
	DrvUART_SetPDMA(UART_PORT1,ENABLE);
 	DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_0);
	DrvPDMA_CHEnablelTransfer(eDRVPDMA_CHANNEL_1);
	
	IntCnt = 0;

	/* Trigger PDMA 10 time and the S/W Flag will be change in PDMA callback funtion */
	while(IsTestOver==FALSE);


	/* Close PDMA Channel */
	DrvPDMA_Close();
	return;  
	
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
/* MAIN function                                                                           	   			   */
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
	DrvGPIO_InitFunction(FUNC_UART1);
	
	/* UART Setting */
    sParam.u32BaudRate 		= 115200;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
	DrvUART_Open(UART_PORT0,&sParam);
	DrvUART_Open(UART_PORT1,&sParam);

	/* PDMA Sample Code: UART1 Tx/Rx Loopback */
	printf("+------------------------------------------------------------------------+\n");
    printf("|                         PDMA Driver Sample Code                        |\n");
    printf("|                                                                        |\n");
    printf("+------------------------------------------------------------------------+\n");                    
	printf("  This sample code will use PDMA to do UART1 loopback test. \n");
	printf("  Test loopback 10 times \n");
	printf("  I/O configuration:\n");
    printf("    GPB4 <--> GPB5\n\n");
    printf("  press any key to continue ...\n");  
	getchar();
	IsTestOver =FALSE;
	PDMA_UART();

    printf("\n\n  PDMA sample code is complete.\n");
    return 0;
}	
























































































































