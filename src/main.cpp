#include <Arduino.h>
#include <Wire.h>
#include "HSensor.h"

// To read out a compensated pressure reading, the Master generates a START condition and sends the Sensor address followed by a
// read bit (1). After the Sensor generates an acknowledge, it will transmit up to 4 bytes of data. The first data byte is the Status Byte
// (8-bit) and the second to fourth bytes are the compensated pressure output (24-bit). The Master must acknowledge the receipt of
// each byte, and can terminate the communication by sending a Not Acknowledge (NACK) bit followed by a Stop bit after receiving
// the required bytes of data.


// Global Variables -----------------------------
volatile bool sensorReady = false;


// App Layer Functions --------------------------

// ISR Interrupt Handler
void DataReady() {
    sensorReady = true;
}


// Main Application ------------------------------

void setup() {
  
  Serial.begin(9600); // initialize serial driver
  Wire.begin(); // initialize I2C driver

  // Interrupt pin setup
  pinMode(INT_PIN, INPUT); // attach sensor EOC to interrupt pin
  attachInterrupt(digitalPinToInterrupt(INT_PIN), DataReady, RISING);


}

void loop() {
  
  if (UpdateSensor() != 0) {
    Serial.println("NOT VALID: Sensor Transmission Error");
    delay(1000);
    return;
  }
  
  int powerStatus = GetPowerStatus();
  int busyStatus = GetBusy();
  int memStatus = GetMemStat();
  int mathStatus = GetMathSat();

  // For all conditions met, read and output pressure data
  if (powerStatus && !busyStatus && !memStatus && !mathStatus) {

    long rawPressure = GetPressureDataRaw();
    int pressure_mmHg = GetPressureData();

    Serial.print("Raw Pressure: ");
    Serial.print(rawPressure);
    Serial.println();
    Serial.print("Pressure (mmHg): ");
    Serial.println(pressure_mmHg);

  } 

  // In case of conditions not met, output NOT VALID with corresponding reason(s)
  else {

    Serial.print("NOT VALID: ");

    if (!powerStatus) Serial.print(" Power Error ");
    if (busyStatus) Serial.print(" Sensor Busy ");
    if (memStatus) Serial.print(" Memory Error "); 
    if (mathStatus) Serial.print(" Math Saturated ");

    Serial.println();

  }
  delay(1000); // wait 1 second between readings

}