// HSensor.h

// Default address, found in datasheet Section 6.3
#define SENSOR_ADDRESS 0x18

// Master Commands from Datasheet Section 6.6.1
#define MEASURE_CMD 0xAA
#define DATA_CMD_1 0x00
#define DATA_CMD_2 0x00

// pin t0 read interrupt
// INT0, PD0, arduino digital pin 3/SCL
#define INT_PIN 3



// put function declarations here:
int UpdateSensor();
int GetPowerStatus();
int GetBusy();
int GetMemStat();
int GetMathSat();
long GetPressureDataRaw(); // in order to preserve 24-bit pressure data, function returns long
int GetPressureData();

