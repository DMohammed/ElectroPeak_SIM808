#include <Arduino.h>
#include <myOLED.h>
#include <Wire.h>
#include <U8g2lib.h>

#define SDA_OLED 19
#define SCL_OLED 23

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/SCL_OLED, /* data=*/SDA_OLED, /* reset=*/U8X8_PIN_NONE); // OLEDs without Reset of the Display
// U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R1, /* clock=*/ SCL_OLED, /* data=*/ SDA_OLED, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

bool OLED::begin()
{
    if (!u8x8.begin())
        return false;
    u8x8.setPowerSave(0);
    return true;
}

void OLED::printOLED(String value, byte x, byte y)
{
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setInverseFont(0);
    u8x8.drawString(0, 0, "SIM808 Traker");
    // u8x8.refreshDisplay(); // only required for SSD1606/7
}