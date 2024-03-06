
#include <Arduino.h>
#include <myGSM.h>
#include <myGPS.h>
#include <pdu.h>
#include <myOLED.h>

// #define debugMode

pdu main_pdu;
GPS GPSGSM;
OLED OLEDGSM;

GSM::GSM()
{
}

bool GSM::begin()
{
    GSMSerial.begin(speed);
    // debug();
    OLEDGSM.printOLED("Connecting...", 0, 2);

    if (!bootGSM())
        while (true) // Problem to boot
        {
        }
    GSMSerial.flush();
    OLEDGSM.clear();
    OLEDGSM.printOLED("Connected", 0, 2);
    serialRead("AT&F", "OK");
    delay(10);
    if (pduMode)
    {
        serialRead("AT+CMGF=0", "OK"); // PDU mode
        delay(10);
        SCAnumber = serialRead("AT+CSCA?", "+CSCA:"); // AT+CSCA? -> +CSCA: "+989350001400",145
        SCAnumber.remove(0, 8);
        SCAnumber = SCAnumber.substring(0, SCAnumber.indexOf('\"'));
    }
    else
        serialRead("AT+CMGF=1", "OK"); // text Mode
    delay(10);
    serialRead("AT+CGPSPWR=1", "OK");
    delay(10);
    serialRead("AT+CGPSOUT=0", "OK");
    delay(10);
    GSMSerial.flush();
    byte incomingSMS = smsChecker();
    OLEDGSM.printOLED("SMS in SIM" + String(incomingSMS), 0, 3);
    if (incomingSMS == 0)
    {
        serialRead("AT+CNMI=2,1,0,0,0", "OK");

        GSMSerial.readStringUntil('\n');
        GSMSerial.readStringUntil('\n');
        GSMSerial.readStringUntil('\n');
        GSMSerial.flush();
        return true;
    }
    else if (incomingSMS > 0)
    {
        Serial.println("Size of incoming :");
        Serial.println(incomingSMS);
        String valueOfIncomingSMS = "";

        for (byte i = 1; i <= incomingSMS; i++)
        {
            GSMSerial.println("AT+CMGR=" + String(i));
            delay(10);

            while (true)
            {
                checker = GSMSerial.readStringUntil('\n');
                checker.trim();
                if (checker.startsWith("+CMGR:"))
                {
                    Serial.print("Base : ");
                    Serial.println(checker);
                }
                else if (checker.startsWith("OK"))
                {

                    Serial.print("retrun : ");
                    Serial.println(checker);
                    processing(valueOfIncomingSMS);
                    break;
                }
                else if (checker.length() > 20)
                {
                    valueOfIncomingSMS = checker;
                }
            }
        }
        serialRead("AT+CNMI=2,1,0,0,0", "OK");
        serialRead("AT+CMGD=1,4", "OK");
        GSMSerial.readStringUntil('\n');
        GSMSerial.readStringUntil('\n');
        GSMSerial.readStringUntil('\n');
        GSMSerial.flush();
        return true;
    }
    return false;
}

