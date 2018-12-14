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
#include <stdio.h>
#include "NUC1xx.h"
#include "DrvRTC.h"
#include "DrvGPIO.h"
/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
#define RTC_GLOBALS

//#define RTC_DEBUG
#ifdef RTC_DEBUG
#define RTCDEBUG     printf
#else
#define RTCDEBUG(...)
#endif


/*---------------------------------------------------------------------------------------------------------*/
/* Global file scope (static) variables                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static PFN_DRVRTC_CALLBACK  *g_pfnRTCCallBack_Tick    = NULL, *g_pfnRTCCallBack_Alarm   = NULL;
                   
static uint32_t volatile g_u32RTC_Count  = 0;
static char g_chHourMode = 0;
static int32_t volatile g_bIsEnableTickInt  = NULL;
static int8_t volatile g_bIsEnableAlarmInt  = NULL;

static uint32_t volatile g_u32Reg, g_u32Reg1,g_u32hiYear,g_u32loYear,g_u32hiMonth,g_u32loMonth,g_u32hiDay,g_u32loDay;
static uint32_t volatile g_u32hiHour,g_u32loHour,g_u32hiMin,g_u32loMin,g_u32hiSec,g_u32loSec;

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     RTC_IRQHandler                                                                            */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               Install ISR to handle interrupt event                                                     */
/*---------------------------------------------------------------------------------------------------------*/

