#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
#include "DHT.h"
#include <ESP8266WiFi.h>
#define DHTPIN D3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const int lval = A0;

const char* ssid     = "Tejaswi";
const char* password = "12345678";

const char* host = "52.89.21.192"; //My MEAN Public IP Address

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  dht.begin();
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();// Turn on the backlight.
  
}

void sendMessage(int val)
{
  Serial.print("Button Val #");
  Serial.println(num);
  num++;


   // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/update?light=";
  url += lval;
  url += "&temp=";
  url += tval;
  url += "&humidity=";
  url += hval;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  
}

void readTemp(){
  float tval = dht.readTemperature(true);
  lcd.setCursor(1,0); lcd.print("TEMP");
  lcd.setCursor(1, 1); lcd.print(tval);
  delay(2000);
  lcd.setCursor(1,0); lcd.print("          ");
  lcd.setCursor(1,1); lcd.print("          ");
}
void readHumidity(){
  float hval = dht.readHumidity();
  lcd.setCursor(1,0); lcd.print("HUMIDITY");
  lcd.setCursor(1, 1); lcd.print(hval);
  delay(2000);
  lcd.setCursor(1,0); lcd.print("          ");
  lcd.setCursor(1,1); lcd.print("          ");
}
void readLight(){
  lcd.setCursor(1,0); lcd.print("LIGHT");
  lcd.setCursor(1, 1); lcd.print(lval);
  delay(2000);
  lcd.setCursor(1,0); lcd.print("          ");
  lcd.setCursor(1,1); lcd.print("          ");
}

void loop() {
  readTemp();
  readHumidity();
  readLight();

  buttonState = analogRead(buttonPin);
  Serial.println(buttonState);
  //if(buttonState != lastVal){
    sendMessage(buttonState);
  //}
  lastVal = buttonState;
  
}
