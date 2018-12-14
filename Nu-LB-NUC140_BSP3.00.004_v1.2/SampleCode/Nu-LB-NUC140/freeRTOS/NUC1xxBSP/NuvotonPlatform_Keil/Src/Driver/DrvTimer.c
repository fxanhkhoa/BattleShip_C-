/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
 
/*---------------------------------------------------------------------------------------------------------*/
/* Includes of system headers                                                                              */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "NUC1xx.h"
#include "DrvTimer.h"
#include "DrvSYS.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define TIMER_EVENT_COUNT   10
#define TICKS_PER_SECOND    100
#define TIMER_OFFSET        0x4

/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static TIMER_EVENT_T tTime0Event[TIMER_EVENT_COUNT],
              		 tTime1Event[TIMER_EVENT_COUNT],
			  		 tTime2Event[TIMER_EVENT_COUNT],
			  		 tTime3Event[TIMER_EVENT_COUNT];

static TIMER_CALLBACK fnCallBack_WDT;

static int32_t  bIsTimer0Initial     = FALSE,
               	bIsTimer1Initial     = FALSE,
			    bIsTimer2Initial     = FALSE,
			    bIsTimer3Initial     = FALSE,
                bIsTimer0Used      	 = FALSE,
                bIsTimer1Used      	 = FALSE,
				bIsTimer2Used      	 = FALSE,
				bIsTimer3Used      	 = FALSE,
                bIsSetTime0Event     = FALSE,
                bIsSetTime1Event     = FALSE,
			    bIsSetTime2Event     = FALSE,
			    bIsSetTime3Event     = FALSE;


static uint32_t volatile uTimer0Count = 0,
                		 uTimer1Count = 0,
                		 uTimer2Count = 0,
                		 uTimer3Count = 0,
						 uTime0EventCount = 0,
                		 uTime1EventCount = 0,
			    		 uTime2EventCount = 0,
                		 uTime3EventCount = 0;

