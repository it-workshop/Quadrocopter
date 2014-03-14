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
//#define DEBUG_DAC

// Oscilloscope freq at pin
#define DEBUG_FREQ_PIN 13

// Oscilloscope MPU bytes pin
//#define DEBUG_MPUBYTES_PIN 4

// Turn off tx to PC (used both in Arduino and Qt apps)
//#define DEBUG_NO_TX_ARDUINO

// Cut AV that is lower than this value (used in PID)
#define PID_AV_MIN 1

// Cut AV that is lower than this value (used in PID)
#define PID_AV_MIN_Z 0.2

// Enable 3rd angular velocity PID (used both in Arduino and Qt apps)
//#define PID_USE_YAW

// Use 3rd angle instead of angular velocity
#define PID_USE_YAW_ANGLE

// Enable MPU-6050 I2C BYPASS
#define USE_MPU_BYPASS

// Enable copter compass (also enables compass rxtx)
#define USE_COMPASS

// if defined: joystick's compass rotates copter; default control
// ifndef    : joystick doesnt rotate copter    ; rotated control
#define COMPASS_ROTATE_COPTER

// Additional debug serial port
//define to enable
#define DEBUG_SERIAL_SECOND Serial1

// Disables PID if throttle < _value_
#define MINIMUM_PID_THROTTLE 0.18

// Minimum throttle: weight < 0
#define MINIMUM_FLYING_THROTTLE 0.55

// Disables PID I (integral <-- 0) if time < _value_ (seconds)
#define MINIMUM_FLYING_TIME 2

#endif
