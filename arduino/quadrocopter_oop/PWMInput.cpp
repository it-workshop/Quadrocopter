#include "PWMInput.h"

const int PINS[PINSN] = {4, 5, 12, 13};
volatile int A[PINSN], B[PINSN];
volatile int S[PINSN];

void PWMInit()
{
    for(int i = 0; i < PINSN; i++)
    {
        attachInterrupt(PINS[i], PWMUpdater, CHANGE);
        A[i] = B[i] = 1;
    }
}

void PWMUpdater()
{
    int t;
    for(int i = 0; i < PINSN; i++)
    {
        t = digitalRead(PINS[i]);

        if(S[i] == HIGH && t == LOW)
            PWMUpdateHL(i);
        else if(S[i] == LOW && t == HIGH)
            PWMUpdateLH(i);

        S[i] = t;
    }
}

void PWMUpdateLH(int i)
{
    int C = micros();
    A[i] = C;
    B[i] = C - B[i];
}

void PWMUpdateHL(int i)
{
    int C = micros();
    A[i] = C - A[i];
    B[i] = micros();
}

int PWMGetValue(int i)
{
    return(100 * A[i] / (A[i] + B[i]));
}
