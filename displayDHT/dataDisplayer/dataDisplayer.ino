#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
#include "DHT.h"
#include <ESP8266WiFi.h>
#define DHTPIN D3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const int lval = A0;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  // The begin call takes the width and height. This
  // Should match the number provided to the constructor.
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();// Turn on the backlight.
  
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
  
}
