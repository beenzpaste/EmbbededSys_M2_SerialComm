#include <Arduino.h>
#include <Wire.h>
#include "HSensor.h"

static int statusByte = 0; // store latest status byte
static long rawPressureData = 0; // store latest pressure data

// To read out a compensated pressure reading, the Master generates a START condition and sends the Sensor address followed by a
// read bit (1). After the Sensor generates an acknowledge, it will transmit up to 4 bytes of data. The first data byte is the Status Byte
// (8-bit) and the second to fourth bytes are the compensated pressure output (24-bit). The Master must acknowledge the receipt of
// each byte, and can terminate the communication by sending a Not Acknowledge (NACK) bit followed by a Stop bit after receiving
// the required bytes of data.

int UpdateSensor() {

    Wire.beginTransmission(SENSOR_ADDRESS);
    // master sends three bytes to sensor to request data
    Wire.write(MEASURE_CMD);
    Wire.write(DATA_CMD_1);
    Wire.write(DATA_CMD_2);

    // end transmission, check for error
    if (Wire.endTransmission() != 0) {
        return -1; // error: transmission failed
    }

    delay(5); // min wait time for data conversion to occur (datasheet table 16)

    // request and read 4 bytes from sensor
    Wire.requestFrom(SENSOR_ADDRESS, 4); // request 4 bytes from sensor

    if (Wire.available() == 4) {
        statusByte = Wire.read(); // read status byte
        rawPressureData = 0;
        rawPressureData |= (long)Wire.read() << 16; // read first byte of pressure data
        rawPressureData |= (long)Wire.read() << 8;  // read second byte of pressure data
        rawPressureData |= (long)Wire.read();       // read third byte of pressure data
    }
    else {
        return -1; // error: not enough data received (<4 bytes)
    }

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
long GetPressureDataRaw(){
    // read raw pressure data from sensor
    return rawPressureData;
}
int GetPressureData(){
    // convert raw pressure data to normalized mmHg data
    // assuming 0300YG: 0 to 300 mmHg range
    // transfer function B: valid output range = 2.5-22.5% of 2^24 counts

    float counts = 16777216.0; // 2^24

    const float OUTPUT_MIN = 0.025 * counts;  // 2.5% of 2^24
    const float OUTPUT_MAX = 0.225 * counts;  // 22.5% of 2^24

    long output = rawPressureData;

    // clamp output to valid range
    if (output < OUTPUT_MIN) output = OUTPUT_MIN;
    if (output > OUTPUT_MAX) output = OUTPUT_MAX;

    // pressure output function from datasheet section 8
    float p_norm = (output - OUTPUT_MIN) / (OUTPUT_MAX - OUTPUT_MIN);

    // convert to mmHg where [p_max, p_min] = [300, 0] mmHg
    // [P_norm * (300 - 0) - 0] == [P_norm * 300]
    float pressure_mmHg = p_norm * 300.0;

    return (int)pressure_mmHg;
}
