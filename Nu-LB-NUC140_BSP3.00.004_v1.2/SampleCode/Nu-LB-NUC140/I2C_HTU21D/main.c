//
// I2C_HTU21D : using I2C to read humidity & temperature from sensor
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
// HTU21D : Digital Relative Humidity Sensor with Temperature Output

// HTU21D connections
// Gnd : GND
// Vdd : 3.3V
// SDA : to I2C1_SDA/PA11 (LQFP100 pin9)
// SCL : to I2C1_SCL/PA10 (LQFP100 pin10)

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "HTU21D.h"	

int32_t main()
{
	uint8_t  reg;
  float    Humidity, Temperature;
	
  SYS_Init();	
  I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY); // max.=400KHz
  printf("I2C1 to HTU21D...\n");
	
  reg   = HTU21D_Init();
  printf("user reg.=%x\n", reg);
	  
  while(1) {
    Humidity    = HTU21D_Humidity();
    Temperature = HTU21D_Temperature();			
    printf("Humidity=%f, Temperature = %f\n", Humidity, Temperature);
  }
}