bool GSM::bootGSM()
{
    Serial.println("********************************");

    pinMode(rstPin, OUTPUT); // pin NRESET GSM Set
    pinMode(enPin, OUTPUT);  // pin PWRKEY GSM set

    // digitalWrite(rstPin, HIGH); // NRESET Trigerd
    // delay(2000);
    digitalWrite(rstPin, HIGH); // NRESET Normal
    digitalWrite(enPin, LOW);   // PWRKEY Normal
    Serial.println("Test Connection");
    GSMSerial.println("AT");
    GSMSerial.println("AT");
    delay(100);
    while (GSMSerial.available())
    {
        checker = GSMSerial.readStringUntil('\n');
        checker.trim();
        Serial.println(checker);
        if (checker.startsWith("OK"))
        {
            Serial.println("System is On");
            return true;
        }
    }
    Serial.println("********************************");
    delay(2000);
GSMTRY:
    digitalWrite(enPin, HIGH); // PWRKEY Trigerd
    delay(3000);
    digitalWrite(enPin, LOW); // PWRKEY Normal
    OLEDGSM.clear();
    Serial.println("Power SIM Key ON");
    OLEDGSM.printOLED("Turn On GSM", 0, 3);
    while (true)
    {
        cunterBootUp++;
        if (GSMSerial.available())
        {
            checker = GSMSerial.readStringUntil('\n');
            if (checker.startsWith("OK"))
                break;
            else if (checker.startsWith("NORMAL POWER DOWN"))
            {
                Serial.println("NORMAL POWER DOWN");
                OLEDGSM.printOLED("GSM is", 0, 5);
                OLEDGSM.printOLED("Shout Down", 0, 6);
                GSMSerial.flush();
                goto GSMTRY;
            }
        }
        GSMSerial.println("AT");
        if (cunterBootUp == 20)
        {
            GSMSerial.flush();
            goto GSMTRY;
        }
        delay(100);
    }
    // debug();
    while (true)
    {

        cunterBootUp++;
        if (GSMSerial.available())
        {
            checker = GSMSerial.readStringUntil('\n');
            Serial.println(checker);
            if (checker.startsWith("RDY"))
                break;
            else if (checker.startsWith("+CPIN: READY"))
            {
                checker.remove(0, 6);
                Serial.print("SIM Detector :");
                Serial.println(checker);
                checker.trim();
                if (!checker.startsWith("READY"))
                    return false;
                break;
            }
        }
        else if (checker.startsWith("NORMAL POWER DOWN"))
        {
            Serial.println("NORMAL POWER DOWN");
            GSMSerial.flush();
            goto GSMTRY;
        }
        else
        {
            Serial.println(checker);
            // checker = "";
        }
        if (cunterBootUp == 50)
        {
            GSMSerial.flush();
            goto GSMTRY;
        }
        delay(100);
    }

    // debug();
    /*
RDY

+CFUN: 1

+CPIN: READY

Call Ready

SMS Ready
*/
    while (true)
    {
        cunterBootUp++;

        if (GSMSerial.available())
        {
            checker = GSMSerial.readStringUntil('\n');
            checker.trim();
            if (checker.startsWith("+CPIN:"))
            {
                checker.remove(0, 6);
                Serial.print("SIM Detector :");
                Serial.println(checker);
                checker.trim();
                if (!checker.startsWith("READY"))
                    return false;
            }
            else if (checker.startsWith("SMS Ready"))
            {
                Serial.println("SIM is Ready");
                return true;
            }
            checker = "";
        }
        delay(20);
        if (cunterBootUp == 1000)
        {
            digitalWrite(rstPin, LOW); // NRESET Trigerd
            delay(2000);
            digitalWrite(rstPin, HIGH); // NRESET Normal
            delay(2000);
            cunterBootUp = 0;
        }
    }
}

String GSM::serialRead(String value, String startOrEndWith, bool startOrEnd, byte numberOfLine)
{
sendAgine:

#ifdef debugMode
    Serial.println("Send Data to GSM :" + value);
#endif

    GSMSerial.println(value);
    delay(100);
    while (true)
    {
        Serial.print('.');
        cunter++;
        if (GSMSerial.available())
        {
            checker = GSMSerial.readStringUntil('\n');
            checker.trim();
#ifdef debugMode
            Serial.println(checker);
#endif
            if (checker.startsWith("+CME ERROR: 58"))
            {
#ifdef debugMode
                Serial.println("Error on send line");
#endif
                goto sendAgine;
            }
            if (startOrEnd)
            {
                if (checker.startsWith(startOrEndWith))
                {
#ifdef debugMode
                    Serial.println("return " + checker);
#endif
                    GSMSerial.flush();
                    return checker;
                }
            }
            else
            {
                if (checker.endsWith(startOrEndWith))
                {
#ifdef debugMode
                    Serial.println("return " + checker);
#endif
                    GSMSerial.flush();
                    return checker;
                }
            }
        }
        delay(100);
        if (cunter == 100)
        {
            cunter = 0;
            GSMSerial.flush();
            goto sendAgine;
        }
    }
}

