/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvUART.h"

#include "Driver\DrvCAN.h"
#include "Driver\DrvSYS.h"

int32_t	i32Ready = FALSE;

/*----------------------------------------------------------------------------*/
/*  Test Menu       														  */
/*----------------------------------------------------------------------------*/
static void TestItem (void)
{
    printf("\n\n");
    printf("+-----------------------------------------------------------+\n");
    printf("|              CAN Sample Program                           |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [0] CAN0 Send Frame Test                                  |\n");
    printf("| [1] CAN0 Receive Frame Test                               |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("| [2] Quit                                                  |\n");
    printf("+-----------------------------------------------------------+\n");
    printf("  NOTE: This sample code must work with CAN transceiver and \n");
    printf("        another CAN device.\n");
    printf("Select key : \n");

}


/*----------------------------------------------------------------------------*/
/*  Callback function														  */
/*----------------------------------------------------------------------------*/
void CAN_TXCallback()
{

	i32Ready = TRUE;
}

void CAN_RXCallback()
{
	STR_CAN_T CAN_RxMsg;

	CAN_RxMsg = DrvCAN_ReadMsg(CAN_PORT0);

	printf("[RX] ID=%d ,Len=%d ,Data=0x%x  \n", CAN_RxMsg.id,CAN_RxMsg.u8cLen,CAN_RxMsg.u32cData[0]);
	
	i32Ready = TRUE;
}

/*----------------------------------------------------------------------------*/
/*  CAN Sample Test1.Send Signal Frame										  */
/*----------------------------------------------------------------------------*/
void CAN_SendFrame()
{
	STR_CAN_T CAN_TxMsg;
	i32Ready = FALSE;
	
	/* Init CAN and configure CAN */
	DrvCAN_Init();	
	DrvCAN_Open(CAN_PORT0,BITRATE_500K_12M);
	
	/* Enable CAN RX INT and Install Callback */
	DrvCAN_EnableInt(CAN_PORT0,INT_TI,(PFN_DRVCAN_CALLBACK*)CAN_TXCallback);

    /* Configure CAN TX Information */
	CAN_TxMsg.id 			= 567 ;
	CAN_TxMsg.u8cLen  		= 4;
	CAN_TxMsg.u32cData[0]	= 0x12345678;
	CAN_TxMsg.u8cFormat     = EXTENDED_FORMAT;
	CAN_TxMsg.u8cType	    = DATA_TYPE;
	CAN_TxMsg.u8cOverLoad   = DISABLE;
												
	/* Check CAN0 and CAN1 is connected */
	printf("Enter Any Key to continue \n");

	getchar();    

	/* Set Acceptance ID */
	DrvCAN_SetAcceptanceFilter(CAN_PORT0,CAN_TxMsg.id);

	/* Set Mask ID */
	DrvCAN_SetMaskFilter(CAN_PORT0,AMR_ALLPASS);
	
	/* Start to send data to CAN BUS */
	DrvCAN_WriteMsg(CAN_PORT0,&CAN_TxMsg);

	
	printf("[TX] ID=%d ,Len=%d ,Data=0x%x  \n", CAN_TxMsg.id,CAN_TxMsg.u8cLen,CAN_TxMsg.u32cData[0]);
	
	/* Check TX is OK, the S/W flag will be set in CAN TX Callback function */
	while(!i32Ready);	

	/* Disable CAN Interrupt */
	DrvCAN_DisableInt(CAN_PORT0,INT_TI);

	/* Close CAN Channel */
	DrvCAN_Close(CAN_PORT0);
	
	printf("CAN Test END");

}

/*----------------------------------------------------------------------------*/
/*  CAN Sample Test2.Receive Signal Frame									  */
/*----------------------------------------------------------------------------*/
void CAN_ReceiveFrame()
{
	STR_CAN_T CAN_TxMsg;
	i32Ready = FALSE;
	
	/* Init CAN and configure CAN */
	DrvCAN_Init();	
	DrvCAN_Open(CAN_PORT0,BITRATE_500K_12M);
	
	/* Enable CAN RX INT and Install Callback */
	DrvCAN_EnableInt(CAN_PORT0,INT_RI,(PFN_DRVCAN_CALLBACK*)CAN_RXCallback);

    /* Configure CAN TX Information */
	CAN_TxMsg.id 			= 567 ;
	CAN_TxMsg.u8cLen  		= 4;
	CAN_TxMsg.u32cData[0]	= 0x12345678;
	CAN_TxMsg.u8cFormat     = EXTENDED_FORMAT;
	CAN_TxMsg.u8cType	    = DATA_TYPE;
	CAN_TxMsg.u8cOverLoad   = DISABLE;
												
	/* Check CAN0 and CAN1 is connected */
	printf("Enter Any Key to continue \n");

	getchar();    

	/* Set Acceptance ID */
	DrvCAN_SetAcceptanceFilter(CAN_PORT0,CAN_TxMsg.id);

	/* Set Mask ID */
	DrvCAN_SetMaskFilter(CAN_PORT0,AMR_ALLPASS);
	
	/* Start to send data to CAN BUS */
	
	
	printf("[TX] ID=%d ,Len=%d ,Data=0x%x  \n", CAN_TxMsg.id,CAN_TxMsg.u8cLen,CAN_TxMsg.u32cData[0]);
	
	/* Check TX is OK, the S/W flag will be set in CAN TX Callback function */
	while(!i32Ready);	

	/* Disable CAN Interrupt */
	DrvCAN_DisableInt(CAN_PORT0,INT_TI);

	/* Close CAN Channel */
	DrvCAN_Close(CAN_PORT0);
	
	printf("CAN Test END");

}





void SysTimerDelay(uint32_t us)
{
    SysTick->LOAD = us * 22; /* Assume the internal 22MHz RC used */
    SysTick->VAL   =  (0x00);
    SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE);

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & (1 << 16)) == 0);
}


/*----------------------------------------------------------------------------*/
/*  MAIN function															  */
/*----------------------------------------------------------------------------*/
int main (void)
{
	STR_UART_T param;
	uint8_t u8Item;
	int32_t bLoop = TRUE;

	UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN = 1;
    /* Waiting for 12M Xtal stalble */
    SysTimerDelay(5000);

	/* Init GPIO and configure UART0 */
	DrvGPIO_InitFunction(FUNC_UART0);
	
    param.u32BaudRate        = 115200;
    param.u8cDataBits        = DRVUART_DATABITS_8;
    param.u8cStopBits        = DRVUART_STOPBITS_1;
    param.u8cParity          = DRVUART_PARITY_NONE;
    param.u8cRxTriggerLevel  = DRVUART_FIFO_1BYTES;
    param.u8TimeOut        	 = 0;
    DrvUART_Open(UART_PORT0, &param);

	while(bLoop)
	{
		TestItem();
		u8Item = getchar();
			
		switch(u8Item)
		{
			case '0':
			{
                CAN_SendFrame();
			    break;
            }
			case '1':
			{
				CAN_ReceiveFrame();
			    break;
            }
			case '2':
			{
				bLoop = FALSE;
			    break;
            }
		}
	}

 	return TRUE;

}







