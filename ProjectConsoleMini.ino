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
int yc = 1;
const char Made[] PROGMEM = "Made By"; 
const char NAME[] PROGMEM =  "Vart Innovations";
const char PressB[] PROGMEM = "Press Start Button!";
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
  u8g2.drawStr((byte)5,(byte)20,buffer);
  u8g2.setFont(u8g2_font_helvB10_tf);
  strcpy_P(buffer, NAME);
  u8g2.drawStr((byte)5, (byte)45,buffer);
  u8g2.sendBuffer();
  delay(2000);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tf);
  strcpy_P(buffer, PressB);
  u8g2.drawStr((byte)10,(byte)40,buffer);
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
    while(true)
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvB10_tf);
      strcpy_P(buffer, (PGM_P)F("Galaga"));
      u8g2.drawStr(15,15,buffer);
      u8g2.drawLine((byte)00, (byte)20, (byte)127,(byte) 20);
      strcpy_P(buffer, (PGM_P)F("Snake"));
      u8g2.drawStr((byte)15,(byte)40,buffer);
      u8g2.drawLine((byte)00, (byte)45, (byte)127, (byte)45);
      strcpy_P(buffer, (PGM_P)F("Flappy Brid"));
      u8g2.drawStr((byte)15,(byte)60,buffer);
      u8g2.drawLine((byte)00, (byte)63, (byte)127, (byte)63);
      u8g2.drawDisc((byte)111,11*(yc*2-1), (byte)5);
      u8g2.sendBuffer();
      yc -= Nav_Button().y;
      delay((byte)100);
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
    arraySize = 0;
    arraySizeEnemy = 0;
    for(int i = 1; i < 4; i++)
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvB14_tf);
      char str[3];                        // Create a char array to hold the string
      itoa(i, str, 10);
      u8g2.drawStr((byte)60,(byte)32,str);
      u8g2.drawLine((byte)0, (byte)0,(byte) 127,(byte)0);
      u8g2.drawLine((byte)127,(byte) 0, (byte)127,(byte)63);
      u8g2.drawLine((byte)127, (byte)63,(byte)0, (byte)63);
      u8g2.drawLine((byte)0, (byte)127,(byte) 0,(byte)0);
      u8g2.sendBuffer();
      delay(1000);
    }
    DirectVector playerPosition;
    if(state == Galaga)
    {
      Serial.println(state);
      playerPosition.x = 10;
      playerPosition.y = 32;
      while(true)
      {
        if(state == MainMenu)
        {
          break;
        }
        u8g2.clearBuffer();
        
        u8g2.drawLine((byte)0, (byte)0, (byte)127,(byte)0);
        u8g2.drawLine((byte)127, (byte)0, (byte)127,(byte)63);
        u8g2.drawLine((byte)127, (byte)63,(byte)0, (byte)63);
        u8g2.drawLine((byte)0, (byte)127, (byte)0,(byte)0);
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
        if(playerPosition.y >= 56)
        {
          playerPosition.y = 55;
        }
        if(playerPosition.y <= 4)
        { 
          playerPosition.y = 5;
        }
        if(digitalRead(MENU_START))
        {
          state = MainMenu;
        }
        if(digitalRead(ACTION_O))
        {
          DirectVector newShot;
          newShot.x = playerPosition.x + 6;
          newShot.y = playerPosition.y;
          addElement(newShot);
        }
        if(random(5) == 0 && millis() % 500 <= 20)
        {
          DirectVector place;
          place.x = 115;
          place.y = random(8,55);
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
            tone(A1, 400);
          }
        }

        for (int i = 0; i < arraySizeEnemy; i++)
        {
          DrawEnemy(enemyList[i]);
          enemyList[i].x -= 1;
          if(enemyList[i].x <= 20)
          {
            // game over
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_helvB14_tf);
            strcpy_P(buffer, (PGM_P)F("Game Over!"));
            u8g2.drawStr((byte)10,(byte)32,buffer);
            u8g2.sendBuffer();
            delay(5000);
            state = MainMenu;
            break;
          }
          for (int j =0; j < arraySize; j++)
          {
            if(((enemyList[i].x + 5) > shotList[j].x) && (shotList[j].x > (enemyList[i].x - 5)) && ((enemyList[i].y + 5) > shotList[j].y) && (shotList[j].y > (enemyList[i].y - 5)))
            {
              removeElement(j);
              removeEnemy(i);
              tone(A1, 400);
            }
          }
        }
        u8g2.sendBuffer();
        delay(25);
      }
    }
    if(state == Snake)
    {
      playerPosition.x = 64;
      playerPosition.y = 32;
      DirectVector dir;
      DirectVector enemyPos;
      enemyPos.x = 0;
      enemyPos.y = 0;
      int score =0;
      dir.x = 1;
      dir.y = 0;
      while(true)
      {
        if(state == MainMenu)
        {
          break;
        }
        u8g2.clearBuffer();
        
        u8g2.drawLine((byte)0, (byte)0, (byte)127,(byte)0);
        u8g2.drawLine((byte)127, (byte)0, (byte)127,(byte)50);
        u8g2.drawLine((byte)127, (byte)50,(byte)0, (byte)50);
        u8g2.drawLine((byte)0, (byte)50, (byte)0,(byte)0);
        char str[3]; 
        itoa(score, str, 10);
        u8g2.setFont(u8g2_font_helvB08_tf);
        u8g2.drawStr(20,60,str);
        if(digitalRead(MENU_START))
        {
          state = MainMenu;
        }
        if(Nav_Button().x > 0 && dir.x != -1 && dir.x != 1)
        {
          dir.x = 1;
          dir.y = 0;
          if (arraySize <= 10) addElement(playerPosition);
        }
        if(Nav_Button().x < 0 && dir.x != 1 && dir.x !=-1)
        {
          dir.x = -1;
          dir.y = 0;
          if (arraySize <= 10) addElement(playerPosition);
        }
        if(Nav_Button().y > 0 && dir.y != -1 && dir.y != 1)
        {
          dir.x = 0;
          dir.y = 1;
          if (arraySize <= 10) addElement(playerPosition);
        }
        if(Nav_Button().y < 0 && dir.y != 1 && dir.y != -1)
        {
          dir.x = 0;
          dir.y = -1;
          if (arraySize <= 10 ) addElement(playerPosition);
        }
        playerPosition.x += dir.x;
        playerPosition.y -= dir.y;

        // position cheak
        if(playerPosition.x >= 127 && inList(playerPosition))
        {
          playerPosition.x = 1;
        }
        if(playerPosition.x <= 0 && inList(playerPosition))
        {
          playerPosition.x = 126;
        }
        if(playerPosition.y >= 45 && inList(playerPosition))
        {
          playerPosition.y = 1;
        }
        if(playerPosition.y <= 0 && inList(playerPosition))
        { 
          playerPosition.y = 44;
        }
        if(enemyPos.x != 0)
        {
          u8g2.drawDisc(enemyPos.x,enemyPos.y,3);
        }
        if(enemyPos.x == 0)
        {
          enemyPos.x = random(1,125);
          enemyPos.y = random(1,44);
        }
        // DrawSnake(playerPosition, score+1);
        DrawShot(playerPosition); // player
        // u8g2.drawPixel(playerPosition.x, playerPosition.y);
        // u8g2.drawPixel(playerPosition.x,playerPosition.y);
        if( (playerPosition.x > (enemyPos.x-5)) && (playerPosition.x < (enemyPos.x + 5)) && (playerPosition.y > (enemyPos.y - 5)) && (playerPosition.y < (enemyPos.y + 5)))
        {
          enemyPos.x = 0;
          score++;
          tone(A1, 400);
        }
        u8g2.sendBuffer();
        delay(25);
      }
    }

    if(state == Flappy_Brid)
    {
      playerPosition.x = 64;
      playerPosition.y = 32;
      DirectVector dir;
      dir.x = 64;
      dir.y =32;
      int score =0;
      while(true)
      {
        if(state == MainMenu)
        {
          break;
        }
        u8g2.clearBuffer();
        
        u8g2.drawLine((byte)0, (byte)0, (byte)127,(byte)0);
        u8g2.drawLine((byte)127, (byte)0, (byte)127,(byte)50);
        u8g2.drawLine((byte)127, (byte)50,(byte)0, (byte)50);
        u8g2.drawLine((byte)0, (byte)50, (byte)0,(byte)0);
        char str[3]; 
        itoa(score, str, 10);
        u8g2.setFont(u8g2_font_helvB08_tf);
        u8g2.drawStr(20,60,str);
        if(digitalRead(MENU_START))
        {
          state = MainMenu;
        }
        if(Nav_Button().y > 0)
        {
          playerPosition.y-=4;
        }
        else {
          playerPosition.y +=1;
        }
        if( playerPosition.y > 50 || playerPosition.y < 0)
        {
          u8g2.clearBuffer();
          u8g2.setFont(u8g2_font_helvB14_tf);
          strcpy_P(buffer, (PGM_P)F("Game Over!"));
          u8g2.drawStr((byte)10,(byte)32,buffer);
          u8g2.sendBuffer();
          delay(5000);
          state = MainMenu;
          break;
        }
        u8g2.drawBox(16, playerPosition.y, 7, 5); // player
        if(millis() % 2000 <= 20)
        {
          dir.x = 110;
          dir.y = random(5,25);
          addElement(dir);
        } 
        for (int i =0; i < arraySize; i++)
        {
          DrawBlocks(shotList[i]);
          shotList[i].x -= 1;
          if(shotList[i].x <= -15)
          {
            removeElement(i);
            score++;
            tone(A1, 400);
          }
        }
        u8g2.sendBuffer();
        delay(25);
      }
    }
  }
}

void drawThickLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t thickness) {
  for (int i = 0; i < thickness; i++) {
    u8g2.drawLine(x0, y0 + i, x1, y1 + i); // For a horizontal thick line
  }
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
bool inList(DirectVector x)
{
  for (int i = 0; i < arraySize; i++) 
  {
    if (shotList[i].x == x.x && shotList[i].y == x.y) 
    {
      return true;
    }
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
  for (int i = 0; i < 6; i++) 
  {
    for (int j = 0; j < 3; j++) 
    {
      u8g2.drawPixel(pos.x + j, pos.y + i);
    }
  }
  u8g2.drawBox(pos.x , pos.y-3, 3,10);
  u8g2.drawBox(pos.x + 3, pos.y , 3, 3);
}

void DrawBlocks(DirectVector pos)
{
  // randome
  int h1 = pos.y;
  u8g2.drawBox(pos.x, 0, 10, h1);
  u8g2.drawBox(pos.x,h1+25,10, 25-h1);

}

void DrawSnake(DirectVector pos, int length)
{
  int totalLength = 0;  // Track total drawn length
  int segmentLength = 0;  // Length of each segment
  
  // First, handle the head of the snake
  if (shotList[0].x == pos.x && shotList[0].y == pos.y) {
    // Handle when the snake hasn't moved much yet
    u8g2.drawPixel(pos.x, pos.y);  // Draw head
    return;
  }

  // Draw the lines between each turn point in shotList
  for (int i = 0; i < arraySize - 1; i++) {
    // Calculate the length of the line segment between shotList[i] and shotList[i + 1]
    if (shotList[i].x == shotList[i + 1].x) {
      // Vertical line
      segmentLength = abs(shotList[i].y - shotList[i + 1].y);
    } else if (shotList[i].y == shotList[i + 1].y) {
      // Horizontal line
      segmentLength = abs(shotList[i].x - shotList[i + 1].x);
    } else {
      // Not a straight line, show an error
      strcpy_P(buffer, (PGM_P)F("Error 483"));
      u8g2.drawStr(64, 32, buffer);
      return;
    }
    
    // Check if drawing the full segment would exceed the snake's length
    if (totalLength + segmentLength > length) {
      // Calculate how much of the segment to draw to match the desired length
      int remainingLength = length - totalLength;

      // Draw only part of the current segment
      if (shotList[i].x == shotList[i + 1].x) {
        // Vertical line
        if (shotList[i].y < shotList[i + 1].y) {
          // Moving down
          u8g2.drawLine(shotList[i].x, shotList[i].y, shotList[i + 1].x, shotList[i].y + remainingLength);
        } else {
          // Moving up
          u8g2.drawLine(shotList[i].x, shotList[i].y, shotList[i + 1].x, shotList[i].y - remainingLength);
        }
      } else if (shotList[i].y == shotList[i + 1].y) {
        // Horizontal line
        if (shotList[i].x < shotList[i + 1].x) {
          // Moving right
          u8g2.drawLine(shotList[i].x, shotList[i].y, shotList[i].x + remainingLength, shotList[i].y);
        } else {
          // Moving left
          u8g2.drawLine(shotList[i].x, shotList[i].y, shotList[i].x - remainingLength, shotList[i].y);
        }
      }
      return;  // Snake is fully drawn, stop here
    } else {
      // Draw the full segment
      u8g2.drawLine(shotList[i].x, shotList[i].y, shotList[i + 1].x, shotList[i + 1].y);
      totalLength += segmentLength;  // Update the total drawn length
    }
  }
}




void DrawSnakeNew(DirectVector dir, int length)
{
  u8g2.drawPixel(dir.x, dir.y);
  for (int i =1; i<=length; i++)
  {

  }
}
void DrawShot(DirectVector pos)
{
  u8g2.drawBox(pos.x, pos.y, 3, 3);
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


