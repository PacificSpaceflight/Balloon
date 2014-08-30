// High-altitude balloon data logger
// from examples by Adafruit and contributors to DallasTemperature

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2  // one wire on digital 2
#define IGNITION_BUS 3  // line burner on digital 3

// flight duration in seconds, currently set to 1.5 hours
#define FLIGHT_DURATION 5400 // 60sec * 60min * 1.5hrs

// ignite for 10 seconds
#define IGNITION_DURATION 10000 // in milliseconds

int line_has_been_cut = 0;
unsigned long elapsedSeconds = 0;

const int chipSelect = 4;
Adafruit_BMP085 bmp;

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

// prep ignition line
  pinMode(IGNITION_BUS, OUTPUT);
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

  String dataString = "Time:";  // string to save to microSD card
  dataString += String(elapsedSeconds);
  dataString += ", Temp1:";
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
  
  ////////////////////////////////////////
  ///////// TIME AND IGNITER  ////////////
  ////////////////////////////////////////

  elapsedSeconds = millis() / 1000.0;
//  Serial.println(elapsedSeconds);
  if(elapsedSeconds > FLIGHT_DURATION && !line_has_been_cut){
    ignite();
  }
  delay(1000);
}

// burn the igniter, cut the line
void ignite(){
  digitalWrite(IGNITION_BUS, HIGH);
//  Serial.println("IGNITION ON");
  delay(IGNITION_DURATION);
  digitalWrite(IGNITION_BUS, LOW);
//  Serial.println("IGNITION OFF");
  line_has_been_cut = 1;
}
