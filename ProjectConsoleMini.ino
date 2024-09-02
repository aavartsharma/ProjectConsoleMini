#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>


#define BUTTON_UP 8
#define BUTTON_LEFT 5
#define BUTTON_DOWN 4
#define BUTTON_RIGHT 2
#define MENU_START 6
#define MENU_SELECT 7
#define ACTION_O 11
#define ACTION_X 12 

#define BUZZER A1

// Create the U8g2 object for SH1106 128x64 I2C display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

String state = "";
const char MainMenu[] PROGMEM = "Main Menu";

struct DirectVector
{
  int x;
  int y;
};


void setup() 
{
  pinMode(BUTTON_UP,INPUT);
  pinMode(BUTTON_LEFT,INPUT);
  pinMode(BUTTON_DOWN,INPUT);
  pinMode(BUTTON_RIGHT,INPUT);
  pinMode(MENU_START,INPUT);
  pinMode(MENU_SELECT,INPUT);
  pinMode(ACTION_O,INPUT);
  pinMode(ACTION_X,INPUT);

  pinMode(BUZZER, OUTPUT);

  

  Serial.begin(9600);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.drawStr(5,20,("Made By"));
  u8g2.setFont(u8g2_font_helvB10_tf);
  u8g2.drawStr(5, 45, ("Vart Innovations"));
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.drawStr(10,40,("Press Start Button!"));
  u8g2.sendBuffer();
  while (true)
  {
    delay(100);
    if(digitalRead(MENU_START))
    {
      state = MainMenu;
      break;
    }
  }
}
int stateNum = 1;
void loop() 
{
  
  if(state == MainMenu)
  {
    int yc = 0;
    int pos = 12;
    while(true)
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvB10_tf);
      u8g2.drawStr(15,15,"Galaga");
      u8g2.drawLine(00, 20, 127, 20);
      u8g2.drawStr(15,40,("Snake"));
      u8g2.drawLine(00, 45, 127, 45);
      u8g2.drawStr(15,60,("Flappy Brid"));
      u8g2.drawLine(00, 63, 127, 63);
      u8g2.drawCircle(100,pos, 5);
      u8g2.sendBuffer();
      yc += Nav_Button().y;
      delay(100);
      Serial.println(yc);
      if(yc < 1)
      {
        yc =3;
      }
      if(yc > 3)
      {
        yc =1;
      }
      if(yc == 1)
      {
        pos = 12;
      }
      if(yc == 2)
      {
        pos =33;
      }
      if(yc == 3)
      {
        pos = 12;
      }
    }
    
  }
  
  //   String b = String(digitalRead(MENU_PAUSE));
  //   String c = String(digitalRead(MENU_SELECT));
  //   String d = String(digitalRead(ACTION_O));
  //   String e = String(digitalRead(ACTION_X));
  //   Serial.println(b);
  //   Serial.println(c);
  //   Serial.println(d);
  //   Serial.println(e);
  //   delay(100);

  // u8g2.clearBuffer();

  // // Draw a rectangle
  // u8g2.drawBox(10, 10, 50, 30); // x, y, width, height

  // // Draw a circle
  // u8g2.drawCircle(90, 32, 20); // x, y (center), radius

  // u8g2.sendBuffer();
  // delay(1000);
}

DirectVector Nav_Button()
{
  DirectVector dir;
  dir.x = 0;
  dir.y =0;
  if(digitalRead(BUTTON_UP))
  {
    dir.y = -1;
  }
  if(digitalRead(BUTTON_LEFT))
  {
    dir.x = -1;
  }
  if(digitalRead(BUTTON_DOWN))
  {
    dir.y = 1;
  }
  if(digitalRead(BUTTON_RIGHT))
  {
    dir.x = 1;
  }
  return dir;
}

// void Show_menu


