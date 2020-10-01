/******************************************

  comm.h - serial communication code to
  use serial port 1 on pins 9 (rx1) and
  10 (tx1) on a Teensy 3.2


******************************************/


#ifndef SERCOM_H
#define SERCOM_H

#include "obd.h"


extern void initCom();
extern void doTable1();
extern void testIO(bool useTextBox);


// prototypes
bool openSerial1();
void flushRx();
bool sendMessage(uint8_t message[]);
void checkDataAvailable(int numBytes);
uint8_t getData(int minBytes);
void printBuffer(uint8_t bytes);
void printHex(uint8_t val);







// constants
const uint16_t BAUD = 10400;          // non-standard bps
const uint32_t SERIAL_TIMEOUT = 5000; // max ms to wait for OBD response
const uint8_t simBytes = 26;
uint8_t simData[3][33] = {
  { 0x02, 0x1A, 0x72, 0x11, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x86, 0x48, 0x86, 0x47, 0x85, 0x5A, 0xFF, 0xFF, 0x7B, 0x00, 0x00, 0x00, 0x80, 0x66, 0x33, 0x1F, 0x1C},
  { 0x02, 0x1A, 0x72, 0x11, 0x00, 0x00, 0x00, 0xE1, 0x9B, 0x87, 0x48, 0x88, 0x47, 0x85, 0x5A, 0xFF, 0xFF, 0x7A, 0x00, 0x00, 0x00, 0x80, 0x66, 0x33, 0x59, 0x7E},
  { 0x02, 0x1A, 0x72, 0x11, 0x00, 0x00, 0x00, 0xE1, 0x9B, 0x87, 0x48, 0x88, 0x47, 0x85, 0x5A, 0xFF, 0xFF, 0x7A, 0x00, 0x00, 0x00, 0x80, 0x66, 0x33, 0x59, 0x7E}
};




bool openSerial1()
{
  Serial1.begin(BAUD);
  Serial.println("Serial1 opened");
  return true;
}


void flushRx()
{
  // read any data remaining in serial buffer
  while (Serial1.available())
  {
    byte b = Serial1.read();
    Serial.print(b);
  }

  // clear rxBuffer array
  for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    rxBuffer[i] = 0;
}



// send message array, first byte is length
bool sendMessage(uint8_t message[])
{
  uint8_t bytes = message[0];
  Serial.print("Sending: ");

  for (uint8_t i = 1; i <= bytes; i++)
  {
    Serial1.write(message[i]);
    printHex(message[i]);
    delay(2);
  }
  Serial.println();
  return true;
}




// check if there's data in the buffer and print in text box
void checkDataAvailable(int numBytes)
{
  String results;
  byte bytes;


  bytes = getData(numBytes);
  if (bytes == 0)
  {
    textBox("No Response");
    Serial.println("No Response");
    return;
  }

  lcd.setTextColor(ILI9341_WHITE);
  lcd.setTextSize(8);
  lcd.setCursor(cursorX, cursorY);

  Serial.print("Response: ");
  for (int i = 0; i < bytes; i++)
  {
    // send to screen
    printHex(rxBuffer[i]);

    // send to lcd
    textBoxHex(rxBuffer[i]);
  }

  printBuffer(bytes);
  Serial.println();
}




// wait for data with timeout
uint8_t getData(int minBytes)
{
  uint8_t index = 0;


  // use simulated data for dev & test
  if (simMode)
  {
    static uint8_t index2 = 0;

    Serial.println("*** Using Simulated Data ***");
    for (index = 0; index < simBytes; index++)
      rxBuffer[index] = simData[index2][index];

    index2 ++;
    if (index2 > 2)
      index2 = 0;

    return simBytes;
  }


  // wait for response or timeout
  uint32_t startTime = millis();
  while (Serial1.available() < minBytes)
  {
    delay(10);
    if ((millis() - startTime) > SERIAL_TIMEOUT)
      return 0;
  }

  while (Serial1.available())
  {
    rxBuffer[index] = Serial1.read();

    if (index < BUFFER_SIZE)
      index++;
    else
    {
      Serial.println("buffer overflow");
      return index;
    }
    delay(10);
  }

  return index;
}




void printBuffer(uint8_t bytes)
{
  uint8_t count = 0;

  for (uint8_t i = 0; i < bytes; i++)
  {
    Serial.print(i);
    Serial.print(': ');

    printHex(rxBuffer[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
}


void printBuffer2(uint8_t bytes)
{
  uint8_t count = 0;

  for (uint8_t i = 0; i < bytes; i++)
  {
    Serial.print(i);
    Serial.print(':');

    printHex(rxBuffer[i]);

    count++;
    if (count > 31)
    {
      Serial.println();
      count = 0;
    }
    else
      Serial.print(" ");
  }
}





#endif
