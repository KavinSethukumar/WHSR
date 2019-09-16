
#define BaudRate 9600
#define NewName "HM10_01"

void setup()
{
  Serial.begin(Baudrate);
  Serial.println(F("Config HM-10"));
  Serial.println(F("Remember that both devices, the Arduino and the HM-10 need to use the same baud rate."));
  Serial.println();
  Serial.println();


  Serial.println(F("AT+NAME") + NewName);
  //Serial.println(F("AT+BAUD")); // Returns the value 0-8, for example, OK+Get:0 
                                  // 0 – 9600 default
                                  // 1 – 19200
                                  // 2 – 38400
                                  // 3 – 57600
                                  // 4 – 115200
                                  // 5 – 4800
                                  // 6 – 2400
                                  // 7 – 1200
                                  // 8 – 230400

  Serial.println(F("AT+BAUDX")); // x is a value from 0 to 8.
}

void loop()
{
}