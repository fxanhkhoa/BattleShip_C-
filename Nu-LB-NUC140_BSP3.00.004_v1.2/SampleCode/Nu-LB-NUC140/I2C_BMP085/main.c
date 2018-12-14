//
// I2C_BMP085 : using I2C to read temperature and pressure from Barometer
//
// EVB    : Nu-LB-NUC140
// MCU    : NUC140VE3CN (LQFP100)
// Sensor : BMP085 (Barometer)
//
// BMP085 Connections :
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
#include "BMP085.h"

int32_t main (void)
{
	char TEXT[16];
	
	uint16_t UT, T; // temperature
	uint32_t UP, P; // pressure
	
	SYS_Init();

	init_LCD(); 
	clear_LCD();
  print_Line(0,"Barometer BMP085");

	I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);
									  
	// BMP085 calibration (read out its EEPROM registers)
  BMP085Calibration();
		
	while(1) {
		// read BMP085
		UT = BMP085_ReadUT();
		UP = BMP085_ReadUP();		
    T = BMP085GetTemperature(UT);
		P = BMP085GetPressure(UP);
    //A = (float)44330 * (1 - pow(((float) P/p0), 0.190295));		
    // print to LCD			
		sprintf(TEXT,"T= %6d .1C", T); print_Line(1,TEXT);
		sprintf(TEXT,"P= %6d  Pa", P); print_Line(2,TEXT);
	}
}
