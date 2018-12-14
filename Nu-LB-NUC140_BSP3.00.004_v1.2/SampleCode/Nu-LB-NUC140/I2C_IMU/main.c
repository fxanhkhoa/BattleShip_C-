//
// I2C_MPU6050 :  using I2C to read IMU and print on Debug Session View
//
// Board       : Nu-LB-NUC140
// MCU         : NUC140VE3CN (LQFP100)
// IMU         : MPU6050 (3-axis accelerometer & 3-axis gyroscope)
//
// MPU6050 Connections :
// SCL : to I2C1-SCL/PA11 (LQFP100 pin9 )
// SDA : to I2C1-SDA/PA10 (LQFP100 pin10)

#include <stdio.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "MPU6050.h"

char Text[16];

int32_t main()
{
	int16_t AccX, AccY, AccZ;
  int16_t GyroX, GyroY, GyroZ;
	
  SYS_Init();
	init_LCD();
	clear_LCD();
  print_Line(0,"IMU: MPU6050");	
  
	I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);
  Init_MPU6050();

  while(1) {
    AccX = Read_MPU6050_AccX();
    AccY = Read_MPU6050_AccY();
    AccZ = Read_MPU6050_AccZ();		

    GyroX = Read_MPU6050_GyroX();
    GyroY = Read_MPU6050_GyroY();
    GyroZ = Read_MPU6050_GyroZ();
			
    sprintf(Text,"x:%6dX:%6d", AccX, GyroX);
	  print_Line(1,Text);
    sprintf(Text,"y:%6dY:%6d", AccY, GyroY);
		print_Line(2,Text);			
    sprintf(Text,"z:%6dZ:%6d", AccZ, GyroZ);
		print_Line(3,Text);			
  }
}
