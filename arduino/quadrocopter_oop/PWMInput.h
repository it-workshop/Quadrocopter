#ifndef PWMINPUT_H
#define PWMINPUT_H

#include <Arduino.h>

#define PINSN 4

extern const int PINS[PINSN];
extern volatile int A[PINSN], B[PINSN];
extern volatile int S[PINSN];

int PWMGetValue(int i);
void PWMUpdateHL(int i);
void PWMUpdateLH(int i);
void PWMUpdater();
void PWMInit();

#endif // PWMINPUT_H
