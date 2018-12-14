/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Include related header files                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvADC.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Define Function Prototypes                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void AdcSingleModeTest(void);
void AdcSingleCycleModeTest(void);
void AdcContScanModeTest(void);
void AdcResultMonitorTest(void);
void AdcIntCallback(uint32_t);
void Cmp0IntCallback(uint32_t);
void Cmp1IntCallback(uint32_t);
uint8_t SingleEndInput_SingleOpModeChannelSelect(uint8_t *);
uint8_t DifferentialInput_SingleOpModeChannelSelect(uint8_t *);
uint8_t SingleEndInput_ScanOpModeChannelSelect(uint8_t *, uint8_t *);
uint8_t DifferentialInput_ScanOpModeChannelSelect(uint8_t *, uint8_t *);

/*---------------------------------------------------------------------------------------------------------*/
/* Define global variables                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t gu8AdcIntFlag;
uint8_t gu8AdcCmp0IntFlag;
uint8_t gu8AdcCmp1IntFlag;


void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}


/* Main function */
int main(void)
{
	uint8_t u8Option;
	STR_UART_T param;
	
    /* Unlock the locked registers before we can access them */
    UNLOCKREG(x);

	/* Enable the 12MHz oscillator oscillation */
	DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1);

    /* Waiting for 12M Xtal stalble */
    SysTimerDelay(5000);
 

	/* HCLK clock source. 0: external 12MHz; 4:internal 22MHz RC oscillator */
	DrvSYS_SetHCLKSource(0);

	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0); /* HCLK clock frequency = HCLK clock source / (HCLK_N + 1) */

	DrvGPIO_InitFunction(FUNC_UART0);
	
	param.u32BaudRate        = 115200;
	param.u8cDataBits        = DRVUART_DATABITS_8;
	param.u8cStopBits        = DRVUART_STOPBITS_1;
	param.u8cParity          = DRVUART_PARITY_NONE;
	param.u8cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
	param.u8TimeOut        	 = 0;
	DrvUART_Open(UART_PORT0, &param);

	printf("\n\n");
	printf("Driver version: %x\n", DrvADC_GetVersion());
	printf("Conversion rate: %d samples/second\n", DrvADC_GetConversionRate());
	while(1)
	{
		/* default setting: single end input, single operation mode, all channel disable, ADC clock frequency = 12MHz/(5+1) */
		DrvADC_Open(ADC_SINGLE_END, ADC_SINGLE_OP, 0, EXT_12MHZ, 5);
		
		printf("\n\n\n");			
		printf("+----------------------------------------------------------------------+\n");		
		printf("|                            ADC sample code                           |\n");		
		printf("+----------------------------------------------------------------------+\n");		
		printf("  [1] ADC single mode test\n");
		printf("  [2] ADC single cycle mode test\n");
		printf("  [3] ADC continuous scan mode test\n");
		printf("  [4] ADC result monitor test\n");
		printf("  [q] Quit\n");

		u8Option = getchar();

 		if(u8Option == '1')
 		{
 			AdcSingleModeTest();
 		}
 		else if(u8Option == '2')
 		{
 			AdcSingleCycleModeTest();
 		}
 		else if(u8Option == '3')
 		{
 			AdcContScanModeTest();
 		}
 		else if(u8Option == '4')
 		{
 			AdcResultMonitorTest();
 		}
 		else if( (u8Option == 'q') || (u8Option == 'Q') )
 		{
 			break;
 		}
	}
	DrvADC_Close();
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: AdcSingleModeTest                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*   ADC single mode test.                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void AdcSingleModeTest()
{
	uint8_t u8Option;
	ADC_INPUT_MODE InputMode;
	uint8_t u8ChannelSelBitwise, u8ChannelNum;
	uint32_t u32ConversionData;
	
	printf("\n== Single mode test ===\n");
	/* Set the ADC operation mode as single mode */
	DrvADC_SetAdcOperationMode(ADC_SINGLE_OP);
	
	while(1)
	{
		printf("Select input mode:\n");
		printf("  [1] Single end input\n");
		printf("  [2] Differential input\n");
		printf("  Other keys: exit single mode test\n");
		u8Option = getchar();
		if(u8Option=='1')
			InputMode = ADC_SINGLE_END;
		else if(u8Option=='2')
			InputMode = ADC_DIFFERENTIAL;
		else
			return ;
		
		/* Set the ADC input mode */
		DrvADC_SetAdcInputMode(InputMode);
		
		if(InputMode==ADC_SINGLE_END)
			u8ChannelNum = SingleEndInput_SingleOpModeChannelSelect(&u8ChannelSelBitwise);        /* Select the active channel */
		else
			u8ChannelNum = DifferentialInput_SingleOpModeChannelSelect(&u8ChannelSelBitwise);     /* Select the active channel */
		
		if(u8ChannelNum==0xFF)
			return ;
		
		/* Set the ADC channel */
		DrvADC_SetAdcChannel(u8ChannelSelBitwise, InputMode);
		
		gu8AdcIntFlag = 0;
		DrvADC_EnableAdcInt(AdcIntCallback, 0);
	
		/* Start A/D conversion */
		DrvADC_StartConvert();
		/* Wait ADC interrupt */
		while(gu8AdcIntFlag==0);
		
		DrvADC_DisableAdcInt();
		
		u32ConversionData = DrvADC_GetConversionData(u8ChannelNum);
		printf("Conversion result: 0x%X (%d)\n\n", u32ConversionData, u32ConversionData);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: AdcSingleCycleModeTest                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*   ADC single cycle scan mode test.                                                                      */
/*---------------------------------------------------------------------------------------------------------*/
void AdcSingleCycleModeTest()
{
	uint8_t u8Option;
	ADC_INPUT_MODE InputMode;
	uint8_t u8ChannelSelBitwise, u8ActiveChannelNum, u8ChannelNum, u8ChannelCount;
	uint32_t u32ConversionData;
	uint8_t au8ActiveChannel[8];
	
	printf("\n== Single cycle scan mode test ===\n");
	/* Set the ADC operation mode as single cycle scan mode */
	DrvADC_SetAdcOperationMode(ADC_SINGLE_CYCLE_OP);
	
	while(1)
	{
		printf("Select input mode:\n");
		printf("  [1] Single end input\n");
		printf("  [2] Differential input\n");
		printf("  Other keys: exit single mode test\n");
		u8Option = getchar();
		if(u8Option=='1')
			InputMode = ADC_SINGLE_END;
		else if(u8Option=='2')
			InputMode = ADC_DIFFERENTIAL;
		else
			return ;
		
		/* Set the ADC input mode */
		DrvADC_SetAdcInputMode(InputMode);
		
		if(InputMode==ADC_SINGLE_END)
		{
			/* Select the active channel */
			u8ActiveChannelNum = SingleEndInput_ScanOpModeChannelSelect(&u8ChannelSelBitwise, au8ActiveChannel);
			
			if(u8ActiveChannelNum==0xFF)
				return ;
			
			/* Set the ADC channel */
			DrvADC_SetAdcChannel(u8ChannelSelBitwise, InputMode);
			
			/* start A/D conversion */
			DrvADC_StartConvert();
			
			/* Wait conversion done */
			while(DrvADC_IsConversionDone()==FALSE);
			
			for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++)
			{
				u8ChannelNum = au8ActiveChannel[u8ChannelCount];
				u32ConversionData = DrvADC_GetConversionData(u8ChannelNum);
				printf("Conversion result of channel %d: 0x%X (%d)\n\n", u8ChannelNum, u32ConversionData, u32ConversionData);
			}
		}
		else /* Differential input mode */
		{
			/* Select the active channel */
			u8ActiveChannelNum = DifferentialInput_ScanOpModeChannelSelect(&u8ChannelSelBitwise, au8ActiveChannel);
			
			if(u8ActiveChannelNum==0xFF)
				return ;
			
			/* Set the ADC channel */
			DrvADC_SetAdcChannel(u8ChannelSelBitwise, InputMode);
			
			/* start A/D conversion */
			DrvADC_StartConvert();
			
			/* Wait conversion done */
			while(DrvADC_IsConversionDone()==FALSE);
			
			for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++)
			{
				u8ChannelNum = au8ActiveChannel[u8ChannelCount] * 2 + 1;
				u32ConversionData = DrvADC_GetConversionData(u8ChannelNum);
				printf("Conversion result of input pair %d: 0x%X (%d)\n\n", au8ActiveChannel[u8ChannelCount], u32ConversionData, u32ConversionData);
			}
		}
		
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: AdcContScanModeTest                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*   ADC continuous scan mode test.                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void AdcContScanModeTest()
{
	uint8_t u8Option;
	ADC_INPUT_MODE InputMode;
	uint8_t u8ChannelSelBitwise, u8ActiveChannelNum, u8ChannelNum, u8ChannelCount;
	uint32_t u32ConversionData;
	uint8_t au8ActiveChannel[8];
	
	printf("\n== Continuous scan mode test ===\n");
	/* Set the ADC operation mode as continuous scan mode */
	DrvADC_SetAdcOperationMode(ADC_CONTINUOUS_OP);
	
	while(1)
	{
		printf("Select input mode:\n");
		printf("  [1] Single end input\n");
		printf("  [2] Differential input\n");
		printf("  Other keys: exit single mode test\n");
		u8Option = getchar();
		if(u8Option=='1')
			InputMode = ADC_SINGLE_END;
		else if(u8Option=='2')
			InputMode = ADC_DIFFERENTIAL;
		else
			return ;
		
		/* Set the ADC input mode */
		DrvADC_SetAdcInputMode(InputMode);
		
		if(InputMode==ADC_SINGLE_END)
		{
			/* Select the active channel */
			u8ActiveChannelNum = SingleEndInput_ScanOpModeChannelSelect(&u8ChannelSelBitwise, au8ActiveChannel);
			
			if(u8ActiveChannelNum==0xFF)
				return ;
			
			/* Set the ADC channel */
			DrvADC_SetAdcChannel(u8ChannelSelBitwise, InputMode);
			
			/* start A/D conversion */
			DrvADC_StartConvert();
			
			/* Wait conversion done */
			while(DrvADC_IsConversionDone()==FALSE);
			
			/* Stop A/D conversion */
			DrvADC_StopConvert();
			for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++)
			{
				u8ChannelNum = au8ActiveChannel[u8ChannelCount];
				u32ConversionData = DrvADC_GetConversionData(u8ChannelNum);
				printf("Conversion result of channel %d: 0x%X (%d)\n\n", u8ChannelNum, u32ConversionData, u32ConversionData);
			}
		}
		else /* Differential input mode */
		{
			/* Select the active channel */
			u8ActiveChannelNum = DifferentialInput_ScanOpModeChannelSelect(&u8ChannelSelBitwise, au8ActiveChannel);
			
			if(u8ActiveChannelNum==0xFF)
				return ;
			
			/* Set the ADC channel */
			DrvADC_SetAdcChannel(u8ChannelSelBitwise, InputMode);
			
			/* start A/D conversion */
			DrvADC_StartConvert();
			
			/* Wait conversion done */
			while(DrvADC_IsConversionDone()==FALSE);
			
			/* Stop A/D conversion */
			DrvADC_StopConvert();
			for(u8ChannelCount=0; u8ChannelCount<u8ActiveChannelNum; u8ChannelCount++)
			{
				u8ChannelNum = au8ActiveChannel[u8ChannelCount] * 2 + 1;
				u32ConversionData = DrvADC_GetConversionData(u8ChannelNum);
				printf("Conversion result of input pair %d: 0x%X (%d)\n\n", au8ActiveChannel[u8ChannelCount], u32ConversionData, u32ConversionData);
			}
		}
		
	}
	
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: AdcResultMonitorTest                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   None.                                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*   ADC result monitor function test.                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void AdcResultMonitorTest()
{
	uint8_t u8CmpChannelNum, u8ChannelSelBitwise, u8CmpMatchCount;
	ADC_INPUT_MODE InputMode = ADC_SINGLE_END;
	
	printf("\n== ADC result monitor test ===\n");
	/* Set the ADC operation mode as continuous scan mode */
	DrvADC_SetAdcOperationMode(ADC_CONTINUOUS_OP);
	
	/* Enable ADC channel 0 */
	u8ChannelSelBitwise = 0x1;
	/* Set the ADC channel */
	DrvADC_SetAdcChannel(u8ChannelSelBitwise, InputMode);
	
	u8CmpChannelNum = 0;
	gu8AdcCmp0IntFlag = 0;
	u8CmpMatchCount = 5;
	/* Enable ADC compare 0. Compare condition: conversion result < 0x800. */
	DrvADC_Adcmp0Enable(u8CmpChannelNum, LESS_THAN, 0x800, u8CmpMatchCount);
	/* enable ADC compare 0 interrupt and set the callback function */
	DrvADC_EnableAdcmp0Int(Cmp0IntCallback, 0);
	
	gu8AdcCmp1IntFlag = 0;
	u8CmpMatchCount = 5;
	/* Enable ADC compare 1. Compare condition: conversion result >= 0x800. */
	DrvADC_Adcmp1Enable(u8CmpChannelNum, GREATER_OR_EQUAL, 0x800,  u8CmpMatchCount);
	/* enable ADC compare 1 interrupt and set the callback function */
	DrvADC_EnableAdcmp1Int(Cmp1IntCallback, 0);
	
	/* start A/D conversion */
	DrvADC_StartConvert();
	/* Wait ADC compare interrupt */
	while( (gu8AdcCmp0IntFlag==0)&&(gu8AdcCmp1IntFlag==0) );
	
	DrvADC_StopConvert();
	DrvADC_DisableAdcmp0Int();
	DrvADC_DisableAdcmp1Int();
	DrvADC_Adcmp0Disable();
	DrvADC_Adcmp1Disable();
	if(gu8AdcCmp0IntFlag==1)
	{
		printf("The conversion result of channel %d is less than 0x800\n", u8CmpChannelNum);
	}
	else
	{
		printf("The conversion result of channel %d is greater or equal to 0x800\n", u8CmpChannelNum);
	}
	
}

/* ADC interrupt callback function */
void AdcIntCallback(uint32_t u32UserData)
{
	gu8AdcIntFlag = 1;
}

/* ADC interrupt callback function */
void Cmp0IntCallback(uint32_t u32UserData)
{
	gu8AdcCmp0IntFlag = 1;
}

/* ADC interrupt callback function */
void Cmp1IntCallback(uint32_t u32UserData)
{
	gu8AdcCmp1IntFlag = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: SingleEndInput_SingleOpModeChannelSelect                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelSelBitwise [in]:  Indicate the selected channel.                                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   0xFF   -- quit.                                                                                       */
/*   Others -- the active channel #                                                                        */
/*                                                                                                         */
/* Description:                                                                                            */
/*   Select active channel.                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t SingleEndInput_SingleOpModeChannelSelect(uint8_t * u8ChannelSelBitwise)
{
	uint8_t u8Option;
	
	printf("Select ADC channel:\n");
	printf("  [0] Channel 0\n");
	printf("  [1] Channel 1\n");
	printf("  [2] Channel 2\n");
	printf("  [3] Channel 3\n");
	printf("  [4] Channel 4\n");
	printf("  [5] Channel 5\n");
	printf("  [6] Channel 6\n");
	printf("  [7] Channel 7\n");
	printf("  Other keys: exit single mode test\n");
	u8Option = getchar();
   
	if(u8Option=='0')
		*u8ChannelSelBitwise = 1;
	else if(u8Option=='1')
		*u8ChannelSelBitwise = 2;
	else if(u8Option=='2')
		*u8ChannelSelBitwise = 4;
	else if(u8Option=='3')
		*u8ChannelSelBitwise = 8;
	else if(u8Option=='4')
		*u8ChannelSelBitwise = 0x10;
	else if(u8Option=='5')
		*u8ChannelSelBitwise = 0x20;
	else if(u8Option=='6')
		*u8ChannelSelBitwise = 0x40;
	else if(u8Option=='7')
		*u8ChannelSelBitwise = 0x80;
	else
		return 0xFF;
	u8Option = u8Option - '0';
	return u8Option;   /* return the the active channel number */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DifferentialInput_SingleOpModeChannelSelect                                                   */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelSelBitwise [in]:  Indicate the selected channel.                                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   0xFF   -- quit.                                                                                       */
/*   Others -- the active channel #                                                                        */
/*                                                                                                         */
/* Description:                                                                                            */
/*   Select active channel.                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DifferentialInput_SingleOpModeChannelSelect(uint8_t * u8ChannelSelBitwise)
{
	uint8_t u8Option;
	
	printf("Select ADC channel:\n");
	printf("  [0] Differential input pair 0\n");
	printf("  [1] Differential input pair 1\n");
	printf("  [2] Differential input pair 2\n");
	printf("  [3] Differential input pair 3\n");
	printf("  Other keys: quit\n");
	u8Option = getchar();
	if(u8Option=='0')
	{
		*u8ChannelSelBitwise = 2;
		u8Option = 1;
	}
	else if(u8Option=='1')
	{
		*u8ChannelSelBitwise = 8;
		u8Option = 3;
	}
	else if(u8Option=='2')
	{
		*u8ChannelSelBitwise = 0x20;
		u8Option = 5;
	}
	else if(u8Option=='3')
	{
		*u8ChannelSelBitwise = 0x80;
		u8Option = 7;
   }
   else
		return 0xFF;
	return u8Option;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: SingleEndInput_ScanOpModeChannelSelect                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelSelBitwise [in]:  Indicate the selected channels.                                            */
/*   pu8ActiveChannel    [out]: Save the channel #.                                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   0xFF   -- quit.                                                                                       */
/*   Others -- number of active channels.                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*   Select active channel.                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t SingleEndInput_ScanOpModeChannelSelect(uint8_t * u8ChannelSelBitwise, uint8_t * pu8ActiveChannel)
{
	uint8_t u8ChannelCount, u8ActiveChannelCount, u8Option;
	
	printf("Select ADC channel:\n");
	u8ActiveChannelCount = 0;
	*u8ChannelSelBitwise = 0;
	for(u8ChannelCount=0; u8ChannelCount<8; u8ChannelCount++)
	{
		printf("  Enable channel %d ? (y/n/q)", u8ChannelCount);
		while(1)
		{
			u8Option = getchar();
			if( (u8Option=='y') || (u8Option=='Y') )
			{
				printf("\n");
				*u8ChannelSelBitwise = *u8ChannelSelBitwise | (1<<u8ChannelCount);
				*(pu8ActiveChannel + u8ActiveChannelCount) = u8ChannelCount;
				u8ActiveChannelCount++;
				break;
			}
			else if( (u8Option=='n') || (u8Option=='N') )
			{
				printf("\n");
				break;
			}
			else if( (u8Option=='q') || (u8Option=='Q') )
			{
				printf("\n");
				return 0xFF;
			}
		}
	}
	return u8ActiveChannelCount;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DifferentialInput_ScanOpModeChannelSelect                                                     */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelSelBitwise [in]:  Indicate the selected channels.                                            */
/*   pu8ActiveChannel    [out]: Save the channel #.                                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*   0xFF   -- quit.                                                                                       */
/*   Others -- number of active channels.                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*   Select active channel.                                                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint8_t DifferentialInput_ScanOpModeChannelSelect(uint8_t * u8ChannelSelBitwise, uint8_t * pu8ActiveChannel)
{
	uint8_t u8ChannelCount, u8ActiveChannelCount, u8Option;
	
	printf("Select ADC channel:\n");
	u8ActiveChannelCount = 0;
	*u8ChannelSelBitwise = 0;
	for(u8ChannelCount=0; u8ChannelCount<4; u8ChannelCount++)
	{
		printf("  Enable differential pair %d ? (y/n/q)", u8ChannelCount);
		while(1)
		{
			u8Option = getchar();
			if( (u8Option=='y') || (u8Option=='Y') )
			{
				printf("\n");
				*u8ChannelSelBitwise = *u8ChannelSelBitwise | (1<<(u8ChannelCount*2+1));
				*(pu8ActiveChannel + u8ActiveChannelCount) = u8ChannelCount;
				u8ActiveChannelCount++;
				break;
			}
			else if( (u8Option=='n') || (u8Option=='N') )
			{
				printf("\n");
				break;
			}
			else if( (u8Option=='q') || (u8Option=='Q') )
			{
				printf("\n");
				return 0xFF;
			}
		}
	}
	return u8ActiveChannelCount;
}