bool GSM::sendMessage(String number, String value)
{
    Serial.println("Message to :" + number + "\t-" + value);
sendAgin1:
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.flush();
    delay(10);
    if (pduMode)
    {
        GSMSerial.println("AT+CMGS=" + String(main_pdu.encoder(SCAnumber, number, value)));
        delay(200);
        while (true)
        {
            cunter++;
            if (GSMSerial.available())
            {
                checker = GSMSerial.readStringUntil('\n');
                checker.trim();
                Serial.println(checker);

                if (checker.startsWith("+CME ERROR: 58")) // ERROR
                {
                    Serial.println("Error on send line");
                    goto sendAgin1;
                }
                if (checker.startsWith("ERROR")) // ERROR
                {
                    Serial.println("Error on send line");
                    goto sendAgin1;
                }
                if (checker.startsWith(">"))
                {
                    GSMSerial.println(main_pdu.sms());
                    delay(100);
                    GSMSerial.write(26);
                    delay(100);
                    while (true)
                    {
                        if (GSMSerial.available())
                        {
                            checker = GSMSerial.readStringUntil('\n');
                            Serial.println(checker);
                            if (checker.startsWith("OK"))
                            {
                                delay(100);
                                serialRead("AT+CMGD=1,4", "OK");
                                GSMSerial.readStringUntil('\n');
                                GSMSerial.readStringUntil('\n');
                                return true;
                            }
                            else if (checker.startsWith("+CMS ERROR: "))
                            {
                                return false;
                            }
                            else if (checker.startsWith("ERROR")) // ERROR
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
    }

    // delay(200);
    // while (true)
    // {
    //     cunter++;
    //     if (GSMSerial.available())
    //     {
    //         checker = GSMSerial.readStringUntil('\n');
    //         checker.trim();
    //         Serial.println(checker);

    //         if (checker.startsWith("+CME ERROR: 58"))
    //         {
    //             Serial.println("Error on send line");
    //             goto sendAgin1;
    //         }
    //         if (checker.startsWith(">"))
    //         {
    //             GSMSerial.println(value);

    //             delay(10);
    //             GSMSerial.write(26);
    //             while (true)
    //             {
    //                 if (GSMSerial.available())
    //                 {
    //                     checker = GSMSerial.readStringUntil('\n');
    //                     Serial.println(checker);
    //                     if (checker.startsWith("OK"))
    //                     {
    //                         delay(100);
    //                         serialRead("AT+CMGD=1,4", "OK");
    //                         GSMSerial.readStringUntil('\n');
    //                         GSMSerial.readStringUntil('\n');
    //                         return true;
    //                     }
    //                     else if (checker.startsWith("+CMS ERROR: "))
    //                     {
    //                         return false;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     Serial.print(',');
    //     delay(50);
    //     if (cunter == 500)
    //     {
    //         cunter = 0;
    //         goto sendAgin1;
    //     }
    // }
}

void GSM::resiveMessage()
{

    if (GSMSerial.available())
    {
        /*
+CMT: "+98123456789","","24/02/28,12:23:38+14"
Test
        */
        checker = GSMSerial.readStringUntil('\n');
        checker.trim();
        Serial.print("_");
        if (checker.startsWith("+CMT:"))
        {
            OLEDGSM.printOLED("New Message", 0, 3);
            Serial.println("New Message to resive");
            if (pduMode)
            {
                checker = GSMSerial.readStringUntil('\n');

                // #ifdef debugMode
                Serial.println(checker);
                // #endif
                processing(checker);
                OLEDGSM.printOLED("processed", 0, 4);
                delay(200);
                OLEDGSM.clear();
                Serial.println("All done");
            }
            else
            {
                Serial.println(checker);
                String mess = GSMSerial.readStringUntil('\n');
                Serial.println(checker);
                processing(checker, mess);
                OLEDGSM.printOLED("processed", 0, 4);
                delay(200);
                OLEDGSM.clear();
                Serial.println("All done");
            }
        }
        else if (checker.startsWith("+CMTI:")) //+CMTI: "SM",
        {
            Serial.println("New Message to resive CMTI");
            OLEDGSM.printOLED("New Message", 0, 3);
            if (pduMode)
            {
                Serial.println(checker);
                checker.remove(0, checker.indexOf(',') + 1);
                checker.trim();
                Serial.println(checker);
                GSMSerial.println("AT+CMGR=" + checker);
                while (true)
                {
                    if (GSMSerial.available())
                    {
                        checker = GSMSerial.readStringUntil('\n');
                        Serial.println(checker);
                        if (checker.length() > 15)
                        {
#ifdef debugMode
                            Serial.println(checker);
#endif
                            processing(checker);
                            OLEDGSM.printOLED("processed", 0, 4);
                            delay(200);
                            OLEDGSM.clear();
                            Serial.println("All done");
                            return;
                        }
                    }
                }
            }
            else
            {
                Serial.println(checker);
                String mess = GSMSerial.readStringUntil('\n');
                Serial.println(checker);
                processing(checker, mess);
                OLEDGSM.printOLED("processed", 0, 4);
                delay(200);
                OLEDGSM.clear();
            }
        }
        else
        {
            Serial.println(checker);
        }
    }
}

void GSM::processing(String input, String text)
{
#ifdef debugMode
    Serial.println(input);
    Serial.println(text);
#endif
    if (text.length() < 1)
    {
        if (input.length() < 2)
            return;
        main_pdu.message(input);
        Serial.print("Number: ");
        Serial.println(main_pdu.PDUe.number);
        Serial.print("Date: ");
        Serial.println(main_pdu.PDUe.date);
        Serial.print("Mess: ");
        Serial.println(main_pdu.PDUe.mess);
        Serial.print("Part: ");
        Serial.print(main_pdu.PDUe.part);
        Serial.print(" ,of Part: ");
        Serial.println(main_pdu.PDUe.maxPart);
        if (main_pdu.PDUe.mess.equals(pass))
        {
            delay(100);
            Serial.println(getPos());
            sendMessage(main_pdu.PDUe.number, "myGPS Location\n" + getPos());
            delay(500);
            serialRead("AT+CMGD=1,4", "OK");
        }
    }
    else
    {
        Serial.print("Number: ");
        Serial.println(input.substring(input.indexOf('+'), input.indexOf(",") - 1));
        input.remove(0, 26);
        Serial.print("Date: ");
        Serial.println(input);
        Serial.print("Mess: ");
        Serial.println(text);
    }
}

String GSM::getPos(bool type)
{
    while (true)
    {
        GSMSerial.println("AT+CGPSSTATUS?");
        delay(100);
        while (true)
        {
            if (GSMSerial.available())
            {
                checker = GSMSerial.readStringUntil('\n');
                // Serial.println(checker);
                if (checker.startsWith("+CGPSSTATUS:")) //+CGPSSTATUS: Location
                {                                       //"Unknown"
                                                        //"Not Fix"
                                                        //"2D Fix"
                                                        //"3D Fix"
                    checker.remove(0, 21);
                    checker.trim();
                    if (checker.startsWith("3D"))
                    {
                        if (type)
                            return "3D Fix";
                        else
                            return getValue();
                    }
                    else if (checker.startsWith("2D"))
                    {
                        if (type)
                            return "2D Fix";
                        else
                            return getValue();
                    }
                    else if (checker.startsWith("OK"))
                    {
                        break;
                    }
                    else if (checker.startsWith("Not"))
                    {
                        if (type)
                            return "Not Fix";
                        else
                            break;
                    }
                    else if (checker.startsWith("Unknown"))
                    {
                        if (type)
                            return "3D Fix";
                        else
                            break;
                    }
                    else
                    {
                        Serial.print('.');
                    }
                }
            }
        }
        // Serial1.println("AT+CGPSSTATUS?");
        GSMSerial.readStringUntil('\n');
        GSMSerial.readStringUntil('\n');
        GSMSerial.flush();
        delay(3000);
    }
}
void GSM::live()
{
    OLEDGSM.clear();
    OLEDGSM.printOLED(getPos(true), 0, 5);
}

String GSM::getValue()
{
    GSMSerial.println("AT+CGPSOUT=3");
    delay(100);
    while (true)
    {
        if (GSMSerial.available())
        {
            checker = GSMSerial.readStringUntil('\n');
            if (checker.startsWith("$GPGGA")) //$GPGGA,134423.000,3540.2997,N,05122.3770,E,1,11,0.96,1247.1,M,-
            {
                GSMSerial.println("AT+CGPSOUT=0");
                delay(500);
                GSMSerial.readStringUntil('\n');
                GSMSerial.readStringUntil('\n');
                GSMSerial.flush();

                if (GPSGSM.decode(checker))
                {
                    return "https://www.google.com/maps/place/" + GPSGSM.GPSData.Latitude + "," + GPSGSM.GPSData.Longitude; // https://www.google.com/maps/place/35.70911333,51.39322667
                }
            }
        }
    }
}

byte GSM::smsChecker()
{
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.flush();
    String numberOFSMS = serialRead("AT+CPMS?", "+CPMS:"); // +CPMS: "SM",0,15,"SM",0,15,"SM",0,15
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.flush();
    numberOFSMS.remove(0, numberOFSMS.indexOf(',') + 1);
    numberOFSMS = numberOFSMS.substring(0, numberOFSMS.indexOf(','));
    numberOFSMS.trim();
    if (numberOFSMS.toInt() > 0)
    {
        Serial.print("There is a message : ");
        Serial.println(numberOFSMS.toInt());
    }
    else
    {
        Serial.println("There is a Noting");
        GSMSerial.readStringUntil('\n');
        GSMSerial.readStringUntil('\n');
        GSMSerial.flush();
        return 0;
    }
    GSMSerial.readStringUntil('\n');
    GSMSerial.readStringUntil('\n');
    GSMSerial.flush();
    return numberOFSMS.toInt();
}

void GSM::debug()
{
    Serial.println("\ndebug mode");
    while (true)
    {
        while (Serial.available())
        {
            checker = Serial.readStringUntil('\t');
            if (checker.startsWith("end"))
            {
                Serial.println("Go to Main");
                return;
            }

            else
                GSMSerial.println(checker);
        }
        while (GSMSerial.available())
        {
            Serial.write(GSMSerial.read()); // Forward what Software Serial received to Serial Port
                                            // //delay(500);
                                            // Serial.println(GSMSerial.readString());
                                            // GSM_Display.showText("\n");
        }
    }
}
