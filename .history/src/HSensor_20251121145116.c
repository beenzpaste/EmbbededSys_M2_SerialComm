#include <Arduino.h>
#include <Wire.h>
#include "HSensor.h"

static int statusByte = 0; // store latest status byte
static long pressureData = 0; // store latest pressure data

// To read out a compensated pressure reading, the Master generates a START condition and sends the Sensor address followed by a
// read bit (1). After the Sensor generates an acknowledge, it will transmit up to 4 bytes of data. The first data byte is the Status Byte
// (8-bit) and the second to fourth bytes are the compensated pressure output (24-bit). The Master must acknowledge the receipt of
// each byte, and can terminate the communication by sending a Not Acknowledge (NACK) bit followed by a Stop bit after receiving
// the required bytes of data.

int updateSensor() {

    Wire.beginTransmission(SENSOR_ADDRESS);
    // master sends three bytes to sensor to request data
    Wire.write(MEASURE_CMD);
    Wire.write(DATA_CMD_1);
    Wire.write(DATA_CMD_2);
    Wire.endTransmission();
    Wire.requestFrom(SENSOR_ADDRESS, 4); // request 4 bytes from sensor

    if (Wire.available() == 4) {
        statusByte = Wire.read(); // read status byte
        pressureData = 0;
        pressureData |= (long)Wire.read() << 16; // read first byte of pressure data
        pressureData |= (long)Wire.read() << 8;  // read second byte of pressure data
        pressureData |= (long)Wire.read();       // read third byte of pressure data
    }

    Wire.endTransmission();

    return 0; // success
}


int GetPowerStatus(){
    // indicated by 6th bit of status byte
    return (statusByte >> 6) & 0x01;
}

int GetBusy(){
    // indicated by 5th bit of status byte
    return (statusByte >> 5) & 0x01;
}
int GetMemStat(){
    // indicated by 2nd bit of status byte
    return (statusByte >>  2) & 0x01;
}
int GetMathSat(){
    // indicated by 0th bit of status byte
    return (statusByte >> 0) & 0x01;
}
int GetPressureDataRaw(){
    // read raw pressure data from sensor
}
int GetPressureData(){
    // read compensated pressure data from sensor
}