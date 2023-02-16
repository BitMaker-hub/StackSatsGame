#include "Wire.h"
#include "Lib/images.h"
#include "utils.h"
#include <TFT_eSPI.h> // Graphics and font library

/**********************⚡ GLOBAL Vars *******************************/
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
extern sButton btnMove;
extern sButton btnSelect;

/*****************⚡ PRINT FUNCTIONS *********************/

#define lineChars   12  //Numero de caracteres por linea
void printTalkText(String txt)
{
  int x=0; 
  tft.setCursor(23,25);
  tft.setTextFont(2);
  //tft.setFreeFont(FMB9);
  tft.setTextColor(TFT_BLACK);
  uint8_t line=0;
  while(true){
    delay(100);   
    tft.print(txt[x]);x++;
    if(x%lineChars == 0){
      line++;
      tft.setCursor(23,25 + (line * 15));
    }
    if(x == txt.length()) break;
  }
}

//Stops process until a key is pressed
void WaitAnyKey(){
  while(true){ 
    btnMove.check();
    btnSelect.check();
    if(btnMove.click()||btnSelect.click()) break; 
  }
}


/******************* BUTTON DETECTION ******************/
//sButton::sButton(byte bPin){ pin = bPin; pinMode(pin, INPUT); } //Constructor
void sButton::init(void){  pinMode(pin, INPUT); }     // Init pushbutton pin
int sButton::click(void){  return clickState; }
void sButton::forceClick(void){ clickState = ForcedClick;} //Generates one click loop

void sButton::check(void)
{
    const  unsigned long ButTimeout    = 250;
    const  unsigned long ButLongClick  = 5000;
           unsigned long msec = millis();

    byte but = digitalRead (pin);
    if (antState != but)  {
        antState = but;
        delay (10);           // **** debounce

        if (LOW == but)  {   // press
            if (msecLst)  { // 2nd press
                msecLst = 0; 
                clickState = DoubleClick;
                Serial.println("DoubleClick --> ");
                return;
            }
            else
                msecLst = 0 == msec ? 1 : msec;
        }
    }

    int elapsed = msec - msecLst;
    if (msecLst && (elapsed > ButTimeout) && (elapsed < ButLongClick))  {
        if(but != LOW) {
          msecLst = 0;
          clickState = SingleClick;
          Serial.println("SingleClick --> ");
          return;
        }
    }

    //LongClick 
    if(msecLst && (but == antState) && (but == LOW)) {
        if(elapsed > ButLongClick) {
          msecLst = 0; 
          clickState = LongClick;
          Serial.println("LongClick --> ");
          return;
        }
    }

    //ForcedClick
    if(clickState == ForcedClick){ 
      clickState = SingleClick;
      return;
    }
    clickState = None;
}

/******************* ICON FUNCTIONS ******************/
sObject::sObject(uint8_t _xPos, uint8_t _yPos, uint8_t _iconType, bool randomX)// Constructor
 {
    xPos = _xPos; yPos = _yPos; iconType = _iconType;
    if(randomX) xPos = random(0,7)*COLUMN_WIDHT;
    isEnabled = true; 
 }

bool sObject::move(void)
{
    if(!isEnabled) return false;
    
    if(iconType != Bolt){
      yPos=yPos+MOVEMENT_GAP;
      if(yPos>FLOOR_POS) { isEnabled=false; return true; }
    }
    else{
      yPos=yPos-MOVEMENT_GAP;
      if(yPos<=CEELLING_POS) isEnabled=false; 
    }
    return false;
}

// ------------- ARRAY of OBJECTS -----------/


int sGameObjects::add(sObject newObject){
    //initialize objects all disabled
    for(int x=0; x<MAX_OBJECTS; x++) 
      if(object[x].isEnabled == false){
        object[x].isEnabled = true;
        object[x].xPos = newObject.xPos;
        object[x].yPos = newObject.yPos;
        object[x].iconType = newObject.iconType;
        return x;
      }

    return -1;   
} 

void sGameObjects::newObject(uint8_t iconType){
  sObject obj(0,0,iconType,true);
  add(obj);  
}

TFT_eSprite objSprite = TFT_eSprite(&tft);  // Invoke library sprite
void sGameObjects::pushToSprite(TFT_eSprite* background){

  objSprite.createSprite(coinWidth,coinHeight); //Gem Sprite
  
  for(int x=0; x<MAX_OBJECTS; x++)
     if(object[x].isEnabled){
       switch(object[x].iconType){
          case BTCcoin: objSprite.pushImage(0, 0, coinWidth, coinHeight, coin); break;
          case VioletGem: objSprite.pushImage(0, 0, gemaVioletaWidth, gemaVioletaHeight, gemaVioleta); break;
          case GreenGem:
          default:      objSprite.pushImage(0, 0, gemaVerdeWidth, gemaVerdeHeight, gemaVerde); break;
       }
       objSprite.pushToSprite(background, object[x].xPos, object[x].yPos, TFT_BLACK);
     }

}

bool sGameObjects::moveAndCheckColisions(int avatarPos, uint8_t &lives, uint16_t &points){

  bool existColision = false;
  for(int x=0; x<MAX_OBJECTS; x++)
     if(object[x].isEnabled){ 
        if(object[x].move()){ //If true means a gem reaches floor +1 point
            points=points+1; 
            existColision = true;
        } 
        if((object[x].yPos > (FLOOR_POS - avatarHeight)) && (object[x].yPos < FLOOR_POS) &&
           //(object[x].xPos >= avatarPos) && (object[x].xPos <= (avatarPos + avatarWidth))){
           (object[x].xPos == avatarPos)){
          //Colision detected
          existColision = true;
          object[x].isEnabled = false; //Disappear object
          switch(object[x].iconType){
            case BTCcoin:  points=points+10; break;
            case VioletGem: 
            case GreenGem: if(lives>0) lives=lives -1;
          }
       }
     }
  return existColision;
}
