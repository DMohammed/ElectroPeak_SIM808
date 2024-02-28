#include <Arduino.h>
#include <myOLED.h>
#include <myGSM.h>
#include <myGPS.h>

OLED mainOLED;
GSM mainGSM;
GPS mainGPS;
String value;
void setup()
{
  Serial.begin(115200);
  mainGSM.begin();
}

void loop()
{
  mainGSM.resiveMessage();
  if (Serial.available())
  {
    value = Serial.readStringUntil('\n');
    if (value.startsWith("test"))
    {
      mainGSM.sendMessage("+989383827728", "testing");
    }
    else if (value.startsWith("debug"))
    {
      mainGSM.debug();
    }
  }
  // delay(500);
}
