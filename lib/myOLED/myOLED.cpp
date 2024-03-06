#include <Arduino.h>
#include <myOLED.h>
#include <Wire.h>
#include <U8g2lib.h>

// #define SDA_OLED 19
// #define SCL_OLED 23

// U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
// U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // OLEDs without Reset of the Display
// U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R1, /* clock=*/ SCL_OLED, /* data=*/ SDA_OLED, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

bool OLED::begin()
{
    if (!u8x8.begin())
        return false;
    u8x8.setPowerSave(0);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.clear();
    u8x8.drawString(0, 0, "ElectroPeak");
    u8x8.drawString(0, 1, "SIM808 Tracker");
    u8x8.refreshDisplay();
    delay(1000);
    return true;
}

void OLED::printOLED(String value, byte x, byte y)
{
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    // u8x8.setInverseFont(0);
    char value_array[value.length() + 1];
    value.toCharArray(value_array, value.length() + 1);
    u8x8.drawString(x, y, value_array);
    // u8x8.refreshDisplay(); // only required for SSD1606/7
}

void OLED::clear(byte line)
{
    if (line == 0)
    {
        u8x8.clearLine(2);
        u8x8.clearLine(3);
        u8x8.clearLine(4);
        u8x8.clearLine(5);
        u8x8.clearLine(6);
        u8x8.clearLine(7);
    }
    else
        u8x8.clearLine(line);
}