void RTC_IRQHandler(void)
{ 
   
	if (RTC->RIIR.TI ==0x1)      		   /* tick interrupt occurred */
	{
 		  RTC->RIIR.TI = 1;
		  
		  g_u32RTC_Count++;                                            /* maintain RTC tick count */

          if (g_pfnRTCCallBack_Tick != NULL)                           /* execute tick callback function */
          {
              g_pfnRTCCallBack_Tick();
          }

    }

 	if (RTC->RIIR.AI ==0x1)                /* alarm interrupt occurred */
    {
          
		  RTC->RIIR.AI = 1;
		  
		  if (g_pfnRTCCallBack_Alarm != NULL) 	                       /* execute alarm callback function */
          {
              g_pfnRTCCallBack_Alarm();
          }

		  DrvRTC_Ioctl(0,DRVRTC_IOC_DISABLE_INT,DRVRTC_ALARM_INT,0);  /* Disable Alarm INT */

    }
}

 
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_SetFrequencyCompensation                                                           */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               fnumber    -[in]            the RTC osillator clock  (KHz)                                */
/* Returns:                                                                                                */
/*               E_SUCCESS					Success.                                                       */
/*               E_DRVRTC_ERR_FCR_VALUE		Wrong Compenation VALUE                                        */
/* DESCRIPTION                                                                                             */
/*                                                                                                         */
/*               Set Frequecy Compenation Data                                                             */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvRTC_SetFrequencyCompensation(float fnumber)
{
    int32_t i32intergerPart  ;
    int32_t i32fractionPart ;
    int32_t i32RegInt,i32RegFra ;

	/* Compute Interger and fraction */
    i32intergerPart = (int32_t) (fnumber) ;
    i32fractionPart = ((int32_t) ( ((fnumber - i32intergerPart) *100)) / 100);
    i32RegInt = i32intergerPart - DRVRTC_FCR_REFERENCE ;
    i32RegFra = ((i32fractionPart) * 60);
    
	/* Judge Interger part is reasonable */
    if ( (i32RegInt < 0) | (i32RegInt > 15) )
    {
        return E_DRVRTC_ERR_FCR_VALUE ;
    }
	
	DrvRTC_WriteEnable();

	outpw(&RTC->FCR, (uint32_t)(i32RegInt >>8 | i32RegFra));

    return E_SUCCESS;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_WriteEnable                                                                        */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               E_SUCCESS		            Success.                                                       */
/*               E_DRVRTC_ERR_FAILED       	FAILED                                                         */
/* DESCRIPTION                                                                                             */
/*               Access Password to AER to make access other register enable                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvRTC_WriteEnable (void)
{
    int32_t i32i = 0;

	int i32retry = 100;
    
	/*-------------------------------------------------------------------------------------------------*/
    /* After 200ms, Access enable wiil auto-clear. As soon as possible to do your setting              */
    /*-------------------------------------------------------------------------------------------------*/
	RETRY:

	i32i = 0;
	
	RTC->AER.AER = 0xA965;
	
    for (i32i = 0 ; i32i < DRVRTC_WAIT_COUNT ; i32i++)
	{
        /*-------------------------------------------------------------------------------------------------*/
        /* check RTC_AER[16] to find out RTC write enable                                                  */
        /*-------------------------------------------------------------------------------------------------*/
 		RTC->AER.AER = 0xA965;
		
		if (RTC->AER.ENF ==1)	
		    break;
	}

	
	if (i32i == DRVRTC_WAIT_COUNT)
    {
        RTCDEBUG ("\nRTC: RTC_WriteEnable, set write enable FAILED!\n");

		i32retry--;

        if(!i32retry)
	   		return E_DRVRTC_ERR_FAILED;
						
		goto RETRY;
    }

    
	return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_Init                                                                               */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               E_SUCCESS            	Success.                                                           */
/*               E_DRVRTC_ERR_EIO		Initial RTC FAILED.                                                */
/* DESCRIPTION                                                                                             */
/*               Initial RTC and install ISR                                                               */
/*---------------------------------------------------------------------------------------------------------*/


int32_t DrvRTC_Init (void)
{
    int32_t i32i =0;
	
	int32_t i32delay=1000;
    /*-----------------------------------------------------------------------------------------------------*/
    /* Initial time data struct and some parameters.                                                       */
    /*-----------------------------------------------------------------------------------------------------*/
    g_pfnRTCCallBack_Alarm = NULL;
    g_pfnRTCCallBack_Tick = NULL;

    g_u32RTC_Count = 0;
	
	UNLOCKREG();
	/* Enable 32K Clock */
	SYSCLK->PWRCON.XTL32K_EN =1;
	  
	/* Waiting for 32K stable */
  	while(i32delay--);
	
	/* Enable RTC Clock */
	SYSCLK->APBCLK.RTC_EN =1;
	
	LOCKREG();

    /*-----------------------------------------------------------------------------------------------------*/
    /* When RTC is power on, write 0xa5eb1357 to RTC_INIR to reset all logic.                              */
    /*-----------------------------------------------------------------------------------------------------*/
	RTC->INIR = DRVRTC_INIT_KEY;

    for (i32i = 0 ; i32i < DRVRTC_WAIT_COUNT ; i32i++)
    {

		if(RTC->INIR == 0x1)  /* Check RTC_INIR[0] to find out RTC reset signal */
        { 
           break;
        }
    }

    if (i32i == DRVRTC_WAIT_COUNT)
    {
        RTCDEBUG("\nRTC: RTC_Init, initial RTC FAILED!\n");
        return E_DRVRTC_ERR_EIO;
    }
	NVIC_EnableIRQ(RTC_IRQn); 
    
    return E_SUCCESS;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_Open                                                                               */
/*                                                                                                         */
/* Parameter:    S_DRVRTC_TIME_DATA_T *sPt            Just Set Current_Timer                               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS        		Success.                                                           */
/*               E_DRVRTC_ERR_EIO		Initial RTC FAILED.                                                */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*               Just Set Current_Timer .                                                                  */
//*--------------------------------------------------------------------------------------------------------*/

int32_t DrvRTC_Open (S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Reg;
    
	RTC_TLR_T tlr = {0};
	RTC_CLR_T clr = {0};
    
    /*-----------------------------------------------------------------------------------------------------*/
    /* DO BASIC JUDGEMENT TO Check RTC time data value is reasonable or not.                               */
    /*-----------------------------------------------------------------------------------------------------*/
    if ( ((sPt->u32Year - DRVRTC_YEAR2000) > 99)|
         ((sPt->u32cMonth == 0) || (sPt->u32cMonth > 12))|
         ((sPt->u32cDay   == 0) || (sPt->u32cDay   > 31)))
    {
        return E_DRVRTC_ERR_CALENDAR_VALUE;
    }

    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)
    {
        if ( (sPt->u32cHour == 0) || (sPt->u32cHour > 12) )
        {
            return E_DRVRTC_ERR_TIMESACLE_VALUE ;
        }
    }
    else if (sPt->u8cClockDisplay == DRVRTC_CLOCK_24)
    {
        if (sPt->u32cHour > 23)
        {
            return E_DRVRTC_ERR_TIMESACLE_VALUE ;
        }
    }
    else
    {
        return E_DRVRTC_ERR_TIMESACLE_VALUE ;
    }

    if ((sPt->u32cMinute > 59) |
        (sPt->u32cSecond > 59) |
        (sPt->u32cSecond > 59))
    {
        return E_DRVRTC_ERR_TIME_VALUE ;
    }
    if (sPt->u32cDayOfWeek > 6)
    {
        return E_DRVRTC_ERR_DWR_VALUE ;
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Important, call RTC_WriteEnable() before write data into any register.                              */
    /* User should be write data as soon as possible.Access enable wiil clear after 200ms                  */
	/*-----------------------------------------------------------------------------------------------------*/
    g_u32Reg = DrvRTC_WriteEnable();
    if (g_u32Reg != 0)
    {
        return E_DRVRTC_ERR_FAILED;
    }
    
    /*-----------------------------------------------------------------------------------------------------*/
    /* Second, set RTC 24/12 hour setting                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)
    {
        DrvRTC_WriteEnable();
 		RTC->TSSR.HR24 = DRVRTC_CLOCK_12;

        /*-------------------------------------------------------------------------------------------------*/
        /* important, range of 12-hour PM mode is 21 upto 32                                               */
        /*-------------------------------------------------------------------------------------------------*/
        if (sPt->u8cAmPm == DRVRTC_PM)
            sPt->u32cHour += 20;
    }
    else                                                                               /* RTC_CLOCK_24 */
    {

        DrvRTC_WriteEnable();
 		RTC->TSSR.HR24 = DRVRTC_CLOCK_24;
        RTCDEBUG ("RTC: 24-hour\n");
    }

    /*-----------------------------------------------------------------------------------------------------*/
    /* Set RTC Calender Loading                                                                            */
    /*-----------------------------------------------------------------------------------------------------*/
    u32Reg    = ((sPt->u32Year - DRVRTC_YEAR2000) / 10) << 20;
    u32Reg    |= (((sPt->u32Year - DRVRTC_YEAR2000) % 10) << 16);
    u32Reg    |= ((sPt->u32cMonth  / 10) << 12);
    u32Reg    |= ((sPt->u32cMonth  % 10) << 8);
    u32Reg    |= ((sPt->u32cDay    / 10) << 4);
    u32Reg    |= (sPt->u32cDay     % 10);
    g_u32Reg = u32Reg;
	
	DrvRTC_WriteEnable();
 	RTC->TSSR.HR24 = DRVRTC_CLOCK_24;
    outpw(&RTC->CLR, (uint32_t)g_u32Reg);

	/*-----------------------------------------------------------------------------------------------------*/
    /* Set RTC Time Loading                                                                                */
    /*-----------------------------------------------------------------------------------------------------*/
    u32Reg     = ((sPt->u32cHour / 10) << 20);
    u32Reg    |= ((sPt->u32cHour % 10) << 16);
    u32Reg    |= ((sPt->u32cMinute / 10) << 12);
    u32Reg    |= ((sPt->u32cMinute % 10) << 8);
    u32Reg    |= ((sPt->u32cSecond / 10) << 4);
    u32Reg    |= (sPt->u32cSecond % 10);
    g_u32Reg = u32Reg;

 	DrvRTC_WriteEnable();
    outpw(&RTC->TLR, (uint32_t)g_u32Reg);

	outpw(&RTC->DWR, sPt->u32cDayOfWeek);

    return E_SUCCESS;

}

 
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_Read                                                                               */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               eTime                    		Currnet_Timer/ Alarm_Time                                  */
/*               S_DRVRTC_TIME_DATA_T *spt   	Time Data                                                  */
/* Returns:                                                                                                */
/*               E_SUCCESS               Success.                                                          */
/*               E_DRVRTC_ERR_EIO		 Initial RTC FAILED.                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*               Read current date/time or alarm date/time from RTC                                        */
//*--------------------------------------------------------------------------------------------------------*/

int32_t DrvRTC_Read (E_DRVRTC_TIME_SELECT eTime, S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Tmp;
    
    sPt->u8cClockDisplay = RTC->TSSR.HR24;                                 /* 12/24-hour */
    sPt->u32cDayOfWeek   = RTC->DWR.DWR;                                   /* Day of week */

    switch (eTime)
    {
        case DRVRTC_CURRENT_TIME:										   /* Get Current Time */
        {
			g_u32hiYear  = RTC->CLR.YEAR10 ;
    		g_u32loYear  = RTC->CLR.YEAR1;
    		g_u32hiMonth = RTC->CLR.MON10;
    		g_u32loMonth = RTC->CLR.MON1;
    		g_u32hiDay   = RTC->CLR.DAY10;
    		g_u32loDay   = RTC->CLR.DAY1;

		    g_u32hiHour =  RTC->TLR.HR10;
    		g_u32loHour =  RTC->TLR.HR1;
    		g_u32hiMin  =  RTC->TLR.MIN10;
    		g_u32loMin  =  RTC->TLR.MIN1;
    		g_u32hiSec  =  RTC->TLR.SEC10;
    		g_u32loSec  =  RTC->TLR.SEC1;
            break;
		}
        case DRVRTC_ALARM_TIME:									 	/* Get Alarm Time */
        {
			g_u32hiYear  = RTC->CAR.YEAR10 ;
    		g_u32loYear  = RTC->CAR.YEAR1;
    		g_u32hiMonth = RTC->CAR.MON10;
    		g_u32loMonth = RTC->CAR.MON1;
    		g_u32hiDay   = RTC->CAR.DAY10;
    		g_u32loDay   = RTC->CAR.DAY1;

		    g_u32hiHour =  RTC->TAR.HR10;
    		g_u32loHour =  RTC->TAR.HR1;
    		g_u32hiMin  =  RTC->TAR.MIN10;
    		g_u32loMin  =  RTC->TAR.MIN1;
    		g_u32hiSec  =  RTC->TAR.SEC10;
    		g_u32loSec  =  RTC->TAR.SEC1;
            break;
        }
        default:
        {
            return E_DRVRTC_ERR_ENOTTY;
        }
    }

    u32Tmp = (g_u32hiYear * 10);									/* Compute to 20XX year */
    u32Tmp+= g_u32loYear;
    sPt->u32Year   =   u32Tmp  + DRVRTC_YEAR2000;
    
    u32Tmp = (g_u32hiMonth * 10);									/* Compute 0~12 month */
    sPt->u32cMonth = u32Tmp + g_u32loMonth;
    
    u32Tmp = (g_u32hiDay * 10);										/* Compute 0~31 day */
    sPt->u32cDay   =  u32Tmp  + g_u32loDay;

    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)					/* Compute12/24 hout */
    {
        u32Tmp = (g_u32hiHour * 10);
        u32Tmp+= g_u32loHour;
        sPt->u32cHour = u32Tmp;                                		/* AM: 1~12. PM: 21~32. */

        if (sPt->u32cHour >= 21)
        {
            sPt->u8cAmPm = DRVRTC_PM;
            sPt->u32cHour -= 20;
        }
        else
        {
            sPt->u8cAmPm = DRVRTC_AM;
        }
        
        u32Tmp = (g_u32hiMin  * 10);
        u32Tmp+= g_u32loMin;
        sPt->u32cMinute = u32Tmp;
        
        u32Tmp = (g_u32hiSec  * 10);
        u32Tmp+= g_u32loSec;
        sPt->u32cSecond = u32Tmp;

    }
    else
    {   															/* RTC_CLOCK_24 */
        u32Tmp = (g_u32hiHour * 10);
        u32Tmp+= g_u32loHour;
        sPt->u32cHour   = u32Tmp;
        
        u32Tmp = (g_u32hiMin  * 10);
        u32Tmp+= g_u32loMin;
        sPt->u32cMinute = u32Tmp;
        
        u32Tmp = (g_u32hiSec  * 10);
        u32Tmp+= g_u32loSec;
        sPt->u32cSecond = u32Tmp;
    }

    return E_SUCCESS;

}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_Write                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               eTime                    		Currnet_Timer/ Alarm_Time                                  */
/*               S_DRVRTC_TIME_DATA_T *sPt      Time Data                                                  */
/* Returns:                                                                                                */
/*               E_SUCCESS               Success.                                                          */
/*               E_DRVRTC_ERR_EIO        Initial RTC FAILED.                                               */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*               Read current date/time or alarm date/time from RTC                                        */
//*--------------------------------------------------------------------------------------------------------*/
int32_t DrvRTC_Write(E_DRVRTC_TIME_SELECT eTime, S_DRVRTC_TIME_DATA_T *sPt)
{
    uint32_t u32Reg;
    /*-----------------------------------------------------------------------------------------------------*/
    /* Check RTC time data value is reasonable or not.                                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    if ( ((sPt->u32Year - DRVRTC_YEAR2000) > 99)|
         ((sPt->u32cMonth == 0) || (sPt->u32cMonth > 12))|
         ((sPt->u32cDay   == 0) || (sPt->u32cDay   > 31)))
    {
        RTCDEBUG ("\nRTC: Year value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if ( (sPt->u32Year - DRVRTC_YEAR2000) > 99 )
    {
        RTCDEBUG ("\nRTC: Year value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if ( (sPt->u32cMonth == 0) || (sPt->u32cMonth > 12) )
    {
        RTCDEBUG ("\nRTC: Month value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if ( (sPt->u32cDay == 0) || (sPt->u32cDay > 31) )
    {
        RTCDEBUG ("\nRTC: Day value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)
    {
        if ( (sPt->u32cHour == 0) || (sPt->u32cHour > 12) )
        {
            RTCDEBUG ("\nRTC: Hour value is incorrect\n");
            return E_DRVRTC_ERR_FAILED;
        }
    }
    else if (sPt->u8cClockDisplay == DRVRTC_CLOCK_24)
    {
        if (sPt->u32cHour > 23)
        {
            RTCDEBUG ("\nRTC: Hour value is incorrect\n");
            return E_DRVRTC_ERR_FAILED;
        }
    }
    else
    {
        RTCDEBUG ("\nRTC: Clock mode is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if (sPt->u32cMinute > 59)
    {
        RTCDEBUG ("\nRTC: Minute value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if (sPt->u32cSecond > 59)
    {
        RTCDEBUG ("\nRTC: Second value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }

    if (sPt->u32cDayOfWeek > 6)
    {
        RTCDEBUG ("\nRTC: Day of week value is incorrect\n");
        return E_DRVRTC_ERR_FAILED;
    }


    /*-----------------------------------------------------------------------------------------------------*/
    /* Important, call RTC_Open() before write data into any register.                                     */
    /*-----------------------------------------------------------------------------------------------------*/
    g_u32Reg = DrvRTC_WriteEnable();
    if (g_u32Reg != 0)
    {
        return E_DRVRTC_ERR_FAILED;
    }

    switch (eTime)
    {

        case DRVRTC_CURRENT_TIME:
            /*---------------------------------------------------------------------------------------------*/
            /* Second, set RTC time data.                                                                  */
            /*---------------------------------------------------------------------------------------------*/

            if (sPt->u8cClockDisplay == DRVRTC_CLOCK_12)
            {
                g_chHourMode = DRVRTC_CLOCK_12;
				RTC->TSSR.HR24 = DRVRTC_CLOCK_12;
                RTCDEBUG ("RTC: 12-hour\n");
                /*-----------------------------------------------------------------------------------------*/
                /* important, range of 12-hour PM mode is 21 upto 32                                       */
                /*-----------------------------------------------------------------------------------------*/
                if (sPt->u8cAmPm == DRVRTC_PM)
                    sPt->u32cHour += 20;
            }
            else                                                                  /* RTC_CLOCK_24 */
            {
                g_chHourMode = DRVRTC_CLOCK_24;
    			RTC->TSSR.HR24 = DRVRTC_CLOCK_24;
                RTCDEBUG ("RTC: 24-hour\n");
            }

  			RTC->DWR.DWR = sPt->u32cDayOfWeek;

		    
			/*---------------------------------------------------------------------------------------------*/
            /* Second, set RTC time data.                                                                  */
            /*---------------------------------------------------------------------------------------------*/

			u32Reg     = ((sPt->u32Year - DRVRTC_YEAR2000) / 10) << 20;
		    u32Reg    |= (((sPt->u32Year - DRVRTC_YEAR2000) % 10) << 16);
		    u32Reg    |= ((sPt->u32cMonth  / 10) << 12);
		    u32Reg    |= ((sPt->u32cMonth  % 10) << 8);
		    u32Reg    |= ((sPt->u32cDay    / 10) << 4);
		    u32Reg    |= (sPt->u32cDay     % 10);
            g_u32Reg = u32Reg;
			DrvRTC_WriteEnable();
          
			outpw(&RTC->CLR, (uint32_t)g_u32Reg);
			RTCDEBUG ("RTC: REG_RTC_CLR[0x%08x]\n", inpw(&RTC->CLR));  
            
		    u32Reg     = ((sPt->u32cHour / 10) << 20);
		    u32Reg    |= ((sPt->u32cHour % 10) << 16);
		    u32Reg    |= ((sPt->u32cMinute / 10) << 12);
		    u32Reg    |= ((sPt->u32cMinute % 10) << 8);
		    u32Reg    |= ((sPt->u32cSecond / 10) << 4);
		    u32Reg    |= (sPt->u32cSecond % 10);
			g_u32Reg = u32Reg;
		
			DrvRTC_WriteEnable();
           	outpw(&RTC->TLR, (uint32_t)g_u32Reg);
			RTCDEBUG ("RTC: REG_RTC_TLR[0x%08x]\n", inpw(&RTC->TLR));   
			
            return E_SUCCESS;


         case DRVRTC_ALARM_TIME:

            g_pfnRTCCallBack_Alarm = NULL;                                /* Initial call back function.*/
            
			/*---------------------------------------------------------------------------------------------*/
            /* Set Calender alarm time data.                                                               */
            /*---------------------------------------------------------------------------------------------*/
		    u32Reg     = ((sPt->u32Year - DRVRTC_YEAR2000) / 10) << 20;
		    u32Reg    |= (((sPt->u32Year - DRVRTC_YEAR2000) % 10) << 16);
		    u32Reg    |= ((sPt->u32cMonth  / 10) << 12);
		    u32Reg    |= ((sPt->u32cMonth  % 10) << 8);
		    u32Reg    |= ((sPt->u32cDay    / 10) << 4);
		    u32Reg    |= (sPt->u32cDay     % 10);
            g_u32Reg = u32Reg;
			DrvRTC_WriteEnable();
			
			outpw(&RTC->CAR, (uint32_t)g_u32Reg);
			RTCDEBUG ("RTC: REG_RTC_CAR[0x%08x]\n", inpw(&RTC->CAR));  
			
						 
            if (g_chHourMode == DRVRTC_CLOCK_12)
            {
                if (sPt->u8cAmPm == DRVRTC_PM)       /* important, range of 12-hour PM mode is 21 upto 32 */
                    sPt->u32cHour += 20;
            }

			/*---------------------------------------------------------------------------------------------*/
            /* Set Time alarm time data.                                                                   */
            /*---------------------------------------------------------------------------------------------*/
		    u32Reg     = ((sPt->u32cHour / 10) << 20);
		    u32Reg    |= ((sPt->u32cHour % 10) << 16);
		    u32Reg    |= ((sPt->u32cMinute / 10) << 12);
		    u32Reg    |= ((sPt->u32cMinute % 10) << 8);
		    u32Reg    |= ((sPt->u32cSecond / 10) << 4);
		    u32Reg    |= (sPt->u32cSecond % 10);

			g_u32Reg = u32Reg;
            DrvRTC_WriteEnable();
           	outpw(&RTC->TAR, (uint32_t)g_u32Reg);
			RTCDEBUG ("RTC: REG_RTC_TAR[0x%08x]\n", inpw(&RTC->TAR));   

            /*---------------------------------------------------------------------------------------------*/
            /* Third, install alarm callback function.                                                     */
            /*---------------------------------------------------------------------------------------------*/
            if (sPt->pfnAlarmCallBack != NULL)
                g_pfnRTCCallBack_Alarm = sPt->pfnAlarmCallBack;
            
			/*---------------------------------------------------------------------------------------------*/
            /* Finally, enable alarm interrupt.                                                            */
            /*---------------------------------------------------------------------------------------------*/

            DrvRTC_Ioctl(0,DRVRTC_IOC_ENABLE_INT,DRVRTC_ALARM_INT,0);
            return E_SUCCESS;


	        default:
	        {
	            return E_DRVRTC_ERR_ENOTTY;
	        }
    }

}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_Ioctl                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               i32Num             Interface number.   always set 0                                       */
/*               eCmd           	Command.                                                               */
/*               u32Arg0            Arguments for the command.                                             */
/*               u32Arg1            Arguments for the command.                                             */
/* Returns:                                                                                                */
/*               E_SUCCESS        			Success.                                                       */
/*               E_DRVRTC_ERR_ENOTTY        Command not support, or parameter incorrect.                   */
/*               E_DRVRTC_ERR_ENODEV        Interface number incorrect.                                    */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*               Support some commands for application.                                                    */
//*--------------------------------------------------------------------------------------------------------*/

