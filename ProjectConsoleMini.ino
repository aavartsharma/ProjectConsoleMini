#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>


#define BUTTON_UP 8
#define BUTTON_LEFT 5
#define BUTTON_DOWN 4
#define BUTTON_RIGHT 2
#define MENU_START 6
#define MENU_SELECT 7
#define ACTION_O 11
#define ACTION_X 12 

#define BUZZER A1

#define MAX_SHOOT 10
#define MAX_ENEMY 6


// Create the U8g2 object for SH1106 128x64 I2C display
// U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int state;
const char Made[] PROGMEM = "Made By"; 
const char NAME[] PROGMEM =  "Vart Innovations";
const char PressB[] PROGMEM = "Press Start Button!";
// const char MainMenu[] PROGMEM = "Main Menu";
// const char Galaga[] = "Galaga";
// const char Snake[] = "Snake";
// const char Flappy_Brid[] = "Flappy Brid";
const char MainMenu = 1;
const int Galaga = 2;
const int Snake = 3;
const int Flappy_Brid = 4;

struct DirectVector
{
  int x;
  int y;
};
DirectVector shotList[MAX_SHOOT];
int arraySize =0;

DirectVector enemyList[MAX_ENEMY];
int arraySizeEnemy =0;
char buffer[20];

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
  strcpy_P(buffer, Made);
  u8g2.drawStr(5,20,buffer);
  u8g2.setFont(u8g2_font_helvB10_tf);
  strcpy_P(buffer, NAME);
  u8g2.drawStr(5, 45,buffer);
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tf);
  strcpy_P(buffer, PressB);
  u8g2.drawStr(10,40,buffer);
  u8g2.sendBuffer();
  while (true)
  {
    delay(100);
    if(digitalRead(MENU_START))
    {
      // strcpy_P(buffer, MainMenu);
      state = MainMenu;
      break;
    }
  }
}
void loop() 
{
  // strcpy_P(buffer, MainMenu);
  Serial.println(state == MainMenu);
  if(state == MainMenu)
  {
    int yc = 1;
    while(true)
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvB10_tf);
      strcpy_P(buffer, (PGM_P)F("Galaga"));
      u8g2.drawStr(15,15,buffer);
      u8g2.drawLine(00, 20, 127, 20);
      strcpy_P(buffer, (PGM_P)F("Snake"));
      u8g2.drawStr(15,40,buffer);
      u8g2.drawLine(00, 45, 127, 45);
      strcpy_P(buffer, (PGM_P)F("Flappy Brid"));
      u8g2.drawStr(15,60,buffer);
      u8g2.drawLine(00, 63, 127, 63);
      u8g2.drawDisc(111,11*(yc*2-1), 5);
      u8g2.sendBuffer();
      yc -= Nav_Button().y;
      delay(100);
      if(yc >= 4)
      {
        yc = 1;
      }
      else if (yc <= 0)
      {
        yc = 3;
      }
      if(digitalRead(ACTION_O))
      {
        if(yc == 1)
        {
          state = Galaga;
        }
        if(yc == 2)
        {
          state = Snake;
        }
        if(yc == 3)
        {
          state = Flappy_Brid;
        }
        break;
      }
    }
    if(state == Galaga)
    {
      // for (int i = 0; i < MAX_SHOOT; i++) 
      // {
      //   shotList[i] = 0;
      // }
      arraySize = 0;
      arraySizeEnemy = 0;
      for(int i = 1; i < 4; i++)
      {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_helvB14_tf);
        char str[3];                        // Create a char array to hold the string
        itoa(i, str, 10);
        u8g2.drawStr(60,32,str);
        u8g2.drawLine(0, 0, 127,0);
        u8g2.drawLine(127, 0, 127,63);
        u8g2.drawLine(127, 63,0, 63);
        u8g2.drawLine(0, 127, 0,0);
        u8g2.sendBuffer();
        delay(1000);
      }
      DirectVector playerPosition;
      
      playerPosition.x = 10;
      playerPosition.y = 32;
      while(true)
      {
        if(state == MainMenu)
        {
          break;
        }
        u8g2.clearBuffer();
        
        u8g2.drawLine(0, 0, 127,0);
        u8g2.drawLine(127, 0, 127,63);
        u8g2.drawLine(127, 63,0, 63);
        u8g2.drawLine(0, 127, 0,0);
        // playerPosition.x += Nav_Button().x;   // can be use to move player forward and backwards
        playerPosition.y -= Nav_Button().y;
        if(playerPosition.x >= 128)
        {
          playerPosition.x = 127;
        }
        if(playerPosition.x < 0)
        {
          playerPosition.x = 0;
        }
        if(playerPosition.y >= 51)
        {
          playerPosition.y = 50;
        }
        if(playerPosition.y <= 4)
        { 
          playerPosition.y = 5;
        }
        if(digitalRead(ACTION_O))
        {
          DirectVector newShot;
          newShot.x = 6 + playerPosition.x;
          newShot.y = playerPosition.y;
          addElement(newShot);
        }
        if(random(5) == 0 && millis() % 500 <= 20)
        {
          DirectVector place;
          place.x = 115;
          place.y = random(8,56);
          addEnemy(place);
        }
        Draw_player(playerPosition);
        for (int i = 0; i < arraySize; i++) 
        {
          DrawShot(shotList[i]);
          shotList[i].x += 2;
          if(shotList[i].x >= 128)
          {
            removeElement(i);
          }
        }

        for (int i = 0; i < arraySizeEnemy; i++)
        {
          DrawEnemy(enemyList[i]);
          // Serial.print(enemyList[i].x);
          // Serial.print("     ");
          // Serial.println(enemyList[i].y);
          enemyList[i].x -= 1;
          if(enemyList[i].x <= 20)
          {
            // game over
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_helvB14_tf);
            strcpy_P(buffer, (PGM_P)F("Game Over!"));
            u8g2.drawStr(10,32,buffer);
            u8g2.sendBuffer();
            delay(5000);
            state = MainMenu;
            break;
          }
          for (int j =0; j < arraySize; j++)
          {
            // Serial.print(enemyList[i].y + 5 );
            // Serial.print("   ");
            // Serial.print(shotList[j].y );
            // Serial.print("   ");
            // Serial.println(enemyList[i].y - 5);
            // Serial.print("   ");
            // Serial.println(((enemyList[i].y + 5) > shotList[j].y > (enemyList[i].y - 5)));
            if(((enemyList[i].x + 5) > shotList[j].x) && (shotList[j].x > (enemyList[i].x - 5)) && ((enemyList[i].y + 5) > shotList[j].y) && (shotList[j].y > (enemyList[i].y - 5)))
            {
              removeElement(j);
              removeEnemy(i);
            }

          }
        }
        u8g2.sendBuffer();
        delay(25);
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

