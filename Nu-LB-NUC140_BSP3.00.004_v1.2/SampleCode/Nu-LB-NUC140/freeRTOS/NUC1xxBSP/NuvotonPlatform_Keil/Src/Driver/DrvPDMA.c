/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Includes of local headers                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#include "DrvPDMA.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

#define MAX_CHANNEL_NUM   			9
#define CHANNEL_OFFSET    			0x100
#define MAX_TRANSFER_BYTE_COUNT     0xFFFF

#define TRIG_EN    					BIT23
#define SAD_SEL    					BIT4|BIT5  		
#define DAD_SEL	   					BIT6|BIT7	 	
#define APB_TWS	   					BIT19|BIT20	 	
#define WAR_BCR_SEL     			BIT12|BIT15     
#define SW_RST     					0x2
#define PDMACEN    					0x1
#define DESTINATION_DIRECTION_BIT   6
#define SOURCE_DIRECTION_BIT        4
#define WAR_BCR_BIT			        12£á£á
#define TRANSFER_WIDTH_BIT    	   	19

#define PDMATABORT_IF   			0x1
#define PDMABLKD_IF     			0x2

#define PDMATABORT_IE   			0x1 
#define BLKD_IE         			0x2 
#define WAR_IE          			0x4

#define PDMA_POINT      			0x3

