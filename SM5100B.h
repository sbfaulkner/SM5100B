#ifndef __SM5100B_H__
#define __SM5100B_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

#define RXPIN   2
#define TXPIN   3
#define RSTPIN  7

#define BUFFERLENGTH  64

enum { SM5100B_SIM_PRESENT, SM5100B_READY, SM5100B_REGISTERED };

class SM5100B
{
public:
  SM5100B();

public:
  void begin();
  char* next();
  int send(char* command);

protected:
  char* flushInput();
  char* processInput();
  void processSIND();

public:
  bool isSimPresent() { return bitRead(flags, SM5100B_SIM_PRESENT); };
  bool isReady() { return bitRead(flags, SM5100B_READY); };
  bool isRegistered() { return bitRead(flags, SM5100B_REGISTERED); };

protected:
  SoftwareSerial modem;
  char buffer[BUFFERLENGTH];
  char* head;
  char* tail;
  int flags;
};

#endif // __SM5100B_H__
