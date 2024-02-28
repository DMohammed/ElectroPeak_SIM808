#ifndef MYGPS_H
#define MYGPS_H

#include <Arduino.h>

class GPS
{
public:
    bool begin();
    void resiveMessage();
    void test();

    // bool getStateAlarm();

    // void beeb(uint16_t with = 100, uint8_t times = 100);

private:
};

#endif
