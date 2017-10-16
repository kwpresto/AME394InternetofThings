//#include <Adafruit_Sensor.h>
#include "DHT.h"
//#include <DHT_U.h>

//use node
#include <ESP8266WiFi.h>

#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Tejaswi";
const char* password = "12345678";


//const char* host = "34.213.11.229"; //Prof's IP Address
//const char* host = "209.147.144.4"; //My IP Address
const char* host = "52.89.21.192"; //My MEAN Public IP Address

const int buttonPin = A0; 
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  pinMode(buttonPin, INPUT);

  Serial.begin(115200);
  delay(10);


  // We start by connecting to a WiFi network

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
}

int lastVal = 1;
int num = 1;

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

  float hval = dht.readHumidity();
  //float t = dht.readTemperature();
  float tval = dht.readTemperature(true);
  
  // We now create a URI for the request
  String url = "/update?light=";
  url += val;
  url += "&temp=";
  url += tval;
  url += "&humidity=";
  url += hval;
  /*
   * url += "&temp=" 
   * url += tval;
   */
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

void loop() {  
  
  buttonState = analogRead(buttonPin);
  Serial.println(buttonState);
  //if(buttonState != lastVal){
    sendMessage(buttonState);
  //}
  lastVal = buttonState;
  /*
  delay(5000);


  Serial.print("connecting to ");
  Serial.println(host);
  
 
  */
}
