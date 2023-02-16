#include <TFT_eSPI.h> // Graphics and font library 

/************** TYPES **********************/
typedef struct {
   uint8_t State;           //Current state
}sObjects;

/**************🍃 Functions ********************/
void printTalkText(String txt);
void WaitAnyKey(void);

/**************🍃 BUTTON ********************/
#define PIN_MOVE          35
#define PIN_SELECT        0
enum { None, SingleClick, DoubleClick, LongClick, ForcedClick };

class sButton
   {  private:
         uint8_t antState;           //Button previous pin state
         uint8_t pin;                //Button physical pin number
         unsigned long msecLst;   //Button last time was pressed
 
      public:
         uint8_t clickState;      //Button click [None, SingleClick, DoubleClick, LongClick]
         sButton(uint8_t bPin){ pin = bPin; pinMode(pin, INPUT); }      // Constructor
         void init(void);         // Init button pin
         void check(void) ;        // Declaracion de funcion externa
         int click(void) ;         // Declaracion de funcion externa
         void forceClick(void);    //Generate a click
   } ;

/************** ICON ********************/
#define MAX_COLUMNS   7
#define COLUMN_WIDHT  19
#define MAX_OBJECTS   30
#define MOVEMENT_GAP  2
#define FLOOR_POS     169
#define CEELLING_POS  0

enum { GreenGem, VioletGem, BTCcoin, Bolt };

class sObject
   {   
      public:
         bool isEnabled;          //Indicates if is icon enabled
         uint8_t yPos;            //Position Y icon
         uint8_t xPos;            //Position x icon
         uint8_t iconType;        //Icon image [GreenGem, BlueGem, BTCcoin, Bolt]
         
         bool move(void);         //move
         sObject(uint8_t _xPos, uint8_t _yPos, uint8_t _iconType, bool randomX);// Constructor
         sObject(void)// Constructor
         {
            xPos = yPos = iconType = 0;
            isEnabled = false; 
         }
   } ;

class sGameObjects
{
  private:
      int add(sObject newObject);//Search place on array an add
  public:
       sObject object[MAX_OBJECTS];   
       void pushToSprite(TFT_eSprite* background);   
       void newObject(uint8_t iconType);
       bool moveAndCheckColisions(int avatarPos, uint8_t &lives, uint16_t &points);
       sGameObjects(void){      // Constructor
        for(int x=0; x<MAX_OBJECTS; x++) object[x].isEnabled = false;
       }
       
};

