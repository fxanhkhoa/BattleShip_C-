//
// TMR_Capture_PWM : Timer Capture PWM output
//
// EVB : Nu-LB-NUC140 (need to manually switch RSTCAPSEL)
// MCU : NUC140VE3CN

// Connection:
// PWM0_CH0/PA12 connected to TC2/PB2

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"

char Text[16];
uint8_t  Timer2_flag =0;
uint32_t Timer2_count =0;

void TMR2_IRQHandler(void)
{		
  TIMER2->TEXCON &= ~(TIMER_TEXCON_RSTCAPSEL_Msk); // set RSTCAPSEL to 0 to read counter
  Timer2_count = TIMER_GetCaptureData(TIMER2);  
  Timer2_flag =1;
  TIMER_ClearCaptureIntFlag(TIMER2);
}


void Init_TimerCapture(void)
{
  TIMER_Open(TIMER2, TMR2_OPERATING_MODE, TMR2_OPERATING_FREQ);
  TIMER_SET_PRESCALE_VALUE(TIMER2, 11);
  // Set compare value as large as possible, so don't need to worry about counter overrun too frequently.
  TIMER_SET_CMP_VALUE(TIMER2, 0xFFFFFF);	
  TIMER_EnableCapture(TIMER2, TMR2_CAPTURE_MODE, TMR2_CAPTURE_EDGE);	
  TIMER_EnableCaptureInt(TIMER2); // Enable timer interrupt
  NVIC_EnableIRQ(TMR2_IRQn);
  TIMER2->TEXCON |= TIMER_TEXCON_RSTCAPSEL_Msk; // set RSTCAPSEL to 1 to reset counter
  TIMER_Start(TIMER2);    
}

void Init_PWM(void)
{
    PWM_ConfigOutputChannel(PWM0, PWM_CH0, 50, 30);
    PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
    PWM_Start(PWM0, PWM_CH_0_MASK);
}

int main(void)
{
	SYS_Init();
  init_LCD();
  clear_LCD();
	
  print_Line(0, "TimerCapture PWM");
	
	Init_PWM();
  Init_TimerCapture();	
	  
  while(1) {
    if (Timer2_flag==1){
			sprintf(Text,"TC2= %5d", Timer2_count);
			print_Line(2,Text);
		}
  }
}
