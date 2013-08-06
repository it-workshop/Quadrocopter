#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MPI 3.141592653589793

#define doubleEps 1E-2

// Architecture
#ifdef __arm__
  #define _arch_arm_
#else
  #define _arch_avr_
#endif

// Disable MPU-6050
//#define DEBUG_NO_MPU

// Do not wait for motors
//#define DEBUG_NO_MOTORS

// Add DAC8512 support and use it as InfoLED
#define DEBUG_DAC

// Oscilloscope freq at pin
#define DEBUG_FREQ_PIN 13

// Oscilloscope MPU bytes pin
//#define DEBUG_MPUBYTES_PIN 4

// Turn off tx to PC (used both in Arduino and Qt apps)
//#define DEBUG_NO_TX_ARDUINO

// Cut AV that is lower than this value (used in PID)
#define PID_AV_MIN 1

// Cut AV that is lower than this value (used in PID)
#define PID_AV_MIN_Z 1

// Enable 3rd angle PID (used both in Arduino and Qt apps)
#define PID_USE_YAW

// Enable copter compass
#define USE_COMPASS

#endif
