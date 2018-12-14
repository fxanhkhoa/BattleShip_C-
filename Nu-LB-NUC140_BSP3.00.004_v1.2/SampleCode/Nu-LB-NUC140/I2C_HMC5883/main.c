//
// I2C_MC5883 : using I2C to read HMC5883 magnentometer/compass
//
// EVB    : Nu-LB-NUC140
// MCU    : NUC140VE3CN (LQFP100)
// Sensor : HMC5883 (magnentometer/compass)
//
// HMC5883 Connections :
// VCC : to VCC33 (3.3V)
// GND : to Gnd
// SCL : to I2C1-SCL / PA11 (LQFP100 pin9)
// SDA : to I2C1-SDA / PA10 (LQFP100 pin10)

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "HMC5883.h"

int32_t main (void)
{
  char Text[16];
  float X,Y,Z;
  SYS_Init();

  init_LCD(); 
  clear_LCD();
  print_Line(0,"Magnentometer");

  I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);
  init_HMC5883();									  
		
  while(1) {
	  sprintf(Text,"x=", X); print_Line(1,Text);
	  sprintf(Text,"y=", Y); print_Line(2,Text);
	  sprintf(Text,"z=", Z); print_Line(3,Text);	
  }
}
