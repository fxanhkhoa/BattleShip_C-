/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvSPI.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"

#define TEST_COUNT 64
void SPI1_Callback(uint32_t u32UserData);

volatile uint32_t SPI1_INT_Flag;


void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}

int main(void)
{
	uint32_t au32SourceData[TEST_COUNT];
	uint32_t au32DestinationData[TEST_COUNT];
	uint32_t u32DataCount;
	STR_UART_T param;
    int32_t i32Err, i;
    uint32_t u32Tmp;
    
    /* Unlock the protected registers */	
	UNLOCKREG();

	/* Enable the 12MHz oscillator oscillation */
	DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1);
 
     /* Waiting for 12M Xtal stalble */
    SysTimerDelay(5000);
 
	/* HCLK clock source. 0: external 12MHz; 4:internal 22MHz RC oscillator */
	DrvSYS_SetHCLKSource(0);

	LOCKREG();

	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0); /* HCLK clock frequency = HCLK clock source / (HCLK_N + 1) */

	DrvGPIO_InitFunction(FUNC_UART0);
	
	param.u32BaudRate        = 115200;
	param.u8cDataBits        = DRVUART_DATABITS_8;
	param.u8cStopBits        = DRVUART_STOPBITS_1;
	param.u8cParity          = DRVUART_PARITY_NONE;
	param.u8cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
	param.u8TimeOut        	 = 0;
	DrvUART_Open(UART_PORT0, &param);
	
	/* set the soure data and clear the destination buffer */
	for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
	{
		au32SourceData[u32DataCount] = u32DataCount;
		au32DestinationData[u32DataCount] = 0;
	}
	
	/* Configure SPI1 as a slaver, 32-bit transaction */
	DrvSPI_Open(eDRVSPI_PORT1, eDRVSPI_SLAVE, eDRVSPI_TYPE1, 32);
	/* Set the trigger mode of slave select pin. */
	DrvSPI_SetTriggerMode(eDRVSPI_PORT1, eDRVSPI_LEVEL_TRIGGER);   /* level trigger */
	/* Set the active level of slave select. */
	DrvSPI_SetSlaveSelectActiveLevel(eDRVSPI_PORT1, eDRVSPI_ACTIVE_LOW_FALLING);
	/* Enable the SPI1 interrupt and install the callback function. */
	DrvSPI_EnableInt(eDRVSPI_PORT1, SPI1_Callback, 0);
	
	/* Configure SPI0 as a master, 32-bit transaction */
	DrvSPI_Open(eDRVSPI_PORT0, eDRVSPI_MASTER, eDRVSPI_TYPE1, 32);
	/* Enable the automatic slave select function of SS0. */
	DrvSPI_EnableAutoCS(eDRVSPI_PORT0, eDRVSPI_SS0);
	/* Set the active level of slave select. */
	DrvSPI_SetSlaveSelectActiveLevel(eDRVSPI_PORT0, eDRVSPI_ACTIVE_LOW_FALLING);
	/* Enable the automatic slave select function and set the specified slave select pin. */
	DrvSPI_EnableAutoCS(eDRVSPI_PORT0, eDRVSPI_SS0);
	/* SPI clock rate 3MHz */
	DrvSPI_SetClock(eDRVSPI_PORT0, 3000000, 0);
	
	printf("\n\n");
	printf("+----------------------------------------------------------------------+\n");		
	printf("|                       SPI Driver Sample Code                         |\n");
	printf("|                                                                      |\n");
    printf("+----------------------------------------------------------------------+\n");
	printf("\n");
	printf("SPI Driver version: %x\n", DrvSPI_GetVersion());
	printf("Configure SPI0 as a master and SPI1 as a slave.\n");
	printf("Bit length of a transaction: 32\n");
	printf("SPI clock rate: %d Hz\n", DrvSPI_GetClock1(eDRVSPI_PORT0));
    printf("The I/O connection for SPI0/SPI1 loopback:\n    SPI0_SS0(GPC0)   <--> SPI1_SS0(GPC8)\n    SPI0_CLK(GPC1)   <--> SPI1_CLK(GPC9)\n");
    printf("    SPI0_MISO0(GPC2) <--> SPI1_MISO0(GPC10)\n    SPI0_MOSI0(GPC3) <--> SPI1_MOSI0(GPC11)\n\n");
	printf("Please connect SPI0 with SPI1, and press any key to start transmission.\n");
	getchar();
    
    /* Clear Tx register of SPI1 to avoid send non-zero data to Master. Just for safe. */
    u32Tmp = 0;
    DrvSPI_SetTxRegister(eDRVSPI_PORT1, &u32Tmp, 1); 

    printf("\nSPI0/1 Loopback test ");
    i32Err = 0;
    for(i=0;i<10000;i++)
    {
        u32DataCount=0;
		SPI1_INT_Flag = 0;

		if((i&0x1FF) == 0)
        {
            putchar('.');
    	}
        /* set the GO_BUSY bit of SPI1 */
    	DrvSPI_SetGo(eDRVSPI_PORT1);
    	/* write the first data of source buffer to Tx register of SPI0. And start transmission. */
    	DrvSPI_SingleWrite(eDRVSPI_PORT0, &au32SourceData[0]);
    
    	while(1)
    	{
    		if(SPI1_INT_Flag==1)
    		{
    			SPI1_INT_Flag = 0;
    			
    			if(u32DataCount<(TEST_COUNT-1))
    			{
                    /* Read the previous retrived data and trigger next transfer. */
    				DrvSPI_SingleRead(eDRVSPI_PORT1, &au32DestinationData[u32DataCount]);
    				u32DataCount++;
    				DrvSPI_SingleWrite(eDRVSPI_PORT0, &au32SourceData[u32DataCount]);
    			}
    			else
    			{
                    /* Just read the previous retrived data but trigger next transfer, because this is the last transfer. */
    				DrvSPI_DumpRxRegister(eDRVSPI_PORT1, &au32DestinationData[u32DataCount], 1);
    				break;
    			}
    		}
    	}
    	
     	for(u32DataCount=0; u32DataCount<TEST_COUNT; u32DataCount++)
    	{
    		if(au32DestinationData[u32DataCount]!=au32SourceData[u32DataCount])
    		{
    		    i32Err = 1;
            }
    	}

        if(i32Err)
            break;
    }

    if(i32Err)
    {	
	    printf(" [FAIL]\n");
    }
    else
    {
        printf(" [OK]\n");
    }

    printf("\n\nSPI driver sample code is complete.\n");

	DrvSPI_Close(eDRVSPI_PORT0);
	DrvSPI_Close(eDRVSPI_PORT1);
	return 1;
}

void SPI1_Callback(uint32_t u32UserData)
{
	SPI1_INT_Flag = 1;
}

