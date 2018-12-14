//
// GPIO_LED : GPIO output to control an on-board red LED
// 
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// low-active output control by GPC12

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"

int main(void)
{
    SYS_Init(); 
    GPIO_SetMode(PA, BIT14, GPIO_MODE_OUTPUT);
	
    while(1) {
	  PA14 =0; // turn on LED
	  CLK_SysTickDelay(100000);	 // Delay 
	  PA14 =1; // turn off LED
	  CLK_SysTickDelay(100000);	 // Delay 			
		}
}
