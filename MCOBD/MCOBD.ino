/*******************3***************************************
  Test tool for development of an OBD interface to
  a 2013 Honda Shadow Aero motorcycle. It uses a
  non-standard K-Line interface at 10400 bps. This
  tool's initial purpose is to test if communication
  can be established between the Teensy and the OBD
  computer.

  Coded for Teensy 3.2

  based on work by gonzo.... Thanks Gonzo!

  Vers 0.10 - Fundamental design, selection of Serial1 port as
              interface to OBD, development of 3 opto-isolator
              design for Rx, Tx, K-line control

  Vers 0.20 - Completed interface testing and initial OBD comunication
              Started dev of data decoding and table design


  Hardware connections:
  WHT - Rx1/D0 - input from Opto-Iso 1 K-line serial data in
  GRN - Tx1/D1 - output to Opto-Iso 2, K-line serial data out
  BLK - D5 - output to Opto-Iso 3, pulls K-line low independant of Tx
  RED - +3.3V to Interface
  UNUSED - A7 - battery voltage thru a resistive divider of 8.2K/39K
  Shield - Ground/Return

  see lcd.h for lcd connections

*********************************************************/

// #define DEBUG_PRINT

#define TITLE_STR "Honda OBD Tool V0.23"


// globals
bool genSerial = false;
bool listenMode = false;
bool simMode = false;

const uint8_t BUFFER_SIZE = 64;
uint8_t rxBuffer[BUFFER_SIZE];
uint8_t txBuffer[BUFFER_SIZE];

// io pins
const int RXD1 = 0;
const int TXD1 = 1;
const int K_LINE = 5;
const int LED = 13;
const int BATT_V = A7;
const int SPARE = 23;


#include "lcd.h"
#include "buttons.h"
//#include "obd.h"
#include "serialCmd.h"





void setup(void)
{
  Serial.begin(57600);
  pinMode(K_LINE, OUTPUT);  // DIO
  digitalWrite(K_LINE, LOW);
  delay(1000);
  Serial.println("Honda MC OBD Tool v 0.22");

  // initalize the lcd display & touchscreen
  lcdInit();

  // draw interface
  int8_t b1 = createButton(20, 40,  "Init Com", OFF, true);
  int8_t b2 = createButton(20, 80,  "Table 1",  OFF, true);
  int8_t b3 = createButton(20, 120, "Table 2",  OFF, true);
  int8_t b4 = createButton(20, 160, "Sim Mode", OFF, true);
  int8_t b5 = createButton(20, 200, "Reset",    OFF, true);

  drawButton(b1);
  drawButton(b2);
  drawButton(b3);
  drawButton(b4);
  drawButton(b5);

  // draw text box
  initTextBox();
}




void loop()
{
  // is button pressed?
  int8_t buttonNum = checkButtons();
  if (buttonNum > 0)
    doButton(buttonNum);
  else
    // is serial command pending
    checkSerialCmd();

  // slow down loop a bit
  delay(100);
}



void doButton(uint8_t buttonNum)
{
  //Serial.print("button = "); Serial.println(buttonNum);

  switch (buttonNum)
  {
    case 1:
      // send init sequence
      updateButton(1, true, true); 
      // init serial port
      openSerial1();
      delay(50);
      initCom();
      textBox("init sent");
      checkDataAvailable(4);
      updateButton(1, false, true);
      break;


    case 2:
      // data table 1
      updateButton(2, true, true);
      Serial.println("requesting table");
      textBox("table 1");
      displayTable11();
      refreshScreen();
      updateButton(2, false, true);
      break;


    case 3:
      updateButton(3, true, true);
      delay(500);
      updateButton(3, false, true);
      break;


    case 4:
      simMode = !simMode;
      updateButton(4, simMode, true);
      delay(1000);
      break;


    case 5:
      updateButton(5, true, true);
      textBox("reset");
      Serial1.end();
      pinMode(RXD1, INPUT);
      pinMode(TXD1, INPUT);
      pinMode(K_LINE, OUTPUT);
      digitalWrite(K_LINE, LOW);
      Serial.println("reset complete");
      delay(1000);
      textBox("done");
      updateButton(5, false, true);
      break;
  }
}






float readBatteryVolts()
{
  // resistor divider scaled to 19V for 1023 counts
  float v = (float)analogRead(BATT_V) * 19.0 / 1023.0;
  printValue("Battery Volts", v);
  return v;
}


void testIO(bool useTextBox)
{
  // test IO
  Serial1.end();
  pinMode(RXD1, OUTPUT);     // serial1 Rx
  pinMode(TXD1, OUTPUT);     // serial1 Tx
  pinMode(K_LINE, OUTPUT);  // DIO

  if (useTextBox) textBox("Setting Rx High");
  digitalWrite(RXD1, HIGH);
  delay(2000);
  if (useTextBox) textBox("Setting Rx Low");
  digitalWrite(RXD1, LOW);
  delay(1000);

  if (useTextBox) textBox("Setting Tx High");
  digitalWrite(TXD1, HIGH);
  delay(2000);
  if (useTextBox) textBox("Setting TX Low");
  digitalWrite(TXD1, LOW);
  delay(1000);

  if (useTextBox) textBox("Setting K-line High");
  digitalWrite(K_LINE, HIGH);
  delay(2000);
  if (useTextBox) textBox("Setting K-line Low");
  digitalWrite(K_LINE, LOW);
  delay(1000);

  // restore serial config
  pinMode(RXD1, INPUT);     // serial1 Rx
  pinMode(TXD1, INPUT);     // serial1 Tx
  Serial1.begin(BAUD);
}
