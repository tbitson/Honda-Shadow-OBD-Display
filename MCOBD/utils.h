/*********************************************
 * utils.h   v1.10
 * 
 * Various helper functions.
 * 
 * 
 *********************************************/


#ifndef UTILS_H
#define UTILS_H

// prototypes
float degCtoF(float degC);
void printHex(uint8_t val);
void printValue(const char* msg);
void printValue(String, int);
void printValue(String, long);
void printValue(String, float);
void printValue(const char*, int, float);
void printArryValue(const char* , uint8_t, float);




float degCtoF(float degC)
{
  return (degC * 9.0 / 5.0 - 32.0);
}




// simple helper functions that print data to the serial port
// undefining DEBUG_PRINT disables all output



void printHex(uint8_t val)
{
  Serial.print("0x");
  if (val < 0x10)
    Serial.print('0');
  Serial.print(val, HEX);
  Serial.print(" ");
}



void printValue(const char* msg)
{
#ifdef DEBUG_PRINT
  Serial.print("msg = ");
  Serial.println(msg);
#endif
}


void printValue(const char* msg, int val)
{
#ifdef DEBUG_PRINT
  Serial.print(msg);
  Serial.print(" = ");
  Serial.println(val);
#endif
}


void printValue(const char* msg, long val)
{
#ifdef DEBUG_PRINT
  Serial.print(msg);
  Serial.print(" = ");
  Serial.println(val);
#endif
}


void printValue(const char* msg, float val)
{
#ifdef DEBUG_PRINT
  Serial.print(msg);
  Serial.print(" = ");
  Serial.println(val, 1);
#endif
}


void printValue(const char* msg, int val1, float val2)
{
#ifdef DEBUG_PRINT
  Serial.print(msg);
  Serial.print(" ");
  Serial.print(val1);
  Serial.print(" = ");
  Serial.println(val2);
#endif
}


void printValue(const char* msg, String val2)
{
#ifdef DEBUG_PRINT
  Serial.print(msg);
  Serial.print(" = ");
  Serial.println(val2);
#endif
}

#endif