static uint32_t volatile _sys_uTimer0TickPerSecond,
						 _sys_uTimer1TickPerSecond,
						 _sys_uTimer2TickPerSecond, 
						 _sys_uTimer3TickPerSecond;



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     GetTimerCLk                                                                       		   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             ch          		-[in] Timer Channel :Could be TMR0,TMR1,TMR2,TMR3                      */
/* Returns:                                                                                                */
/*               Current Timer Clock                                                                       */
/* Description:                                                                                            */
/*               The function is used to get Uart clock                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t GetTimerCLk(TIMER_CHANNEL ch)
{
	volatile uint32_t clk =0 ;
	
	
	if(ch == TMR0 )
	{
		if(SYSCLK->CLKSEL1.TMR0_S == 0)			
		{
			clk = DrvSYS_GetEXTClock() * 1000;	/* Option 0: Get External Clock From DrvSYS Setting */
		}
		else if(SYSCLK->CLKSEL1.TMR0_S == 1)
		{
			clk = 32000;						/* Option 1: 32K */
		}
		else if(SYSCLK->CLKSEL1.TMR0_S == 2)
		{
			clk = DrvSYS_GetPLLClock() * 1000 ;	/* Option 2: HCLK */
		}
		else if(SYSCLK->CLKSEL1.TMR0_S == 3)
		{
												/* Option 3: External Trigger */
		}
		else 
		{
			clk = 22000000; 					/* Option 4: 22MHz*/
		}

	}
	else if(ch == TMR1 )
	{
    	if(SYSCLK->CLKSEL1.TMR1_S == 0)			
		{
			clk = DrvSYS_GetEXTClock() * 1000;	/* Option 0: Get External Clock From DrvSYS Setting */
		}
		else if(SYSCLK->CLKSEL1.TMR1_S == 1)
		{
			clk = 32000;						/* Option 1: 32K */
		}
		else if(SYSCLK->CLKSEL1.TMR1_S == 2)
		{
			clk = DrvSYS_GetPLLClock() * 1000 ;	/* Option 2: HCLK */
		}
		else if(SYSCLK->CLKSEL1.TMR1_S == 3)
		{
												/* Option 3: External Trigger */
		}
		else 
		{
			clk = 22000000; 					/* Option 4: 22MHz*/
		}
	}
	else if(ch == TMR2 )
    {
	    if(SYSCLK->CLKSEL1.TMR2_S == 0)			
		{
			clk = DrvSYS_GetEXTClock() * 1000;	/* Option 0: Get External Clock From DrvSYS Setting */
		}
		else if(SYSCLK->CLKSEL1.TMR2_S == 1)
		{
			clk = 32000;						/* Option 1: 32K */
		}
		else if(SYSCLK->CLKSEL1.TMR2_S == 2)
		{
			clk = DrvSYS_GetPLLClock() * 1000 ;	/* Option 2: HCLK */
		}
		else if(SYSCLK->CLKSEL1.TMR2_S == 3)
		{
												/* Option 3: External Trigger */
		}
		else 
		{
			clk = 22000000; 					/* Option 4: 22MHz*/
		}
	
	}
	else if(ch == TMR3 )
    {
	    if(SYSCLK->CLKSEL1.TMR3_S == 0)			
		{
			clk = DrvSYS_GetEXTClock() * 1000;	/* Option 0: Get External Clock From DrvSYS Setting */
		}
		else if(SYSCLK->CLKSEL1.TMR3_S == 1)
		{
			clk = 32000;						/* Option 1: 32K */
		}
		else if(SYSCLK->CLKSEL1.TMR3_S == 2)
		{
			clk = DrvSYS_GetPLLClock() * 1000 ;	/* Option 2: HCLK */
		}
		else if(SYSCLK->CLKSEL1.TMR3_S == 3)
		{
												/* Option 3: External Trigger */
		}
		else 
		{
			clk = 22000000; 					/* Option 4: 22MHz*/
		}
	}
	else 
    	return E_DRVTIMER_CHANNEL;
			

	return clk;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_GetStatus                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      Register TISR data                                                                        */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               This function is used to return read TIMER TISR register to get timer interrupt status    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_GetStatus(TIMER_CHANNEL ch)
{
	int32_t IsStatus;

	if(ch == TMR0 )
		IsStatus =  TIMER0->TISR.TIF ;
	else if(ch == TMR1 )
    	IsStatus =  TIMER1->TISR.TIF ;
	else if(ch == TMR2 )
    	IsStatus =  TIMER2->TISR.TIF ;
	else if(ch == TMR3 )
    	IsStatus =  TIMER3->TISR.TIF ;
	else 
    	return E_DRVTIMER_CHANNEL;

	return IsStatus;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER0 ISR                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/* Returns:      NONE                                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The TIMER0 default ISR                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void TMR0_IRQHandler(void)
{
    int32_t i;
   
    uTimer0Count++;

 	TIMER0->TISR.TIF =1;
 
 	if (bIsSetTime0Event)                               /* Timer Event Handle                               */
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime0Event[i].active)
            {
                tTime0Event[i].curTick--;
                if (tTime0Event[i].curTick == 0)
                {
                    (*tTime0Event[i].funPtr)(tTime0Event[i].transParam);
                    tTime0Event[i].curTick = tTime0Event[i].initTick;
                }
            }
        }
    }
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     Timer1_ISR                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      NONE                                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The TIMER1 default ISR                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void)
{
    int32_t i;
    
    uTimer1Count++;

  	TIMER1->TISR.TIF =1;
    if (bIsSetTime1Event)                           /* Timer Event Handle                                  */
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime1Event[i].active)
            {
                tTime1Event[i].curTick--;
                if (tTime1Event[i].curTick == 0)
                {
                    (*tTime1Event[i].funPtr)(tTime1Event[i].transParam);
                    tTime1Event[i].curTick = tTime1Event[i].initTick;
                }
            }
        }
    }
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     Timer2_ISR                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      NONE                                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The TIMER2 default ISR                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void TMR2_IRQHandler(void)
{

    int32_t i;
    
    uTimer2Count++;

	TIMER2->TISR.TIF =1;
    if (bIsSetTime2Event)                           /* Timer Event Handle                                  */
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime2Event[i].active)
            {
                tTime2Event[i].curTick--;
                if (tTime2Event[i].curTick == 0)
                {
                    (*tTime2Event[i].funPtr)(tTime2Event[i].transParam);
                    tTime2Event[i].curTick = tTime2Event[i].initTick;
                }
            }
        }
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     Timer3_ISR                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      NONE                                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The TIMER3 default ISR                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void TMR3_IRQHandler(void)
{

    int32_t i;
    
    uTimer3Count++;

	TIMER3->TISR.TIF =1;
    if (bIsSetTime3Event)                           /* Timer Event Handle                                  */
    {
        for (i=0; i<TIMER_EVENT_COUNT; i++)
        {
            if (tTime3Event[i].active)
            {
                tTime3Event[i].curTick--;
                if (tTime3Event[i].curTick == 0)
                {
                    (*tTime3Event[i].funPtr)(tTime3Event[i].transParam);
                    tTime3Event[i].curTick = tTime3Event[i].initTick;
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_SetTimerEvent                                                                       */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel    - [in],  TMR0/TMR1/TMR2/TMR3                          		                   */
/*               uTimeTick  - [in],  The tick value which want to execute event                            */
/*               pvFun      - [in],  The event function pointer                                            */
/*               parameter  - [in],  a parameter,was defined by user,which send to callback function       */
/*                                                                                                         */
/* Returns:      val        - [out],  The event number which contains this event                           */
/* Description:                                                                                            */
/*               This function is used to install a timer event				                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_SetTimerEvent(TIMER_CHANNEL channel, uint32_t uTimeTick, TIMER_CALLBACK pvFun ,uint32_t parameter)
{
    int32_t volatile i;
    int32_t val=0;


    switch (channel)
    {
        case TMR0:
        {
            bIsSetTime0Event = TRUE;
            uTime0EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime0Event[i].active  == FALSE)
                {
                    tTime0Event[i].active   	= TRUE;
                    tTime0Event[i].initTick 	= uTimeTick;
                    tTime0Event[i].curTick  	= uTimeTick;
                    tTime0Event[i].funPtr   	= (TIMER_CALLBACK)pvFun;
                    tTime0Event[i].transParam   = parameter;
                    val = i;
                    break;
                }
            }
            break;
        }
        case TMR1:
        {
            bIsSetTime1Event = TRUE;
            uTime1EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime1Event[i].active   == FALSE)
                {
                    tTime1Event[i].active   	= TRUE;
                    tTime1Event[i].initTick 	= uTimeTick;
                    tTime1Event[i].curTick  	= uTimeTick;
                    tTime1Event[i].funPtr   	= (TIMER_CALLBACK)pvFun;
                    tTime1Event[i].transParam   = parameter;
                    val = i;
                    break;
                }
            }
            break;
        }

        case TMR2:
        {
            bIsSetTime2Event = TRUE;
            uTime2EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime2Event[i].active   == FALSE)
                {
                    tTime2Event[i].active   	= TRUE;
                    tTime2Event[i].initTick 	= uTimeTick;
                    tTime2Event[i].curTick  	= uTimeTick;
                    tTime2Event[i].funPtr   	= (TIMER_CALLBACK)pvFun;
                    tTime2Event[i].transParam   = parameter;
                    val = i;
                    break;
                }
            }
            break;
        }

        case TMR3:
        {
            bIsSetTime3Event = TRUE;
            uTime3EventCount++;
            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                if (tTime3Event[i].active   == FALSE)
                {
                    tTime3Event[i].active   	= TRUE;
                    tTime3Event[i].initTick 	= uTimeTick;
                    tTime3Event[i].curTick  	= uTimeTick;
                    tTime3Event[i].funPtr   	= (TIMER_CALLBACK)pvFun;
                    tTime3Event[i].transParam   = parameter;
                    val = i;
                    break;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return val;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_ClearTimerEvent                                                                     */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel    - [in],  TMR0/TMR1/TMR2/TMR3  					                               */
/*               uTimeEventNo - [in],EVENT No. it could be 0 ~ TIMER_EVENT_COUNT-1                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to remove a installed event                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_ClearTimerEvent(TIMER_CHANNEL channel, uint32_t uTimeEventNo)
{
    switch (channel)
    {
        case TMR0:
        {
            tTime0Event[uTimeEventNo].active = FALSE;
            uTime0EventCount--;
            if (uTime0EventCount == 0)
            {
                bIsSetTime0Event = FALSE;
            }
            break;
        }
        case TMR1:
        {
            tTime1Event[uTimeEventNo].active = FALSE;
            uTime1EventCount--;
            if (uTime1EventCount == 0)
            {
                bIsSetTime1Event = FALSE;
            }
            break;
        }

        case TMR2:
        {
            tTime2Event[uTimeEventNo].active = FALSE;
            uTime2EventCount--;
            if (uTime2EventCount == 0)
            {
                bIsSetTime2Event = FALSE;
            }
            break;
        }

        case TMR3:
        {
            tTime3Event[uTimeEventNo].active = FALSE;
            uTime3EventCount--;
            if (uTime3EventCount == 0)
            {
                bIsSetTime3Event = FALSE;
            }
            break;
        }
        default:
        {
            break;
        }
    }
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvTIMER_EnableInt                                                                        */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel    - [in],  TMR0/TMR1/TMR2/TMR3                                                   */
/*                                                                                                         */
/* Returns:      E_SUCCESS / TIMER_ERR_CHANNEL /                                                           */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               This function is used to enable Interuppt                                                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_EnableInt(TIMER_CHANNEL channel)
{
    switch (channel)
    {
        case TMR0:
        {
            TIMER0->TCSR.IE =1 ;
			NVIC_EnableIRQ(TMR0_IRQn); 
            break;
        }
        case TMR1:
        {
            TIMER1->TCSR.IE =1 ;
			NVIC_EnableIRQ(TMR1_IRQn); 
            break;
        }
        case TMR2:
        {
            TIMER2->TCSR.IE =1 ;
			NVIC_EnableIRQ(TMR2_IRQn); 
            break;
        }

        case TMR3:
        {
            TIMER3->TCSR.IE =1 ;
			NVIC_EnableIRQ(TMR3_IRQn); 
            break;
        }

        default:
        {
			return E_DRVTIMER_CHANNEL;
        }
    }
    
    return E_SUCCESS ;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_ResetTicks                                                                          */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel    - [in],  TMR0/TMR1/TMR2/TMR3                                                   */
/*                                                                                                         */
/* Returns:      E_SUCCESS / E_DRVTIMER_CHANNEL                                                            */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               This function is used to reset TIMER Tick.                                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_ResetTicks(TIMER_CHANNEL channel)
{
    switch (channel)
    {
        case TMR0:
        {
            uTimer0Count = 0;
            break;
        }
        case TMR1:
        {
            uTimer1Count = 0;
            break;
        }

        case TMR2:
        {
            uTimer2Count = 0;
            break;
        }

        case TMR3:
        {
            uTimer3Count = 0;
            break;
        }
        default:
        {
            return E_DRVTIMER_CHANNEL;
        }
    }
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_Init                                                                                */
/*                                                                                                         */
/* Parameters:   NONE                                                                                      */
/*                                                                                                         */
/* Returns:      NONE                                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               This function is used to initial TIMER when system boot up.                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_Init(void)
{

    bIsTimer0Initial    = TRUE;
    bIsTimer1Initial    = TRUE;
	bIsTimer2Initial    = TRUE;
	bIsTimer3Initial    = TRUE;

}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     CalTimerInitValue                                                                         */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32ClockSrc    - [in], The source clock of Timer.                                         */
/*               u32TicksPerSec - [in], The ticks per second.                                              */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               Return 32 bits unsigned integer where                                                     */
/*               bits [ 0:24] - The initial value for TCMPR.                                               */
/*               bits [25:32] - The value of pre scaler.                                                   */
/* Description:                                                                                            */
/*               This function is used to calculate the prescal and value of TCMPR(Initial counter value). */
/*---------------------------------------------------------------------------------------------------------*/
static uint32_t CalTimerInitValue(uint32_t u32ClockSrc, uint32_t u32TicksPerSec)
{
    uint32_t u32PreScale;
    uint32_t u32InitValue;

    if((u32ClockSrc < 2) || (u32TicksPerSec == 0))
        return (uint32_t)-1;

    for(u32PreScale = 1;u32PreScale < 256;u32PreScale++)
    {
        u32InitValue = u32ClockSrc / (u32TicksPerSec * u32PreScale);

        /* The initial value must > 1 */
        if(u32InitValue > 1)
            break;   
    
    }
    
    return (((u32PreScale - 1) << 24) | u32InitValue);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_Open                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel        - [in], TMR0/TMR1/TMR2/TMR3                                                */
/*               uTickPerSecond - [in], Tick / second                                                      */
/*               Mode           - [in], Operation Mode One-Shot / Periodic / Toggle /UNINTERREUPT          */
/*                                                                                                         */
/* Returns:      E_SUCCESS / E_DRVTIMER_CMD / E_DRVTIMER_EIO                                               */
/* Description:                                                                                            */
/*               This function is used to set and start TIMER                                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_Open(TIMER_CHANNEL channel, uint32_t uTicksPerSecond, TIMER_OPMODE mode )
{
    uint32_t i;
    uint32_t uRegTcmpr, uRegTcr;

    if ( (bIsTimer0Initial == FALSE)|(bIsTimer1Initial == FALSE)|(bIsTimer2Initial == FALSE)|(bIsTimer3Initial == FALSE))
    {
        return E_DRVTIMER_EIO;

    }

    uRegTcr = (uint32_t)( 0x60000000 | ( mode << 27) );        /* Start Timer and set operation mode       */
    
	

    switch (channel)
    {
        case TMR0:
        {
            if ( (bIsTimer0Initial != TRUE)|(bIsTimer0Used   != FALSE))
                return E_DRVTIMER_EIO;
            
            bIsTimer0Used   = TRUE;
           
		   	SYSCLK->APBCLK.TMR0_EN =1;
			
			outpw((uint32_t)&TIMER0->TCSR ,0 );                         /* disable timer                              */
           
		    TIMER0->TISR.TIF = 1;                             /* write 1 to clear for safty                 */

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime0Event[i].active = FALSE;
            }
            uTimer0Count = 0;
			
			_sys_uTimer0TickPerSecond = uTicksPerSecond;
            		
            uRegTcmpr = CalTimerInitValue(GetTimerCLk(TMR0), uTicksPerSecond); 

			if(uRegTcmpr == 0xFFFFFFFF)		
			{
				return E_DRVTIMER_CMD;			
			}

			TIMER0->TCMPR = (uRegTcmpr << 8) >> 8;
            uRegTcr |= (uRegTcmpr >> 24);
		
			outpw((uint32_t)&TIMER0->TCSR,  (uint32_t)uRegTcr);   
			
			NVIC_EnableIRQ(TMR0_IRQn); 

            break;
        }

        case TMR1:
        {
            if ( (bIsTimer1Initial != TRUE)|(bIsTimer1Used   != FALSE))
            {
                return E_DRVTIMER_EIO;
            }

            bIsTimer1Used   = TRUE;

            SYSCLK->APBCLK.TMR1_EN =1;
			
			outpw((uint32_t)&TIMER1->TCSR ,0 );                       	/* disable timer                               */
            
			TIMER1->TISR.TIF =1;                            	/* write 1 to clear for safty                  */

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime1Event[i].active = FALSE;
            }
            uTimer1Count = 0;
            _sys_uTimer1TickPerSecond = uTicksPerSecond;

            uRegTcmpr = CalTimerInitValue(GetTimerCLk(TMR1), uTicksPerSecond); 

			if(uRegTcmpr == 0xFFFFFFFF)		
			{
				return E_DRVTIMER_CMD;			
			}

			TIMER1->TCMPR = (uRegTcmpr << 8) >> 8;
            uRegTcr |= (uRegTcmpr >> 24);
		
			outpw((uint32_t)&TIMER1->TCSR,  (uint32_t)uRegTcr);   

			NVIC_EnableIRQ(TMR1_IRQn); 

            break;
        }


        case TMR2:
        {
            if ( (bIsTimer2Initial != TRUE)|(bIsTimer2Used   != FALSE))
            {
                return E_DRVTIMER_EIO;
    
            }
            bIsTimer2Used   = TRUE;

            SYSCLK->APBCLK.TMR2_EN =1;
			
			outpw((uint32_t)&TIMER2->TCSR ,0 );                              	/* disable timer                               */
            TIMER2->TISR.TIF =1;                       		      	/* write 1 to clear for safty                  */


            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime2Event[i].active = FALSE;
            }
            uTimer2Count = 0;
            _sys_uTimer2TickPerSecond = uTicksPerSecond;

             uRegTcmpr = CalTimerInitValue(GetTimerCLk(TMR2), uTicksPerSecond); 

			if(uRegTcmpr == 0xFFFFFFFF)		
			{
				return E_DRVTIMER_CMD;			
			}

			TIMER2->TCMPR = (uRegTcmpr << 8) >> 8;
            uRegTcr |= (uRegTcmpr >> 24);
		
			outpw((uint32_t)&TIMER2->TCSR,  (uint32_t)uRegTcr);   

			NVIC_EnableIRQ(TMR2_IRQn); 

            break;
        }

		case TMR3:
        {
            if ( (bIsTimer3Initial != TRUE)|(bIsTimer3Used   != FALSE))
            {
                return E_DRVTIMER_EIO;
    
            }

            bIsTimer3Used   = TRUE;

            SYSCLK->APBCLK.TMR3_EN =1;

			outpw((uint32_t)&TIMER3->TCSR ,0 );                               /* disable timer                               */
            TIMER3->TISR.TIF =1;                                   	/* write 1 to clear for safty                  */

            for (i=0; i<TIMER_EVENT_COUNT; i++)
            {
                tTime3Event[i].active = FALSE;
            }
            uTimer3Count = 0;
            _sys_uTimer3TickPerSecond = uTicksPerSecond;
            
            uRegTcmpr = CalTimerInitValue(GetTimerCLk(TMR3), uTicksPerSecond); 

			if(uRegTcmpr == 0xFFFFFFFF)		
			{
				return E_DRVTIMER_CMD;			
			}

			TIMER3->TCMPR = (uRegTcmpr << 8) >> 8;
            uRegTcr |= (uRegTcmpr >> 24);
		
			outpw((uint32_t)&TIMER3->TCSR,  (uint32_t)uRegTcr);   

			NVIC_EnableIRQ(TMR3_IRQn); 

            break;
        }

        default:
        {
            return E_DRVTIMER_CMD ;
 
        }
    }
    return E_SUCCESS;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_GetTicks                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel      - [in], TMR0/TMR1/TMR2/TMR3                                                  */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               uTimer0Count - [out], Timer0 Current ticks                                                */
/*               uTimer1Count - [out], Timer1 Current ticks                                                */
/*               uTimer2Count - [out], Timer2 Current ticks                                                */
/*               uTimer3Count - [out], Timer3 Current ticks                                                */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to return Timer ticks                                               */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvTIMER_GetTicks(TIMER_CHANNEL channel)
{

    switch (channel)
    {
        case TMR0:
        {
            return uTimer0Count;
  
        }
        case TMR1:
        {
            return uTimer1Count;

        }	
        case TMR2:
        {
            return uTimer2Count;

        }
        case TMR3:
        {
            return uTimer3Count;

        }
        default:
        {
            break;
        }
    }
    return 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_Delay                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               uTicks - [in], The delay time and it is depend on Timer CLK.                              */
/*                                                                                                         */
/* Returns:      None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to set a delay time if necessary. The TIMER0 is used in this delay  */
/*               function thus it needs to be opened and initialized first.                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvTIMER_Delay(uint32_t uTicks)
{
    uint32_t volatile btime;

    btime = DrvTIMER_GetTicks(TMR0);
    
    while (1)
    {
        if ((DrvTIMER_GetTicks(TMR0) - btime) > uTicks)
        {
            break;
        }
    }

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_Ioctl                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel - [in],   TMR0/TMR1/TMR2/TMR3                                                     */
/*               uCmd    - [in],   Reference TIMER_IOCRL_Table                                             */
/*               uArg1   - [in],   The first parameter for IOCRL                                           */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS          Success                                                                */
/*               E_DRVTIMER_CMD     Command Error                                                          */
/*                                                                                                         */
/* Description:                                                                                            */
/*               To process the general control of timer.                                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_Ioctl(TIMER_CHANNEL channel, TIMER_CMD uCmd,uint32_t uArg1)
{

    switch (uCmd)
    {
        case TIMER_IOC_START_COUNT :		  	/* Start Timer Count */
        {
            if(channel == TMR0)
				TIMER0->TCSR.CEN =1 ;
			else if(channel == TMR1)
				TIMER1->TCSR.CEN =1 ;
			else if(channel == TMR2)
				TIMER2->TCSR.CEN =1 ;
			else if(channel == TMR3)
				TIMER3->TCSR.CEN =1 ;
			else
				return E_DRVTIMER_CHANNEL;						

			break;
        }
        case TIMER_IOC_STOP_COUNT :				/* Stop Timer Count */
        {
            if(channel == TMR0)
				TIMER0->TCSR.CEN =0 ;
			else if(channel == TMR1)
				TIMER1->TCSR.CEN =0 ;
			else if(channel == TMR2)
				TIMER2->TCSR.CEN =0 ;
			else if(channel == TMR3)
				TIMER3->TCSR.CEN =0 ;
			else
				return E_DRVTIMER_CHANNEL;						

            break;
        }
        case TIMER_IOC_ENABLE_INT :			   	/* Enable Specified Channel Interrupt */
        {
			if(channel == TMR0)
				TIMER0->TCSR.IE =1 ;
			else if(channel == TMR1)
				TIMER1->TCSR.IE =1 ;
			else if(channel == TMR2)
				TIMER2->TCSR.IE =1 ;
			else if(channel == TMR3)
				TIMER3->TCSR.IE =1 ;
			else
				return E_DRVTIMER_CHANNEL;
            break;
        }

        case TIMER_IOC_DISABLE_INT :			/* Disable Specified Channel Interrupt */
        {
			if(channel == TMR0)
				TIMER0->TCSR.IE =0 ;
			else if(channel == TMR1)
				TIMER1->TCSR.IE =0 ;
			else if(channel == TMR2)
				TIMER2->TCSR.IE =0 ;
			else if(channel == TMR3)
				TIMER3->TCSR.IE =0 ;
			else
				return E_DRVTIMER_CHANNEL;

            break;
        }

        case TIMER_IOC_RESET_TIMER :			/* Reset specified channel */
        {
			if(channel == TMR0)
				TIMER0->TCSR.CRST =1 ;
			else if(channel == TMR1)
				TIMER1->TCSR.CRST =1 ;
			else if(channel == TMR2)
				TIMER2->TCSR.CRST =1 ;
			else if(channel == TMR3)
				TIMER3->TCSR.CRST =1 ;
			else
				return E_DRVTIMER_CHANNEL;
            break;
        }

        case TIMER_IOC_SET_PRESCALE  :			/* Set Specified channel prescale parameter */
        {
     		if(channel == TMR0)
				TIMER0->TCSR.PRESCALE = uArg1 ;
			else if(channel == TMR1)
				TIMER1->TCSR.PRESCALE = uArg1 ;
			else if(channel == TMR2)
				TIMER2->TCSR.PRESCALE = uArg1 ;
			else if(channel == TMR3)
				TIMER3->TCSR.PRESCALE = uArg1 ;
			else
				return E_DRVTIMER_CHANNEL;
            break;
        }

        case TIMER_IOC_SET_INITIAL_COUNT :	  	/* Set Inital Count value */
        {
            if(channel == TMR0)
				TIMER0->TCMPR = uArg1 ;
			else if(channel == TMR1)
				TIMER1->TCMPR = uArg1 ;
			else if(channel == TMR2)
				TIMER2->TCMPR = uArg1 ;
			else if(channel == TMR3)
				TIMER3->TCMPR = uArg1 ;
			else
				return E_DRVTIMER_CHANNEL;

            break;
        }

        default:
        {
            return E_DRVTIMER_CMD;
  
        }
    }

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     TIMER_Close                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               channel 	- [in],  TMR0 / TMR1 / TMR2 / TMR3                                             */
/* Returns:                                                                                                */
/*               E_SUCCESS          Success                                                                */
/*               E_DRVTIMER_CMD     Command Error                                                          */
/* Description:                                                                                            */
/*               The function is used to disable timer                                                     */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_Close(TIMER_CHANNEL channel)
{

    if (channel > TMR3)
	{
        return E_DRVTIMER_CMD;
    }

    switch (channel)
    {
        case TMR0:
        {
            bIsTimer0Used       = FALSE;
            bIsSetTime0Event    = FALSE;
            DrvTIMER_Ioctl(TMR0, TIMER_IOC_DISABLE_INT, 0);
            DrvTIMER_Ioctl(TMR0, TIMER_IOC_RESET_TIMER, 0);
            break;

        }
        case TMR1:
        {
            bIsTimer1Used       = FALSE;
            bIsSetTime1Event    = FALSE;
            DrvTIMER_Ioctl(TMR1,TIMER_IOC_DISABLE_INT,  0);
            DrvTIMER_Ioctl(TMR1, TIMER_IOC_RESET_TIMER, 0);
            break;

        }
        case TMR2:
        {
            bIsTimer2Used       = FALSE;
            bIsSetTime2Event    = FALSE;
            DrvTIMER_Ioctl(TMR2, TIMER_IOC_DISABLE_INT,0);
            DrvTIMER_Ioctl(TMR2, TIMER_IOC_RESET_TIMER,0);
            break;

        }

        case TMR3:
        {
            bIsTimer3Used       = FALSE;
            bIsSetTime3Event    = FALSE;
            DrvTIMER_Ioctl(TMR3, TIMER_IOC_DISABLE_INT,0);
            DrvTIMER_Ioctl(TMR3, TIMER_IOC_RESET_TIMER,0);
            break;

        }
        default:
        {
            return E_DRVTIMER_CMD ;
        }
    }

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: WDT_InstallISR                                                                                */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               TIMER_CALLBACK pvNewISR                     ISR name                                      */
/*                                                                                                         */
/* Returns:      mOldVect                                                                                  */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The function is used to install watching Dog ISR                                          */
/*---------------------------------------------------------------------------------------------------------*/
void DrvWDT_InstallISR(TIMER_CALLBACK pvNewISR)
{
	WDT->WTCR.WTIE =1 ;
	fnCallBack_WDT = (TIMER_CALLBACK)pvNewISR; 
	NVIC_EnableIRQ(WDT_IRQn); 
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: WDT_SetInterval                                                                               */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               nWdtInterval -[in] ,  Wathching Dog time-out Interval                                     */
/*                                                                                                         */
/* Returns:      E_SUCCESS                                                                                 */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The function is used to set the "Interval Level" of WDT Time-out                          */
/*---------------------------------------------------------------------------------------------------------*/
static int32_t WDT_SetInterval(WDT_INTERVAL nWdtInterval)
{

	WDT->WTCR.WTIS = nWdtInterval;

    return E_SUCCESS;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     WDT_Open                                                                                  */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               hander   -[in]  NULL                                                                      */
/*               level    -[in]  WDT time-out level. It could be LEVEL0~7                                  */
/*                                                                                                         */
/* Returns:      E_SUCCESS                                                                                 */
/* Description:                                                                                            */
/*               The function is used to set WDT Interval and Star Timer to count                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvWDT_Open(int32_t hander ,WDT_INTERVAL level)
{
    SYSCLK->APBCLK.WDG_EN = 1;    

    WDT_SetInterval(level);              /* Select WatchDog Timer Interval                                 */
    
    WDT->WTCR.WTE = 1 ;                   /* Start WatchDog Timer                                           */
 
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     WDT_IRQHandler                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      NONE                                                                                      */
/* Side effects:                                                                                           */
/* Description:                                                                                            */
/*               The function is default Watching Dog Interrupt Service Rotiune                            */
/*---------------------------------------------------------------------------------------------------------*/
void WDT_IRQHandler(void)
{

    WDT->WTCR.WTIF =1;                  				/* Clear INT32 Flag                                */

    if(fnCallBack_WDT)                                  /* Install WDT Callback Function                    */
    {
         fnCallBack_WDT(NULL);
    }

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     WDT_Ioctl                                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               hander 		-[in]   NULL                                                               */
/*               uCmd           -[in]   WDT IOCTL command                                                  */
/*               uArg1   		-[in]   First argument of the command                                      */
/*                                                                                                         */
/* Returns:      E_SUCCESS                                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function of Watching Dog Timer IO Control API                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvWDT_Ioctl(int32_t hander ,WDT_CMD uCmd , uint32_t uArg1)
{
    switch (uCmd)
    {
        case WDT_IOC_START_TIMER  :
        {
 			WDT->WTCR.WTE = 1 ;
            break ;
        }

        case WDT_IOC_STOP_TIMER :
        {
 			WDT->WTCR.WTE = 0 ;
            break ;
        }

        case WDT_IOC_ENABLE_INT :
        {
 			WDT->WTCR.WTIE = 1 ;
            break ;
        }

        case WDT_IOC_DISABLE_INT :
        {
  			WDT->WTCR.WTIE = 0 ;
            break ;
        }

        case WDT_IOC_RESET_TIMER:
        {
 			WDT->WTCR.WTR = 1 ;
            break ;
        }
        case WDT_IOC_ENABLE_RESET_FUNC :
        {
			WDT->WTCR.WTRE = 1 ;
            break ;
        }
        case WDT_IOC_DISABLE_RESET_FUNC:
        {
 			WDT->WTCR.WTRE = 0 ;
            break ;
        }

        case WDT_IOC_SET_INTERVAL:
        {
            WDT_SetInterval((WDT_INTERVAL)uArg1);

            break ;
        }
       
		default :
        {
            return E_DRVTIMER_CMD;
        }
    }

    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     WDT_Close                                                                                 */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      E_SUCCESS                                                                                 */
/* Description:                                                                                            */
/*               The function is used to Stop Wathch Dog Timer and Disable WDT Interrupt                   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvWDT_Close(void)
{
    DrvWDT_Ioctl(0,WDT_IOC_STOP_TIMER ,0);    /* Stop Timer                                                */
    DrvWDT_Ioctl(0,WDT_IOC_DISABLE_INT ,0);   /* Disable INT                                               */


    SYSCLK->APBCLK.WDG_EN = 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     WDT_ResetCount                                                                            */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               NONE                                                                                      */
/*                                                                                                         */
/* Returns:      NONE                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to reset WDT Tick to avoid time-out to restart system.              */
/*---------------------------------------------------------------------------------------------------------*/
void DrvWDT_ResetCount(void)
{
    WDT->WTCR.WTR =1 ;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvTIMER_GetVersion                                                                           */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*              None                                                                                       */
/*                                                                                                         */
/* Returns:                                                                                                */
/*              None                                                                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*              Get the version number of Timer/WDT driver.                                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvTIMER_GetVersion(void)
{
	return DRVTIMER_VERSION_NUM;
}

