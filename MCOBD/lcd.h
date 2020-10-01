/*************************************************
   lcd.h - initialization stuff for demo-ing buttons
   using an TFT LCD display on teensy. See the demo
   code from the Teensyduino library for details.

   Version 1.1

 ***************************************************/


#ifndef LCD_H
#define LCD_H


#include <Wire.h>
#include "ILI9341_t3.h"
#include <XPT2046_Touchscreen.h>
#include <font_Arial.h>
#include <font_ArialBold.h>

#define TEENSY_32

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000


// lcd interface
#define TFT_DC    9   // Serial 2 Rx
#define TFT_CS    10  // Serial 2 Tx
#define TFT_RST   255 // (unused, tie to +3.3)
#define TFT_SDI   11  // SPI DOUT  
#define TFT_SCK   13  // SPI SCLK
#define TFT_SDO   12  // SPI DIN


#define TS_CS_PIN  8  // Serial 3 Tx
#define TS_IRQ_PIN 2  // DIO


// ref lcd size
#define LCD_WIDTH   320
#define LCD_HEIGHT  240

// textbox on main screen
#define TB_POS_X  150
#define TB_POS_Y  40
#define TB_WIDTH  160
#define TB_HEIGHT 200

#define CURSOR_Y_HOME   45
#define CURSOR_X_HOME   155
#define CURSOR_LIMIT_X  300
#define CURSOR_LIMIT_Y  190


// include buttons after defines
#include "buttons.h"


// globals
ILI9341_t3 lcd = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_SDI, TFT_SCK, TFT_SDO);
XPT2046_Touchscreen ts(TS_CS_PIN);
uint16_t cursorX;
uint16_t cursorY;


void lcdInit()
{
  lcd.begin();
  lcd.setRotation(3);
  lcd.fillScreen(ILI9341_BLACK);
  delay(1000);

  printValue("LCD Width ", lcd.width());
  printValue("LCD Height", lcd.height());

  ts.begin();
  ts.setRotation(1);

  lcd.fillScreen(ILI9341_BLACK);
  lcd.setTextColor(ILI9341_WHITE);
  lcd.setFont(Arial_14_Bold);
  lcd.setCursor(40, 0);
  lcd.print(TITLE_STR);
}



void refreshScreen()
{
  lcd.fillScreen(ILI9341_BLACK);
  lcd.setTextColor(ILI9341_WHITE);
  lcd.setFont(Arial_14_Bold);
  lcd.setCursor(40, 0);
  lcd.print(TITLE_STR);
  drawAllButtons();
  lcd.drawRoundRect(TB_POS_X, TB_POS_Y, TB_WIDTH, TB_HEIGHT, 4, ILI9341_RED);
}


void initTextBox()
{
  lcd.fillRoundRect(TB_POS_X, TB_POS_Y, TB_WIDTH, TB_HEIGHT, 4, ILI9341_BLACK);
  lcd.drawRoundRect(TB_POS_X, TB_POS_Y, TB_WIDTH, TB_HEIGHT, 4, ILI9341_RED);

  cursorX = CURSOR_X_HOME;
  cursorY = CURSOR_Y_HOME;
}


void textBox(String msg)
{
  // if at the bottom, erase and start over
  if (cursorY > CURSOR_LIMIT_Y)
    initTextBox();

  lcd.setTextColor(ILI9341_WHITE);
  lcd.setFont(Arial_8);
  lcd.setCursor(cursorX, cursorY);
  lcd.print(msg);
  //Serial.println(msg);
  cursorY += 12;
}


void textBoxHex(byte val)
{
  // if at the bottom, erase and start over
  if (cursorY > CURSOR_LIMIT_Y)
    initTextBox();

  lcd.setTextColor(ILI9341_WHITE);
  lcd.setFont(Arial_8);
  lcd.setCursor(cursorX, cursorY);
  lcd.print("0x");
  if (val < 0x10)
    lcd.print("0");
  lcd.print(val, HEX);
  cursorX += 32;
  if (cursorX > CURSOR_LIMIT_X)
  {
    cursorX = CURSOR_X_HOME;
    cursorY += 12;
  }
}


/*
   ILI9341 library reference

   ILI9341_t3(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12);
   void begin(void);
   void sleep(bool enable);
   void pushColor(uint16_t color);
   void fillScreen(uint16_t color);
   void drawPixel(int16_t x, int16_t y, uint16_t color);
   void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
   void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
   void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
   void fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
   void fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
   void fillScreenVGradient(uint16_t color1, uint16_t color2);
   void fillScreenHGradient(uint16_t color1, uint16_t color2);
   void setRotation(uint8_t r);
   void setScroll(uint16_t offset);
   void invertDisplay(boolean i);
   void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

   adafruit GFX library
   void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
   void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
   void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
   void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
   void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
   void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
   void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
   void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
   void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
   void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
   void setCursor(int16_t x, int16_t y);
   void getCursor(int16_t *x, int16_t *y);
   void setTextColor(uint16_t c);
   void setTextColor(uint16_t c, uint16_t bg);
   void setTextSize(uint8_t s);
   uint8_t getTextSize();
   void setTextWrap(boolean w);
   boolean getTextWrap();
   virtual size_t write(uint8_t);
   int16_t width(void)  { return _width; }
   int16_t height(void) { return _height; }
   uint8_t getRotation(void);
   void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
   void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
   int16_t getCursorX(void) const { return cursor_x; }
   int16_t getCursorY(void) const { return cursor_y; }
   void setFont(const ILI9341_t3_font_t &f) { font = &f; }
   void setFontAdafruit(void) { font = NULL; }
   void drawFontChar(unsigned int c);
   void measureChar(uint8_t c, uint16_t* w, uint16_t* h);
   uint16_t fontCapHeight() { return (font) ? font->cap_height : textsize * 8; }
   uint16_t fontLineSpace() { return (font) ? font->line_space : textsize * 8; }
   uint16_t fontGap() { return (font) ? font->line_space - font->cap_height : 0; };
   uint16_t measureTextWidth(const char* text, int chars = 0);
   uint16_t measureTextHeight(const char* text, int chars = 0);
   int16_t strPixelLen(char * str);

   // Color definitions here for reference, already defined in adafruit_GFX.h
   #define ILI9341_BLACK       0x0000         0,   0,   0
   #define ILI9341_NAVY        0x000F         0,   0, 128
   #define ILI9341_DARKGREEN   0x03E0         0, 128,   0
   #define ILI9341_DARKCYAN    0x03EF         0, 128, 128
   #define ILI9341_MAROON      0x7800       128,   0,   0
   #define ILI9341_PURPLE      0x780F       128,   0, 128
   #define ILI9341_OLIVE       0x7BE0       128, 128,   0
   #define ILI9341_LIGHTGREY   0xC618       192, 192, 192
   #define ILI9341_DARKGREY    0x7BEF       128, 128, 128
   #define ILI9341_BLUE        0x001F         0,   0, 255
   #define ILI9341_GREEN       0x07E0         0, 255,   0
   #define ILI9341_CYAN        0x07FF         0, 255, 255
   #define ILI9341_RED         0xF800       255,   0,   0
   #define ILI9341_MAGENTA     0xF81F       255,   0, 255
   #define ILI9341_YELLOW      0xFFE0       255, 255,   0
   #define ILI9341_WHITE       0xFFFF       255, 255, 255
   #define ILI9341_ORANGE      0xFD20       255, 165,   0
   #define ILI9341_GREENYELLOW 0xAFE5       173, 255,  47
   #define ILI9341_PINK        0xF81F
*/
#endif
