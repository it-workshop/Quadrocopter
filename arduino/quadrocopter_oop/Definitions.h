#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MPI 3.141592653589793

#define doubleEps 1E-2

// Do not wait for motors
//#define DEBUG_NO_MOTORS

// Add DAC8512 support and use it as InfoLED
//#define DEBUG_DAC

// Oscilloscope freq at pin
#define DEBUG_FREQ_PIN 4

// Turn off tx to PC (used both in Arduino and Qt apps)
//#define DEBUG_NO_TX_ARDUINO

// Scale PID values (used both in Arduino and Qt apps)
#define SERIAL_PID_COEFF 100

// Scale AV values (used both in Arduino and Qt apps)
#define SERIAL_GYRO_COEFF 8

// Cut AV that is lower than this value (used in PID)
#define PID_AV_MIN 1

#endif
