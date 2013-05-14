#include <SoftwareSerial.h>
#include <SM5100B.h>

SM5100B modem;
int startTime;
int tickTime = 0;
bool isSimPresent = false;
bool isReady = false;
bool isRegistered = false;

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
  println("Detecting state...");
}

void loop()
{
  modem.next();

  if (isSimPresent != modem.isSimPresent())
  {
    if (isSimPresent = modem.isSimPresent())
      println("SIM inserted");
    else
      println("SIM removed");
  }

  if (isReady != modem.isReady())
  {
    if (isReady = modem.isReady())
      println("AT module ready");
    else
      println("AT module not ready");
  }

  if (isRegistered != modem.isRegistered())
  {
    if (isRegistered = modem.isRegistered())
      println("Registered to network");
    else
      println("Not registered to network");
  }
}
