//
// USBD_HID_Keyboard : emulate USB keyboard using 3x3 keypad
//                     key 1~9 input 'a'~'i'
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "hid_kb.h"
#include "Scankey.h"

int32_t main(void)
{
    SYS_Init();
    OpenKeyPad(); 

    USBD_Open(&gsInfo, HID_ClassRequest, NULL);

    /* Endpoint configuration */
    HID_Init();
    USBD_Start();
    NVIC_EnableIRQ(USBD_IRQn);

    while(1)
    {
        HID_UpdateKbData();
    }
}
