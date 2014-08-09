// High-altitude balloon data logger
// made by Pacific Spaceflight 

// sketch consists of Arduino SD datalog example
// with Adafruit's BMP085 read example

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

const int chipSelect = 4;
Adafruit_BMP085 bmp;
  
void setup(){
//  Serial.begin(9600);
//  while(!Serial) {}

  if (!bmp.begin()) {
//    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while(1) {}
  }
//  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
//    Serial.println("Card failed, or not present");
    return;
  }
//  Serial.println("card initialized.");
}

void loop(){
//  Serial.print("Pressure at sealevel (calculated) = ");
//  Serial.print(bmp.readSealevelPressure());
//  Serial.println(" Pa");
//
//  Serial.print("Real altitude = ");
//  Serial.print(bmp.readAltitude(101500));
//  Serial.println(" meters");

  String dataString = "Temp:";  // string to save to microSD card
  
  dataString += String(bmp.readTemperature()); // celsius
  dataString += ", Pressure:";
  dataString += String(bmp.readPressure());  // pascals
  dataString += ", Altitude:";
  dataString += String(bmp.readAltitude());  // meters

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
//    Serial.println(dataString);
  }
  else {
//    Serial.println("error opening datalog.txt");
  }
  
  delay(1000);
}
