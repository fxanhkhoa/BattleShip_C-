//
// GPIO_7seg_keypad : 3x3 keypad inpt and display on 7-segment LEDs
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"
#include "Scankey.h"

// display an integer on four 7-segment LEDs
void Display_7seg(uint16_t value)
{
  uint8_t digit;
	digit = value / 1000;
	CloseSevenSegment();
	ShowSevenSegment(3,digit);
	CLK_SysTickDelay(5000);
			
	value = value - digit * 1000;
	digit = value / 100;
	CloseSevenSegment();
	ShowSevenSegment(2,digit);
	CLK_SysTickDelay(5000);

	value = value - digit * 100;
	digit = value / 10;
	CloseSevenSegment();
	ShowSevenSegment(1,digit);
	CLK_SysTickDelay(5000);

	value = value - digit * 10;
	digit = value;
	CloseSevenSegment();
	ShowSevenSegment(0,digit);
	CLK_SysTickDelay(5000);
}

int main(void)
{
	uint8_t i;
	
    SYS_Init();
		GPIO_SetMode(PA, BIT14, GPIO_MODE_OUTPUT);
    //OpenSevenSegment();
	  OpenKeyPad_new();
		PA14 = 0;
		
 	  while(1) 
		{
			i=ScanKey_new();
			if (i == 1) // up
			{
				PA14 =1 ; // turn on LED
			}
			else if (i == 2) // down
			{
				
			}
			else if (i == 3) // right
			{
				
			}
			else if (i == 4) // left
			{
				
			}
			else if (i == 5) // OK
			{
				
			}
			else if (i == 6) // CANCEL
			{
				
			}
			else if (i == 0)
			{
				PA14 = 0;
			}
	  }
}
