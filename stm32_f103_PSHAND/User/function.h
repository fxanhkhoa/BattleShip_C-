#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#ifdef __cplusplus
extern "C" {
#endif 
	
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <stdio.h>

void U_Print(USART_TypeDef * USARTx, int a);
void U_Print_uint8(USART_TypeDef * USARTx, uint8_t *a);
void U_Print_uint8_for(USART_TypeDef * USARTx, uint8_t buffer[], int len);
void U_Print_Char(USART_TypeDef * USARTx, char *s);
void U_Print_float(USART_TypeDef * USARTx, float a);
	
	
#ifdef __cplusplus
}
#endif

#endif /* _FUNCTION_H_ */