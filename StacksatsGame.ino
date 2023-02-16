#include "workflow.h"
#include "utils.h"
#include <bootloader_random.h>

sButton btnMove(PIN_MOVE);
sButton btnSelect(PIN_SELECT);

uint8_t myGameState = STATE_INIT;

void setup() {

  Serial.begin(SERIAL_BAUD);                    // Init Serial port
  Init_TFT();                                   // Init TFT
  bootloader_random_enable();
}


void loop() {
  
  while(true){
    /***** Print menu options ***********/
    switch(myGameState){
      case STATE_INIT:    WF_INIT(); break;
      case STATE_INGAME:  WF_INGAME(); break;
    }
    delay(10);
  }
}
