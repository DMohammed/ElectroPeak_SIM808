#ifndef MYGSM_H
#define MYGSM_H

#include <Arduino.h>
#define pass "123"

#if defined(ESP8266)
#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
#define rstPin 4
#define enPin 5
#define speed 9600
SoftwareSerial GSMSerial(rxPin, txPin);
#elif defined(ESP32)
#pragma message "ESP32 stuff happening!"
#define GSMSerial Serial2
#define rstPin 4
#define enPin 5
#define speed 115200

#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3
#define rstPin 4
#define enPin 5
#define speed 9600
#define pduMode false
SoftwareSerial GSMSerial(rxPin, txPin);
#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
#define GSMSerial Serial1
#define rstPin 22
#define enPin 23
#define speed 115200
#define pduMode true
#endif

class GSM
{
public:
    GSM();

    bool begin();
    void resiveMessage();
    bool sendMessage(String number, String value);
    void debug();
    void live();

private:
    String checker;
    String out;
    byte cunterBootUp = 0;
    byte cunter = 0;
    String IMEI;
    String SCAnumber;
    struct mySMSData
    {
        String number;
        String date;
        String value;
    };
    mySMSData SMSData;

    bool bootGSM();
    String serialRead(String value, String startOrEndWith, bool startOrEnd = true, byte numberOfLine = -1);
    void processing(String input, String text = "");
    byte smsChecker();
    String getValue();
    String getPos(bool type = false);
};

#endif
