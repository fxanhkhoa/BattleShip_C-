#include "NUC1xx.h"

extern uint32_t gExtClock;


/*------------------------------------------------------------------------------
  Initial UART0
 *------------------------------------------------------------------------------*/
void BaudRateCalculator(uint32_t clk, uint32_t baudRate, UART_BAUD_T *baud)
{
    uint32_t divx, div, divClk;
    uint32_t i;
    int32_t absMin;
    int32_t tmp;

    divClk = clk / (baudRate / 100);
    absMin = 999999;
    
    i = 16;
    for(i=1;i<=16;i++)
    {
        tmp = divClk / i;
        tmp = clk / (tmp / 100) / i - baudRate;
        if(tmp < 0) tmp = -tmp;
        if(tmp < absMin)
        {
            divx = i;
            absMin = tmp;
        }    
    
        /* DivX must > 8 */
        if(i==1)
            i = 8;

    }
    div = clk / divx / baudRate;    

    tmp = div - 2;
    if(tmp < 0) tmp = 0;
    if(divx == 16)
    {
        /* DIVX disabled */
        baud->DIV = tmp;
        baud->DIVX_EN = 0;
        baud->DIVX1 = 0;
        baud->DIVX = 0xF;
    }
    else if(divx == 1)
    {
        /* DIVX enabled and X = 1 */
        tmp = tmp | (0x3 << 16);
        baud->DIV = tmp;
        baud->DIVX_EN = 1;
        baud->DIVX1 = 1;
        baud->DIVX = 0xF;
    }
    else
    {
        /* DIVX enabled and X > 8 & X < 16 */
        baud->DIV = tmp;
        baud->DIVX_EN = 1;
        baud->DIVX1 = 0;
        baud->DIVX = i;
    }
}


void UartInit(void)
{
    /* Multi-Function Pin: Enable UART0:Tx Rx */
    SYS->GPBMFP.UART0_RX = 1;
    SYS->GPBMFP.UART0_TX = 1;

    /* Configure GCR to reset UART0 */
    SYS->IPRSTC2.UART0_RST = 1;
    SYS->IPRSTC2.UART0_RST = 0;

    /* Enable UART clock */
    SYSCLK->APBCLK.UART0_EN = 1;

    /* Select UART clock source */
    SYSCLK->CLKSEL1.UART_S = 0;

    /* Data format */
    UART0->LCR.WLS = 3;

    /* Configure the baud rate */
    //BaudRateCalculator(gExtClock, 115200, &UART0->BAUD);
    *((__IO uint32_t *)&UART0->BAUD) = 0x3F000066; /* This setting is for 115200bsp with 12Mhz clock source */
}