int32_t DrvRTC_Ioctl (int32_t i32Num, E_DRVRTC_CMD eCmd, uint32_t u32Arg0, float fArg1)
{
    int32_t i32Ret;
    uint32_t u32Reg;
    S_DRVRTC_TICK_T *ptick;

    if (i32Num != 0)
        return E_DRVRTC_ERR_ENODEV;

    switch (eCmd)
    {

        case DRVRTC_IOC_IDENTIFY_LEAP_YEAR:						   /* Check leap year */
        {
   			if(RTC->LIR.LIR == 0x1)
            {
                u32Arg0 = DRVRTC_LEAP_YEAR;
                RTCDEBUG("\nRTC: It's a leap year\n");
            }
            else
            {
                u32Arg0 = 0;
                RTCDEBUG("\nRTC: It's not a leap year\n");
            }
            break;
        }
        case DRVRTC_IOC_SET_TICK_MODE:							   /* Set Tick Interrupt */
        {
            ptick = (S_DRVRTC_TICK_T *) u32Arg0;
            
            if (g_bIsEnableTickInt== FALSE)
            {            
            	DrvRTC_Ioctl(0,DRVRTC_IOC_DISABLE_INT,DRVRTC_TICK_INT,0);
            	g_bIsEnableTickInt = TRUE;
	        }    
            g_u32RTC_Count = 0;

            u32Reg = DrvRTC_WriteEnable();
            if (u32Reg != 0)
            {
                return E_DRVRTC_ERR_EIO ;
            }
            if (ptick->ucMode > DRVRTC_TICK_1_128_SEC)             /* Tick mode 0 to 7 */
            {
                return E_DRVRTC_ERR_ENOTTY ;
            }
            
  			RTC->TTR.TTR = ptick->ucMode;            

            if (ptick->pfnTickCallBack != NULL)
            {
                g_pfnRTCCallBack_Tick = ptick->pfnTickCallBack;
            }
            else
            {
                g_pfnRTCCallBack_Tick = NULL;
            }
            
            /*---------------------------------------------------------------------------------------------*/
            /* Reset tick interrupt status if program enable tick interrupt before.                        */
            /*---------------------------------------------------------------------------------------------*/
            if (g_bIsEnableTickInt== TRUE)
            {

                DrvRTC_Ioctl(0,DRVRTC_IOC_ENABLE_INT,DRVRTC_TICK_INT,0);
                return E_SUCCESS;
            }
            break;
        }

        case DRVRTC_IOC_GET_TICK:							  	/* Get Currnt Tick  */
        {
            u32Arg0 = g_u32RTC_Count;
            break;
        }

        case DRVRTC_IOC_RESTORE_TICK:							/* Reset tick count */
        {
            g_u32RTC_Count = 0;
            break;
        }

        case DRVRTC_IOC_ENABLE_INT:								/* Enable Specified Interrupt */
        {
            int32_t i32Ret;

            i32Ret = DrvRTC_WriteEnable();
            if (i32Ret != 0)
    		{	
                return E_DRVRTC_ERR_EIO;
            }
            switch ((DRVRTC_INT_SOURCE)u32Arg0)
            {

                case DRVRTC_TICK_INT:
                {
                    g_bIsEnableTickInt   = TRUE;
   					RTC->RIER.TIER = 1; 
   					break;
                }
                case DRVRTC_ALARM_INT:
                {
                    g_bIsEnableAlarmInt  = TRUE;
    				RTC->RIER.AIER = 1; 
                    break;
                }
                default:
                {
                    return E_DRVRTC_ERR_ENOTTY;

                }
            }
            break;
        }
        case DRVRTC_IOC_DISABLE_INT:						  /* Disable Interrupt */
        {
            int32_t i32Ret;

            i32Ret = DrvRTC_WriteEnable();
            if (i32Ret != 0)
            {
                return E_DRVRTC_ERR_EIO;
            }            
			switch ((DRVRTC_INT_SOURCE)u32Arg0)
            {
                case DRVRTC_TICK_INT:
                {
                    g_bIsEnableTickInt   = FALSE;
    				RTC->RIER.TIER = 0; 
					RTC->RIIR.TI = 0; 
                    break;
                }
                case DRVRTC_ALARM_INT:
                {
                    g_bIsEnableAlarmInt  = FALSE;
    				RTC->RIER.AIER = 0; 
					RTC->RIIR.AI = 0; 
                    break;
                }

                case DRVRTC_ALL_INT:
                {
                    g_bIsEnableTickInt   = FALSE;
                    g_bIsEnableAlarmInt  = FALSE;
					outpw(RTC_BASE + 0x28 , 0);  
					outpw(RTC_BASE + 0x2C , 3);  

                    break;
                }
                default:
                {
                    return E_DRVRTC_ERR_ENOTTY;
                }
            }


            break;
        }

        case DRVRTC_IOC_SET_FREQUENCY:						/* Set Frequency Compesation */
        {
            i32Ret= DrvRTC_SetFrequencyCompensation(u32Arg0) ;
            if (i32Ret != 0)
            {
                return E_DRVRTC_ERR_ENOTTY;
            }
            break;
        }

        default:
        {
            return E_DRVRTC_ERR_ENOTTY;
        }
    }

    return E_SUCCESS;
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvRTC_Close                                                                              */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*               None            							                                               */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success.                                                         */
/*               E_DRVRTC_ERR_ENODEV      Interface number incorrect.                                      */
/*                                                                                                         */
/* DESCRIPTION                                                                                             */
/*               Disable NVIC channel of RTC and both tick and alarm interrupt..                           */
/*---------------------------------------------------------------------------------------------------------*/

int32_t DrvRTC_Close (void)
{

    g_bIsEnableTickInt = FALSE;
    
  	NVIC_DisableIRQ(RTC_IRQn);
   
    DrvRTC_Ioctl(0,DRVRTC_IOC_DISABLE_INT,DRVRTC_ALL_INT,0);

    return E_SUCCESS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvRTC_InstallISR                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None	                                                         						   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to install RTC callback function                                    */
/*---------------------------------------------------------------------------------------------------------*/
void DrvRTC_InstallISR(PFN_DRVRTC_CALLBACK callback,int32_t i32para)
{
	 if(i32para ==0)
		g_pfnRTCCallBack_Tick  = callback;
	 else
	 	g_pfnRTCCallBack_Alarm = callback;

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvRTC_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None	                                                         						   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvRTC version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvPWM version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvRTC_GetVersion (void)
{
	return DRVRTC_VERSION_NUM;
}