static PFN_DRVPDMA_CALLBACK  g_pfnPDMACallback[12][3]  = {
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0},
								{0,0,0}};								

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_Init				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to initialize PDMA      	                                           */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMA_Init(void)
{
    UNLOCKREG();
	/* Enable PDMA Clock */
	SYSCLK->AHBCLK.PDMA_EN  =1;		 	
	LOCKREG();
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_Close				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 None                                                                                      */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is disable all PDMA channel clock and AHB PDMA clock                         */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMA_Close(void)
{
   	/* Disable All PDMA clock */
	outpw(&PDMA_GCR->GCRCSR,0x0);
	
	/* Disable PDMA clock */
	UNLOCKREG();
	SYSCLK->AHBCLK.PDMA_EN  =0;
	LOCKREG();					
	
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_Open				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel	-[in]       PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                                */
/*               sParam	   	-[in]	    - the struct parameter to configure PDMA   	         			   */
/*                         It include of                                                                   */
/*                            sSrcAddr 		- Source Address 											   */									        
/*                            sDestAddr   	- Destination Address										   */
/*                            u8TransWidth 	- Transfer Width                        					   */
/*                            u8Mode 		- Operation Mode   			                                   */
/*                            i32ByteCnt   	- Byte Count                                                   */
/* Returns:                                                                                                */
/*               E_SUCCESS   					Sueccess                                                   */
/*               E_DRVPDMA_ERR_PORT_INVALID     Wrong Port parameter                                       */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is configure PDMA setting     	                                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_Open(E_DRVPDMA_CHANNEL_INDEX sChannel,STR_PDMA_T *sParam)
{						  
	volatile uint32_t u32SFR;
	volatile int32_t i = 1000;

    /*-----------------------------------------------------------------------------------------------------*/
    /* Check PDMA channel                                                                                  */
    /*-----------------------------------------------------------------------------------------------------*/
    if (sChannel > eDRVPDMA_CHANNEL_8)
    {
		return E_DRVPDMA_ERR_PORT_INVALID;
    }

	/*-----------------------------------------------------------------------------------------------------*/
    /* Set PDMA settings                                                                                   */
    /*-----------------------------------------------------------------------------------------------------*/
	outpw(&PDMA_GCR->GCRCSR,inpw(&PDMA_GCR->GCRCSR) | 1<<(sChannel+8) );   	/* Enable Channel Clock */
	while(i--);	  /* Need a delay to make sure Clock open OK */
    
	u32SFR = (uint32_t)&PDMA0->CSR + sChannel * CHANNEL_OFFSET;			  	/* Enable PDMA Channel */
    outpw(u32SFR, inpw(u32SFR) | PDMACEN);  


	u32SFR = (uint32_t)&PDMA0->SAR + sChannel * CHANNEL_OFFSET;			   	/* Set Source Address */	
	outpw(u32SFR,sParam->sSrcAddr.u32Addr);	
	
	u32SFR = (uint32_t)&PDMA0->DAR + sChannel * CHANNEL_OFFSET;				/* Set Destination Address */
	outpw(u32SFR,sParam->sDestAddr.u32Addr);	
	
	u32SFR = (uint32_t)&PDMA0->CSR + sChannel * CHANNEL_OFFSET;				/* Set Control Register */
	outpw(u32SFR,inpw(u32SFR)|(sParam->sSrcAddr.eAddrDirection 	<<	4));
	outpw(u32SFR,inpw(u32SFR)|(sParam->sDestAddr.eAddrDirection <<	6));
	outpw(u32SFR,inpw(u32SFR)|(sParam->u8TransWidth 			<<	19));
	outpw(u32SFR,inpw(u32SFR)|(sParam->u8Mode					<<	2));

	u32SFR = (uint32_t)&PDMA0->BCR + sChannel * CHANNEL_OFFSET;				/* Set Byte Count Register */
	outpw(u32SFR,inpw(u32SFR)|(sParam->i32ByteCnt));
	
	return E_SUCCESS;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_IsCHBusy   		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               TRUE:  The Channel is busy.                                                               */
/*               FALSE: The Channel is usable.                                                             */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to Get Channel Enable/Disable status                                 */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_IsCHBusy(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    if (eChannel > MAX_CHANNEL_NUM)	    									/* Check Channel is valid */
        return E_DRVPDMA_FALSE_INPUT;    
    
    u32SFR = (uint32_t)(&PDMA0->CSR) + eChannel * CHANNEL_OFFSET;
    
    if (inpw(u32SFR) &  TRIG_EN)											/* Check Channel is triggering or not */
        return TRUE;
    else
        return FALSE;    
}



/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetTransferLength	                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               pu32TransferLength     -[out]    The data pointer to save transfer length                 */
/* Returns:                                                                                                */
/*                                                                                                         */
/* Side effects:                                                                                           */
/*               E_SUCCESS     Success                                                                     */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to  get channel transfer length setting                              */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetTransferLength(E_DRVPDMA_CHANNEL_INDEX eChannel, uint32_t* pu32TransferLength)
{
    volatile uint32_t u32SFR;
    
    if (eChannel > MAX_CHANNEL_NUM)											/* Check Channel is valid */
        return E_DRVPDMA_FALSE_INPUT; 
            
	u32SFR = (uint32_t)&PDMA0->BCR + eChannel * CHANNEL_OFFSET;
    
    *pu32TransferLength = inpw(u32SFR);									   	/* Get Transfer Length */
    
    return E_SUCCESS;       
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_SetAPBTransferWidth				                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               eTransferWidth	   	-[in]   Transfer width.It could be                                     */
/*                                              eDRVPDMA_WIDTH_32BITS                                      */
/*                                              eDRVPDMA_WIDTH_8BITS                                       */
/*                                              eDRVPDMA_WIDTH_16BITS                                      */
/* Returns:                                                                                                */
/*               E_SUCCESS                  Success                                                        */
/*               E_DRVPDMA_FALSE_INPUT      invalid argument                                               */
/* Description:                                                                                            */
/*               The function is used to set APB transfer width for channelx                               */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_SetAPBTransferWidth(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_TRANSFER_WIDTH eTransferWidth)
{
    volatile uint32_t u32SFR;
    
    if ((eChannel > MAX_CHANNEL_NUM) || (eChannel ==0))					   	/* Check Channel is valid */
        return E_DRVPDMA_FALSE_INPUT; 
            
    u32SFR = (uint32_t)&PDMA0->CSR + eChannel * CHANNEL_OFFSET;
    
	outpw(u32SFR,(inpw(u32SFR) & ~APB_TWS) | (eTransferWidth << TRANSFER_WIDTH_BIT));
        
    return E_SUCCESS;     													/* Set PDMA Transfer Length */
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetAPBTransferWidth                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               Transfer width from channel                                                               */
/* Description:                                                                                            */
/*               The function is used to get transfer width from channelx                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetAPBTransferWidth(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;

	volatile int32_t u32Width;
    																		/* Check Channel is valid */
    if (eChannel > MAX_CHANNEL_NUM)
        return E_DRVPDMA_FALSE_INPUT; 
            
    u32SFR = (uint32_t)&PDMA0->CSR + eChannel * CHANNEL_OFFSET;			   	/* Get Transfer Length */
    
	u32Width = (inpw(u32SFR) & APB_TWS) >> TRANSFER_WIDTH_BIT;    
    
    return u32Width;    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_SetCHForAPBDevice                                                                 */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               eDevice	   		-[in]   APB device   					          	         		   */
/*                                          It includes eDRVPDMA_SPI0~3,eDRVPDMA_UART0~1,                  */
/*                                                eDRVPDMA_USB,eDRVPDMA_ADC                                */
/*               eRWAPB             -[in]   Access Direction                                               */
/*                                          It includes eDRVPDMA_WRITE_APB/eDRVPDMA_READ_APB               */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_SUCCESS            	Success                       									   */
/*               E_DRVPDMA_FALSE_INPUT  invalid Argument                								   */
/* Description:                                                                                            */
/*               The function is used to select PDMA channel for APB device                                */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_SetCHForAPBDevice(
    E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_APB_DEVICE 	eDevice,
    E_DRVPDMA_APB_RW 		eRWAPB    
)
{
    if (eChannel > MAX_CHANNEL_NUM) 						 				/* Check Channel is valid */
        return E_DRVPDMA_FALSE_INPUT; 

   
	switch(eDevice)
	{
		case eDRVPDMA_SPI0:												    /* Set SPI0 PDMA Channel */
			if(eRWAPB)	  
				PDMA_GCR->PDSSR0.SPI0_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI0_RXSEL	=	eChannel;
	   		break;

	   	case eDRVPDMA_SPI1:													/* Set SPI1 PDMA Channel */
			if(eRWAPB)	  
				PDMA_GCR->PDSSR0.SPI1_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI1_RXSEL	=	eChannel;
	   		break;
																			
		case eDRVPDMA_SPI2:													/* Set SPI2 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR0.SPI2_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI2_RXSEL	=	eChannel;
	   		break;
																		   	
		case eDRVPDMA_SPI3:													/* Set SPI3 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR0.SPI3_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR0.SPI3_RXSEL	=	eChannel;
	   		break;

		case eDRVPDMA_UART0:											   	/* Set UART0 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR1.UART0_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR1.UART0_RXSEL	=	eChannel;
	   		break;
		case eDRVPDMA_UART1:												/* Set UART1 PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR1.UART1_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR1.UART1_RXSEL	=	eChannel;
	   		break;
		case eDRVPDMA_USB:													/* Set USB PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR1.USBD_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR1.USBD_RXSEL	=	eChannel;
	   		break;
		case eDRVPDMA_ADC:													/* Set ADC PDMA Channel */
			if(eRWAPB)	
				PDMA_GCR->PDSSR1.ADC_TXSEL	=	eChannel;
			else
				PDMA_GCR->PDSSR1.ADC_RXSEL	=	eChannel;
	   		break;
		default:
			return 	E_DRVPDMA_FALSE_INPUT;
	}
 
    return E_SUCCESS;  
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCHForAPBDevice	                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/*               eDevice	   		-[in]   CH for APB device 			          	         			   */
/*                                          It includes eDRVPDMA_SPI0~3,eDRVPDMA_UART0~1,                  */
/*                                                eDRVPDMA_USB,eDRVPDMA_ADC                                */
/* Returns:                                                                                                */
/*               PDMA Channel for APB device                                                               */
/* Description:                                                                                            */
/*               The function is used to  get PDMA channel for APB device                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetCHForAPBDevice(E_DRVPDMA_APB_DEVICE eDevice,E_DRVPDMA_APB_RW eRWAPB)
{
  
	switch(eDevice)
	{
		case eDRVPDMA_SPI0:
			if(eRWAPB)	  
				return PDMA_GCR->PDSSR0.SPI0_TXSEL;						 	/* Get SPI0 PDMA Channel */
			else
				return PDMA_GCR->PDSSR0.SPI0_RXSEL;
	   		
	   	case eDRVPDMA_SPI1:												 	/* Get SPI1 PDMA Channel */
			if(eRWAPB)	  
				return PDMA_GCR->PDSSR0.SPI1_TXSEL;
			else
				return PDMA_GCR->PDSSR0.SPI1_RXSEL;
	   		
		case eDRVPDMA_SPI2:												 	/* Get SPI2 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR0.SPI2_TXSEL;
			else
				return PDMA_GCR->PDSSR0.SPI2_RXSEL;
	   																	 	
		case eDRVPDMA_SPI3:													/* Get SPI3 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR0.SPI3_TXSEL;
			else
				return PDMA_GCR->PDSSR0.SPI3_RXSEL;
	   		
		case eDRVPDMA_UART0:											 	/* Get UART0 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR1.UART0_TXSEL;
			else
				return PDMA_GCR->PDSSR1.UART0_RXSEL;
	   		
		case eDRVPDMA_UART1:											 	/* Get UART1 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR1.UART1_TXSEL;
			else
				return PDMA_GCR->PDSSR1.UART1_RXSEL;
	   		
		case eDRVPDMA_USB:													/* Get SPI0 PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR1.USBD_TXSEL;
			else
				return PDMA_GCR->PDSSR1.USBD_RXSEL;
	   		
		case eDRVPDMA_ADC:													/* Get ADC PDMA Channel */
			if(eRWAPB)	
				return PDMA_GCR->PDSSR1.ADC_TXSEL;
			else
				return PDMA_GCR->PDSSR1.ADC_RXSEL;
	   		
		default:
			return 	E_DRVPDMA_FALSE_INPUT;
	}
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_CHSoftwareReset                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               E_SUCCESS               Success                                                           */
/*               E_DRVPDMA_FALSE_INPUT   invalid Argument                                                  */
/* Description:                                                                                            */
/*               The function is used to software reset Channelx                                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_CHSoftwareReset(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    if (eChannel > MAX_CHANNEL_NUM)											/* Check PDMA Channel is Valid */
        return E_DRVPDMA_FALSE_INPUT;
 
 	u32SFR = (uint32_t)&PDMA0->CSR + eChannel * CHANNEL_OFFSET;
    
    outpw(u32SFR, inpw(u32SFR) | SW_RST);  								    /* Set S/W Reset Bit */
    
    return E_SUCCESS;     
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_CHEnablelTransfer                                                                 */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel			-[in]   PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                            */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVPDMA_FALSE_INPUT    invalid Argument                                                 */
/* Description:                                                                                            */
/*               The function is used to enable PDMA data read or write transfer                           */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_CHEnablelTransfer(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    																		/* Check PDMA Channel is Valid */
    if (eChannel > MAX_CHANNEL_NUM)
        return E_DRVPDMA_FALSE_INPUT;
            
   	u32SFR = (uint32_t)&PDMA0->CSR + eChannel * CHANNEL_OFFSET;
    
    outpw(u32SFR, inpw(u32SFR) | TRIG_EN | PDMACEN);  					   	/* PDMA Channl & Trigger Enable */
    
    return E_SUCCESS;      
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_EnableInt			                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVPDMA_FALSE_INPUT    invalid Argument                                                 */
/* Description:                                                                                            */
/*               The function is used to enable Interrupt for channelx                                     */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_EnableInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->IER + eChannel * CHANNEL_OFFSET;

    if (eChannel > MAX_CHANNEL_NUM)									  		/* Check PDMA Channel is Valid */
        return E_DRVPDMA_FALSE_INPUT;

    outpw(u32SFR, inpw(u32SFR) | eIntSource);  
    																		/* Enable Specified Interrupt */
	NVIC_EnableIRQ(PDMA_IRQn);  
   
    return E_SUCCESS;      
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_DisableInt  		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to disable Interrupt for channelx                                    */
/*---------------------------------------------------------------------------------------------------------*/

void DrvPDMA_DisableInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource)
{
    volatile uint32_t u32SFR;
    
	u32SFR = (uint32_t)&PDMA0->IER + eChannel * CHANNEL_OFFSET;

	outpw(u32SFR, inpw(u32SFR) & ~eIntSource);  						   /* Disable Specified Interrupt */
              
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_IsIntEnabled   	                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               TRUE:     The Specified Source Interrupt is enable                                        */
/*               FASLE:    The Specified Source Interrupt is disable                                       */
/* Description:                                                                                            */
/*               The function is used to check if the specified interrupt source is enabled in Channelx    */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_IsIntEnabled(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_ENABLE eIntSource )
{
    volatile uint32_t u32SFR;
    
	u32SFR = (uint32_t)&PDMA0->ISR + eChannel * CHANNEL_OFFSET; 
    
    switch(eIntSource)					/* Read ISR Register and check Specified Interrupt Enable or not */
    {														
    	case eDRVPDMA_TABORT:
		    return inpw(u32SFR) & PDMATABORT_IE;     			/* Target Abort Interrupt */
    		
    	case eDRVPDMA_BLKD:
		    return inpw(u32SFR) & BLKD_IE;     					/* Transfer Done Interrupt */
    															
    	case eDRVPDMA_WAR:
		    return inpw(u32SFR) & WAR_IE;     					/* Wrap Around Interrupt */
    		
		default :
	    	return E_DRVPDMA_FALSE_INPUT;		    		
	}    
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_ClearInt			                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to clear interrupt status for channelx                               */
/*---------------------------------------------------------------------------------------------------------*/
void DrvPDMA_ClearInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_FLAG eIntFlag)
{
    volatile uint32_t u32SFR;

	u32SFR = (uint32_t)&PDMA0->ISR + eChannel * CHANNEL_OFFSET; 
    
    outpw(u32SFR, eIntFlag);
}
						  

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_PollInt    		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               eIntFlag	   			-[in]     Intterrupt source: eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/* Returns:                                                                                                */
/*               TRUE:     The Source Interrupt Flag is set                                                */
/*               FASLE:    The Source Interrupt Flag is clear                                              */
/* Description:                                                                                            */
/*               The function is used to polling channel interrupt status                                  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t	DrvPDMA_PollInt(E_DRVPDMA_CHANNEL_INDEX eChannel,E_DRVPDMA_INT_FLAG eIntFlag)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t) & PDMA0->ISR + eChannel * CHANNEL_OFFSET; 
    return inpw(u32SFR) & eIntFlag;
}
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetInternalBufPointer				                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Internal Buffer Pointer                                                                   */
/* Description:                                                                                            */
/*               The function is used to  Get Internal Buffer Pointer                                      */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetInternalBufPointer(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    if ((eChannel > MAX_CHANNEL_NUM) )
        return E_DRVPDMA_FALSE_INPUT;    
   
   	u32SFR = (uint32_t)&PDMA0->POINT + eChannel * CHANNEL_OFFSET;     

    return (inpw(u32SFR) & PDMA_POINT);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetSharedBufData                                                                  */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Shard Buffer Content register                                                             */
/* Description:                                                                                            */
/*               The function is used to get shared buffer content from Channelx                           */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetSharedBufData(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->SBUF + eChannel * CHANNEL_OFFSET;  
    
    return inpw(u32SFR);
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     PDMA_IRQHandler     		                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 None 												                                       */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to  PDMA ISR                                                         */
/*---------------------------------------------------------------------------------------------------------*/
void PDMA_IRQHandler(void)
{
    volatile uint32_t u32IntStatus;
    volatile uint32_t u32WraparoundStatus;
	volatile uint32_t u32SFR;
    volatile int32_t  i32Channel= 0 ;
	volatile uint32_t u32ISR;
  	u32SFR = inpw(&PDMA_GCR->GCRISR) ;
  
	for(i32Channel=0;i32Channel<MAX_CHANNEL_NUM;i32Channel++)  
	{


		if(u32SFR & (1<<i32Channel))  										 		/* Check INT Channel */	
		{

		    u32ISR = (uint32_t)(&PDMA0->ISR) + i32Channel * CHANNEL_OFFSET ; 		/* Get INT status */
			
			u32IntStatus = inpw(u32ISR);

		
			if(u32IntStatus & PDMATABORT_IF)			 							/* Check Target Abort INT and clear */
			{
	    	   outpw(u32ISR ,PDMATABORT_IF);

			   if (g_pfnPDMACallback[i32Channel][0] != 0)    		
		    		(*g_pfnPDMACallback[i32Channel][0])(0);							/* Target Abort callback funtion */
				
			}
			else if(u32IntStatus & PDMABLKD_IF)			 							/* Transfer Done INT and clear */
			{
				outpw(u32ISR ,PDMABLKD_IF);

				if (g_pfnPDMACallback[i32Channel][1] != 0)    			
			    	(*g_pfnPDMACallback[i32Channel][1])(0);							/* Transfer Done INT callback funtion */
			}
			else 										 							/* Wrap Around INT */
			{
		    	u32WraparoundStatus = inpw(u32ISR) & 0x0F00;
		    	
				if (u32WraparoundStatus)
		    	{																	/* Check Wrap Around Method and clear */
					if (u32WraparoundStatus & 0x0400)
						u32WraparoundStatus = 0x0400;
					else		   
						u32WraparoundStatus = 0x0100; 	
			    						
					outpw(u32ISR ,u32WraparoundStatus);	   			

					if (g_pfnPDMACallback[i32Channel][2] != 0)		    						
			  			(*g_pfnPDMACallback[i32Channel][2])(u32WraparoundStatus); 	/* Wrap Around INT callback funtion */
	 		
					
		    	}		
			}

			
		}
	
	}

}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_InstallCallBack                                                                   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/*               E_DRVPDMA_INT_ENABLE	-[in]     Intterrupt source :eDRVPDMA_TABORT/eDRVPDMA_BLKD         */
/*               pfncallback            -[in]     The callback function pointer                            */
/* Returns:                                                                                                */
/*               E_SUCCESS                Success                                                          */
/*               E_DRVPDMA_FALSE_INPUT    invalid Argument                                                 */
/* Description:                                                                                            */
/*               The function is used to install call back function for Channelx & Interrupt source        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_InstallCallBack(
	E_DRVPDMA_CHANNEL_INDEX eChannel, 
    E_DRVPDMA_INT_ENABLE eIntSource,
	PFN_DRVPDMA_CALLBACK pfncallback   
)
{
    volatile uint32_t u32SFR;
      
	u32SFR = (uint32_t)&PDMA0->IER + eChannel * CHANNEL_OFFSET;
    
    if (eChannel > MAX_CHANNEL_NUM)								   	/* Check PDMA Channel is Valid */
        return E_DRVPDMA_FALSE_INPUT;

	g_pfnPDMACallback[eChannel][eIntSource>>1] = pfncallback;	    /* Install Callback funtion */
	
	NVIC_EnableIRQ(PDMA_IRQn);     

    return E_SUCCESS;      
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCurrentSourceAddr                                                              */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Current Source Address from channelx                                                      */
/* Description:                                                                                            */
/*               The function is used to get current source address from channelx                          */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetCurrentSourceAddr(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->CSAR + eChannel * CHANNEL_OFFSET;    

    return inpw(u32SFR);      
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCurrentDestAddr                                                                */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Current destination address form channelx                                                 */
/* Description:                                                                                            */
/*               The function is used to get current destination address from channelx                     */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetCurrentDestAddr(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
	u32SFR = (uint32_t)&PDMA0->CDAR + eChannel * CHANNEL_OFFSET;
    
    return inpw(u32SFR);      
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetCurrentTransferCount				                                           */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 eChannel				-[in]     PDMA Source: eDRVPDMA_CHANNEL_0 ~ 8                      */
/* Returns:                                                                                                */
/*               Get Current Transfer Count from channelx                                                  */
/* Description:                                                                                            */
/*               The function is used to get current transfer count from channelx                          */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvPDMA_GetCurrentTransferCount(E_DRVPDMA_CHANNEL_INDEX eChannel)
{
    volatile uint32_t u32SFR;
    
    u32SFR = (uint32_t)&PDMA0->CBCR + eChannel * CHANNEL_OFFSET;
    
	return inpw(u32SFR);   
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvPDMA_GetVersion                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             None                                                                                 	   */
/* Returns:                                                                                                */
/*               Version Number                                                                            */
/* Description:                                                                                            */
/*               The function is used to get DrvPDMA Version Number                                        */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvPDMA_GetVersion(void)
{
	return DRVPDMA_VERSION_NUM;
}


