
#include <Arduino.h>
#include <myGPS.h>
// https://receiverhelp.trimble.com/alloy-gnss/en-us/NMEA-0183messages_GGA.html
bool GPS::decode(String value)               //$GPGGA,134423.000,3540.2997,N,05122.3770,E,1,11,0.96,1247.1,M,-
{                                            //
    value.remove(0, value.indexOf(',') + 1); // remove $GPGGA,
    value.remove(0, value.indexOf(',') + 1); // remove 134423.000, UTC Time
    GPSData.Latitude = fix(value);           // get and fix value Latitude
    value.remove(0, value.indexOf(',') + 1); // remove 3540.2997, Latitude
    // GPSData.North = (value.substring(0, 1).equals("N")) ? true : false;  //
    value.remove(0, value.indexOf(',') + 2); // remove N,0 North
    GPSData.Longitude = fix(value);          // get and fix value Longitude
    value.remove(0, value.indexOf(',') + 1); // remove 5122.3770, Longitude
    // GPSData.East = (value.substring(0, 1).equals("E")) ? true : false;   //
    value.remove(0, value.indexOf(',') + 1); // remove E, East
    // GPSData.Quality = value.substring(0, 1).toInt();                     // get Quality
    value.remove(0, value.indexOf(',') + 1); // remove 11, Quality
    // GPSData.SUv = value.substring(0, value.indexOf(',')).toInt();        // get SVs
    value.remove(0, value.indexOf(',') + 1); // remove 1, SVs
    // GPSData.HDOP = value.substring(0, value.indexOf(',')).toFloat();     // get HDOP
    value.remove(0, value.indexOf(',') + 1); // remove 0.96, HDOP
    // GPSData.Altitude = fix(value); // Altitude
    value.remove(0, value.indexOf(',') + 1); // remove 1247.1, Altitude
    return true;
}
// fix value of Latitude and Longitude to posision
String GPS::fix(String value)
{
    cash[0] = value.substring(0, value.indexOf('.') - 2);                  // 35
    cash[1] = value.substring(2, value.indexOf('.'));                      // 40
    cash[2] = value.substring(value.indexOf('.') + 1, value.indexOf(',')); // 2997
    cash[1] = cash[1] + cash[2];
    float convert = (cash[1]).toFloat() / 60;
    cash[1] = String(convert, 4);
    cash[1].remove(cash[1].indexOf('.'), 1);
    return cash[0] + "." + cash[1];
}
