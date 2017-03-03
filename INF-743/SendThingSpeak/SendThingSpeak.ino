#include <WiFi.h>
#include "rgb_lcd.h"
#include <TH02_dev.h>
#include "Arduino.h"
#include "Wire.h" 

rgb_lcd lcd;

const char* server = "184.106.153.149";
char* ssid = "WiFI - SSID";
char* password = "Password";

void setup() {
  Serial.begin(9600);        // start serial for output
  Serial.println("****Setup begin****");
  
  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(127, 127, 127);
  lcd.clear();

  // WIFI
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Power up,delay 150ms,until voltage is stable */
  delay(150);
  /* Reset HP20x_dev */
  TH02.begin();

  delay(100);
  Serial.println("****Setup done****");
}

void loop() {
  // put your main code here, to run repeatedly:
  printData();
  delay(2000);
}

void printData() {
  lcd.clear();

  float temp = getTemp();
  char t[80];
  sprintf(t, "T: %fC", temp);
  lcd.print(t);
  
  lcd.setCursor(0, 1);

  float humidity = getHumidity();
  char h[80];
  sprintf(h, "H: %f%%", humidity);
  lcd.print(h);

  // Build again... fuc** C
  char ctemp[80];
  sprintf(ctemp, "%f", temp);
  char chumi[80];
  sprintf(chumi, "%f", humidity);
  updateThingSpeakServer(String(ctemp), String(chumi));
}

float getTemp() {
  float temper = TH02.ReadTemperature();
  return temper;
}

float getHumidity() {
  float humidity = TH02.ReadHumidity();
  return humidity;
}

void updateThingSpeakServer(String temperature, String humidity){
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("connected");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" degrees Celcius : "); 
    Serial.println(humidity);

    String command = "curl --data \"api_key=YOUR_KEY&field1=";
    command += temperature;
    command += "&field2=";
    command += humidity;
    command += "\" https://api.thingspeak.com/update";
    Serial.println(command);
    
    char finalCommand[130] ;
    command.toCharArray(finalCommand, 130);
    system(finalCommand);
    Serial.println("final:");
    Serial.println(finalCommand);
  }
  delay(20000); // ThingSpeak will only accept updates every 15 seconds.
}
