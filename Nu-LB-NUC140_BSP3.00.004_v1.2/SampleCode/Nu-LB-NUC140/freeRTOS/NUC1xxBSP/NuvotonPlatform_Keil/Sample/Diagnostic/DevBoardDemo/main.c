/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"


//#define ENABLE_PLL  1


typedef volatile unsigned char  vu8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))

extern uint32_t SystemFrequency;

#define outpw(port,value)	*((volatile unsigned int *)(port))=value

volatile uint32_t msTicks;      /* counts 1ms timeTicks */


extern uint32_t g_timer0Ticks;
extern uint32_t g_timer1Ticks;
extern uint32_t g_timer2Ticks;
extern uint32_t g_timer3Ticks;

extern void TimerInit(void);
extern void PwmInit(void);
extern void UartInit(void);

/* Frequency table */
uint16_t gToneTbl[14] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988};
uint8_t gPlayTbl[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
uint16_t gPlaySize = sizeof(gPlayTbl);
uint16_t gPlayIndex = 0;

uint32_t gExtClock = 12000000;


void LoopDelay(uint32_t delayCnt)
{
    while(delayCnt--)
    {
        __NOP();
        __NOP();
    }
}


/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    /* Clear count flag */
    msTicks++;                  /* increment counter necessary in Delay() */
}


/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

void Delay2(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = g_timer0Ticks;
  while ((g_timer0Ticks - curTicks) < dlyTicks);
}

void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}


/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)
{
    int8_t ch;
    int32_t loopCnt;
    uint32_t preTicks;
    uint32_t preMsTicks;
    uint32_t preTicks2;
    int8_t preGPB15, GPB15;
    uint32_t estiSysFreq;

    UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
    SYSCLK->PWRCON.OSC10K_EN = 1;
    SYSCLK->PWRCON.XTL32K_EN = 1;

    /* Waiting for Xtal stalble */
    SysTimerDelay(5000);

    SYSCLK->CLKSEL0.STCLK_S = 4; /* Use internal 22Mhz */
    SYSCLK->CLKSEL1.UART_S = 0;  /* Use external 12Mhz */
 
 
 #ifdef ENABLE_PLL
    
    /* USE PLL as CPU clock source */
    SYSCLK->PLLCON.PD = 0;      /* Enable PLL */
    LoopDelay(1000);            /* Waiting PLL stable */
    SYSCLK->CLKSEL0.HCLK_S = 2; /* Select HCLK source as PLL */ 
    LoopDelay(100);             /* A small delay for safe */
    SYSCLK->CLKDIV.HCLK_N = 0;  /* Further divider if it is required */
 
     /* The default CPU clock source is 48MHz. */
    SystemFrequency = 48000000;

 #else
    SYSCLK->CLKSEL0.HCLK_S = 7; /* Select HCLK source as 22MHz */ 
    SYSCLK->CLKDIV.HCLK_N = 0;  /* Further divider if it is required */
    /* The default CPU clock source is 22MHz. It may have some deviation actually due to RC circuit. */
    SystemFrequency = 22000000;

 #endif


    /* Init UART0 as 115200 bps, 8 bits, no-parity, 1 stop bit. */
    UartInit();

    printf("+-----------------------------------------------------------+\n");
    printf("|             Development Board Demo Program                |\n");
    printf("|                                                           |\n");
    printf("+-----------------------------------------------------------+\n");

    
    /* setup clocks */   
    if(SysTick_Config(SystemFrequency / 100))
    {
        /* Setup SysTick Timer for 1 msec interrupts  */
        printf("ERROR: System Tick initial fail!\n");
        while (1); /* Capture error */ 
    }

    //playSize = sizeof(playTbl);

    /* Initial Timer 0 ~ 3. The clock source of timer 3 is 22MHz, others are 12Mhz*/
    TimerInit();

    /* Initial PWM0 to generate 4k Hz to GPA12 */
    PwmInit();

    /* Init the KEY at GPB15 */
    GPIOB->PMD.PMD15 = 0;
    preGPB15 = (GPIOB->PIN >> 15) & 1;

    /* Init the GPA7 for LED */
    GPIOA->PMD.PMD7 = 1;
    GPIOA->DOUT = (1 << 7);

    /* Init GPA12 to keep buzzer input low when no PWM output */
    GPIOA->PMD.PMD12 = 1;
    GPIOA->DOUT = GPIOA->DOUT & (~(1 << 12));
    
    /* Init GPB12 to mirror the GPA12 due to the buzzer is connected to GPB12 */
    GPIOB->PMD.PMD12 = 1;
    GPIOB->DOUT = (GPIOB->DOUT & (~(1 << 12))); /* Init to low to keep buzzer silence */

    preMsTicks = msTicks;
    preTicks = g_timer0Ticks;
    preTicks2 = g_timer0Ticks;
    loopCnt = 0;
    while(1)
    {
        /* The loopCnt per second could be a rough index for CPU performance */
        loopCnt++;

        if(g_timer0Ticks >= preTicks + 100)
        {
            /* Use external 12Mhz to calculate the actural clock source of SysTick */ 
            estiSysFreq = (msTicks - preMsTicks) * (SystemFrequency/100);

            printf("SysTick:%d TMR0:%d TMR1:%d TMR2:%d TMR3:%d CNT:%d CPU:%d\n", msTicks - preMsTicks, g_timer0Ticks,
            g_timer1Ticks, g_timer2Ticks, g_timer3Ticks,loopCnt, estiSysFreq);
            loopCnt = 0;

            preTicks = g_timer0Ticks;
            preMsTicks = msTicks;



            /* Toggle the GPC0 to check the frequency */
            GPIOC->DOUT = !GPIOC->DOUT;
        }
        
        /* Check GPB15 every 10ms */
        if(g_timer0Ticks >= preTicks2 + 10)
        {
            preTicks2 = g_timer0Ticks;

            GPB15 = ((GPIOB->PIN >> 15) & 1);
            if(preGPB15 != GPB15)
            {
                if(GPB15 == 0)
                {
                    /* Change the tone */
                    gPlayIndex++;
                    if(gPlayIndex >= gPlaySize)
                        gPlayIndex = 0;
                    
                    PWMA->CNR0 = (gExtClock / 2) / gToneTbl[gPlayTbl[gPlayIndex]];
                    PWMA->CMR0 = PWMA->CNR0 / 2;

                    printf("Key pressed! Tone = %dHz\n", gToneTbl[gPlayTbl[gPlayIndex]]);
                }
                else
                {
                    printf("Key released!\n");
                }
                
                preGPB15 = GPB15;
            }
        }

        /* Use PWM output as KEY tone. Using KEY(GPB15) to select the GPA12 is PWM or GPIO low */
        SYS->GPAMFP.PWM0 = !((GPIOB->PIN >> 15) & 1);

        /* Mirror the GPA12 to GPB12 due to the buzzer is connected to GPB12 */
        GPIOB->DOUT = (GPIOB->DOUT & (~(1 << 12))) | (GPIOA->PIN & (1 << 12));

        /* Set LED(GPA7) to show KEY(GPB15) status */
        GPIOA->DOUT = ((GPIOA->DOUT) & (~(1 << 7))) | (((GPIOB->PIN >> 15) & 1) << 7);

        if(UART0->FSR.RX_EMPTY != 1)
        {
            ch = getchar();

            /* You may handle the char input here */

            if(ch == 'r')
            {
                /* Reset Chip */
                M32(0x50000008) = 0x1;
            
            }

        }
    }
  
}




