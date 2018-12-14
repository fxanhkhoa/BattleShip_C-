//
// proj_IoT
//
// EVB : Nu-LB-NUC140 (need to manually switch RSTCAPSEL)
// MCU : NUC140VE3CN
// Peripherals: 
//       LCD display                  by SPI3
//       Luminance Detection (GL5516) by ADC0
//       Ultrasound Ranger     (SR04) by Timer2-Capture
//       Motion Detection   (MPU6050) by I2C1
//       Bluetooth connection  (HC05) by UART1
//       WiFi      connection (ESP01) by UART0

// GL5516 connections:
// GL5516-pin1 to VCC
// GL5516-pin2 to ADC0/PA0, then thru 100Kohm to Gnd
 
// SR-04 connections:
// Trig connected to PB6
// Echo connected to TC2/PB2 (TC0_PB15, TC1_PE5, TC2_PB2, TC3_PB3)

// MPU6050 connections: (3-axis accelerometer & 3-axis gyroscope)
// VCC : Vcc33
// GND : Gnd
// SCL : PA11/I2C1-SCL
// SDA : PA10/I2C1-SDA
// XDA : N.C.
// XCL : N.C.
// AD0 : N.C.
// INT : N.C.

// Bluetooth module (HC05)
// pin1 KEY  : N.C
// pin2 VCC  : to VCC +5V
// pin3 GND  : to GND
// pin4 TXD  : to UART1-RX/PB4
// pin5 RXD  : to UART1-TX/PB5
// pin6 STATE: N.C.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "TFT.h"
#include "MPU6050.h"
#define PI 3.1415926535

// Global Defines
#define PRINT_LUMINANCE // Enable display of Luminance detected by Photoresistor
#define PRINT_RANGER    // Enable display of Distance detected by Ultrasound Sensor 
#define PRINT_AHRS      // Enable display of AHRS measured by IMU
#define PRINT_BT        // Enable display of BT message

// Global Variables
char Text[16];            // for Display
uint32_t u32ADC0value;    // for Luminance
uint8_t  Timer2_Flag =0;
uint32_t Timer2_Count =0;
float    SR04_Range;      // for Distance
float    theta, psi, phi; // for AHRS

char     BT_Command=' ';          // for BT command
uint8_t  BTcomRbuf[256];          // for UART1 read buffer
volatile uint8_t BTcomRbytes = 0; // UART1 read buffer pointer
volatile uint8_t BT_Flag =0;      // Bluetooth Flag
char     BT_Message[16];          // Bluetooth Message

// ADC for Photoresistor GL5516
void ADC_IRQHandler(void)
{
  uint32_t u32Flag;

  // Get ADC conversion finish interrupt flag
  u32Flag = ADC_GET_INT_FLAG(ADC, ADC_ADF_INT);

  if(u32Flag & ADC_ADF_INT) {
	u32ADC0value = ADC_GET_CONVERSION_DATA(ADC, 0); // read ADC0
  }
  ADC_CLR_INT_FLAG(ADC, u32Flag);
}

void Init_ADC(void)
{
  ADC_Open(ADC, ADC_INPUT_MODE, ADC_OPERATION_MODE, ADC_CHANNEL_MASK);
  ADC_POWER_ON(ADC);
  ADC_EnableInt(ADC, ADC_ADF_INT);
  NVIC_EnableIRQ(ADC_IRQn);
  ADC_START_CONV(ADC);	
}

// Timer2-capture for SR04
void TMR2_IRQHandler(void)
{		
  TIMER2->TEXCON &= ~(TIMER_TEXCON_RSTCAPSEL_Msk); // set RSTCAPSEL to 0 to read counter
  Timer2_Count = TIMER_GetCaptureData(TIMER2);  
  Timer2_Flag =1;
  TIMER_ClearCaptureIntFlag(TIMER2);
}

void Init_TimerCapture(void)
{
	TIMER_Open(TIMER2, TMR2_OPERATING_MODE, TMR2_OPERATING_FREQ);
  TIMER_SET_PRESCALE_VALUE(TIMER2, 11);
  // Set compare value as large as possible, so don't need to worry about counter overrun too frequently.
  TIMER_SET_CMP_VALUE(TIMER2, 0xFFFFFF);	
  TIMER_EnableCapture(TIMER2, TMR2_CAPTURE_MODE, TMR2_CAPTURE_EDGE);	
  TIMER_EnableCaptureInt(TIMER2); // Enable timer interrupt
  NVIC_EnableIRQ(TMR2_IRQn);
}

void Init_SR04_Trig(void)
{
  GPIO_SetMode(PB, BIT6, GPIO_MODE_OUTPUT); // for SR04 Trig
  PB6=0;
}

void Init_PWM0(void)
{
  PWM_ConfigOutputChannel(PWM0, PWM_CH0, 50, 3);
  PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
  PWM_Start(PWM0, PWM_CH_0_MASK);
  CLK_SysTickDelay(500000);		
}

