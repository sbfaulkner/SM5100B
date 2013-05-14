#include <SM5100B.h>

SM5100B::SM5100B() : modem(RXPIN, TXPIN)
{
  flushInput();
  flags = 0;
}

void SM5100B::begin()
{
  modem.begin(9600);
  flags = 0;
  digitalWrite(RSTPIN, HIGH);
  delay(12000);
  digitalWrite(RSTPIN, LOW);
}

char* SM5100B::next()
{
  if (modem.available())
  {
    char c = modem.read();

    if (c == '\n')
      return processInput();

    if (c != '\r')
      *tail++ = c;
  }

  return NULL;
}

int SM5100B::send(char* command)
{
  int bytes = modem.print(command);
  modem.write('\r');
  return bytes + 1;
}

//
// Initial response data...
//
// +SIND: 1
//
// +SIND: 10,"SM",1,"FD",1,"LD",1,"MC",1,"RC",1,"ME",1
//
// +SIND: 11
//
// +SIND: 3
//
// +SIND: 4

char* SM5100B::flushInput()
{
  char* input = head;
  head = tail = &buffer[0];
  return *input ? input : NULL;
}

char* SM5100B::processInput()
{
  *tail = 0;

  if (strncmp(head, "+SIND: ", 7) == 0)
  {
    processSIND();
    flushInput();
    return NULL;
  }

  return flushInput();
}

void SM5100B::processSIND()
{
  switch (atoi(head + 7))
  {
    case 0: // SIM card removed
      bitClear(flags, SM5100B_SIM_PRESENT);
      break;
    case 1: // SIM card inserted
      bitSet(flags, SM5100B_SIM_PRESENT);
      break;
    case 2: // Ring melody
      break;
    case 3: // AT module is partially ready
      bitClear(flags, SM5100B_READY);
      break;
    case 4: // AT module is totally ready
      bitSet(flags, SM5100B_READY);
      break;
    case 5: // ID of released calls
      break;
    case 6: // Released call whose ID=<idx>
      break;
    case 7: // The network service is available for an emergency call
    case 8: // The network is lost
      bitClear(flags, SM5100B_REGISTERED);
      break;
    case 9: // Audio ON
      break;
    case 10: // Show the status of each phonebook after init phrase
      break;
    case 11: // Registered to network
      bitSet(flags, SM5100B_REGISTERED);
      break;
  }
}
