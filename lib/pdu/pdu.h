#ifndef pdu_h
#define pdu_h

#define BUFFER_SIZE 255

class pdu
{
public:
    pdu();
    bool message(String input);
    int encoder(String SCAnumber, String number, String message);
    const char *sms();
    struct myPDUe
    {
        String number;
        String date;
        String mess;
        int part;
        int maxPart;
        bool overFlow;
    };
    myPDUe PDUe;

private:
};
#endif
