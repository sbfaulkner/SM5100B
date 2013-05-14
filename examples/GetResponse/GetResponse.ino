#include <SoftwareSerial.h>
#include <SM5100B.h>

SM5100B modem;
int startTime;
int tickTime = 0;

enum { INITIALIZING, ATSENT } state = INITIALIZING;

void println(char* message)
{
  int elapsed = millis() - startTime;
  Serial.print(elapsed);
  Serial.print("ms : ");
  Serial.println(message);
}

void setup()
{
  startTime = millis();
  Serial.begin(9600);

  println("Initializing...");
  modem.begin();
}

void loop()
{
  char* response = modem.next();

  switch (state)
  {
    case INITIALIZING:
      if (modem.isReady())
      {
        println("Sending AT...");
        modem.send("AT");
        state = ATSENT;
      }
      break;
    case ATSENT:
      if (strcmp(response, "OK"))
      {
        println("OK");
        while (true)
          ;
      }
      break;
  }
}
