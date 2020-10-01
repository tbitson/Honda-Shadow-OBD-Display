/****************************************
   obd.h - OBD messages and utilities


 *****************************************/

#ifndef OBD_H
#define OBD_H

#include "serialCom.h"
#include "table.h"


// prototypes
extern bool openSerial1();
extern void flushRx();
extern bool sendMessage(uint8_t message[]);
extern void checkDataAvailable(int numBytes);
extern uint8_t getData(int minBytes);
extern void printBuffer(uint8_t bytes);
extern void printHex(uint8_t val);

bool checksum(uint8_t, uint8_t);


// timing in millisecs
const uint32_t TIMESTART = 500;   // quiet time before starting communications
const uint32_t TIMEKLOW  = 70;    // K-line is pulled low for this time
const uint32_t TIMEKHIGH = 200;   // quiet time after low pulse before wakeup
const uint32_t TIMEINIT  = 200;   // time between wakeup and initialise

// bus messages
// first byte is length of message
uint8_t WAKEUP[] = {0x04, 0xFE, 0x04, 0xFF, 0xFF};        // no response expected
uint8_t INIT[] = {0x05, 0x72, 0x05, 0x00, 0xF0, 0x99};  // initialize communications
uint8_t REQTAB10[] = {0x07, 0x72, 0x07, 0x72, 0x10, 0x00, 0x14, 0xF0}; // request all of table 11 data, should reply with 26 bytes
uint8_t REQTAB11[] = {0x07, 0x72, 0x07, 0x72, 0x11, 0x00, 0x14, 0xF0}; // request all of table 11 data, should reply with 26 bytes
uint8_t REQTAB20[] = {0x07, 0x72, 0x07, 0x72, 0x20, 0x00, 0x14, 0xF0}; // request all of table 11 data, should reply with 26 bytes
uint8_t REQTAB60[] = {0x07, 0x72, 0x07, 0x72, 0x60, 0x00, 0x14, 0xF0}; // request all of table 11 data, should reply with 26 bytes


/*
 command  8 bytes: 0x07, 0x72, 0x07, 0x72, 0x10, 0x00, 0x14, 0xF0
 response 5 bytes: 0x02 0x1A 0x72 0x11 0x00
  locations & conversions of known key values
  Func       Table  Location  Conversion        Size       Units
  -----------|------|--------|----------------|-----------|--------
  rpm         11    0,1       A * 256 + B       uint16     rpm
  tps_v       11    2         A * 5.0 / 256.0   float      volts
  tps_p       11    3         A / 16.0          float      %
  ect_c       11    4         A - 40            int8       degs c
  ect_v       11    5         A * 5.0 / 256.0   float      volts
  iat_v       11    6         A * 5.0 / 256.0   float      volts
  iat_c       11    7         A - 40            int8       deg C
  map_v       11    8         A * 5.0/256.0     float      volts
  map_kPa     11    9         A                 uint8      kPa
  battV       11    12        A / 10.0;         float      volts
  kph         11    13        A                 uint8      KPH
  fuel_inj    11    14,15     A * 256 + B       uint16     time (ms?)

  TBD:
  gear
  Kick stand
  brake
  fan

*/


//  table data globals
uint8_t   kph;
uint16_t fi;

// display globals
float mph, ectf, battery;
uint16_t rpm;
uint8_t gear;


// index positions in rxbuffer, add offset for echo & response header
const byte OFFSET = 12;
const byte RPM_H = OFFSET + 0;
const byte RPM_L = OFFSET + 1;
const byte TPS_P = OFFSET + 3;
const byte ECT_C = OFFSET + 4;
const byte IAT_C = OFFSET + 7;
const byte BATT  = OFFSET + 12;
const byte SPD   =  OFFSET + 13;
const byte FI_H  =  OFFSET + 14;
const byte FI_L  =  OFFSET + 15;



void initCom()
{
  // note - logic inverted: high turns on optocoupler, which pulls K-Line low
  Serial.println("obd init");

  flushRx();
  delay(TIMESTART);

  // pulse k-line
  digitalWrite(K_LINE, ON);
  delay(TIMEKLOW);
  digitalWrite(K_LINE, OFF);
  delay(TIMEKHIGH);

  sendMessage(WAKEUP);
  delay(TIMEINIT);
  sendMessage(INIT);
}



// just dump table data to screen for dev
void dumpTable(uint8_t tableNum)
{
  byte b;

  bool loop = true;

  while (loop)
  {
    Serial.println("request table data");
    if (tableNum == 10)
      sendMessage(REQTAB10);
    else if (tableNum == 11)
      sendMessage(REQTAB11);
    else if (tableNum == 20)
      sendMessage(REQTAB20);
    else if (tableNum == 60)
      sendMessage(REQTAB60);


    delay(500);
    while (Serial1.available())
    {
      b = Serial1.read();
      printHex(b);
      delay(10);
    }

    Serial.println();
    delay(1000);

    // exit on touch screen press
    if (ts.touched())
    {
      Serial.println("exiting");
      loop = false;
    }
  }

  Serial.println("done");
}



// reqest and decode table aa data
bool getTable11()
{
  Serial.println("request table 11 data");
  sendMessage(REQTAB11);

  // wait for at least 4 bytes returned
  byte b = getData(4);
  if (b > 0)
  {
    if (checksum(0, b))
    {
      printBuffer(b);

      rpm = rxBuffer[RPM_H] * 256 + rxBuffer[RPM_L];
      mph = (float)rxBuffer[SPD] * 0.6214;
      gear = 0;
      ectf = (((float)rxBuffer[ECT_C] - 40.0) * 9.0 / 5.0) + 32.0;
      fi = rxBuffer[FI_H] * 256 + rxBuffer[FI_L];
      battery = (float)rxBuffer[BATT] / 10.0;

      return true;
    }
  }
  return false;
}



bool checksum(uint8_t start, uint8_t end)
{
  uint16_t cs = 0;

  for (uint8_t i = start; i < end; i++)
    cs += rxBuffer[i];

  cs = 0x100 - (0x00FF && cs);
  Serial.print("calc checksum = 0x");
  Serial.println(cs, HEX);
  Serial.print("xmit checksum = 0x");
  Serial.println(rxBuffer[25], HEX);

  if (cs == rxBuffer[25])
  {
    Serial.println("*** Checksums Matched ***");
    return true;
  }

  Serial.println("*** Checksums NOT Matched ***");
  return true;
}






#endif
