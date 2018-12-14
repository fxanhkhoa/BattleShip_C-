//
// I2C_IMU_Kalman : to calculate AHRS using Kalman filter
//                  using Timer to calculate AHRS every 10ms (100Hz)
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
#include "TFT.h"
#include "MPU6050.h"
#define PI 3.14159265359
#define RAD_TO_DEG  180/PI

// Complementary filter parameters
#define ACCEL_SENSITIVITY 16384  // 32768/2 = 16384 LSB per 1g Gravity
#define GYRO_SENSITIVITY  16.384 // 32768/2000 = 16.384 LSB per degree/s
#define ALPHA 0.98

// Global Variables for IMU
char Text[16];
int16_t Gyro_out[3], Acc_out[3];
float dt = 0.01;
int16_t Gyro_rate[2];
float Gyro_pitch, Acc_pitch;
float Gyro_roll,  Acc_roll;
float Comp_pitch, Comp_roll;
float GyroX_cal, GyroY_cal, GyroZ_cal;
float AccX_cal,  AccY_cal,  AccZ_cal;

// Global Variables for Kalman filter prediction
float Kalman_pitch, Kalman_roll;
float Q=0.1; // Prediction Estimate Initial Guess
float R=5;   // Prediction Estimate Initial Guess
float P00=0.1; // Preidction Estiamte Iintial Guess
float P11=0.1; // Preidction Estiamte Iintial Guess
float P01=0.1; // Preidction Estiamte Iintial Guess
float Kk0, Kk1;

uint8_t Timer_Flag;

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

// IMU initial calibration : read 100 times and average the value
void calibrate_IMU(void)
{
	uint8_t i;
	int32_t gyroX_sample, gyroY_sample, gyroZ_sample;
	int32_t accX_sample,  accY_sample,  accZ_sample;
	for (i=0; i<100; i++){
	  getGyroscopeReading(Gyro_out);
	  getAccelerometerReading(Acc_out);
		gyroX_sample += Gyro_out[0];
		gyroY_sample += Gyro_out[1];
		gyroZ_sample += Gyro_out[2];
		accX_sample  += Acc_out[0];
		accY_sample  += Acc_out[1];
		accZ_sample  += Acc_out[2];
		CLK_SysTickDelay(500);
	}
	GyroX_cal = gyroX_sample / 100;
	GyroY_cal = gyroY_sample / 100;
	GyroZ_cal = gyroZ_sample / 100;	
	AccX_cal  = accX_sample  / 100;
	AccY_cal  = accY_sample  / 100;
	AccZ_cal  = accZ_sample  / 100 - ACCEL_SENSITIVITY; // 1g Gravity offset
}

void LCD_Display(void)
{
	sprintf(Text,"pitch=%3d %3d", (int)Kalman_pitch, (int)Comp_pitch);
	print_Line(1, Text);
  sprintf(Text,"roll =%3d %3d", (int)Kalman_roll,  (int)Comp_roll);
	print_Line(2, Text);
}

void Complementary_filter(void)
{
  // Complimentary Filter
  Acc_pitch = atan(Acc_out[0]/ sqrt(pow(Acc_out[1],2)+pow(Acc_out[2],2))) * RAD_TO_DEG;	
  Acc_roll  = atan(Acc_out[1]/ sqrt(pow(Acc_out[0],2)+pow(Acc_out[2],2))) * RAD_TO_DEG;

  Gyro_rate[0]= Gyro_out[0] /GYRO_SENSITIVITY;
  Gyro_pitch  = Gyro_pitch + Gyro_rate[0] *dt;
	
  Gyro_rate[1]= Gyro_out[1] /GYRO_SENSITIVITY;	
  Gyro_roll   = Gyro_roll +  Gyro_rate[1] *dt;

  Comp_pitch = (Comp_pitch + Gyro_rate[0] *dt) * ALPHA + Acc_pitch * (1 - ALPHA);
  Comp_roll  = (Comp_roll  + Gyro_rate[1] *dt) * ALPHA + Acc_roll  * (1 - ALPHA);
}

void Kalman_filter(void)
{
  //--- Kalman Filter ---
  // Time Update step 1
  Kalman_pitch = Kalman_pitch + ((Gyro_out[0] - GyroX_cal) / GYRO_SENSITIVITY) * dt; 
  Kalman_roll  = Kalman_roll  - ((Gyro_out[1] - GyroY_cal) / GYRO_SENSITIVITY) * dt;	
  // Measurement Update step 1
  //   Projected error covariance terms from derivation result
  P00 += dt * (2 * P01 + dt * P11); 
  P01 += dt * P11; 
  P00 += dt * Q;
  P11 += dt * Q;
	
  Kk0 = P00 / (P00 + R);
  Kk1 = P01 / (P01 + R); 
	
  // Measurement Update step 2
  Kalman_pitch += (Acc_pitch - Kalman_pitch) * Kk0; 
  Kalman_roll  += (Acc_roll  - Kalman_roll)  * Kk0;	
	
  // Measurement Update step 3
  P00 *= (1 - Kk0);
  P01 *= (1 - Kk1);
  P11 -= Kk1 * P01;	
}

void TMR0_IRQHandler(void)
{
  getGyroscopeReading(Gyro_out);
  getAccelerometerReading(Acc_out);
	Complementary_filter(); // calculate pitch & roll using Complementary filter
	Kalman_filter();        // calculate pitch & roll using Kalman filter
	Timer_Flag=1;	
  TIMER_ClearIntFlag(TIMER0);
}

void Init_Timer(void)
{
  TIMER_Open(TIMER0, TMR0_OPERATING_MODE, TMR0_OPERATING_FREQ);
  TIMER_EnableInt(TIMER0);
  NVIC_EnableIRQ(TMR0_IRQn);
  TIMER_Start(TIMER0);	
}

int32_t main()
{
  SYS_Init();
	init_LCD();
	clear_LCD();
	print_Line(0,"Kalman filter");
	
  I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);	
  Init_MPU6050();
	calibrate_IMU();

  Timer_Flag=0;	
	Init_Timer();

  while(1) {
		if (Timer_Flag==1) 
		{	
			LCD_Display();
			Timer_Flag=0;
		} 
	}
}
