#ifndef PID_H
#define PID_H

#include "RVector3D.h"

template <typename T> class PID
{
private:
    T Kp, Ki, Kd;

    T data0;

    T eIntegral;
    T ePrev;

    T yMin;
    T yMax;

    //temp
    T e, eDerivative; //error
    T y; //correction

    void prepare(T, double);
    void iteration();

public:
    PID();

    T getKp();
    T getKi();
    T getKd();

//    void setKp(T);
//    void setKi(T);
//    void setKd(T);

    void setKp(double);
    void setKi(double);
    void setKd(double);

    void setKpKiKd(double, double, double);

    void setYMin(T);
    void setYMax(T);

    void setData0(T); // sets needed value
    T getData0();

    void reset();
    T getY(T, double); // iteration
    T getY(T, double, T); // iteration
};

#endif // PID_H
