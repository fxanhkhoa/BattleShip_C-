/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "DrvFMC.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_Write				                                               	   		   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32addr 	- [in]		Flash address include APROM, LDROM, Data Flash, and Config         */
/*               u32data 	- [in]		Data     				   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Write data into Flash include APROM, LDROM, Data Flash, and Config	                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_Write(uint32_t u32addr, uint32_t u32data)
{
    FMC->ISPCMD.FCTRL = 1;
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 1;
    FMC->ISPADR	= u32addr;
	FMC->ISPDAT	= u32data;
	FMC->ISPTRG.ISPGO = 1;	

	if (FMC->ISPCON.ISPFF == 1)
	{
		FMC->ISPCON.ISPFF = 1;
		return E_DRVFMC_ERR_ISP_FAIL;
	}
	
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_Read				                                               	   		   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32addr 	- [in]		Flash address include APROM, LDROM, Data Flash, and Config         */
/*               u32data 	- [out]		Data     				   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Read data from Flash include APROM, LDROM, Data Flash, and Config	                       */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_Read(uint32_t u32addr, uint32_t * u32data)
{ 
    FMC->ISPCMD.FCTRL = 0;
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 0;
    FMC->ISPADR	= u32addr;
	FMC->ISPDAT	= 0;
	FMC->ISPTRG.ISPGO = 1;    
	
	if (FMC->ISPCON.ISPFF == 1)
	{
		FMC->ISPCON.ISPFF = 1;
		return E_DRVFMC_ERR_ISP_FAIL;
	}
	
	*u32data = FMC->ISPDAT;
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_Erase				                                               	   		   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32addr 	- [in]		Flash page base address or Config0 address                         */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*             Page Erase Flash include APROM, LDROM, Data Flash, and Config. The erase unit is 512 bytes  */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_Erase(uint32_t u32addr)
{
    FMC->ISPCMD.FCTRL = 2;
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 1;
    FMC->ISPADR	= u32addr;
	FMC->ISPTRG.ISPGO = 1;	

	if (FMC->ISPCON.ISPFF == 1)
	{
		FMC->ISPCON.ISPFF = 1;
		return E_DRVFMC_ERR_ISP_FAIL;
	}
	
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_ReadCID				                                               	   		   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32data 	- [out]		Data     				   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Read Company ID                             	                       					   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_ReadCID(uint32_t * u32data)
{
    FMC->ISPCMD.FCTRL = 0xB;
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 0;
	FMC->ISPADR	= 0;
    FMC->ISPTRG.ISPGO = 1;
	
	if (FMC->ISPCON.ISPFF == 1)
	{
		FMC->ISPCON.ISPFF = 1;
		return E_DRVFMC_ERR_ISP_FAIL;
	} 
    
	*u32data = FMC->ISPDAT; 
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_ReadDID				                                               	   		   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32data 	- [out]		Data     				   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Read Device ID                             	                       					   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_ReadDID(uint32_t * u32data)
{
	FMC->ISPCMD.FCTRL = 0xC;    
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 0;
	FMC->ISPADR	= 0;
    FMC->ISPTRG.ISPGO = 1;
	
	if (FMC->ISPCON.ISPFF == 1)
	{
		FMC->ISPCON.ISPFF = 1;
		return E_DRVFMC_ERR_ISP_FAIL;
	} 
    
	*u32data = FMC->ISPDAT; 
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_ReadPID				                                               	   		   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32data 	- [out]		Data     				   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Read Product ID                             	                       					   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_ReadPID(uint32_t * u32data)
{
	FMC->ISPCMD.FCTRL = 0xC;    
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 0;
	FMC->ISPADR	= 0x04;
    FMC->ISPTRG.ISPGO = 1;
	
	if (FMC->ISPCON.ISPFF == 1)
	{
		FMC->ISPCON.ISPFF = 1;
		return E_DRVFMC_ERR_ISP_FAIL;
	} 
    
	*u32data = FMC->ISPDAT; 
    return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableISP				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable 	- [in]		1:Enable / 0:Disable   	   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               None																		   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable/Disable ISP function                       	                       				   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableISP(int32_t i32Enable)
{
	FMC->ISPCON.ISPEN = (i32Enable)? 1: 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_BootSelect				                                               	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               boot 	- [in]		APROM / LDROM    				   									   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               None																		   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Select next boot from APROM or LDROM                           	       				   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_BootSelect(E_FMC_BOOTSELECT boot)
{
	FMC->ISPCON.BS = (boot)? 1: 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_GetBootSelect				                                         	   		   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None    				   							                            		   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               APROM      The current boot select setting is in APROM						   			   */
/*               LDROM      The current boot select setting is in LDROM		     			   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               To return current boot select setting.                           	       				   */
/*---------------------------------------------------------------------------------------------------------*/
E_FMC_BOOTSELECT DrvFMC_GetBootSelect(void)
{
	return (E_FMC_BOOTSELECT)FMC->ISPCON.BS;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableLDUpdate				                                               	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable 	- [in]		1:Enable / 0:Disable   	   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               None																		   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable/Disable LDROM update function                            	               		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableLDUpdate(int32_t i32Enable)
{	
	FMC->ISPCON.LDUEN = (i32Enable)? 1: 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnableConfigUpdate				                                          	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable 	- [in]		1:Enable / 0:Disable   	   										   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               None																		   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable/Disable Config update function when MCU is running in APROM or LDROM       		   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnableConfigUpdate(int32_t i32Enable)
{	
	FMC->ISPCON.CFGUEN = (i32Enable)? 1: 0;
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_EnablePowerSaving				                                               	   	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               i32Enable 	- [in]		1:Enable / 0:Disable   	   										   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               None																		   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Enable/Disable flash power saving function                    	                       	   */
/*---------------------------------------------------------------------------------------------------------*/
void DrvFMC_EnablePowerSaving(int32_t i32Enable)
{	
	FMC->FATCON.FPSEN = (i32Enable)? 1: 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_ReadDataFlashBaseAddr				                                               	   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               None    				   										   						   */
/*                     		                           	   												   */
/* Returns:                                                                                                */
/*               Data Flash base address													   			   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Read Data Flash base address                            	                       	   	   */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvFMC_ReadDataFlashBaseAddr(void)
{	
	return FMC->DFBADR;
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_WriteConfig				                                               	  	   		   */
/*                                                                                                         */
/* Parameters:                                                                                             */
/*               u32data0 	- [in]		Config0 data       												   */
/*               u32data1 	- [in]		Config1 data      				   								   */
/*                     		                           	   												   */
/*               									      		                           				   */
/* Returns:                                                                                                */
/*               0		 Success																		   */
/*              <0       Failed when illegal condition occurs					                  		   */
/*               							                                                               */
/* Description:                                                                                            */
/*               Erase Config and write data into Config0 and Config1	                       			   */
/*---------------------------------------------------------------------------------------------------------*/
int32_t DrvFMC_WriteConfig(uint32_t u32data0, uint32_t u32data1)
{		
	if ( !DrvFMC_Erase(CONFIG0) )
		return E_DRVFMC_ERR_ISP_FAIL;
	
	if ( !DrvFMC_Write(CONFIG0, u32data0) )
		return E_DRVFMC_ERR_ISP_FAIL;

	return DrvFMC_Write(CONFIG1, u32data1);
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function: DrvFMC_GetVersion                                                                             */
/*                                                                                                         */
/* Parameters:        	                                                                                   */
/*               None	                                                         						   */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               The DrvFMC version number                                                                 */
/*                                                                                                         */
/* Description:                                                                                            */
/*               This function is used to get the DrvFMC version number                                    */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t DrvFMC_GetVersion(void)
{
	return DRVFMC_VERSION_NUM;
}

