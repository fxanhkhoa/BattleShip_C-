#include "function.h"

void U_Print_uint8_for(USART_TypeDef * USARTx, uint8_t buffer[], int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, buffer[i]);
		USART_ClearFlag(USARTx, USART_FLAG_TXE);
	}
}

void U_Print_uint8(USART_TypeDef * USARTx, uint8_t *a)
{
	while (*a)
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *a);
		USART_ClearFlag(USARTx, USART_FLAG_TXE);
		*a++;
	}
}

void U_Print(USART_TypeDef * USARTx, int a)
{
	char temp[20] = "";
	int i;
	int leng = sprintf(temp, "%03d", a);
	for (i = 0; i < leng; i++)
	{
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
			USART_SendData(USARTx,temp[i]);
			USART_ClearFlag(USARTx, USART_FLAG_TXE);

	}
}

void U_Print_Char(USART_TypeDef * USARTx, char *s)
{
	while (*s)
	{
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, *s);
		USART_ClearFlag(USARTx, USART_FLAG_TXE);
		*s++;
	}
}

void U_Print_float(USART_TypeDef * USARTx, float a)
{
	char temp[20] = "";
	int i;
	int leng = sprintf(temp, "%3.2f", a);
	for (i = 0; i < leng; i++)
	{
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
			USART_SendData(USARTx,temp[i]);
			USART_ClearFlag(USARTx, USART_FLAG_TXE);
	}
}
