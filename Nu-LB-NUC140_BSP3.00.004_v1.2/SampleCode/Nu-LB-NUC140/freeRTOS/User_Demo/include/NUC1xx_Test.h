/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#ifndef __NUC1XX_Test_H__
#define __NUC1XX_Test_H__

#include "FreeRTOS.h"
#include "task.h"

void vStartUARTTasks( unsigned portBASE_TYPE uxPriority );
void vStartThreadTasks( void );
void vStartSemBinTasks( void );
void vStartMutexTasks( void );
void vStartQueueTasks( void );

#endif	


