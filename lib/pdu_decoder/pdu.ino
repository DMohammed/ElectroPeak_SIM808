// decode a pdu format message
#include "pdu_decoder.h"
#include <EEPROM.h>

String pr[16][16] = {
  //  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
  { "؀", "؁", "؂", "؃", "؄", "؅", "؆", "؇", "؈", "؉", "؊", "؋", "،",
    "؍", "؎", "؏"
  },// 0
  { "ؐ", "ؑ", "ؒ", "ؓ", "ؔ", "ؕ", "ؖ", "ؗ", "ؘ", "ؙ", "ؚ", "؛", "؜", "؝", "؞",
    "؟"
  },// 1
  { "ؠ", "ء", "آ", "أ", "ؤ", "إ", "ئ", "ا", "ب", "ة", "ت", "ث", "ج", "ح",
    "خ", "د"
  },// 2
  { "ذ", "ر", "ز", "س", "ش", "ص", "ض", "ط", "ظ", "ع", "غ", "ػ", "ؼ", "ؽ",
    "ؾ", "ؿ"
  },// 3
  { "ـ", "ف", "ق", "ك", "ل", "م", "ن", "ه", "و", "ى", "ي", "ً", "ٌ", "ٍ", "َ",
    "ُ"
  },                                                              // 4
  {"ِ", "ّ", "ْ", "ٓ", "ٔ", "ٕ", "ٖ", "ٗ", "٘", "ٙ", "ٚ", "ٛ", "ٜ", "ٝ", "ٞ ", " ٟ"}, // 5
  { "٠", "١", "٢", "٣", "٤", "٥", "٦", "٧", "٨", "٩", "٪", "٫", "٬", "٭",
    "ٮ", "ٯ"
  },// 6
  { "ٰ", "ٱ", "ٲ", "ٳ", "ٴ", "ٵ", "ٶ", "ٷ", "ٸ", "ٹ", "ٺ", "ٻ", "ټ", "ٽ", "پ",
    "ٿ"
  },// 7
  { "ڀ", "ځ", "ڂ", "ڃ", "ڄ", "څ", "چ", "ڇ", "ڈ", "ډ", "ڊ", "ڋ", "ڌ", "ڍ",
    "ڎ", "ڏ"
  },// 8
  { "ڐ", "ڑ", "ڒ", "ړ", "ڔ", "ڕ", "ږ", "ڗ", "ژ", "ڙ", "ښ", "ڛ", "ڜ", "ڝ",
    "ڞ", "ڟ"
  },// 9
  { "ڠ", "ڡ", "ڢ", "ڣ", "ڤ", "ڥ", "ڦ", "ڧ", "ڨ", "ک", "ڪ", "ګ", "ڬ", "ڭ",
    "ڮ", "گ"
  },// A
  { "ڰ", "ڱ", "ڲ", "ڳ", "ڴ", "ڵ", "ڶ", "ڷ", "ڸ", "ڹ", "ں", "ڻ", "ڼ", "ڽ",
    "ھ", "ڿ"
  },// B
  { "ۀ", "ہ", "ۂ", "ۃ", "ۄ", "ۅ", "ۆ", "ۇ", "ۈ", "ۉ", "ۊ", "ۋ", "ی", "ۍ",
    "ێ", "ۏ"
  },// C
  { "ې", "ۑ", "ے", "ۓ", "۔", "ە", "ۖ", "ۗ", "ۘ", "ۙ", "ۚ", "ۛ", "ۜ", "۝", "۞ ",
    "۟"
  },// D
  { "۠", "ۡ", "ۢ", "ۣ", "ۤ", "ۥ", "ۦ", "ۧ", "ۨ", "۩", "۪", "۫", "۬", "ۭ", "ۮ",
    "ۯ"
  },// E
  { "۰", "۱", "۲", "۳", "۴", "۵", "۶", "۷", "۸", "۹", "ۺ", "ۻ", "ۼ", "۽",
    "۾", "ۿ"
  }
};

