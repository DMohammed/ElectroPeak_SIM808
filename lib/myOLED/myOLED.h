#ifndef MYOLED_H
#define MYOLED_H

class OLED
{
public:
    bool begin();
    void printOLED(String value, byte x, byte y);

private:
};

#endif
