//
// proj_IMU_DCservo : project sample code for DC servo motor controlled 
//                    by tilt angle measured by reading IMU
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
// IMU : MPU6050 
//
// Connections :
// MPU6050 - 3-axis accelerometer & 3-axis gyroscope
// PA11 /I2C1-SCL connected to MPU6050-SCL
// PA10 /I2C1-SDA connected to MPU6050-SDA
//
// SG5010 DC servo
// pin1 : signal to PA12/PWM0_CH0
// pin2 : Vcc
// pin3 : Gnd

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "MPU6050.h"
#define PI 3.14159265359

void Init_PWM(void)
{
	  PWM_ConfigOutputChannel(PWM0, PWM_CH0, 50, 3);	
	  PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
    PWM_Start(PWM0, PWM_CH_0_MASK);
}

int32_t main()
{
	  int8_t duty_cycle;
    int16_t AccX, AccY, AccZ;
    float theta, psi, phi;
	  int16_t pitch;
	
    SYS_Init();	
    I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);	
    Init_MPU6050();
	  Init_PWM();

    while(1) {
      AccX = Read_MPU6050_AccX();
      AccY = Read_MPU6050_AccY();
      AccZ = Read_MPU6050_AccZ();		
      //printf("Accelerometer: %6d, %6d, %6d\n", AccX, AccY, AccZ);
			
      // calculate tilt (degree = radians *180 / PI)
      theta = atan(AccX / sqrt(pow(AccY,2)+pow(AccZ,2))) *180 /PI;		
      psi   = atan(AccY / sqrt(pow(AccX,2)+pow(AccZ,2))) *180 /PI;
      phi   = atan(sqrt(pow(AccX,2)+pow(AccY,2))/AccZ)   *180 /PI;
//      printf("theta=%d, psi=%d, phi=%d\n", (int) theta, (int) psi, (int) phi);
			
		  pitch = (int) theta;
      duty_cycle = (pitch + 90)/18 +3; // duty_cycle = 3~13%
//      printf("pitch=%d, duty_cycle=%d\n", pitch, duty_cycle);
		
			PWM_ConfigOutputChannel(PWM0, PWM_CH0, 50, duty_cycle);
			CLK_SysTickDelay(100000);
    }
}
