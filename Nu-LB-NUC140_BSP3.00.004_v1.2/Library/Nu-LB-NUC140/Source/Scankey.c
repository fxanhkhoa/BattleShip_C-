/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright (c) Nuvoton Technology Corp. All rights reserved.                                             */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <NUC100Series.h>
#include "GPIO.h"
#include "Scankey.h"
#include "SYS_init.h"

void delay(void)
{
	int j;
	for(j=0;j<1000;j++);
}

void OpenKeyPad(void)
{
  GPIO_SetMode(PA, BIT0, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT1, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT2, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT3, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT4, GPIO_MODE_QUASI);
  GPIO_SetMode(PA, BIT5, GPIO_MODE_QUASI);						
}

void OpenKeyPad_new(void)
{
	GPIO_SetMode(PD, BIT7, GPIO_MODE_QUASI);
	GPIO_SetMode(PC, BIT1, GPIO_MODE_QUASI);
	GPIO_SetMode(PC, BIT2, GPIO_MODE_QUASI);
	GPIO_SetMode(PC, BIT3, GPIO_MODE_QUASI);
	GPIO_SetMode(PA, BIT12, GPIO_MODE_QUASI);
	GPIO_SetMode(PA, BIT13, GPIO_MODE_QUASI);
}

uint8_t ScanKey_new(void)
{
	
	if (PD7 == 0) return 1; // up
	else if (PC1 == 0) return 2; // down
	else if (PC2 == 0) return 3; // right
	else if (PC3 == 0) return 4; // left
	else if (PA12 == 0) return 5; // OK
	else if (PA13 == 0) return 6; // Cancel
	
	return 0;
}

uint8_t ScanKey(void)
{
  PA0=1; PA1=1; PA2=0; PA3=1; PA4=1; PA5=1;
	if (PA3==0) return 1;
	if (PA4==0) return 4;
	if (PA5==0) return 7;
  PA0=1; PA1=0; PA2=1; PA3=1; PA4=1; PA5=1;
	if (PA3==0) return 2;
	if (PA4==0) return 5;
	if (PA5==0) return 8;
  PA0=0; PA1=1; PA2=1; PA3=1; PA4=1; PA5=1;
	if (PA3==0) return 3;
	if (PA4==0) return 6;
	if (PA5==0) return 9;
	return 0;
}
