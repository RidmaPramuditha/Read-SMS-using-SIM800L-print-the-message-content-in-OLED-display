#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 64, &Wire); //OLED Display
String Grsp;


SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

 
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();

}

void loop()
{
  updateSerial();


}

void updateSerial()
{

  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port

  }
  while (mySerial.available())
  {
    String sms = mySerial.readString();
    Serial.println(sms);
    int index = sms.indexOf(';');
    String message = sms.substring(index);
    Serial.println("Message is :" + message );
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print(sms);
    oled.display();
  }
}
