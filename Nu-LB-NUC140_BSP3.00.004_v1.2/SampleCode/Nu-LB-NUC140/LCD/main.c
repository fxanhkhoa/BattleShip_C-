//
// LCD: display Text on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN  (LQPF-100)
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

// Global variables
volatile uint8_t g_u8DeviceAddr;
volatile uint8_t g_au8TxData[3];
volatile uint8_t g_u8RxData;
volatile uint8_t g_u8DataLen;
volatile uint8_t g_u8EndFlag = 0;

typedef void (*I2C_FUNC)(uint32_t u32Status);
static I2C_FUNC s_I2C1HandlerFn = NULL;

void I2C1_IRQHandler(void)
{
    uint32_t u32Status;

    u32Status = I2C_GET_STATUS(I2C1);
    if(I2C_GET_TIMEOUT_FLAG(I2C1))
    {
        I2C_ClearTimeoutFlag(I2C1); // Clear I2C1 Timeout Flag
    }
    else
    {
        if(s_I2C1HandlerFn != NULL)
            s_I2C1HandlerFn(u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Rx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterRx(uint32_t u32Status)
{
    if(u32Status == 0x08)                       // START has been transmitted and prepare SLA+W
    {
        I2C_SET_DATA(I2C1, (g_u8DeviceAddr << 1));    // Write SLA+W to Register I2CDAT
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x18)                  // SLA+W has been transmitted and ACK has been received
    {
        I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x20)                  // SLA+W has been transmitted and NACK has been received
    {
        I2C_STOP(I2C1);
        I2C_START(I2C1);
    }
    else if(u32Status == 0x28)                  // DATA has been transmitted and ACK has been received
    {
        if(g_u8DataLen != 2)
        {
            I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STA_SI);
        }
    }
    else if(u32Status == 0x10)                  // Repeat START has been transmitted and prepare SLA+R
    {
        I2C_SET_DATA(I2C1, ((g_u8DeviceAddr << 1) | 0x01));   // Write SLA+R to Register I2CDAT 
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x40)                  // SLA+R has been transmitted and ACK has been received
    {
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x58)                  // DATA has been received and NACK has been returned
    {
        g_u8RxData = (unsigned char) I2C_GET_DATA(I2C1);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STO_SI);
        g_u8EndFlag = 1;
    }
    else
    {
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2C Tx Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_MasterTx(uint32_t u32Status)
{
    if(u32Status == 0x08)                        // START has been transmitted
    {
        I2C_SET_DATA(I2C1, g_u8DeviceAddr << 1); // Write SLA+W to Register I2CDAT
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x18)                   // SLA+W has been transmitted and ACK has been received
    {
        I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
    }
    else if(u32Status == 0x20)                  // SLA+W has been transmitted and NACK has been received
    {
        I2C_STOP(I2C1);
        I2C_START(I2C1);
    }
    else if(u32Status == 0x28)                  // DATA has been transmitted and ACK has been received
    {
        if(g_u8DataLen != 3)
        {
            I2C_SET_DATA(I2C1, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STO_SI);
            g_u8EndFlag = 1;
        }
    }
    else
    {
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

void Close_I2C(void)
{
    I2C_DisableInt(I2C1);       // Disable I2C1 interrupt generation
    NVIC_DisableIRQ(I2C1_IRQn); // Disable NVIC I2C1 interrupt input
    I2C_Close(I2C1);            // Disable I2C1 control module
    CLK_DisableModuleClock(I2C1_MODULE); // Disable I2C module clock
}

void Init_I2C(void)
{
    I2C_Open(I2C1, I2C1_CLOCK_FREQUENCY);
    printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C1)); // get I2C1 clock

    I2C_EnableInt(I2C1);       // Enable I2C1 interrupt generation
    NVIC_EnableIRQ(I2C1_IRQn); // Enable NVIC I2C1 interrupt input
}

void EEPROM_Write(uint16_t addr, uint8_t data)
{
	uint8_t i;
  g_u8DeviceAddr = 0xA0; // 24LC64 device address = 0xA0
	g_au8TxData[0] = (uint8_t)((addr & 0xFF00) >> 8);
  g_au8TxData[1] = (uint8_t)(addr & 0x00FF);
	g_au8TxData[2] = data;

  g_u8DataLen = 0;
  g_u8EndFlag = 0;

  s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterTx; // I2C function to write data to slave
  I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STA); // I2C as master sends START signal 
  while(g_u8EndFlag == 0);                  // Wait I2C Tx Finish 
  g_u8EndFlag = 0;
}

uint8_t EEPROM_Read(uint16_t addr)
{
  s_I2C1HandlerFn = (I2C_FUNC)I2C_MasterRx; // I2C function to read data from slave
  g_u8DataLen = 0;
  g_u8DeviceAddr = 0xA0;
	g_au8TxData[0] = (uint8_t)((addr & 0xFF00) >> 8);
  g_au8TxData[1] = (uint8_t)(addr & 0x00FF);
	
  I2C_SET_CONTROL_REG(I2C1, I2C_I2CON_STA);      
  while(g_u8EndFlag == 0);// Wait I2C Rx Finish
  g_u8EndFlag = 0;
	
	return g_u8RxData;
}

void delay_loop()
{
	uint32_t i,j;
	for (i = 0; i < 3; i ++)
	{
		for (j = 0; j < 60000; j++);
	}
}

int main(void)
{
	int8_t keyin=0;
	uint8_t i = 0;
	uint8_t data[9];
	uint8_t j = 0;
	uint8_t delay = 60000;
	char out[9] = "";
	
	SYS_Init();
	init_LCD();
	clear_LCD();
	Init_I2C();
	
	OpenKeyPad();
	while (1)
		{
			print_Line(0, "1-GHI EEPROM");
			print_Line(1, "2-DOC EEPROM");
			
			do {keyin=ScanKey();} 
			while (keyin == 0);
			
			if (keyin == 1) // Write EEPROM
			{
				clear_LCD();
				print_Line(1, "Now write");
				i = 0;
				delay_loop();
				while (i < 8) 
				{
					keyin = ScanKey();
					if (keyin > 0)
					{
						switch(keyin)
						{
							case 1:
								out[i] = 'A';
								//print_Line(0,"A");
								break;
							case 2:
								out[i] = 'B';
								//print_Line(0,"B");
								break;
							case 3:
								out[i] = 'C';
								break;
							case 4:
								out[i] = 'D';
								break;
							case 5:
								out[i] = 'E';
								break;
							case 6:
								out[i] = 'F';
								break;
							case 7:
								out[i] = 'G';
								break;
							case 8:
								out[i] = 'H';
								break;
							case 9:
								out[i] = 'I';
								break;
							default:
								break;
						}
						print_Line(0, out);
						EEPROM_Write(i, out[i]);
						i++;
						delay_loop();
					}
				}
			}
			
			else if (keyin == 2) // Read EEPROM
			{
				clear_LCD();
				print_Line(1, "Now read");
				while (1);
			}
		}
}
