#ifndef MYGPS_H
#define MYGPS_H

#include <Arduino.h>

class GPS
{
public:
    bool decode(String value);
    struct myGPSData // Latitude	Longitude	Altitude	Fix	Fix quality
    {
        // String Altitude;
        String Latitude;
        String Longitude;
        // byte Status;
        // byte SUv;
        // bool North;
        // bool East;
        // byte Quality;
        // float HDOP;
    };
    myGPSData GPSData;

private:
    String cash[3];
    String fix(String value);
};

#endif
