//
// UART_TX : UART Transmit a string to USB-UART com port
//                  
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN (LQFP100)
// UART: UART0 set at baudrate=115200, databit=8, stopbit=1, paritybit=0, flowcontrol=None
//
// USB-UART Connections
// 3V3   to Vcc
// TXD   to PB0 /UART0-RX
// RXD   to PB1 /UART0-TX
// GND   to Gnd
// +5V   N.C.

#include <stdio.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"

void Init_UART(void)
{
	UART_Open(UART0,14400);
}

int32_t main()
{
	char Text[16];
	uint8_t a = 'A';
	
  SYS_Init();	
  Init_UART();

  sprintf(Text, "UART0 Transmit\r\n");

  while(1) {
	  UART_Write(UART0, Text, 16);
		CLK_SysTickDelay(100000);
  }
}
