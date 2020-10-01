/*****************************************
   serialCmd.h - serial interface to a
   computer allowing execution of routines
   without using lcd, primarily for development.

   version 1.02  05Jun20

*/


#ifndef SERCMD_H
#define SERCMD_h


#include "serialCom.h"


// prototypes
void showMenu();
void serialCommand();


void showMenu()
{
  Serial.println();
  Serial.println(F("*** SerialCommands ***"));
  Serial.println(F("i) Init OBD Bus"));
  Serial.println(F("d) check serial Data"));
  Serial.println(F("p) print Rx Buffer"));
  Serial.println(F("c) clear buffer"));
  Serial.println(F("k) toggle K-line"));
  Serial.println(F("o) Open serial port"));
  Serial.println(F("e) end serial port"));
  Serial.println(F("t) dump Table to screen"));

  Serial.println(F("0) Table10"));
  Serial.println(F("1) Table11"));
  Serial.println(F("2) Table20"));
  Serial.println(F("6) Table60"));
  Serial.println(F("3) Display"));
  Serial.println(F("R) Refresh screen"));
  Serial.println();
}



void checkSerialCmd()
{
  char c;
  uint8_t b;

  if (Serial.available())
  {
    // read 1st character
    c = Serial.read();

    // ignore non-printable characters
    if (c <= 32 || c >= 127)
      return;

    Serial.print("-> ");
    Serial.println(c);

    switch (c) {

      case '?':
        showMenu();
        break;


      case 'i':
        Serial.println("Init OBD Bus");
        // open obd serial port & clear
        openSerial1();
        delay(50);
        updateButton(1, true, true);
        initCom();
        textBox("init sent");
        checkDataAvailable(4);
        updateButton(1, false, true);
        Serial.println("done");
        break;


      case 'd':
        Serial.println("get serial data");
        b = getData(1);
        if (b > 0)
          printBuffer(b);
        Serial.println("done");
        break;


      case 'p':
        printBuffer(BUFFER_SIZE);
        break;


      case 'c':
        flushRx();
        break;


      case 'o':
        Serial1.begin(BAUD);
        delay(100);
        Serial.println("Port Opened");
        break;


      case 'e':
        Serial1.end();
        delay(100);
        Serial.println("port closed");
        break;


      case '0':
        // data table 10
        updateButton(2, true, true);
        Serial.println("table10");
        dumpTable(10);
        refreshScreen();
        break;


      case '1':
        // data table 11
        updateButton(2, true, true);
        Serial.println("table11");
        dumpTable(11);
        refreshScreen();
        break;

      case '2':
        // data table 20
        updateButton(2, true, true);
        Serial.println("table20");
        dumpTable(20);
        refreshScreen();
        break;

      case '6':
        // data table 60
        updateButton(2, true, true);
        Serial.println("table60");
        dumpTable(60);
        refreshScreen();
        break;


      case '3':
        break;


      case 'R':
        refreshScreen();
        break;


      default:
        Serial.println("invalid command");
    }
  }
}
#endif
