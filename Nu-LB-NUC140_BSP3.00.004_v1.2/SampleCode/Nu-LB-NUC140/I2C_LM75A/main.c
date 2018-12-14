//
// I2C_LM75A : using I2C to read Temperature sensor
//
// EVB   : Nu-LB-NUC140
// MCU   : NUC140VE3CN LQFP100
// Sensor: LM75A (Temperature sensor)
//
// LM75A Connections :
// SCL : to I2C1-SCL/PA11 (LQFP100 pin9 )
// SDA : to I2C1-SDA/PA10 (LQFP100 pin10)

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LM75A.h"

int32_t main()
{
    int8_t  msb, lsb;
    uint16_t tmp16;
	
    SYS_Init();	
    I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);
    printf("I2C1 interface to LM75A\n");

    while(1) {
      tmp16 = Read_LM75A_Temp();
      msb   = (tmp16 & 0xFF00)>>8;
      lsb   = (tmp16 & 0x00FF)>>5;
      if (msb<0) lsb = ~lsb;
      printf("Temperature = %d.%d\n", msb, lsb);	
    }
}
