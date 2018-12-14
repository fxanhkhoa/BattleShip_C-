/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "Driver\DrvFMC.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvSYS.h"

#define KEY_ADDR    0x20003FFC      /* The location of signature */
#define SIGNATURE   0x21557899  /* The signature word is used by AP code to check if simple LD is finished */

#define CONFIG0_SETTING     0xFFFFFFFE  /* Enable data flash */
#define CONFIG1_SETTING     0x0001F000  /* The data flash base is 0x1F000 */

#define LDROM_BASE      0x00100000
#define CONFIG_BASE     0x00300000
#define PAGE_SIZE       512

extern uint32_t loaderImageBase;
extern uint32_t loaderImageLimit;

void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}


/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function									                                           			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
	int32_t  i32Ret, i32Err;
	uint32_t u32Data, i;
	STR_UART_T sParam;
    uint32_t u32DataFlashBase;
    uint32_t config0Bak, config1Bak;
    int8_t ch;
  
 	/* Unlock protected registers to write ISP Control Register (ISPCON) */
    UNLOCKREG();

    SYSCLK->PWRCON.XTL12M_EN = 1;
   /* Waiting for Xtal stalble */
    SysTimerDelay(5000);


	/* Set UART Pin */
	SYS->GPBMFP.UART0_RX 	= 1;
	SYS->GPBMFP.UART0_TX 	= 1;
   
	/* UART Setting */
    sParam.u32BaudRate 		= 115200;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
	DrvUART_Open(UART_PORT0,&sParam);
    
    printf("\n\n");

    
	/* Enable ISP function */
	DrvFMC_EnableISP(1);

    /* Check the signature to check if Simple LD code is finished or not */
    if(inpw(KEY_ADDR) == SIGNATURE)
    {
        /* Just clear SIGNATURE and finish the sample code if Simple LD code has been executed. */
        outpw(KEY_ADDR, 0);
        /* Read BS */
        printf("  Boot Mode .................................. ");
        if(DrvFMC_GetBootSelect() == APROM)
            printf("[APROM]\n");
        else
        {
            printf("[LDROM]\n");
            printf("  WARNING: The driver sample code must execute in AP mode!\n");
        }

/*---------------------------------------------------------------------------------------------------------*/
/*  Program data flash      						                                           			   */
/*---------------------------------------------------------------------------------------------------------*/

    	/* Read Data Flash base address */
    	u32DataFlashBase = DrvFMC_ReadDataFlashBaseAddr();
    	printf("  Data Flash Base Address .................... [0x%08x]\n", u32DataFlashBase);
    
        printf("  Erase Data Flash ........................... ");
    	
    	/* Page Erase Data Flash */
    	for(i=0;i<4096;i+=PAGE_SIZE)
            DrvFMC_Erase(u32DataFlashBase + i);
    	
    	/* Erase Verify */
        i32Err = 0;
    	for(i = u32DataFlashBase; i < (u32DataFlashBase+4096); i += 4) 
        {     
    		DrvFMC_Read(i, &u32Data);
            
            if(u32Data != 0xFFFFFFFF)
            { 
                i32Err = 1;
            }         
        }
        if(i32Err)
            printf("[FAIL]\n");
        else
            printf("[OK]\n");
    	
    
        printf("  Program Data Flash ......................... ");
    	/* Program Data Flash and read out data to compare it */
        for(i = u32DataFlashBase; i < (u32DataFlashBase+4096); i += 4) 
        {
            DrvFMC_Write(i, i);
        }
    
        i32Err = 0;
        for(i = u32DataFlashBase; i < (u32DataFlashBase+4096); i += 4) 
        {
    		DrvFMC_Read(i, &u32Data);
            if(u32Data != i)
            { 
               i32Err = 1;
            }        
        }
        if(i32Err)
            printf("[FAIL]\n");
        else
            printf("[OK]\n");

        
        goto lexit;
    }
    
    printf("\n\n");
    printf("+-------------------------------------------------------------------------+\n");
    printf("|                                                                         |\n");
    printf("|                  NUC1xx Data Flash Configure Demo Code                  |\n");
    printf("|                                                                         |\n");
    printf("+-------------------------------------------------------------------------+\n");
    printf("|                                                                         |\n");
    printf("|  [1] The last 4kBytes of AP-ROM will be configured as DATA Flash        |\n");
    printf("|  [2] Disable DATA Flash                                                 |\n");
    printf("|                                                                         |\n");
    printf("+-------------------------------------------------------------------------+\n");
    printf(" Select:");
    ch = getchar();
    printf("%c\n", ch);


    /* Read BS */
    printf("  Boot Mode .................................. ");
    if(DrvFMC_GetBootSelect() == APROM)
        printf("[APROM]\n");
    else
    {
        printf("[LDROM]\n");
        printf("  WARNING: The driver sample code must execute in AP mode!\n");
        goto lexit;
    }

	/* Read Company ID */
	i32Ret = DrvFMC_ReadCID(&u32Data);
	if (i32Ret == 0)
		printf("  Company ID ................................. [0x%08x]\n", u32Data);

	/* Read Device ID */
	i32Ret = DrvFMC_ReadDID(&u32Data);
	if (i32Ret == 0)
		printf("  Device ID .................................. [0x%08x]\n", u32Data);

	/* Read Data Flash base address */
	u32Data = DrvFMC_ReadDataFlashBaseAddr();
	printf("  Data Flash Base Address .................... [0x%08x]\n", u32Data);

    /* Read and backup the configuration settings */
    printf("  Read config0 ............................... ");
    DrvFMC_Read(CONFIG_BASE, &config0Bak);
    printf("[0x%08x]\n", config0Bak);

    printf("  Read config1 ............................... ");
    DrvFMC_Read(CONFIG_BASE+4, &config1Bak);
    printf("[0x%08x]\n", config1Bak);

    /* Configuration region write test */
    DrvFMC_EnableConfigUpdate(1);
    printf("  Erase config region ........................ ");
    DrvFMC_Erase(CONFIG_BASE);
    DrvFMC_Read(CONFIG_BASE, &u32Data);
    if(u32Data != 0xFFFFFFFF)
        printf("[FAIL]\n");
    else
        printf("[OK]\n");

    if(ch == '1')
    {
        printf("  Program config region ...................... ");
        DrvFMC_Write(CONFIG_BASE, CONFIG0_SETTING);
        DrvFMC_Write(CONFIG_BASE+4, CONFIG1_SETTING);
        i32Err = 0;    
        DrvFMC_Read(CONFIG_BASE, &u32Data);
        if(u32Data != CONFIG0_SETTING)
            i32Err = 1;
        DrvFMC_Read(CONFIG_BASE+4, &u32Data);
        if(u32Data != CONFIG1_SETTING)
            i32Err = 1;
    
        if(i32Err)
            printf("[FAIL]\n");
        else
            printf("[OK]\n");
    }
 

    /* Read and backup the configuration settings */
    printf("  Read config0 ............................... ");
    DrvFMC_Read(CONFIG_BASE, &config0Bak);
    printf("[0x%08x]\n", config0Bak);

    printf("  Read config1 ............................... ");
    DrvFMC_Read(CONFIG_BASE+4, &config1Bak);
    printf("[0x%08x]\n", config1Bak);

lexit:

 	/* Disable ISP function */
	DrvFMC_EnableConfigUpdate(0);
    DrvFMC_EnableISP(0);

	/* Lock protected registers */
	LOCKREG();
    
	printf("\nNUC1xx Data Flash Configure Demo Completed.\n");

}




