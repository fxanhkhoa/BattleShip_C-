#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>
#include "stm32f10x_tim.h"
#include <math.h>
#include "function.h"

//#include "cortexm/ExceptionHandlers.h"

#define LEDPORTCLK          RCC_APB2Periph_GPIOC
#define LEDPORT             GPIOC
#define LEDB                GPIO_Pin_13

#define Rad2Dree       57.295779513082320876798154814105

#define BUTTONPORTSOURCE    GPIO_PortSourceGPIOA
#define BUTTONPINSOURCE     GPIO_PinSource0

/* Private function prototypes -----------------------------------------------*/
  #ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
       set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
  #endif /* __GNUC__ */


void led_toggle(void);
uint8_t* build_buffer();
u16 timer;
uint8_t id = 1, command, dim;
int x = 0, y = 0;
//u32 RegValue = 0;
uint8_t buffer[10] = "";
uint8_t input_buffer[10] = "";
uint8_t pos = 0;
	
GPIO_InitTypeDef GPIO_InitStruct;

int main(int argc, char* argv[])
{
	GPIO_InitTypeDef GPIO;
	I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef timerInitStructure;
	
	int i;
	float heading[3];

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/* Set Led */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Set motor */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
		
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
/**************************************************************************************************
*											Timer
**************************************************************************************************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 
		timerInitStructure.TIM_Prescaler = 36000;
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = 2-1;
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &timerInitStructure);
		TIM_Cmd(TIM4, ENABLE);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		
		NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
		/*----- NVIC Timer interrupt -----*/
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
/*************************************************************************************
*																	USART_Init
**************************************************************************************/
			
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	USART.USART_BaudRate = 9600;
	USART.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART.USART_StopBits = USART_StopBits_1;
	USART.USART_WordLength = USART_WordLength_8b;
	USART.USART_Parity = USART_Parity_No;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		/*---- Configure USART1 ----*/
			USART_Init(USART1, &USART);
			//USART_Init(USART2, &USART);
    /*---- Enable RXNE interrupt ----*/
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
			//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		/*---- Enable TXE ----*/
			//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    /*---- Enable USART1 global interrupt ----*/
			NVIC_EnableIRQ(USART1_IRQn);
		/*---- USART ENABLE ----*/
			USART_Cmd(USART1, ENABLE);
			//USART_Cmd(USART2, ENABLE);
		/*------ TX-Pin PA9 & RX-Pin PA10 -----*/
			
			GPIO.GPIO_Pin = GPIO_Pin_9;
			GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, & GPIO);
			
			GPIO.GPIO_Pin = GPIO_Pin_10;
			GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO);
		/*---- TX-Pin PA2 & RX-Pin PA3 ----*/
			GPIO.GPIO_Pin = GPIO_Pin_2;
			GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, & GPIO);
			
			GPIO.GPIO_Pin = GPIO_Pin_3;
			GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO);

	/*--------- GPIO Init A3->B7, A1 ----------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* Set button */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// B3 ->LEFT
	// B4 -> UP
	// B5 -> DOWN
	// B6 -> RIGHT
	// B7 -> OK
	//U_Print_Char(USART1, "gogogog");
	
	//1st init x,y
	x = 0;
	y = 0;
	pos = 0;
	
	while (1)
	{
		if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_3)) // A3
		{
			led_toggle();
			y--;
			command = 7;
			//U_Print_uint8(USART1, build_buffer());
			U_Print_uint8_for(USART1, build_buffer(), 10);
					// Turn on LED on PA0 (LED circuit is active low)
					//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_3));
		}
		
		if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_4)) // A4
		{
			led_toggle();
			x--;
			command = 7;
			//U_Print_uint8(USART1, build_buffer());
			U_Print_uint8_for(USART1, build_buffer(), 10);
					// Turn on LED on PA0 (LED circuit is active low)
					//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			while (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_4));
		}
		
		if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_5)) // A5
		{
			led_toggle();
			x++;
			command = 7;
			//U_Print_uint8(USART1, build_buffer());
			U_Print_uint8_for(USART1, build_buffer(), 10);
					// Turn on LED on PA0 (LED circuit is active low)
					//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_5));
		}
		
		if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_6)) // A6
		{
			led_toggle();
			y++;
			command = 7;
			//U_Print_uint8(USART1, build_buffer());
			U_Print_uint8_for(USART1, build_buffer(), 10);
			//U_Print_Char(USART1, "go gog o");
					// Turn on LED on PA0 (LED circuit is active low)
					//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_6));
		}
		if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_7)) // A7 OK
		{
			command = 2;
			U_Print_uint8_for(USART1, build_buffer(), 10);
					// Turn on LED on PA0 (LED circuit is active low)
					//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_7));
		}
		/*
		if (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_8)) // A8 Ban
		{
			if (dim == 1) dim = 2;
			else if (dim == 2) dim = 1;
					// Turn on LED on PA0 (LED circuit is active low)
					//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			while (!(GPIO_ReadInputData(GPIOA) & GPIO_Pin_8));
		}
		*/
	}
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
//GETCHAR_PROTOTYPE
//{
//  return GetUSART(USART1);
//}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

uint8_t *build_buffer()
{
	int i;
	buffer[0] = id;
	buffer[1] = command;
	if (x == 8) x = 0;
	if (x == -1) x = 7;
	buffer[2] = x;
	if (y == 8) y = 0;
	buffer[3] = y;
	buffer[4] = dim;
	buffer[5] = 0;
	buffer[6] = 0;
	buffer[7] = 0;
	buffer[8] = 0;
	buffer[9] = 42; // dau *
	/*for (i = 5; i < 20; i++)
	{
		buffer[i] = 0;
	}*/
	return buffer;
}

void led_toggle(void)
{
    /* Read LED output (GPIOA PIN8) status */
    uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
     
    /* If LED output set, clear it */
    if(led_bit == (uint8_t)Bit_SET)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    }
    /* If LED output clear, set it */
    else
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        led_toggle();
    }
}

void EXTI0_IRQHandler()
{
		if(EXTI_GetITStatus(EXTI_Line0) != RESET)//Judge whether a line break
		{
			EXTI_ClearITPendingBit(EXTI_Line0);   //Remove LINE interrupt flag bit
			led_toggle();
		} 	    
} 

void EXTI9_5_IRQHandler()
{
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)//Judge whether a line break
		{
			EXTI_ClearITPendingBit(EXTI_Line5);   //Remove LINE interrupt flag bit
			led_toggle();
		} 	    
} 

void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				timer ++;
				//time_now++;
    }
}

void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
			input_buffer[pos] = USART_ReceiveData(USART1);
			pos++;
			if (pos >= 10)
			{
				if (input_buffer[1] == 69)
				{
					GPIO_SetBits(GPIOB, GPIO_Pin_12);
					timer = 0;
					while (timer < 100);
					GPIO_ResetBits(GPIOB, GPIO_Pin_12);
				}
			}
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}
