/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvSYS.h"


void GPABCallback(uint32_t u32GpaStatus, uint32_t u32GpbStatus)
{
    printf("GPAB Interrupt! GPA:0x%04x  GPB:0x%04x\n", u32GpaStatus, u32GpbStatus);

    /* Toggle LED */
    outpw((uint32_t)&GPIOA->DOUT, inpw((uint32_t)&GPIOA->PIN) ^ (1 << 7));
}

void GPCDECallback(uint32_t u32GpcStatus, uint32_t u32GpdStatus, uint32_t u32GpeStatus)
{
    printf("GPCDE Interrupt! GPC:0x%04x  GPD:0x%04x  GPE:0x%04x\n", u32GpcStatus, u32GpdStatus, u32GpeStatus);
 
    /* Toggle LED (GPA7) */
    outpw((uint32_t)&GPIOA->DOUT, inpw((uint32_t)&GPIOA->PIN) ^ (1 << 7));
}

void EINT0Callback(void)
{
    printf("EINT0 Interrupt!\n");
    
    /* Toggle LED (GPA7) */
    outpw((uint32_t)&GPIOA->DOUT, inpw((uint32_t)&GPIOA->PIN) ^ (1 << 7));
}

void EINT1Callback(void)
{
    printf("EINT1 Interrupt!\n");
    
    /* Toggle LED (GPA7) */
    outpw((uint32_t)&GPIOA->DOUT, inpw((uint32_t)&GPIOA->PIN) ^ (1 << 7));
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
/* MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/    

int main (void)
{
	STR_UART_T param;
    int32_t i32Err;

	UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
    /* Waiting for 12M Xtal stalble */
    SysTimerDelay(5000);

	DrvGPIO_InitFunction(FUNC_UART0);
	
    param.u32BaudRate        = 115200;
    param.u8cDataBits        = DRVUART_DATABITS_8;
    param.u8cStopBits        = DRVUART_STOPBITS_1;
    param.u8cParity          = DRVUART_PARITY_NONE;
    param.u8cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
    param.u8TimeOut        	 = 0;
    DrvUART_Open(UART_PORT0, &param);
   
	printf("\n\n");
    printf("+-----------------------------------------------------------------------+\n");
    printf("|                      GPIO Driver Sample Code                          |\n");
    printf("+-----------------------------------------------------------------------+\n");


/*---------------------------------------------------------------------------------------------------------*/
/* Basic Setting                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/    
    /*-----------------------------------------------------------------------------------------------------*/
    /* Configure Bit0 in GPIOA to Output pin and Bit1 in GPIOA to Input pin then close it                  */
    /*-----------------------------------------------------------------------------------------------------*/
	printf("  >> Please connect GPA0 and GPA1 first <<\n");
	
	DrvGPIO_Open(GPA,0, IO_OUTPUT);
	DrvGPIO_Open(GPA,1, IO_INPUT);

	DrvGPIO_ClrBit(GPA,0);
	
    i32Err = 0;
    printf("  GPIO Input/Output test ................................ ");

	if(DrvGPIO_GetBit(GPA,1)!=0)
	{
		i32Err = 1;
	}

	DrvGPIO_SetBit(GPA,0);
	
	if(DrvGPIO_GetBit(GPA,1)!=1)
	{
		i32Err = 1;
	}

    DrvGPIO_Close(GPA,0);
	DrvGPIO_Close(GPA,1);
 
    if(i32Err)
    {
	    printf("[FAIL]\n");
        printf("\n  (Please make sure GPA0 and GPA1 are connected)\n");
    }
    else
    {
        printf("[OK]\n");
    }

/*---------------------------------------------------------------------------------------------------------*/
/* GPIO Interrupt Test                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/    
    
    printf("\n  GPB13, GPC0, GPB14 and GPB15 are used to test key interrupt\n  and control LED (GPA7)\n");    

    /* Configure GPA7 for LED control */
    DrvGPIO_Open(GPA, 7, IO_OUTPUT);

    /* Configure general GPIO interrupt */
    DrvGPIO_Open(GPB, 13, IO_INPUT);
    /* The Quasi-bidirection mode could be used as input with pull up enable */
    DrvGPIO_Open(GPC, 0, IO_QUASI);
    DrvGPIO_SetIntCallback(GPABCallback, GPCDECallback);
    DrvGPIO_EnableInt(GPB, 13, IO_RISING, MODE_EDGE);
    /* IO_FALLING means low level trigger if it is in level trigger mode */
    DrvGPIO_EnableInt(GPC, 0, IO_FALLING, MODE_LEVEL); 
    DrvGPIO_SetDebounceTime(3, DBCLKSRC_HCLK);    
    DrvGPIO_EnableDebounce(GPB, 13);
    DrvGPIO_EnableDebounce(GPC, 0);

    /* Configure external interrupt */
    DrvGPIO_EnableEINT0(IO_FALLING, MODE_EDGE, EINT0Callback);
    DrvGPIO_EnableEINT1(IO_BOTH_EDGE, MODE_EDGE, EINT1Callback);

    /* Waiting for interrupts */
    while(1);

}




