//
// SPI_RFID-RC522 : SPI interfacing with RC522 RFID reader
//
// MCU : NUC140VE3CN
// EVB : Nu-LB-NUC140
// SPI : RFID-RC522
//
// RC522 RFID Reader
// SDA (SS) : connected to GPC8 (SPI1_SS)
// SCK (SCK): connected to GPC9 (SPI1_CLK)
// MOSI     : connected to GPC11(SPI1_MOSI)
// MISO     : connected to GPC10(SPI1_MISO)
// IRQ      : no connection
// GND      : connected to Gnd
// RST      : connected to 3.3V
// VCC      : connected to 3.3V

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "SPI_RC522.h"

unsigned char UID[4],Temp[4]                                       ;
unsigned char RF_Buffer[18]                                        ;
unsigned char Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}   ; // Mifare One ????

char          MBRX[30]                                             ;
char          MBKeyTP[30]                                          ;
char          Event                                                ;
unsigned char DISP_MODE,i                                            ; // ????????
unsigned char des_on       = 0                                     ; // DES????

void RED_LED_Flash(uint8_t no)
{
	uint8_t i;
	for (i=0; i<no; i++) {
		PC12=0;
		CLK_SysTickDelay(100000);
		PC12=1;
		CLK_SysTickDelay(100000);
	}
}

void Auto_Reader(void)
{
	char Text[16];
  while(1)
  {
    if(PcdRequest(0x52,Temp)==MI_OK)
    {
      if(Temp[0]==0x04&&Temp[1]==0x00)  
				print_Line(1,"MFOne-S50    ");
      else if(Temp[0]==0x02&&Temp[1]==0x00)
				print_Line(1,"MFOne-S70    ");
      else if(Temp[0]==0x44&&Temp[1]==0x00)
				print_Line(1,"MF-UltraLight");
      else if(Temp[0]==0x08&&Temp[1]==0x00)
				print_Line(1,"MF-Pro       ");
      else if(Temp[0]==0x44&&Temp[1]==0x03)
				print_Line(1,"MF Desire    ");
      else
        print_Line(1,"Unknown Card ");
			
      if(PcdAnticoll(UID)==MI_OK)
      { 
        sprintf(Text,"UID :%x %x %x %x", UID[0],UID[1],UID[2],UID[3]);
				print_Line(2,Text);
        RED_LED_Flash(1); 
        //while (!(IFG1 & UTXIFG0));
        //TXBUF0 = '\n';
      }
			else 
			  print_Line(2,"UID :           ");
    }
		else 
			print_Line(1,"No Card      ");    
  } 
}

void Init_SPI()
{
	SPI_Open(RC522_SPI_PORT, SPI_MASTER, SPI_MODE_0, RC522_SPI_DATAWIDTH, RC522_SPI_SPEED);
	SPI_DisableAutoSS(RC522_SPI_PORT);
	SPI_SET_MSB_FIRST(RC522_SPI_PORT);
}

int main( void )
{
	SYS_Init();

	GPIO_SetMode(PC, (BIT12 | BIT13), GPIO_MODE_OUTPUT);
	PC12=1; PC13=1;
	
	Init_SPI();  // initialize SPI1
	
	init_LCD();  // initialize LCD
	clear_LCD();
	print_Line(0,"SPI RC522");
	
  RED_LED_Flash(2);	
	
  PcdReset();
  PcdAntennaOn();
	
  Auto_Reader();
}
