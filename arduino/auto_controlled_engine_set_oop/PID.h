#ifndef PID_H
#define PID_H

#include "RVector3D.h"

#define PID_COMPONENTS_N 3

template <typename T> class PID
{
private:
    T Kp, Ki, Kd;

    T data0;

    T eIntegral;
    T ePrev;

    T yMin;
    T yMax;

    T PMin;
    T PMax;

    T IMin;
    T IMax;

    T DMin;
    T DMax;

    //temp
    T e, eDerivative; //error
    T y; //correction

    T PID_C[PID_COMPONENTS_N]; //last corrections (debug info)
    T P, I, D;

    void prepare(T, double);
    void iteration();

public:
    PID();

    T getKp();
    T getKi();
    T getKd();

    void setKp_x(double);
    void setKi_x(double);
    void setKd_x(double);

    void setKp_y(double);
    void setKi_y(double);
    void setKd_y(double);

    void setKp(double);
    void setKi(double);
    void setKd(double);

    void setKpKiKd(double, double, double);

    void setYMin(T);
    void setYMax(T);

    void setPMin(T);
    void setPMax(T);

    void setIMin(T);
    void setIMax(T);

    void setDMin(T);
    void setDMax(T);

    void setPMinMax_x(double);
    void setIMinMax_x(double);
    void setDMinMax_x(double);

    void setPMinMax_y(double);
    void setIMinMax_y(double);
    void setDMinMax_y(double);

    void setData0(T); // sets needed value
    T getData0();

    void reset();
    T getY(T, double); // iteration
    T getY(T, double, T); // iteration

    T* getLastPID();
};

#include "PID.cpp"

#endif // PID_H
