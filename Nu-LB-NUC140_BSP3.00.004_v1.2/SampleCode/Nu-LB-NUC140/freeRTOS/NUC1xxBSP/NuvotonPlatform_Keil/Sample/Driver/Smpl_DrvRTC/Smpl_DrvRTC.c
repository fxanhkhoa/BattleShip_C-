/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include "NUC1xx.h"
#include "Driver/DrvUART.h"
#include "Driver/DrvRTC.h"
#include "Driver/DrvGPIO.h"
#include "Driver/DrvSYS.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile uint32_t  g_u32TICK = FALSE;
volatile int32_t   g_bAlarm  = FALSE;


/*---------------------------------------------------------------------------------------------------------*/
/* RTC Tick Callback function                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvRTC_TickISR(void)
{
	S_DRVRTC_TIME_DATA_T sCurTime;
	
	/* Get the currnet time */
	DrvRTC_Read(DRVRTC_CURRENT_TIME, &sCurTime);		
	
	printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);				
	
	g_u32TICK++;
}




/*---------------------------------------------------------------------------------------------------------*/
/* RTC Alarm Callback function                                                                             */
/*---------------------------------------------------------------------------------------------------------*/
void DrvRTC_AlarmISR(void)
{
	
	printf("Alarm!!\n");
	
	g_bAlarm = TRUE; 
}


static void TestItem (void)
{
    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|                    RTC Sample Program                     |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [0] Time Display Test                                     | \n");
    printf("| [1] Alarm Test                                            | \n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [2] Quit                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("Select key : \n");
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
/* RTC Test Sample 				                                                                           */
/* Test Item					                                                                           */
/*  1. Time Display Test																				   */	
/*	   Use RTC Tick interrupt to display time every one second.                      					   */
/*  2. Alarm Test																						   */
/*     Get the current and alarm after 10 seconds														   */	
/*---------------------------------------------------------------------------------------------------------*/
int32_t main()								   
{
	
	S_DRVRTC_TIME_DATA_T sInitTime;
	int32_t bLoop = TRUE;
	uint8_t u8Item;
	
	STR_UART_T sParam;
	
	UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
   /* Waiting for Xtal stalble */
    SysTimerDelay(5000);

	/* UART Setting */
    sParam.u32BaudRate = 115200;
    sParam.u8cDataBits = DRVUART_DATABITS_8;
    sParam.u8cStopBits = DRVUART_STOPBITS_1;
    sParam.u8cParity   = DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel = DRVUART_FIFO_1BYTES;

	/* Set UART Pin */
	DrvGPIO_InitFunction(FUNC_UART0);

	/* Set UART Configuration */
	if(DrvUART_Open(UART_PORT0,&sParam) == E_SUCCESS)
	{		
		/* RTC Initialize */
		DrvRTC_Init();	
		
		/* Time Setting */
		sInitTime.u32Year 		= 2009;
		sInitTime.u32cMonth 	= 1;	
		sInitTime.u32cDay 		= 19;
		sInitTime.u32cHour 		= 13;
		sInitTime.u32cMinute 	= 20;
		sInitTime.u32cSecond 	= 0;
		sInitTime.u32cDayOfWeek = DRVRTC_MONDAY;
		sInitTime.u8cClockDisplay = DRVRTC_CLOCK_24;			

		/* Initialization the RTC timer */
		if(DrvRTC_Open(&sInitTime) !=E_SUCCESS)
		{
			printf("RTC Open Fail!!\n");
			return FALSE;
		}		
		
		while(bLoop)
		{
			TestItem();
			u8Item = getchar();
			
			switch(u8Item)
			{
				case '0':
				{
					S_DRVRTC_TICK_T sTick;
					
					printf("\n0. RTC Time Display Test (Exit after 5 seconds)\n");
						
					/* Set Tick property */
					sTick.ucMode = DRVRTC_TICK_1_SEC;
					sTick.pfnTickCallBack = DrvRTC_TickISR;		
					
					/* Set Tick setting */
					DrvRTC_Ioctl(0,DRVRTC_IOC_SET_TICK_MODE, (uint32_t)&sTick,0);

					/* Enable RTC Tick Interrupt and install tick call back function */
					DrvRTC_Ioctl(0,DRVRTC_IOC_ENABLE_INT, (uint32_t)DRVRTC_TICK_INT,0);	
					g_u32TICK = 0;
				
					NVIC_EnableIRQ(RTC_IRQn); 
					while(g_u32TICK < 5);

					/* Disable RTC Tick Interrupt */
					DrvRTC_Ioctl(0,DRVRTC_IOC_DISABLE_INT, (uint32_t)DRVRTC_TICK_INT,0);							
					break;
				}
				case '1':
				{
					S_DRVRTC_TIME_DATA_T sCurTime;	
					
					printf("\n1. DrvRTC Alarm Test (Alarm after 10 seconds)\n");
						
					g_bAlarm = FALSE;	
				
					/* Get the currnet time */
					DrvRTC_Read(DRVRTC_CURRENT_TIME, &sCurTime);
				

					/* Set Alarm call back function */
					sCurTime.pfnAlarmCallBack = DrvRTC_AlarmISR;

					printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
					
					/* The alarm time setting */	
					sCurTime.u32cSecond = sCurTime.u32cSecond + 10; 	
					
					/* Set the alarm time (Install the call back function and enable the alarm interrupt)*/
					DrvRTC_Write(DRVRTC_ALARM_TIME,&sCurTime);
							
					NVIC_EnableIRQ(RTC_IRQn); 
					
					while(!g_bAlarm);
					
					printf("Current Time:%d/%02d/%02d %02d:%02d:%02d\n",sCurTime.u32Year,sCurTime.u32cMonth,sCurTime.u32cDay,sCurTime.u32cHour,sCurTime.u32cMinute,sCurTime.u32cSecond);
								
					break;				
				}
				case '2':
					bLoop = FALSE;
					break;
				default:
					printf("Wrong Item\n");
					break;			
			}
			
		}
		
		/* Disable RTC Clock */		
		DrvRTC_Close();
		
		/* Disable UART Clock */
		DrvUART_Close(UART_PORT0);
		
		return TRUE;
	}
	else
		return FALSE;

}



	 