// UART1 to receive data from Bluetooth HC-05
void UART1_IRQHandler(void)
{
  uint8_t c, i;
  uint32_t u32IntSts = UART1->ISR;
	
  if(u32IntSts & UART_IS_RX_READY(UART1)) // check ISR on & RX is ready
  {
		while (!(UART1->FSR & UART_FSR_RX_EMPTY_Msk)){ // check RX is not empty
			c = UART_READ(UART1); // read UART RX data
			if (c!='\n') {        // check line-end 
				BTcomRbuf[BTcomRbytes] = c;
				BTcomRbytes++;
			} else {
				BT_Command=BTcomRbuf[0];       // BT command = first charactor received
				for (i=0; i<BTcomRbytes; i++)	 BT_Message[i]=BTcomRbuf[i]; // store received data to Message
				for (i=BTcomRbytes; i<16; i++) BT_Message[i]=' ';
				BTcomRbytes=0;                 // clear Read buffer pointer
				BT_Flag=1;	                   // set flag when BT command input
				break;
			}
		}		
	}
}

void Init_UART1(void)
{ 
  UART_Open(UART1, 9600);                       // set UART1 baud rate
  UART_ENABLE_INT(UART1, UART_IER_RDA_IEN_Msk); // enable UART1 interrupt (triggerred by Read-Data-Available)
  NVIC_EnableIRQ(UART1_IRQn);		                // enable Cortex-M0 NVIC interrupt for UART1
}


// GPIO to drive DC motor / LEDs
void Init_GPIO(void)
{
  GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
  GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
  GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT);
  GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT);
  PC12=0; PC13=0; PC14=0; PC15=0;
}

void Motor_Forward(void) // To move forward
{ PC12=1; PC13=0; PC14=1; PC15=0;}	// GPIO pins control Motor

void Motor_Backward(void) // To move backward
{ PC12=0; PC13=1; PC14=0; PC15=1;}

void Motor_Right(void) // To turn right
{ PC12=1; PC13=0; PC14=0; PC15=1;}

void Motor_Left(void) // To turn left
{ PC12=0; PC13=1; PC14=1; PC15=0;}	

void Motor_Stop(void) // To stop motors
{ PC12=0; PC13=0; PC14=0; PC15=0;}

//-----------------------------Service Routines-------------------------------------
void Ultrasound(void)
{
  SR04_Range=555;
  PB6=1;
  CLK_SysTickDelay(7);
  PB6=0;			
  TIMER2->TEXCON |= TIMER_TEXCON_RSTCAPSEL_Msk;    // set RSTCAPSEL to 1 to reset counter
			
  TIMER_Start(TIMER2);		
  CLK_SysTickDelay(30000);
  TIMER_Stop(TIMER2);
}

void Check_BTCommand(void)
{ 
	if (BT_Flag==1) { // check if bluetooth sent new command
	   if      (BT_Command=='F') Motor_Forward();
	   else if (BT_Command=='B') Motor_Backward();
	   else if (BT_Command=='R') Motor_Right();
	   else if (BT_Command=='L') Motor_Left();
	   else if (BT_Command=='S') Motor_Stop();		
		 BT_Flag=0;		
	}
}

void AHRS(void)
{
  int16_t AccX, AccY, AccZ;
  AccX = Read_MPU6050_AccX();
  AccY = Read_MPU6050_AccY();
  AccZ = Read_MPU6050_AccZ();	
			
  // calculate tilt (degree = radians *180 / PI)
  theta = atan(AccX / sqrt(pow(AccY,2)+pow(AccZ,2))) *180 /PI;		
  psi   = atan(AccY / sqrt(pow(AccX,2)+pow(AccZ,2))) *180 /PI;
  phi   = atan(sqrt(pow(AccX,2)+pow(AccY,2))/AccZ)   *180 /PI;	
}

void Display_Message(void)
{
	#ifdef PRINT_LUMINANCE
	sprintf(Text,"ADC0: %4d",u32ADC0value);
	print_Line(0, Text);
	#endif
	
	#ifdef PRINT_RANGER
	if (Timer2_Flag==1) {
		SR04_Range = Timer2_Count * 340 /20000;
		sprintf(Text,"SR04: %5d cm", (int) SR04_Range);	
		print_Line(1,Text);
		Timer2_Flag=0;	
	}
	#endif
	
	#ifdef PRINT_AHRS
	sprintf(Text,"AHRS:%3d %3d %3d", (int)theta, (int)psi, (int)phi);
	print_Line(2, Text);
	#endif
	
	#ifdef PRINT_BT
	print_Line(3, BT_Message);
	#endif
}

//-----------------main loop----------------------
int main(void)
{
  SYS_Init();
  init_LCD();
  clear_LCD();
  print_Line(0, "proj IoT");	
	
  // Initialization for Ultrasound Ranger (SR04)
  Init_SR04_Trig();      // for SR04 Trig
  Init_TimerCapture();	 // for SR04 Echo
  // Initialization for reading Photoresistor (GL5516)
  Init_ADC();
  // Initialization for Inertial Measurment (MPU6050)
  I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);	// I2C1 at 400KHz
  Init_MPU6050();                       
  // Initialization for GPIOs driving DC motor / LEDs
  Init_GPIO();
  // Initialization for Bluetooth HC-05
  Init_UART1(); // UART1 at 9600bps
	
  while(1) {
    Ultrasound();
    AHRS();
    Check_BTCommand();
    Display_Message();	
  }
}