String en[6][16] = {
  //  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
  { " ", "!", " ", "#", "$", "%", "&", " ", "(", ")", "*", "+", ",", "-", ".", "/"},
  { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ": ", "; ", "<", " = ", ">", " ? "},
  { "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O"},
  { "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\"", "]", "^", "_"},
  { "`", "a", "b", "c", "d", "e", "f", "g", "h", "I", "j", "k", "l", "m", "n", "o"},
  { "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~"}
};




/////////////////////////////////////
String SMS, Sender, Carect_Sender, Content;
unsigned int SMS_length;
void ResiveSMS() {
  SMS = "0791893905004100240C9189393828778200009160423261548121C8329BFD5620D38AE61B1D6EB7CB649002F9BE83E86F10F90DA2A3D373"; // read the incoming data
  // as string
  Serial.println("Resive New Masseg");
  SMS.remove(0, 22);
  SMS_length = SMS.length();
  Sender = SMS;
  Sender.remove(13, SMS_length - 13);
  Carect_Sender = Sender;
  for (int i = 0; i < 15; i = i + 2) {
    Carect_Sender.setCharAt(1 + i, Sender.charAt(i));
    Carect_Sender.setCharAt(i, Sender.charAt(i + 1));
  }
  Carect_Sender = "+" + Carect_Sender;
  Serial.println("Sender is : " + Carect_Sender);
  Content = SMS;
  Content.remove(0, 32);
  if (SMS.charAt(15) == '8') {
    Serial.println("This is ASCI");
    Serial.println(Unicode(Content));
  } else if (SMS.charAt(15) == '0') {
    Serial.println("This is En");
    Serial.println(pdu_decode(Content));
  }

  // Serial.print("Content is : ");
  // Serial.println(Content);
  Serial.println(pdu_decode("C8329BFD5620D38AE61B1D6EB7CB649002F9BE83E86F10F90DA2A3D373"));
}

String Unicode(String Input) {
  Serial.println(Input);

  byte data_byte[4] = {0, 0, 0, 0};
  int selector0, selector1 = 0;
  String Output;
  //  Output.remove(Input.length() / 4,Input.length() - Input.length() / 4);
  for (int i = 0; i <= Input.length(); i = i + 4) {
    for (int j = 0; j < 4; j++) {
      data_byte[j] = byte(Input.charAt(i + j) - 48);
      if ((char)data_byte[j] >= 10) {
        data_byte[j] = data_byte[j] - 7;
      }
      //      Serial.print(data_byte[j]);
      //      Serial.print("  ");
    }
    //    Serial.println("  ");
    selector0 = ((data_byte[0] * 10) + data_byte[1]);
    selector1 = ((data_byte[2] * 10) + data_byte[3]);

    if (selector0 == 6) {
      Output = Output + pr[data_byte[2]][data_byte[3]]; // convert to dec
    } else if (selector0 == 0) {
      Output = Output + en[data_byte[2] - 2][data_byte[3]]; // convert to dec
    }
    //    else if (selector0 == 0 && selector1 == 10) {
    //      Output = Output + "\n";
    //    }
    //    Serial.println(Output);
    //    delay(2000);
  }

  return Output;
}

void setup() {

  Serial.begin(115200);
  //  Serial2.begin(115200);
  //  Serial3.begin(9600);
  Serial.println("Micro is Start");
  ResiveSMS();
}

void loop() {}












/*

  +CMT: ,48
  0791893905004100240C9189393828778200009160423261548121C8329BFD5620D38AE61B1D6EB7CB649002F9BE83E86F10F90DA2A3D373

  +CIEV: "MESSAGE",1

  +CMT: ,97
  0791893905004100240C918939382877820008916042320262814E06330644062706450020068606370648063106CC0020000A0622064206270628062806CC064606450020062706480646002006280631064606270645064700200627064806A906CC00200634062F






*/
