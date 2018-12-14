//
// PWM : PWM1 Ch1 Output to Ch2 Capture 
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN  (LQPF-100)

// Connections:
// PWM1-CH1/PE5 connected to PWM1-CH2/PE0

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"

void PWMB_IRQHandler(void)
{
//    uint32_t u32PwmIntFlag;
    uint32_t u32CapIntFlag1;

    /* Handle PWMB Capture function */
    u32CapIntFlag1 = PWMB->CCR2;

    /* PWMB channel 2 Capture interrupt */
    if(u32CapIntFlag1 & PWM_CCR2_CAPIF2_Msk)
    {
        PWMB->CCR2 &= (PWM_CCR_MASK | PWM_CCR2_CAPIF2_Msk);
    }
}

/*--------------------------------------------------------------------------------------*/
/* Capture function to calculate the input waveform information                         */
/* u32Count[4] : Keep the internal counter value when input signal rising / falling     */
/*               happens                                                                */
/*                                                                                      */
/* time    A    B     C     D                                                           */
/*           ___   ___   ___   ___   ___   ___   ___   ___                              */
/*      ____|   |_|   |_|   |_|   |_|   |_|   |_|   |_|   |_____                        */
/* index              0 1   2 3                                                         */
/*                                                                                      */
/* The capture internal counter down count from 0x10000, and reload to 0x10000 after    */
/* input signal falling happens (Time B/C/D)                                            */
/*--------------------------------------------------------------------------------------*/
void CalPeriodTime(PWM_T *PWM, uint32_t u32Ch)
{
    uint16_t u32Count[4];
    uint32_t u32i;
    uint16_t u16RisingTime, u16FallingTime, u16HighPeriod, u16LowPeriod, u16TotalPeriod;

    /* Clear Capture Falling Indicator (Time A) */
    PWM_ClearCaptureIntFlag(PWM, u32Ch, PWM_CAPTURE_INT_FALLING_LATCH);

    /* Wait for Capture Falling Indicator  */
    while(PWM_GetCaptureIntFlag(PWM, u32Ch) < 2);

    /* Clear Capture Falling Indicator (Time B)*/
    PWM_ClearCaptureIntFlag(PWM, u32Ch, PWM_CAPTURE_INT_FALLING_LATCH);

    u32i = 0;

    while(u32i < 4)
    {
        /* Wait for Capture Falling Indicator */
        while(PWM_GetCaptureIntFlag(PWM, u32Ch) < 2);

        /* Clear Capture Falling and Rising Indicator */
        PWM_ClearCaptureIntFlag(PWM, u32Ch, PWM_CAPTURE_INT_FALLING_LATCH | PWM_CAPTURE_INT_RISING_LATCH);

        /* Get Capture Falling Latch Counter Data */
        u32Count[u32i++] = PWM_GET_CAPTURE_FALLING_DATA(PWM, u32Ch);

        /* Wait for Capture Rising Indicator */
        while(PWM_GetCaptureIntFlag(PWM, u32Ch) < 2);

        /* Clear Capture Rising Indicator */
        PWM_ClearCaptureIntFlag(PWM, u32Ch, PWM_CAPTURE_INT_RISING_LATCH);

        /* Get Capture Rising Latch Counter Data */
        u32Count[u32i++] = PWM_GET_CAPTURE_RISING_DATA(PWM, u32Ch);
    }

    u16RisingTime = u32Count[1];

    u16FallingTime = u32Count[0];

    u16HighPeriod = u32Count[1] - u32Count[2];

    u16LowPeriod = 0x10000 - u32Count[1];

    u16TotalPeriod = 0x10000 - u32Count[2];

    printf("Rising Count=%d, Falling Count=%d,\nHigh Period = %d, Low Period = %d, Total Period = %d.\n\n",
           u16RisingTime, u16FallingTime, u16HighPeriod, u16LowPeriod, u16TotalPeriod);
}

int32_t main(void)
{
    SYS_Init();
    UART_Open(UART0, 115200);
	  printf("PWM1 ch1 output to ch2 capture\n\n");

    while(1)
    {
        PWM_ConfigOutputChannel(PWM1, PWM_CH1, 250, 30);
        PWM_EnableOutput(PWM1, PWM_CH_1_MASK);
        PWM_Start(PWM1, PWM_CH_1_MASK);
			  printf("PWM1 ch1 output 250Hz duty-cycle=30 percent\n");

        PWM_ConfigCaptureChannel(PWM1, PWM_CH2, 250, 0);
        PWM1->PBCR = 1;
        PWM_EnableCaptureInt(PWM1, PWM_CH2, PWM_CAPTURE_INT_FALLING_LATCH);
        NVIC_EnableIRQ((IRQn_Type)(PWMB_IRQn));
        PWM_Start(PWM1, PWM_CH_2_MASK);
        PWM_EnableCapture(PWM1, PWM_CH_2_MASK);

        while(PWM1->PDR2 == 0);  // Wait until PWMB channel 2 Timer start to count

        CalPeriodTime(PWM1, PWM_CH2);

        PWM_Stop(PWM1, PWM_CH_1_MASK);
        while(PWM1->PDR1 != 0);
        PWM_ForceStop(PWM1, PWM_CH_1_MASK);
        PWM_DisableOutput(PWMB, PWM_CH_1_MASK);
        NVIC_DisableIRQ((IRQn_Type)(PWMB_IRQn));
        PWM_Stop(PWMB, PWM_CH_2_MASK);
        while(PWM1->PDR2 != 0);
        PWM_ForceStop(PWM1, PWM_CH_2_MASK);
        PWM_DisableCapture(PWM1, PWM_CH_2_MASK);

        PWM_ClearCaptureIntFlag(PWM1, PWM_CH2, PWM_CAPTURE_INT_FALLING_LATCH);
    }
}




