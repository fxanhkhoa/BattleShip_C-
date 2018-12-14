#include "NUC1xx.h"

extern uint32_t gExtClock;

void PwmInit(void)
{

    /* Enable PWM */
    SYSCLK->APBCLK.PWM01_EN = 1;

    /* Reset PWM */
    SYS->IPRSTC2.PWM03_RST = 1;
    SYS->IPRSTC2.PWM03_RST = 0;
    SYS->IPRSTC2.PWM47_RST = 1;
    SYS->IPRSTC2.PWM47_RST = 0;

    /* Select clock source */
    SYSCLK->CLKSEL1.PWM01_S = 0; /* Use external 12Mhz */

    /* Multi-funciton pin */
    SYS->GPAMFP.PWM0 = 1;

    PWMA->PPR.CP01 = 1;
    PWMA->CSR.CSR0 = 4;
    PWMA->PCR.CH0MOD = 1; /* Auto-reload */
    PWMA->PCR.CH0EN = 1;
    PWMA->CNR0 = (gExtClock/2) / 4000; /* Freq = 4000 Hz */
    PWMA->CMR0 = PWMA->CNR0 / 2;   /* duty-cycle = 50 % */

    /* Output enable */
    PWMA->POE.PWM0 = 1;

}
