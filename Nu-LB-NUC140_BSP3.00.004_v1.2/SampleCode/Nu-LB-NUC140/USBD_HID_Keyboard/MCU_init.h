//Define Clock source
#define MCU_CLOCK_SOURCE      
#define MCU_CLOCK_SOURCE_HXT    // HXT, LXT, HIRC, LIRC 
#define MCU_CLOCK_FREQUENCY     50000000  //Hz

//Define MCU Interfaces
#define MCU_INTERFACE_USB
#define USB_CLOCK_DIVIDER     1

//#define MCU_INTERFACE_CLKO
//#define CLK_FRQDIV_SOURCE_HXT  // HXT, LXT, HCLK, HIRC
//#define CLKO_CLOCK_DIVIDER     2 //CKO freq. = Clock Source freq. / 2^(CLKO_CLOCK_DIVIDER + 1);
//#define PIN_CLKO_PB12
