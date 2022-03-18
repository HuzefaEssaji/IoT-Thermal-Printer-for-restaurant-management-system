#include <SoftwareSerial.h>
#include<WiFi.h>
#include <Adafruit_Thermal.h> // Thermal printer library.
// please download these library from arduino ide or adafruit website.


#define TX_PIN 5 //  labeled RX on printer
#define RX_PIN 16 //  labeled TX on printer

SoftwareSerial MYSerial2(RX_PIN, TX_PIN,true); // true is passed to invert the rs232 signal
Adafruit_Thermal printer(&MYSerial2);

// WiFi Definitions
const char* ssid = "SEND-TEXT";
const char* password = "";

int led1 = 21 ,led2 = 18 , led3 = 19; //led 3 blue
//WiFiServer server(80); // wifi server port

void setup(){

  pinMode(led1,OUTPUT); //green led
  pinMode(led2,OUTPUT); // red led
  pinMode(TX_PIN,OUTPUT);

   //Serial.begin(115200);   // baudrate for serial monitor // uncomment this for debugging or printing messages on serial monitor
   MYSerial2.begin(9600); // baudrate for communicating with thermal printer // coomnet this line for debugging
   delay(10);

   WiFi.mode(WIFI_AP);
   WiFi.softAP(ssid, password);

   server.begin();

   digitalWrite(led1,LOW);
   digitalWrite(led2,HIGH);
   digitalWrite(led3,HIGH);

}

void loop() {
  // Check of client has connected
  WiFiClient client = server.available();

  if(!client) {
    digitalWrite(led3,HIGH);
    return;
  }
  else
  {
  digitalWrite(led3,LOW);
    // Read the request line
  String request = client.readString();
  request = request.substring(205);
  delay(300);

 //Serial.println(request); // uncomment this line for printing messsages on serial printer
  printer.justify('C');
  printer.boldOn();
  printer.setSize('L');                                             // these functions can be modified as per the text formatting required.
  printer.println(request);
  printer.boldOff();
  printer.justify('L');
  printer.setSize('s');


  digitalWrite(led1,HIGH);
  delay(2500);
  digitalWrite(led1,LOW);
  delay(500);
  client.flush();

   // JSON response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: application/json\r\n\r\n";
  s += "{\"data\":{\"message\":\"success\",\"value\":\"";
  s += "\"}}\r\n";
  s += "\n";

  client.print(s);
  delay(1);
//  Serial.println("Client disconnected");
    }
}
