/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------------------------------------*/
/* Include related headers                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#include "NUC1xx.h"
#include "core_cm0.h"
#include "DrvADC.h"
#include "DrvSYS.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static DRVADC_ADC_CALLBACK *g_ptADCCallBack;
static DRVADC_ADCMP0_CALLBACK *g_ptADCMP0CallBack;
static DRVADC_ADCMP1_CALLBACK *g_ptADCMP1CallBack;
static uint32_t g_pu32UserData[3];

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Open                                                                                              */
/*                                                                                                                    */
/* Parameters:        	                                                                                              */
/*   InputMode           [in]:  Specify the type of the analog input signal.                                          */
/*                              It might be single-end or differential input.                                         */
/*   OpMode              [in]:  Specify the operation mode.                                                           */
/*                              It might be single, single cycle scan or continuous scan mode.                        */
/*   u8ChannelSelBitwise [in]:  Specify the input channels.                                                           */
/*                              For example, "u8ChannelSelBitwise=0x1(00000001b)" means the ADC channel 0 is enabled; */
/*                              "u8ChannelSelBitwise=0x19(00011001b)" means the ADC channel 0, 3 and 4 are enabled.   */
/*   ClockSrc            [in]:  Specify the clock source of ADC clock.                                                */
/*   u8AdcDivisor        [in]:  Determine the ADC clock frequency.                                                    */
/*                              ADC clock frequency = ADC clock source frequency / ( u8AdcDivisor + 1 )               */
/*                                                                                                                    */
/* Returns:                                                                                                           */
/*   None.                                                                                                            */
/*                                                                                                                    */
/* Description:                                                                                                       */
/*   Enable the ADC function. Configure the corresponding pins of the specified channels as analog input pins.        */
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvADC_Open(ADC_INPUT_MODE InputMode, ADC_OPERATION_MODE OpMode, uint8_t u8ChannelSelBitwise, ADC_CLK_SRC ClockSrc, uint8_t u8AdcDivisor)
{
	/* reset ADC */
	SYS->IPRSTC2.ADC_RST = 1;
	SYS->IPRSTC2.ADC_RST = 0;
	
	/* ADC clock source */
	SYSCLK->CLKSEL1.ADC_S = ClockSrc;
	
	/* Set ADC divisor */
	SYSCLK->CLKDIV.ADC_N = u8AdcDivisor;
	/* ADC engine clock enable */
	SYSCLK->APBCLK.ADC_EN = 1;
	/* ADC enable */
	ADC->ADCR.ADEN = 1;
	
	if( InputMode == ADC_SINGLE_END )
		ADC->ADCR.DIFF = 0;     /* single end input */
	else
		ADC->ADCR.DIFF = 1;     /* differential input */
	
	if( OpMode == ADC_SINGLE_OP )
		ADC->ADCR.ADMD = 0;     /* single mode */
	else if( OpMode == ADC_SINGLE_CYCLE_OP )
		ADC->ADCR.ADMD = 2;     /* single-cycle scan mode */
	else
		ADC->ADCR.ADMD = 3;     /* continuous scan mode */
	
	ADC->ADCHER.CHEN = u8ChannelSelBitwise;
	if(u8ChannelSelBitwise&1)
	{
		SYS->GPAMFP.ADC0 = 1;   /* configure GPA0 as analog input pin */
		GPIOA->PMD.PMD0 = 0;    /* configure GPA0 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC0 = 0;   /* configure GPA0 as GPIO */
	}
	
	if(u8ChannelSelBitwise&2)
	{
		SYS->GPAMFP.ADC1 = 1;   /* configure GPA1 as analog input pin */
		GPIOA->PMD.PMD1 = 0;    /* configure GPA1 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC1 = 0;   /* configure GPA1 as GPIO */
	}
	
	if(u8ChannelSelBitwise&4)
	{
		SYS->GPAMFP.ADC2 = 1;   /* configure GPA2 as analog input pin */
		GPIOA->PMD.PMD2 = 0;    /* configure GPA2 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC2 = 0;   /* configure GPA2 as GPIO */
	}
	
	if(u8ChannelSelBitwise&8)
	{
		SYS->GPAMFP.ADC3 = 1;   /* configure GPA3 as analog input pin */
		GPIOA->PMD.PMD3 = 0;    /* configure GPA3 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC3 = 0;   /* configure GPA3 as GPIO */
	}
	
	if(u8ChannelSelBitwise&0x10)
	{
		SYS->GPAMFP.ADC4 = 1;   /* configure GPA4 as analog input pin */
		GPIOA->PMD.PMD4 = 0;    /* configure GPA4 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC4 = 0;   /* configure GPA4 as GPIO */
	}
	
	if(u8ChannelSelBitwise&0x20)
	{
		SYS->GPAMFP.ADC5 = 1;   /* configure GPA5 as analog input pin */
		GPIOA->PMD.PMD5 = 0;    /* configure GPA5 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC5 = 0;   /* configure GPA5 as GPIO */
	}
	
	if(u8ChannelSelBitwise&0x40)
	{
		SYS->GPAMFP.ADC6 = 1;   /* configure GPA6 as analog input pin */
		GPIOA->PMD.PMD6 = 0;    /* configure GPA6 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC6 = 0;   /* configure GPA6 as GPIO */
	}
	
	if(u8ChannelSelBitwise&0x80)
	{
		SYS->ALTMFP.PA7_S21 = 0;
		SYS->GPAMFP.ADC7_SS21 = 1;   /* configure GPA7 as analog input pin */
		GPIOA->PMD.PMD7 = 0;         /* configure GPA7 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC7_SS21 = 0;   /* configure GPA7 as GPIO */
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Close                                                                                  */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC function, ADC engine clock and ADC interrupt.                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_Close(void)
{     
	ADC->ADCR.ADEN = 0;                /* ADC disable */
	SYSCLK->APBCLK.ADC_EN = 0;         /* ADC engine clock disable */
	ADC->ADCR.ADIE = 0;
	NVIC_DisableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetAdcChannel                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    u8ChannelSelBitwise [in]:  Specify the analog input channels.                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Select ADC input channels.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetAdcChannel(uint8_t u8ChannelSelBitwise, ADC_INPUT_MODE InputMode)
{     
	ADC->ADCHER.CHEN = u8ChannelSelBitwise;
	if( (u8ChannelSelBitwise&1) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&2)) )
	{
		SYS->GPAMFP.ADC0 = 1;   /* configure GPA0 as analog input pin */
		GPIOA->PMD.PMD0 = 0;    /* configure GPA0 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC0 = 0;   /* configure GPA0 as GPIO */
	}
	
	if(u8ChannelSelBitwise&2)
	{
		SYS->GPAMFP.ADC1 = 1;   /* configure GPA1 as analog input pin */
		GPIOA->PMD.PMD1 = 0;    /* configure GPA1 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC1 = 0;   /* configure GPA1 as GPIO */
	}
	
	if( (u8ChannelSelBitwise&4) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&8)) )
	{
		SYS->GPAMFP.ADC2 = 1;   /* configure GPA2 as analog input pin */
		GPIOA->PMD.PMD2 = 0;    /* configure GPA2 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC2 = 0;   /* configure GPA2 as GPIO */
	}
	
	if(u8ChannelSelBitwise&8)
	{
		SYS->GPAMFP.ADC3 = 1;   /* configure GPA3 as analog input pin */
		GPIOA->PMD.PMD3 = 0;    /* configure GPA3 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC3 = 0;   /* configure GPA3 as GPIO */
	}
	
	if( (u8ChannelSelBitwise&0x10) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&0x20)) )
	{
		SYS->GPAMFP.ADC4 = 1;   /* configure GPA4 as analog input pin */
		GPIOA->PMD.PMD4 = 0;    /* configure GPA4 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC4 = 0;   /* configure GPA4 as GPIO */
	}
	
	if(u8ChannelSelBitwise&0x20)
	{
		SYS->GPAMFP.ADC5 = 1;   /* configure GPA5 as analog input pin */
		GPIOA->PMD.PMD5 = 0;    /* configure GPA5 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC5 = 0;   /* configure GPA5 as GPIO */
	}
	
	if( (u8ChannelSelBitwise&0x40) || ((InputMode==ADC_DIFFERENTIAL)&&(u8ChannelSelBitwise&0x80)) )
	{
		SYS->GPAMFP.ADC6 = 1;   /* configure GPA6 as analog input pin */
		GPIOA->PMD.PMD6 = 0;    /* configure GPA6 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC6 = 0;   /* configure GPA6 as GPIO */
	}
	
	if(u8ChannelSelBitwise&0x80)
	{
		SYS->ALTMFP.PA7_S21 = 0;
		SYS->GPAMFP.ADC7_SS21 = 1;   /* configure GPA7 as analog input pin */
		GPIOA->PMD.PMD7 = 0;         /* configure GPA7 as input mode */
	}
	else
	{
		SYS->GPAMFP.ADC7_SS21 = 0;   /* configure GPA7 as GPIO */
	}
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_ConfigAdcChannel7                                                                                 */
/*                                                                                                                    */
/* Parameters:        	                                                                                              */
/*   Ch7Src [in]:  Specify the analog input source.                                                                   */
/*                                                                                                                    */
/* Returns:                                                                                                           */
/*   None.                                                                                                            */
/*                                                                                                                    */
/* Description:                                                                                                       */
/*   Select the input signal source of channel 7.                                                                     */
/*   ADC channel 7 is a multi-function channel.                                                                       */
/*   Its input source could come from a external signal, a internal bandgap voltage or a internal temperature sensor. */
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvADC_ConfigAdcChannel7(ADC_CH7_SRC Ch7Src)
{
	ADC->ADCHER.PRESEL = Ch7Src;
	if(Ch7Src==INT_TEMPERATURE_SENSOR)
		SYS->TEMPCR = 1;   /* Temperature sensor enable */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetAdcInputMode                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    InputMode [in]:  Specify the input mode.                                                             */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADC input mode.                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetAdcInputMode(ADC_INPUT_MODE InputMode)
{
 	if( InputMode == ADC_SINGLE_END )
		ADC->ADCR.DIFF = 0;     /* single end input */
	else
		ADC->ADCR.DIFF = 1;     /* differential input */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetAdcOperationMode                                                                    */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    OpMode [in]:  Specify the operation mode.                                                            */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADC operation mode.                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetAdcOperationMode(ADC_OPERATION_MODE OpMode)
{
 	if( OpMode == ADC_SINGLE_OP )
		ADC->ADCR.ADMD = 0;         /* single mode */
	else if( OpMode == ADC_SINGLE_CYCLE_OP )
		ADC->ADCR.ADMD = 2;         /* single-cycle scan mode */
	else
		ADC->ADCR.ADMD = 3;         /* continuous scan mode */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetAdcClkSrc                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    ClockSrc [in]:  Specify the ADC clock source.                                                        */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADC clock source.                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetAdcClkSrc(ADC_CLK_SRC ClockSrc)
{
 	SYSCLK->CLKSEL1.ADC_S = ClockSrc; /* ADC clock source */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SetAdcDivisor                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    u8AdcDivisor [in]:  Specify the divisor value.                                                       */
/*                        ADC clock frequency = ADC clock source frequency / ( u8AdcDivisor + 1 )          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the divisor value of ADC clock.                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_SetAdcDivisor(uint8_t u8AdcDivisor)
{
 	SYSCLK->CLKDIV.ADC_N = u8AdcDivisor;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableAdcInt                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    Callback    [in]:  The callback function of ADC interrupt.                                           */
/*    u32UserData [in]:  The user's data to pass to the callback function.                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the ADC interrupt and setup callback function.                                                */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableAdcInt(DRVADC_ADC_CALLBACK Callback, uint32_t u32UserData)
{
	g_ptADCCallBack = Callback;
	g_pu32UserData[0] = u32UserData;
	ADC->ADSR.ADF =1;     /* clear the A/D interrupt flags for safe */
	ADC->ADCR.ADIE = 1;
	NVIC_EnableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableAdcInt                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC interrupt.                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableAdcInt(void)
{
	ADC->ADCR.ADIE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableAdcmp0Int                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    Callback    [in]:  The callback function of ADC result monitor 0 (compare 0) interrupt.              */
/*    u32UserData [in]:  The user's data to pass to the callback function.                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the ADC compare 0 interrupt and setup callback function.                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableAdcmp0Int(DRVADC_ADCMP0_CALLBACK Callback, uint32_t u32UserData)
{
	g_ptADCMP0CallBack = Callback;
	g_pu32UserData[1] = u32UserData;
	ADC->ADSR.CMPF0 = 1;        /* clear the A/D interrupt flags for safe */
	ADC->ADCMPR[0].CMPIE = 1;
	NVIC_EnableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableAdcmp0Int                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC compare 0 interrupt.                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableAdcmp0Int(void)
{
	ADC->ADCMPR[0].CMPIE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_EnableAdcmp1Int                                                                        */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    Callback    [in]:  The callback function of ADC result monitor 1 (compare 1) interrupt.              */
/*    u32UserData [in]:  The user's data to pass to the callback function.                                 */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Enable the ADC compare 1 interrupt and setup callback function.                                      */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_EnableAdcmp1Int(DRVADC_ADCMP1_CALLBACK Callback, uint32_t u32UserData)
{
	g_ptADCMP1CallBack = Callback;
	g_pu32UserData[2] = u32UserData;
	ADC->ADSR.CMPF1 = 1;        /* clear the A/D interrupt flags for safe */
	ADC->ADCMPR[1].CMPIE = 1;
	NVIC_EnableIRQ(ADC_IRQn);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_DisableAdcmp1Int                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Disable the ADC compare 1 interrupt.                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_DisableAdcmp1Int(void)
{
	ADC->ADCMPR[1].CMPIE = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: ADC_IRQHandler                                                                                */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    ADC Interrupt Service Routine.                                                                       */
/*    According to the interrupt flags to call the corresponding callback function.                        */
/*---------------------------------------------------------------------------------------------------------*/
void ADC_IRQHandler(void)
{
	if(ADC->ADSR.ADF==1)
	{
		if(g_ptADCCallBack)
			g_ptADCCallBack(g_pu32UserData[0]);
		
		/* clear the A/D conversion flag */
		/* "ADC->ADSR.ADF = 1;" is not recommended. It may clear CMPF0 and CMPF1. */
		outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1);
	}
	
	if(ADC->ADSR.CMPF0==1)
	{
		if(g_ptADCMP0CallBack)
			g_ptADCMP0CallBack(g_pu32UserData[1]);
		
		/* clear the A/D compare flag 0 */
		/* "ADC->ADSR.CMPF0 = 1;" is not recommended. It may clear ADF and CMPF1. */
		outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x2);
	}
	
	if(ADC->ADSR.CMPF1==1)
	{
		if(g_ptADCMP1CallBack)
			g_ptADCMP1CallBack(g_pu32UserData[2]);
		
		/* clear the A/D compare flag 1 */
		/* "ADC->ADSR.CMPF1 = 1;" is not recommended. It may clear ADF and CMPF0. */
		outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x4);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_GetConversionRate                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    Conversion rate.                                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Return the A/D conversion rate (sample/second.)                                                      */
/*    The conversion rate depends on the clock source of ADC clock.                                        */
/*    It needs 20 ADC clocks to complete an A/D conversion.                                                */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_GetConversionRate(void)
{
	if( SYSCLK->CLKSEL1.ADC_S == 0 )           /* external 12MHz crystal clock */
	{
		return (12000000/(SYSCLK->CLKDIV.ADC_N + 1)/20);
	}
	else if( SYSCLK->CLKSEL1.ADC_S == 1 )      /* PLL clock */
	{
		return ( DrvSYS_GetPLLClock()*1000/(SYSCLK->CLKDIV.ADC_N + 1)/20);
	}
	else                                       /* internal 22MHz RC oscillator clock */
	{
		return (22000000/(SYSCLK->CLKDIV.ADC_N + 1)/20);
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_ExtTriggerEnable                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    TriggerCondition [in]:  Specify the trigger condition.                                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Allow the external trigger pin STADC (PB8) to be the trigger source of ADC.                          */
/*    The trigger condition could be low-level/high-level/falling-edge/positive-edge.                      */
/*    TRGE bit and ADST bit should be clear to 0 before changing TRGS bit.                                 */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_ExtTriggerEnable(ADC_EXT_TRI_COND TriggerCondition)
{
	GPIOB->PMD.PMD8 = 0;                  /* Configure GPB8 as an input pin */
	ADC->ADCR.ADST = 0;                   /* Stop A/D conversion */
	ADC->ADCR.TRGEN = 0;                  /* Disable the trigger source of STADC pin */
	ADC->ADCR.TRGS = 0;                   /* Select the STADC pin as the hardware trigger source */
	ADC->ADCR.TRGCOND = TriggerCondition; /* Set the trigger condition */
	ADC->ADCR.TRGEN = 1;                  /* Enable the trigger source of STADC pin */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_ExtTriggerDisable                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Prohibit the external ADC trigger.                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_ExtTriggerDisable(void)
{
	ADC->ADCR.TRGEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_StartConvert                                                                           */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Set the ADST bit of ADCR to start the A/D conversion action.                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_StartConvert(void)
{
	/* clear the A/D conversion interrupt flag (ADF) */
	/* "ADC->ADSR.ADF = 1;" is not recommended. It may clear CMPF0 and CMPF1 */
	outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1);
	ADC->ADCR.ADST = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_StopConvert                                                                            */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    None.                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Clear the ADST bit of ADCR to stop the A/D conversion action.                                        */
/*---------------------------------------------------------------------------------------------------------*/
void DrvADC_StopConvert(void)
{
	ADC->ADCR.ADST = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsConversionDone                                                                       */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE:  the conversion action is finished.                                                             */
/*    FALSE: the conversion action is in progress.                                                         */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the conversion action is finished or not.                                              */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsConversionDone(void)
{
	if(ADC->ADSR.ADF==0)
		return FALSE;
	else
		return TRUE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_GetConversionData                                                                      */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    u8ChannelNum [in]:  Specify the ADC channel.                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    12-bit conversion result. If the channel number is out of range, returns E_DRVADC_CHANNELNUM.        */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Get the conversion result of the specified ADC channel.                                              */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_GetConversionData(uint8_t u8ChannelNum)
{
	if(u8ChannelNum>7)
		return E_DRVADC_CHANNELNUM;
	return (ADC->ADDR[u8ChannelNum].RSLT);
}

/*------------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_PdmaEnable                                                                                */
/*                                                                                                            */
/* Parameters:        	                                                                                      */
/*    None.                                                                                                   */
/*                                                                                                            */
/* Returns:                                                                                                   */
/*    None.                                                                                                   */
/*                                                                                                            */
/* Description:                                                                                               */
/*    Enable the PDMA data transfer function. When PDMA transfer is enabled, the ADIE bit must be set to '0'. */
/*------------------------------------------------------------------------------------------------------------*/
void DrvADC_PdmaEnable(void)
{
	ADC->ADCR.ADIE = 0;
	ADC->ADCR.PTEN = 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_PdmaDisable                                                                              */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the PDMA data transfer function.                                                               */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_PdmaDisable(void)
{
	ADC->ADCR.PTEN = 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsDataValid                                                                            */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelNum [in]:  Specify the ADC channel.                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE: valid.                                                                                         */
/*    FALSE: invalid.                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the conversion data is valid or not.                                                   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsDataValid(uint8_t u8ChannelNum)
{
	volatile uint8_t u8ChannelSelBitwise[8]={ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
	
	if(u8ChannelNum>7)
		return FALSE;
	if( ADC->ADSR.VALID & u8ChannelSelBitwise[u8ChannelNum] )
		return TRUE;
	else
		return FALSE;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsDataOverrun                                                                          */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*   u8ChannelNum [in]:  Specify the ADC channel.                                                          */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE: overrun.                                                                                       */
/*    FALSE: non-overrun.                                                                                  */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the conversion data is overrun or not.                                                 */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsDataOverrun(uint8_t u8ChannelNum)
{
	volatile uint8_t u8ChannelSelBitwise[8]={ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
	
	if(u8ChannelNum>7)
		return FALSE;
	if( ADC->ADSR.OVERRUN & u8ChannelSelBitwise[u8ChannelNum] )
		return TRUE;
	else
		return FALSE;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Adcmp0Enable                                                                             */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    u8CmpChannelNum [in]:  Specify the channel number that want to compare.                                */
/*    CmpCondition    [in]:  Specify the compare condition.                                                  */
/*                           LESS_THAN for the condition of "less than the compare data";                    */
/*                           GREATER_OR_EQUAL for the condition of "greater or equal to the compare data."   */
/*    u16CmpData      [in]:  Specify the compare data.                                                       */
/*    u8CmpMatchCount [in]:  Specify the compare match count.                                                */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    E_DRVADC_ARGUMENT: one of the input arguments is out of the range.                                     */
/*    E_SUCCESS: the compare function is enabled.                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Enable the ADC result monitor 0 and configure the necessary settings.                                  */
/*-----------------------------------------------------------------------------------------------------------*/
ERRCODE DrvADC_Adcmp0Enable(uint8_t u8CmpChannelNum, ADC_COMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t u8CmpMatchCount)
{
	if(u8CmpChannelNum>7)
		return E_DRVADC_ARGUMENT;
	
	if(u16CmpData>0x0FFF)
		return E_DRVADC_ARGUMENT;
	
	if(u8CmpMatchCount>0x0F)
		return E_DRVADC_ARGUMENT;
	
	ADC->ADCMPR[0].CMPCH = u8CmpChannelNum;
	ADC->ADCMPR[0].CMPCOND = CmpCondition;
	ADC->ADCMPR[0].CMPD = u16CmpData;
	ADC->ADCMPR[0].CMPMATCNT = u8CmpMatchCount;
	ADC->ADCMPR[0].CMPEN = 1;
	
	return E_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Adcmp0Disable                                                                            */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the ADC result monitor 0.                                                                      */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_Adcmp0Disable(void)
{
	ADC->ADCMPR[0].CMPEN = 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Adcmp1Enable                                                                             */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    u8CmpChannelNum [in]:  Specify the channel number that want to compare.                                */
/*    CmpCondition    [in]:  Specify the compare condition.                                                  */
/*                           LESS_THAN for the condition of "less than the compare data";                    */
/*                           GREATER_OR_EQUAL for the condition of "greater or equal to the compare data."   */
/*    u16CmpData      [in]:  Specify the compare data.                                                       */
/*    u8CmpMatchCount [in]:  Specify the compare match count.                                                */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    E_DRVADC_ARGUMENT: one of the input arguments is out of the range.                                     */
/*    E_SUCCESS: the compare function is enabled.                                                            */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Enable the ADC result monitor 1 and configure the necessary settings.                                  */
/*-----------------------------------------------------------------------------------------------------------*/
ERRCODE DrvADC_Adcmp1Enable(uint8_t u8CmpChannelNum, ADC_COMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t u8CmpMatchCount)
{
	if(u8CmpChannelNum>7)
		return E_DRVADC_ARGUMENT;
	
	if(u16CmpData>0x0FFF)
		return E_DRVADC_ARGUMENT;
	
	if(u8CmpMatchCount>0x0F)
		return E_DRVADC_ARGUMENT;
	
	ADC->ADCMPR[1].CMPCH = u8CmpChannelNum;
	ADC->ADCMPR[1].CMPCOND = CmpCondition;
	ADC->ADCMPR[1].CMPD = u16CmpData;
	ADC->ADCMPR[1].CMPMATCNT = u8CmpMatchCount;
	ADC->ADCMPR[1].CMPEN = 1;
	
	return E_SUCCESS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_Adcmp1Disable                                                                            */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the ADC result monitor 1.                                                                      */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_Adcmp1Disable(void)
{
	ADC->ADCMPR[1].CMPEN = 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SelfCalEnable                                                                            */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Enable the self calibration function.                                                                  */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_SelfCalEnable (void)
{
	ADC->ADCALR.CALEN = 1;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_IsCalDone                                                                              */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*    None.                                                                                                */
/*                                                                                                         */
/* Returns:                                                                                                */
/*    TRUE:  the self calibration action is finished.                                                      */
/*    FALSE: the self calibration action is in progress.                                                   */
/*                                                                                                         */
/* Description:                                                                                            */
/*    Check whether the self calibration action is finished or not.                                        */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_IsCalDone(void)
{
	if(ADC->ADCALR.CALDONE == 1)
		return TRUE;
	else
		return FALSE;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_SelfCalDisable                                                                           */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Disable the self calibration function.                                                                 */
/*-----------------------------------------------------------------------------------------------------------*/
void DrvADC_SelfCalDisable (void)
{
	ADC->ADCALR.CALEN = 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/* Function: DrvADC_GetVersion                                                                               */
/*                                                                                                           */
/* Parameters:        	                                                                                     */
/*    None.                                                                                                  */
/*                                                                                                           */
/* Returns:                                                                                                  */
/*    Version number.                                                                                        */
/*                                                                                                           */
/* Description:                                                                                              */
/*    Get the version number of NUC100 ADC driver.                                                           */
/*-----------------------------------------------------------------------------------------------------------*/
uint32_t DrvADC_GetVersion (void)
{
	return DRVADC_VERSION_NUM;
}

