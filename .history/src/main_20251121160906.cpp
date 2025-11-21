#include <Arduino.h>
#include <Wire.h>
#include "HSensor.h"

// To read out a compensated pressure reading, the Master generates a START condition and sends the Sensor address followed by a
// read bit (1). After the Sensor generates an acknowledge, it will transmit up to 4 bytes of data. The first data byte is the Status Byte
// (8-bit) and the second to fourth bytes are the compensated pressure output (24-bit). The Master must acknowledge the receipt of
// each byte, and can terminate the communication by sending a Not Acknowledge (NACK) bit followed by a Stop bit after receiving
// the required bytes of data.

// GLOBAL VARIABLES --------------------

void setup() {
  
  Serial.begin(9600); // initialize serial driver
  Wire.begin(); // initialize I2C driver

}

void loop() {
  
  UpdateSensor(); 
  int powerStatus = GetPowerStatus();
  int busyStatus = GetBusy();
  int memStatus = GetMemStat();
  int mathStatus = GetMathSat();
  if (busyStatus & !busyStatus & !memStatus & !mathStatus) {
    long rawPressure = GetPressureDataRaw();
    int pressure_mmHg = GetPressureData();
    Serial.print(", Raw Pressure: ");
    Serial.print(rawPressure);
    Serial.print(", Pressure (mmHg): ");
    Serial.println(pressure_mmHg);
  } else {
    Serial.print("NOT VALID: ");
    if (powerStatus) {
      Serial.print(" Power Error ");
    }
    if (busyStatus) {
      Serial.print(" Sensor Busy ");
    }
    if (memStatus) {
      Serial.print(" Memory Saturated "); 
    }
    if (mathStatus) {
      Serial.print(" Math Saturated ");
    }
    Serial.println();
  }
  delay(1000); // wait 1 second between readings

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}