#ifndef PID_H
#define PID_H

#include "RVector3D.h"

class PID
{
private:
    RVector3D Kp, Ki, Kd;

    RVector3D data0;

    RVector3D e_integral;
    RVector3D e_prev;

    RVector3D y_min;
    RVector3D y_max;

public:
    PID();

    RVector3D get_Kp();
    RVector3D get_Ki();
    RVector3D get_Kd();

    void set_Kp(RVector3D);
    void set_Ki(RVector3D);
    void set_Kd(RVector3D);

    void set_Kp(double);
    void set_Ki(double);
    void set_Kd(double);

    void seRVector3D0(RVector3D); // sets needed value

    RVector3D get_y(RVector3D, double); // iteration
};

#endif // PID_H
