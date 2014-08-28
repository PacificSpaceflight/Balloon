// High-altitude balloon data logger
// from examples by Adafruit and contributors to DallasTemperature

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int chipSelect = 4;
Adafruit_BMP085 bmp;

#define ONE_WIRE_BUS 10
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

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
  
  sensors.begin();
  sensors.getDeviceCount();
  sensors.isParasitePowerMode();
  
  sensors.getAddress(insideThermometer, 0);
  sensors.getAddress(outsideThermometer, 1);

  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

  sensors.getResolution(insideThermometer);
  sensors.getResolution(outsideThermometer);


//  Serial.println("card initialized.");
}

void loop(){

  ///////////////////////////////////////////////
  //////////  BEGIN OneWire DS18S20  ////////////
  ///////////////////////////////////////////////


  sensors.requestTemperatures();

  float temp1 = sensors.getTempC(insideThermometer);
  float temp2 = sensors.getTempC(outsideThermometer);
 
  ////////////////////////////////////////
  /////////// BEGIN BMP TEMP  ////////////
  ////////////////////////////////////////
  
//  Serial.print("Pressure at sealevel (calculated) = ");
//  Serial.print(bmp.readSealevelPressure());
//  Serial.println(" Pa");
//
//  Serial.print("Real altitude = ");
//  Serial.print(bmp.readAltitude(101500));
//  Serial.println(" meters");

  String dataString = "Temp1:";  // string to save to microSD card
  dataString += String(bmp.readTemperature() * 1.8 + 32.0);
  dataString += ", Temp2:";
  dataString += String(temp1 * 1.8 + 32.0);
  dataString += ", Temp3:";
  dataString += String(temp2 * 1.8 + 32.0);
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
