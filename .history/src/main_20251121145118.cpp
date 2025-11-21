#include <Arduino.h>
#include <Wire.h>

// To read out a compensated pressure reading, the Master generates a START condition and sends the Sensor address followed by a
// read bit (1). After the Sensor generates an acknowledge, it will transmit up to 4 bytes of data. The first data byte is the Status Byte
// (8-bit) and the second to fourth bytes are the compensated pressure output (24-bit). The Master must acknowledge the receipt of
// each byte, and can terminate the communication by sending a Not Acknowledge (NACK) bit followed by a Stop bit after receiving
// the required bytes of data.


void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}