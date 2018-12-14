#include "NUC1xx.h"


extern uint32_t SystemFrequency;

volatile uint32_t g_timer0Ticks = 0;
volatile uint32_t g_timer1Ticks = 0;
volatile uint32_t g_timer2Ticks = 0;
volatile uint32_t g_timer3Ticks = 0;

void TMR0_IRQHandler(void)
{
    g_timer0Ticks++;
    TIMER0->TISR.TIF = 1;
}

void TMR1_IRQHandler(void)
{
    g_timer1Ticks++;
    TIMER1->TISR.TIF = 1;
}

void TMR2_IRQHandler(void)
{
    g_timer2Ticks++;
    TIMER2->TISR.TIF = 1;
}

void TMR3_IRQHandler(void)
{
    g_timer3Ticks++;
    TIMER3->TISR.TIF = 1;
}

void TimerInit(void)
{
    uint32_t timerClkSrc;
    TIMER_TCSR_T tcsr = {0};

    /* Configure GCR to reset Timer */
    SYS->IPRSTC2.TMR0_RST = 1;
    SYS->IPRSTC2.TMR0_RST = 0;
    SYS->IPRSTC2.TMR1_RST = 1;
    SYS->IPRSTC2.TMR1_RST = 0;
    SYS->IPRSTC2.TMR2_RST = 1;
    SYS->IPRSTC2.TMR2_RST = 0;
    SYS->IPRSTC2.TMR3_RST = 1;
    SYS->IPRSTC2.TMR3_RST = 0;

    /* Enable Timer clock */
    SYSCLK->APBCLK.TMR0_EN = 1;
    SYSCLK->APBCLK.TMR1_EN = 1;
    SYSCLK->APBCLK.TMR2_EN = 1;
    SYSCLK->APBCLK.TMR3_EN = 1;

    /* Select Timer clock source */
    SYSCLK->CLKSEL1.TMR0_S = 0;
    SYSCLK->CLKSEL1.TMR1_S = 0;
    SYSCLK->CLKSEL1.TMR2_S = 0;
    SYSCLK->CLKSEL1.TMR3_S = 4; /* 22Mhz clock source */
    
    timerClkSrc = 12000000;

    /* Timer reload value */
    TIMER0->TCMPR    = timerClkSrc / 100;
    TIMER1->TCMPR    = timerClkSrc / 100;
    TIMER2->TCMPR    = timerClkSrc / 100;
    TIMER3->TCMPR    = 22000000 / 100;

    /* Start timer */
    tcsr.nDBGACK_EN = 1;
    tcsr.IE = 1;
    tcsr.CACT = 1;
    tcsr.CRST = 1;
    tcsr.CEN = 1;
    tcsr.PRESCALE = 0;
    tcsr.TDR_EN = 1;
    tcsr.MODE = 1;
    REGCOPY(TIMER0->TCSR, tcsr);
    REGCOPY(TIMER1->TCSR, tcsr);
    REGCOPY(TIMER2->TCSR, tcsr);
    REGCOPY(TIMER3->TCSR, tcsr);
    

    NVIC_SetPriority (TMR0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_SetPriority (TMR1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_SetPriority (TMR2_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_SetPriority (TMR3_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_EnableIRQ(TMR1_IRQn);
    NVIC_EnableIRQ(TMR2_IRQn);
    NVIC_EnableIRQ(TMR3_IRQn);

}

uint32_t GetTimer0Ticks(void)
{
    return g_timer0Ticks;
}

uint32_t GetTimer1Ticks(void)
{
    return g_timer1Ticks;
}

uint32_t GetTimer2Ticks(void)
{
    return g_timer2Ticks;
}

uint32_t GetTimer3Ticks(void)
{
    return g_timer3Ticks;
}



    
