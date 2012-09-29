#ifndef PID_H
#define PID_H

#include "RVector3D.h"

class PID
{
private:
    RVector3D Kp, Ki, Kd;

    RVector3D data0;

    RVector3D eIntegral;
    RVector3D ePrev;

    RVector3D yMin;
    RVector3D yMax;

public:
    PID();

    RVector3D getKp();
    RVector3D getKi();
    RVector3D getKd();

    void setKp(RVector3D);
    void setKi(RVector3D);
    void setKd(RVector3D);

    void setKp(double);
    void setKi(double);
    void setKd(double);

    void setKpKiKd(double, double, double);

    void setYMin(RVector3D);
    void setYMax(RVector3D);

    void setData0(RVector3D); // sets needed value
    RVector3D getData0();

    void reset();

    RVector3D getY(RVector3D, double); // iteration
};

#endif // PID_H
