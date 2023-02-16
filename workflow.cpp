#include <EEPROM.h>
#include "workflow.h"
#include "utils.h"
#include "Wire.h"
#include <TFT_eSPI.h> // Graphics and font library 

/**********************⚡ GLOBAL Vars *******************************/
extern TFT_eSPI tft;  // Invoke library, pins defined in User_Setup.h
extern sButton btnMove;
extern sButton btnSelect;
extern uint8_t myGameState;

void printTalkText(String txt);

/*****************⚡ TFT WORK *********************/

void Init_TFT(void){

  /******** INIT DISPLAY ************/
  tft.init();
  //tft.setRotation(1);
  tft.setSwapBytes(true);// Swap the colour byte order when rendering

}

void WF_INIT(void){
  /******** PRINT INIT SCREEN *****/
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, initWidth, initHeight, initImage);
  
  delay(2000);
 
  /******** PRINT START SCREEN *****/
  tft.pushImage(0, 0, startWidth, startHeight, startImage);
  WaitAnyKey();

  /******** PRINT first monkey "I have a new GEM" *****/ 
  tft.pushImage(0, 0, monkeyWidth, monkeyHeight, monkeyImage);
  WaitAnyKey();
  
  /******** PRINT bitcoiner "Use Lightning" *****/  
  tft.pushImage(0, 0, bitcoinerWidth, bitcoinerHeight, bitcoinerImage);
  WaitAnyKey();

  /******** PRINT monkey SHIT *****/ 
  tft.pushImage(0, 0, monkey_talkWidth, monkey_talkHeight, monkey_talk);
  String text= String("NO BRO, THIS") + 
                      "IS HUGUE!   " + 
                      "100x FOR    " +
                      "SHURE!!";
  printTalkText(text);
  WaitAnyKey();

  /******** PRINT bitcoiner SHIT *****/ 
  tft.pushImage(0, 0, btc_talkWidth, btc_talkHeight, btc_talk);
  text =        String("DO YOU WANNA")  + 
                       "FIGHT? I'VE " + 
                       "LIGHTNING   " + 
                       "POWER!!";
  printTalkText(text);
  WaitAnyKey();

  /******** PRINT monkey FIGHT *****/ 
  tft.pushImage(0, 0, monkey_talkWidth, monkey_talkHeight, monkey_talk);
  tft.setTextFont(4);
  printTalkText("  OK! FIGHT!");
  WaitAnyKey();

  myGameState = STATE_INGAME;
  
}

/*****************⚡ GAME *********************/

void gamePrintHeader(uint8_t lives, uint16_t points){
  tft.fillRect(0,0,135,30, TFT_BLACK); //Print header
  if(lives>= 1) tft.pushImage(7, 8, heartWidth, heartHeight, heart); //Print 1rst Heard
  if(lives>= 2) tft.pushImage(27, 8, heartWidth, heartHeight, heart); //Print 2nd Heard
  if(lives>= 3) tft.pushImage(47, 8, heartWidth, heartHeight, heart); //Print 3rd Heard
  tft.setFreeFont(FMB9);
  tft.setTextColor(TFT_WHITE);
  //tft.setCursor(82,20);
  tft.setTextDatum(R_BASELINE);
  tft.drawString(String(points), 125, 20, GFXFF);
}

TFT_eSprite personaje = TFT_eSprite(&tft);  // Invoke library sprite
TFT_eSprite background = TFT_eSprite(&tft);  // Invoke library sprite

void WF_INGAME(void){
  
  tft.fillScreen(TFT_BLACK);
  
  /******** PRINT HEADER **************/
  gamePrintHeader(3,0);

  /******** CREATE SPRITES ***********/
  background.createSprite(135,209); //Background Sprite
  personaje.createSprite(19,33); //Avatar Sprite
  personaje.pushImage(0, 0, avatarWidth, avatarHeight, avatar); //Print avatar
  background.setSwapBytes(true);

  /********* CREATE OBJECTS **********/
  sGameObjects myObjects;
  
  int avatarPos = 57;
  int avatarPosPrev = 0;
  uint8_t lives = 3;
  uint16_t points = 0;
  unsigned long elapsed = millis();
  unsigned long elapsed2 = millis();
  while(true){
  
    /**** CHECK KEYS ****/
    btnMove.check();
    btnSelect.check();
    if(btnMove.click()&&(avatarPos<(135-avatarWidth-2))) avatarPos = avatarPos + avatarWidth;
    if(btnSelect.click()&&(avatarPos>=avatarWidth)) avatarPos = avatarPos - avatarWidth;

    /**** CREATE OBJECTS ***/
    if((millis() - elapsed2)>500){
      uint8_t rand = random(0,10); 
      
      if(rand < 5) myObjects.newObject(GreenGem);
      else if(rand < 9) myObjects.newObject(VioletGem);
      else if(rand == 9) myObjects.newObject(BTCcoin);
      
      elapsed2 = millis();
    }
    
    /**** OBJECTS MOVE ***/
    if((millis() - elapsed)>5){
      background.pushImage(0, 0, backG1Width, backG1Height, backG1); 
      myObjects.pushToSprite(&background);    
      personaje.pushToSprite(&background,avatarPos, 152, TFT_BLACK);
      background.pushSprite(0,31);
      if(myObjects.moveAndCheckColisions(avatarPos, lives, points)) gamePrintHeader(lives, points); //If exists colision repaint header
      elapsed = millis();
      if(lives == 0) break; //END GAME exit While
    }
  }

  //END GAME
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 79, GameOverWidth, GameOverHeight, GameOver);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("SCORE", 67, 50, GFXFF);
  tft.setTextColor(TFT_RED);
  tft.drawString(String(points), 67, 68, GFXFF);
  
  WaitAnyKey();
  myGameState = STATE_INIT;
}