void addElement(DirectVector newElement) 
{
  if (arraySize < MAX_SHOOT) 
  {
    shotList[arraySize] = newElement;
    arraySize++;
  }
}

void addEnemy(DirectVector newElement) 
{
  if (arraySizeEnemy < MAX_ENEMY) 
  {
    enemyList[arraySizeEnemy] = newElement;
    arraySizeEnemy++;
  }
}

void removeElement(int index)
{
  for (int i = index; i < arraySize - 1; i++) 
  {
    shotList[i] = shotList[i + 1];
  }
  arraySize--;
}

void removeEnemy(int index)
{
  for (int i = index; i < arraySizeEnemy - 1; i++) 
  {
    enemyList[i] = enemyList[i + 1];
  }
  arraySizeEnemy--;
}

void Draw_player(DirectVector pos) 
{
  for (int i = 0; i < 9; i++) 
  {
    for (int j = 0; j < 3; j++) 
    {
      u8g2.drawPixel(pos.x + j, pos.y + i);
    }
  }
  u8g2.drawBox(pos.x , pos.y , 3, 9);
  u8g2.drawBox(pos.x + 3, pos.y + 3, 3, 3);
}

void DrawShot(DirectVector pos)
{
  u8g2.drawBox(pos.x + 3, pos.y + 3, 3, 3);
}

void DrawEnemy(DirectVector pos)
{
  u8g2.drawTriangle(pos.x-5, pos.y, pos.x+5, pos.y +5, pos.x +5, pos.y-5);
}

DirectVector Nav_Button()
{
  DirectVector dir;
  dir.x = 0;
  dir.y =0;
  if(digitalRead(BUTTON_UP))
  {
    dir.y = 1;
  }
  if(digitalRead(BUTTON_LEFT))
  {
    dir.x = -1;
  }
  if(digitalRead(BUTTON_DOWN))
  {
    dir.y = -1;
  }
  if(digitalRead(BUTTON_RIGHT))
  {
    dir.x = 1;
  }
  return dir;
}

// void Show_menu


