/************************************
     table.h - displays the decoded
     data in a table format

     version 0.1   07Jun20

 ************************************/

#include "lcd.h"
#include "obd.h"


extern bool getTable11();


#define COLS  3
#define ROWS  6

const uint16_t CellX[] = {0, 150, 240, 320};
const uint16_t CellY[] = {40, 70, 100, 130, 160, 190};
const uint16_t CellW[] = {148, 68, 76};
const uint16_t CellH = 28;

const uint16_t txtOffset = 8;


#define TABLE_TITLE "Honda OBD Data Table 1"


// display globals
extern float mph, ectf, battery;
extern uint16_t rpm;
extern uint8_t gear;



void initTable()
{
  uint8_t i;

  lcd.fillScreen(ILI9341_BLACK);
  lcd.drawRoundRect(0, 0, LCD_WIDTH, LCD_HEIGHT, 4, ILI9341_RED);

  lcd.setTextColor(ILI9341_WHITE);
  lcd.setFont(Arial_14_Bold);
  lcd.setCursor(40, 10);
  lcd.print(TABLE_TITLE);


  for (i = 0; i < ROWS; i++)
    lcd.drawFastHLine(1, CellY[i], LCD_WIDTH - 1, ILI9341_DARKGREY);

  for (i = 1; i < COLS; i++)
    lcd.drawFastVLine(CellX[i], 40, 150, ILI9341_DARKGREY);

}



void displayTable11()
{
   initTable();
   
  bool disp = true;

  while (getTable11() && disp)
  {
    lcd.setTextColor(ILI9341_WHITE);
    lcd.setFont(Arial_14);

    // row 0 y = 0
    lcd.fillRect(CellX[0] + 2, CellY[0] + 2, CellW[0], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[0] + txtOffset, CellY[0] + txtOffset);
    lcd.print("Speed");

    lcd.fillRect(CellX[1] + 2, CellY[0] + 2, CellW[1], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[1] + txtOffset, CellY[0] + txtOffset);
    lcd.print(mph);

    lcd.fillRect(CellX[2] + 2, CellY[0] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[2] + txtOffset, CellY[0] + txtOffset);
    lcd.print("MPH");


    // row 1 y = 1
    lcd.fillRect(CellX[0] + 2, CellY[1] + 2, CellW[0], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[0] + txtOffset, CellY[1] + txtOffset);
    lcd.print("Engine");

    lcd.fillRect(CellX[1] + 2, CellY[1] + 2, CellW[1], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[1] + txtOffset, CellY[1] + txtOffset);
    lcd.print(rpm);

    lcd.fillRect(CellX[2] + 2, CellY[1] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[2] + txtOffset, CellY[1] + txtOffset);
    lcd.print("RPM");


    // row 2 y = 2
    lcd.fillRect(CellX[0] + 2, CellY[2] + 2, CellW[0], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[0] + txtOffset, CellY[2] + txtOffset);
    lcd.print("Gear");

    lcd.fillRect(CellX[1] + 2, CellY[2] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[1] + txtOffset, CellY[2] + txtOffset);
    lcd.print(gear);

    lcd.fillRect(CellX[2] + 2, CellY[2] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[2] + txtOffset, CellY[2] + txtOffset);
    lcd.print("");


    // row 3 y = 3
    lcd.fillRect(CellX[0] + 2, CellY[3] + 2, CellW[0], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[0] + txtOffset, CellY[3] + txtOffset);
    lcd.print("Battery");

    lcd.fillRect(CellX[1] + 2, CellY[3] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[1] + txtOffset, CellY[3] + txtOffset);
    lcd.print(battery);

    lcd.fillRect(CellX[2] + 2, CellY[3] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[2] + txtOffset, CellY[3] + txtOffset);
    lcd.print("Volts");



    // row 4
    lcd.fillRect(CellX[0] + 2, CellY[4] + 2, CellW[0], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[0] + txtOffset, CellY[4] + txtOffset);
    lcd.print("ECT");

    lcd.fillRect(CellX[1] + 2, CellY[4] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[1] + txtOffset, CellY[4] + txtOffset);
    lcd.print(ectf);

    lcd.fillRect(CellX[2] + 2, CellY[4] + 2, CellW[2], CellH, ILI9341_BLACK);
    lcd.setCursor(CellX[2] + txtOffset, CellY[4] + txtOffset);
    lcd.print("Degs");

    delay(1000);

    // is screen touched?
    if (ts.touched())
      disp = false;
  }
}
