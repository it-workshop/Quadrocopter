#include "PWMInput.h"

const int PINS[PINSN] = {3, 4, 5, 12};
volatile int A[PINSN], B[PINSN], S[PINSN];
volatile int RA[PINSN], RB[PINSN];

double PWMMin[PINSN] = {0.0571646341, 0.0598323171, 0.0606980273, 0.0562329886};
double PWMAvg[PINSN] = {0.0821867462, 0.0821670202, 0.0610012208, 0.0818106460};
double PWMMax[PINSN] = {0.1077961672, 0.1060151606, 0.0954377178, 0.1079105292};

void PWMInit()
{
    for(int i = 0; i < PINSN; i++)
    {
        attachInterrupt(PINS[i], PWMUpdater, CHANGE);
        A[i] = B[i] = -1;
        RA[i] = 1;
        RB[i] = 1;
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
    if(B[i] != -1)
    {
        B[i] = C - B[i];
        RB[i] = B[i];
    }
}

void PWMUpdateHL(int i)
{
    int C = micros();
    if(A[i] != -1)
    {
        A[i] = C - A[i];
        RA[i] = A[i];
    }
    B[i] = C;
}

double PWMGetValue(int i)
{
    double v = 1.0 * RA[i] / (RA[i] + RB[i]);
    if(v != v) v = PWMAvg[i];

    v -= PWMAvg[i];

    if(i != 2)
        v *= 2;

    v /= PWMMax[i] - PWMMin[i];

    if(i == 2)
    {
        if(v < 0) v = 0;
        if(v > 1) v = 1;
    }
    else
    {
        if(v < -1) v = -1;
        if(v > 1) v = 1;
    }
    return(v);
}
