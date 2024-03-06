#include <Arduino.h>
#include <myOLED.h>
#include <myGSM.h>

OLED mainOLED;
GSM mainGSM;
String value;
byte cunter;
void setup()
{
  Serial.begin(115200);

  mainOLED.begin();
  mainGSM.begin();
  mainOLED.printOLED("All Set", 0, 5);
  delay(1000);
  mainOLED.clear();

  // mainGSM.debug();
}

void loop()
{
  mainGSM.resiveMessage();
  if (Serial.available())
  {
    value = Serial.readStringUntil('\n');
    if (value.startsWith("debug"))
    {
      mainGSM.debug();
    }
  }
  delay(100);
  cunter++;
  if (cunter == 20)
  {
    mainGSM.live();
  }
}
