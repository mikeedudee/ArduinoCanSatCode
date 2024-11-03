/* SD CARD */
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10; // SD input pin
File myFile;               // Setting the File library as "myFile"

/* GPS Global Variables and constants */
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;                                                                                                // Setting the TinyGPSPlus library as "gps"
int RXPin = 4; int TXPin = 3; int GPSBaud = 9600;                                                               // Pins for Transmit & Receive, and GPS Tranmission Bandwidth
int MonthVar, DayVar, YearVar, HourVar, MinutesVar, SecondVar, CentiSecondVar; float latitudeVar, longitudeVar; // Call the Variables from the GPSModule Library
SoftwareSerial gpsSerial(RXPin, TXPin);

/* MS5611 — Altimeter */
#include <Wire.h>
#include <MS5611.h>
MS5611 ms5611;                                                                        // Setting the MS5611 library as "ms5611"
double realTemperature, referencePressure; long realPressure; float relativeAltitude; // Call the Variables from the MS5611Module Library

/* Thermistor */
float T0 = 298.15;                              // Temperature at Sea
float R0 = 10000;                               // Resistance 10Kohms
float B  = 3435;                                // Reference value
float Dmeasured, voltage, Temp, C, Rthermistor; // Declare global variables

void setup() {
  Serial.begin(9600);
  SDCardFunction(); GPSFunction(); MS5611Function(); // Call the Main Functions of the Libraries
                            
  for (int i = 5; i < 10; i++) { // LED pins [exception: 8 for Buzzer]
    pinMode(i, OUTPUT);
  }
}

void loop() {
  GPSSerialAvailability(); MS5611Readings(); ThermistorModule(); // Call the Main Functions of the Tabs
  int TimeElapsedVar = millis() / 1000;                          // Initializing the values for elapsed time since run-up
  float averageTemperature = (realTemperature + C)/2;            // Averaging the Temperature from MS5611 and Thermistor

  // FORMAT: Temperature, Pressure, Altitude, Latitude, Longitude, Month, Day, Year, Hour, Min, Sec, Time Elapsed, Data Written (0 or 1)
  double dataArray[] = {relativeAltitude, realPressure, averageTemperature, TimeElapsedVar, latitudeVar, longitudeVar};

  // Print data to the serial monitor
  for (int d = 0; d < sizeof(dataArray) / sizeof(dataArray[0]); d++) {
    Serial.print(dataArray[d]);
    Serial.print(" | ");
  }

  // Open file for writing
  myFile = SD.open("FD.txt", FILE_WRITE);  // FD - File Data

  if (myFile) {
    bool errorOccurred = false;
    for (int i = 0; i < sizeof(dataArray) / sizeof(dataArray[0]); i++) {
      if (myFile.println(dataArray[i]) == 0) {
        errorOccurred = true; // If write fails, set error flag
        Serial.println("0");
        break;  // Exit the loop to avoid further writes
      }
    }
    myFile.close(); // Ensure the file is closed, even if an error occurred
    Serial.println(errorOccurred ? "0" : "1");
  } else {
    Serial.println("0");  // Failed to open file
  }

  //analogWrite(8, 150);
  digitalWrite(5, HIGH);
  delay(50);
  analogWrite(8, 0);
  digitalWrite(5, LOW);
  delay(50);
}
