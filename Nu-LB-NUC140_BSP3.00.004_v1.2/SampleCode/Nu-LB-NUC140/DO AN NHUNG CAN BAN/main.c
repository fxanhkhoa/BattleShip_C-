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

uint8_t id = 1, command, dim = 1;
int x = 0, y = 0;
//u32 RegValue = 0;
uint8_t buffer[10] = "";
uint8_t input_buffer[10] = "";
uint8_t pos = 0;

uint8_t *build_buffer();

void UART02_IRQHandler(void)
{
	uint8_t c, i;
	uint32_t u32IntSts = UART0->ISR;
	
	if(u32IntSts & UART_IS_RX_READY(UART0)) // check ISR on & RX is ready
  {
		while (!(UART0->FSR & UART_FSR_RX_EMPTY_Msk)){ // check RX is not empty
			c = UART_READ(UART0); // read UART RX data
			PA14 ^= 1;
			
		}		
	}
}

void Init_UART(void)
{
	UART_Open(UART0,14400);
}


int main(void)
{
	uint8_t i;
	char Text[16];
	
	SYS_Init();
	GPIO_SetMode(PA, BIT14, GPIO_MODE_OUTPUT);
	OpenKeyPad_new();
	Init_UART();
	//UART_ENABLE_INT(UART0, UART_IER_RDA_IEN_Msk); // enable UART0 interrupt (triggerred by Read-Data-Available)
  //NVIC_EnableIRQ(UART02_IRQn);		              // enable Cortex-M0 NVIC interrupt for UART02
	PA14 = 0;
		
	// 1st init
	x = 0;
	y = 0;
	pos = 0;
	
	sprintf(Text, "UART0 Transmit\r\n");
	UART_Write(UART0, Text, 16);
	
	while(1) 
	{
		UART_Write(UART0, Text, 16);
		CLK_SysTickDelay(100000);
		i=ScanKey_new();
		if (i == 1) // up
		{
			PA14 =1 ; // turn on LED
			x--;
			command = 7;
			UART_Write(UART0, build_buffer(), 10);
			while (i != 0) i = ScanKey_new();
		}
		else if (i == 2) // down
		{
			PA14 =1 ; // turn on LED
			x++;
			command = 7;
			UART_Write(UART0, build_buffer(), 10);
			while (i != 0) i = ScanKey_new();
		}
		else if (i == 3) // right
		{
			PA14 =1 ; // turn on LED
			y++;
			command = 7;
			UART_Write(UART0, build_buffer(), 10);
			while (i != 0) i = ScanKey_new();
		}
		else if (i == 4) // left
		{
			PA14 =1 ; // turn on LED
			y--;
			command = 7;
			UART_Write(UART0, build_buffer(), 10);
			while (i != 0) i = ScanKey_new();
		}
		else if (i == 5) // OK
		{
			PA14 =1 ; // turn on LED
			command = 2;
			UART_Write(UART0, build_buffer(), 10);
			while (i != 0) i = ScanKey_new();
		}
		else if (i == 6) // CANCEL
		{
			PA14 =1 ; // turn on LED
			if (dim == 1) 
			{
				dim = 2;
			}
			else if (dim == 2)
			{
				dim = 1;
			}
			while (i != 0) i = ScanKey_new();
		}
		else if (i == 0)
		{
			PA14 = 0;
		}
	}
}

uint8_t *build_buffer()
{
	int i;
	buffer[0] = id;
	buffer[1] = command;
	if (x == 8) x = 0;
	if (x == -1) x = 7;
	buffer[2] = x;
	if (y == 8) y = 0;
	if (y == -1) y = 7;
	buffer[3] = y;
	buffer[4] = dim;
	buffer[5] = 0;
	buffer[6] = 0;
	buffer[7] = 0;
	buffer[8] = 0;
	buffer[9] = 42; // dau *
	/*for (i = 5; i < 20; i++)
	{
		buffer[i] = 0;
	}*/
	return buffer;
}
