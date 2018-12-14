//
// I2C_MPU6050_Tilt : using I2C to read Accelerometer to calculate tilt angles
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN (LQFP100)
// IMU : MPU6050 (3-axis accelerometer & 3-axis gyroscope)
// 
// MPU6050 Connections
// SCL : to I2C1-SCL/PA11 (LQFP100 pin9 )
// SDA : to I2C1-SDA/PA10 (LQFP100 pin10)

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "MPU6050.h"
#define PI 3.14159265359
#define RAD_TO_DEG  180/PI // degree = radians * 180 /  PI

char Text[16];

int32_t main()
{
  int16_t AccX, AccY, AccZ;
  float theta, psi, phi;
	
  SYS_Init();
	init_LCD();
	clear_LCD();
	print_Line(0,"MPU6050 Tilt");
	
  I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);	
  Init_MPU6050();

  while(1) {
    AccX = Read_MPU6050_AccX();
    AccY = Read_MPU6050_AccY();
    AccZ = Read_MPU6050_AccZ();				
			
    // calculate tilt (degree = radians *180 / PI)
    theta = atan(AccX / sqrt(pow(AccY,2)+pow(AccZ,2))) * RAD_TO_DEG;		
    psi   = atan(AccY / sqrt(pow(AccX,2)+pow(AccZ,2))) * RAD_TO_DEG;
    phi   = atan(sqrt(pow(AccX,2)+pow(AccY,2))/AccZ)   * RAD_TO_DEG;
		
    sprintf(Text,"theta=%6d", (int) theta);
		print_Line(1, Text);
    sprintf(Text,"psi  =%6d", (int) psi);
		print_Line(2, Text);
    sprintf(Text,"phi  =%6d", (int) phi);
		print_Line(3, Text);
    }
}
