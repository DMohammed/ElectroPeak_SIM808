#include "Arduino.h"
#include <pdulib.h>
#include <pdu.h>
pdu::pdu()
{
}
char const *inpdu[] = {
    //"07917952140230F2040C91795277777777000012012161335221A061F1985C369FD169F59ADD76BFE171F99C5EB7DFF1797D503824168D476452B964369D4F68543AA556AD576C561B168FC965F3199D56AFD96DF71B1E97CFE975FB1D9FD707854362D1784426954B66D3F98446A5536AD57AC566B561F1985C369FD169F59ADD76BFE171F99C5EB7DFF1797D503824168D476452B964369D4F68543AA556AD576C561B93CD68" // full length
};

PDU mypdu = PDU(BUFFER_SIZE);
bool pdu::message(String input)
{

    // Prepare the character array (the buffer)
    char char_array[input.length() + 1];

    // Copy it over
    input.toCharArray(char_array, input.length() + 1);

    // Serial.print("Buffer Size ");
    // Serial.println(BUFFER_SIZE);
    if (mypdu.decodePDU(char_array))
    {
        Serial.println("-------------------------------");
        PDUe.overFlow = mypdu.getOverflow();
        if (PDUe.overFlow)
        {
            Serial.println("Buffer Overflow, partial message only");
        }
        PDUe.number = String(mypdu.getSender());

        String time = mypdu.getTimeStamp(); // 23061516060114
        int year = 2000 + time.substring(0, 2).toInt();
        int month = time.substring(2, 4).toInt();
        int day = time.substring(4, 6).toInt();
        int hour = time.substring(6, 8).toInt();
        int minute = time.substring(8, 10).toInt();
        int second = time.substring(10, 12).toInt();
        float time_zone = (time.substring(12, 14).toFloat() / 4);
        // String timestamp = year,"/",month,"/",day," ",hour,":",minute,":",second,":","  GMT+",time_zone;
        String x = "/";
        String y = "  ";
        String z = ":";
        String a = "  GMT+";
        PDUe.date = year + x + month + x + day + y + hour + z + minute + z + second + a + time_zone;
        PDUe.mess = String(mypdu.getText());
        int *concat = mypdu.getConcatInfo();
        PDUe.part = concat[1];
        PDUe.maxPart = concat[2];
        return true;
    }
    else
    {
        return false;
    }
}

int pdu::encoder(String SCAnumber, String number, String message)
{
    char SCAnumber_array[SCAnumber.length() + 1];
    SCAnumber.toCharArray(SCAnumber_array, SCAnumber.length() + 1);

    char number_array[number.length() + 1];
    number.toCharArray(number_array, number.length() + 1);

    char message_array[message.length() + 1];
    message.toCharArray(message_array, message.length() + 1);

    mypdu.setSCAnumber(SCAnumber_array);
    Serial.println(mypdu.encodePDU(number_array, message_array));
    return mypdu.encodePDU(number_array, message_array);
}

const char *pdu::sms()
{
    return mypdu.getSMS();
}