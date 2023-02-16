#include "Lib/images.h"
#include "Lib/Free_Fonts.h"
//#include "Lib/font.h"

/********** 🍃 STATES ************************/
#define STATE_INIT        1
#define STATE_INGAME      2
/********** 🍃 GENERAL ************************/
#define SERIAL_BAUD       115200   // baudrate debug
#define STX               2        // Start of text
#define ETX               3        // end of text

const int wdtTimeout = 3000;  //time in ms to trigger the watchdog

/************** TYPES **********************/

/**************🍃 TFT functions ********************/
void Init_TFT(void);
void WF_INIT(void);
void WF_INGAME(void);
void clrWorkArea(void);



