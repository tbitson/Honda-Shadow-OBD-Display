/******************************************************
   buttons.h - GUI button handler

   Version 1.10  tbitson  20Jun20

   Up to 12 button my be configured, decoded, and checked.
   Button state may be true (on), or false (off).
   Buttons may be active or inActive (greyed out)

   Varibales:
   int16_t X - starting X location
   int16_t Y - starting Y location
   String Label = text displayed in button. Approx 8 chars will fit
   bool State - ON or OFF. Off buttons have black background, on buttons are red
   bool Active - true or flase. Inactive buttons are greyed out and can't be selected

   todo:
   add hideButton() option

******************************************************/


#ifndef BUTTONS_H
#define BUTTONS_H


#include <XPT2046_Touchscreen.h>

#include "lcd.h"
#include "utils.h"

extern ILI9341_t3 lcd;
extern XPT2046_Touchscreen ts;

#define OFF false
#define ON  true

#define BUTTON_ERROR  -1


// prototypes
int8_t createButton(int16_t x, int16_t y, String label, bool state, bool active);
void drawButton(int8_t id);
void drawAllButtons(void);
void updateButton(int8_t id, bool state, bool active);
bool getButtonState(int8_t id);
bool getButtonActive(int8_t id);
int8_t checkButtons(void);
void discardButton(int8_t id);
void printButtonStatus(int8_t id);


// globals
int8_t nextID = 1;



// button constants and vars
#define MAX_BUTTONS 5
const int16_t BUTTON_HEIGHT = 36;
const int16_t BUTTON_WIDTH = 120;
const int16_t RADIUS = 4;
//int8_t id = 0;



struct Button {
  bool inUse;
  bool active;
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
  const char* label;
  bool state;
};


// button struct array
Button buttons[MAX_BUTTONS + 1];



int8_t createButton(int16_t x, int16_t y, const char* label, bool state, bool active)
{
  int8_t id;

  printValue("create button");

  // find first empty array location
  for (int8_t i = 1; i <= MAX_BUTTONS; i++)
  {
    if (!buttons[i].inUse)
    {
      id = nextID;
      buttons[id].x1 = x;
      buttons[id].x2 = x + BUTTON_WIDTH;
      buttons[id].y1 = y;
      buttons[id].y2 = y + BUTTON_HEIGHT;
      buttons[id].label = label;
      buttons[id].state = state;
      buttons[id].active = active;
      buttons[id].inUse = true;

      //printButtonStatus(id);

      nextID++;
      return id;
    }
  }
  Serial.println("Error - All Buttons in Use!");
  return BUTTON_ERROR;
}




void drawButton(int8_t id)
{
  printValue("drawButton");
  printButtonStatus(id);

  if (buttons[id].inUse == false)
    return;

  lcd.drawRoundRect(buttons[id].x1, buttons[id].y1, BUTTON_WIDTH, BUTTON_HEIGHT, RADIUS, ILI9341_YELLOW);
  lcd.fillRoundRect(buttons[id].x1 + 1, buttons[id].y1 + 1, BUTTON_WIDTH - 2, BUTTON_HEIGHT - 2, RADIUS, ILI9341_BLACK);

  if (buttons[id].active)
  {
    if (buttons[id].state == ON)
      lcd.fillRoundRect(buttons[id].x1 + 1, buttons[id].y1 + 1, BUTTON_WIDTH - 2, BUTTON_HEIGHT - 2, RADIUS, ILI9341_RED);
    else
      lcd.fillRoundRect(buttons[id].x1 + 1, buttons[id].y1 + 1, BUTTON_WIDTH - 2, BUTTON_HEIGHT - 2, RADIUS, ILI9341_BLACK);
  }

  if (buttons[id].active)
    lcd.setTextColor(ILI9341_WHITE);
  else
    lcd.setTextColor(ILI9341_DARKGREY);

  int16_t vPad = 12;
  int16_t pixelLen = lcd.strPixelLen((char*)buttons[id].label);
  int16_t hPad = (BUTTON_WIDTH - pixelLen) / 2;
  printValue("hPad", hPad);
  printValue("label", buttons[id].label);

  lcd.setFont(Arial_14);
  lcd.setCursor(buttons[id].x1 + hPad, buttons[id].y1 + vPad);
  lcd.print(buttons[id].label);
}



void drawAllButtons()
{
  for (int8_t i = 1; i <= MAX_BUTTONS; i++)
  {
    if (buttons[i].inUse)
      drawButton(i);
  }
}



void updateButton(int8_t id, bool state, bool active)
{
  printValue("update button");
  //printButtonStatus();

  buttons[id].state = state;
  buttons[id].active = active;

  drawButton(id);
}



bool getButtonState(int8_t id)
{
  return buttons[id].state;
}



bool getButtonActive(int8_t id)
{
  return buttons[id].active;
}



int8_t checkButtons()
{
  // is screen touched?
  if (!ts.touched())
    return 0;

  else
  {
    // Retrieve touch point
    TS_Point p = ts.getPoint();

    // map touch point using the calibration vals
    int x = map(p.x, TS_MINX, TS_MAXX, 0, lcd.width());
    int y = map(p.y, TS_MINY, TS_MAXY, 0, lcd.height());

    printValue("pos x", x);
    printValue("pos y", y);
    printValue("pressure", p.z);
    printValue("");

    // loop through button array
    for (int8_t i = 1; i <= MAX_BUTTONS; i++)
    {
      // only check active buttons
      if (buttons[i].active)
      {
        //printValue("button x1", buttons[i].x1);
        //printValue("button x2", buttons[i].x2);
        //printValue("button y1", buttons[i].y1);
        //printValue("button y2", buttons[i].y2);

        // check button coordinates to see if one matches touch point
        if (x > buttons[i].x1 && x < buttons[i].x2 && y > buttons[i].y1 && y < buttons[i].y2)
        {
          // have a match, now wait for button release
          while (ts.touched())
            delay(40);
          return i;
        }
      }
    }

    // no match, wait for button release
    while (ts.touched())
      delay(40);

    // print error message & return 0
    Serial.print("Invalid Button ");
    printValue("x", x);
    printValue("y", y);
    return 0;
  }
  return 0;
}


// free up position in array
void discardButton(int8_t id)
{
  buttons[id].inUse = false;
  buttons[id].label = "";
}



void printButtonStatus(int8_t id)
{
  printValue("num", id);
  printValue("x1", buttons[id].x1);
  printValue("y1", buttons[id].y1);
  printValue("label", buttons[id].label);
  printValue("active", buttons[id].active);
  printValue("in use", buttons[id].inUse);
}


#endif
