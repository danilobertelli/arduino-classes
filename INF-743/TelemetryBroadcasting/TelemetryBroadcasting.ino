#include "rgb_lcd.h"
#include <TH02_dev.h>
#include "Arduino.h"
#include "Wire.h" 

rgb_lcd lcd;

void setup() {
  Serial.begin(9600);        // start serial for output
  Serial.println("****Setup begin****");
  
  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(127, 127, 127);
  lcd.clear();

  /* Power up,delay 150ms,until voltage is stable */
  delay(150);
  /* Reset HP20x_dev */
  TH02.begin();
  delay(100);

  system("rfkill unblock bluetooth >/dev/null 2>&1");
  system("killall bluetoothd >/dev/null 2>&1");
  system("sleep 3 >/dev/null 2>&1");
  system("hciconfig hci0 up >/dev/null 2>&1");
  system("hciconfig hci0 leadv 3 >/dev/null 2>&1");
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

  updateEddystoneInfo(temp);
}

float getTemp() {
  float temper = TH02.ReadTemperature();
  return temper;
}

float getHumidity() {
  float humidity = TH02.ReadHumidity();
  return humidity;
}

void updateEddystoneInfo(float temperature){
    // TODO: Change the system command to update the Eddystone TML beacon
    //       Including the TLM uptime, broadcast counter, and temperature
    String command = "hcitool -i hci0 cmd 0x08 0x0008 1a 02 01 06 03 03 aa fe 11 16 aa fe 20 00 00 00 ";
    String newTemp = String((int) temperature, HEX);
    command += newTemp;
    command += " 00 00 00 00 00 00 00 00 00 00 >/dev/null 2>&1";
    Serial.println(command);
    char finalCommand[125] ;
    command.toCharArray(finalCommand, 125);
    system(finalCommand);

    delay(2000);
    // advertise a url
    //system("hcitool -i hci0 cmd 0x08 0x0008 1b 02 01 06 03 03 aa fe 13 16 aa fe 10 00 00 6d 6f 74 6f 72 6f 6c 61 07 2e 62 72 2f 00 00 00 00 >/dev/null 2>&1");
}
