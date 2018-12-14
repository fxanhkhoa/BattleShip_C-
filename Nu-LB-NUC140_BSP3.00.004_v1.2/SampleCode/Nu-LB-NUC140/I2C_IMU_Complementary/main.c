//
// I2C_IMU_Complementary : to calculate AHRS using Complementary filter
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

// Complementary filter parameters
#define ACCEL_SENSITIVITY 16384  // 32768/2 = 16384 LSB per 1g Gravity
#define GYRO_SENSITIVITY  16.384 // 32768/2000 = 16.384 LSB per degree/s
#define ALPHA 0.98

// Global Variables for IMU
char Text[16];
float dt = 0.01; // sample time = 100Hz (10ms)
int16_t Gyro_out[3], Acc_out[3];
int16_t Gyro_rate[2];
float Gyro_pitch, Acc_pitch;
float Gyro_roll,  Acc_roll;
float Comp_pitch, Comp_roll;

void getGyroscopeReading(int16_t Gyro[3])
{
	Gyro[0]=Read_MPU6050_GyroX();
	Gyro[1]=Read_MPU6050_GyroY();
	Gyro[2]=Read_MPU6050_GyroZ();
}

void getAccelerometerReading(int16_t Acc[3])
{
	Acc[0]=Read_MPU6050_AccX();
	Acc[1]=Read_MPU6050_AccY();
	Acc[2]=Read_MPU6050_AccZ();
}

int32_t main()
{
  SYS_Init();
  init_LCD();
  clear_LCD();
  print_Line(0,"Complementary");
	
  I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);	
  Init_MPU6050();
  Comp_pitch=0;
  Comp_roll=0;
  
  while(1) {
		getGyroscopeReading(Gyro_out);
		getAccelerometerReading(Acc_out);
		
		// IMU's X = pitch
		Acc_pitch = atan(Acc_out[0]/ sqrt(pow(Acc_out[1],2)+pow(Acc_out[2],2))) * RAD_TO_DEG;
		Gyro_rate[0]= Gyro_out[0] /GYRO_SENSITIVITY;
		Gyro_pitch  = Gyro_pitch + Gyro_rate[0] *dt;	
		// IMU's Y = roll
		Acc_roll  = atan(Acc_out[1]/ sqrt(pow(Acc_out[0],2)+pow(Acc_out[2],2))) * RAD_TO_DEG;
		Gyro_rate[1]= Gyro_out[1] /GYRO_SENSITIVITY;	
		Gyro_roll   = Gyro_roll +  Gyro_rate[1] *dt;
		// Complimentary Filter
		Comp_pitch = (Comp_pitch + Gyro_rate[0] *dt) * ALPHA + Acc_pitch * (1 - ALPHA);
		Comp_roll  = (Comp_roll  + Gyro_rate[1] *dt) * ALPHA + Acc_roll  * (1 - ALPHA);
		
    sprintf(Text,"pitch=%3d %3d", (int)Comp_pitch, (int)Acc_pitch);
		print_Line(1, Text);
    sprintf(Text,"roll =%3d %3d", (int)Comp_roll,  (int)Acc_roll);
		print_Line(2, Text);
    }
}
