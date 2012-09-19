#include "PID.h"
#include "RVector3D.h"


RVector3D PID::get_y(RVector3D data, double dt)
{
    //difference between requested and current data
    RVector3D e = data0 - data;

    //discrete derivative
    RVector3D e_derivative = (e - e_prev) / dt;

    //discrete integral
    e_integral += e * dt;

    //correction
    RVector3D y = e % Kp + e_integral % Ki + e_derivative % Kd;

    e_prev = e;

    for(int i = 0; i < 3; i++)
    {
        if(y.value_by_axis_index(i) < y_min.value_by_axis_index(i))
            y.value_by_axis_index(i) = y_min.value_by_axis_index(i);

        if(y.value_by_axis_index(i) > y_max.value_by_axis_index(i))
            y.value_by_axis_index(i) = y_max.value_by_axis_index(i);
    }

    return(y);
}

PID::PID()
{
    e_integral = RVector3D();
    data0 = RVector3D();
    e_prev = RVector3D();
}

RVector3D PID::get_Kp()
{
    return(Kp);
}

RVector3D PID::get_Ki()
{
    return(Ki);
}

RVector3D PID::get_Kd()
{
    return(Kd);
}

void PID::set_Kp(RVector3D arg)
{
    Kp = arg;
}

void PID::set_Ki(RVector3D arg)
{
    Ki = arg;
}

void PID::set_Kd(RVector3D arg)
{
    Kd = arg;
}

void PID::set_Kp(double arg)
{
    Kp = arg;
}

void PID::set_Ki(double arg)
{
    Ki = arg;
}

void PID::set_Kd(double arg)
{
    Kd = arg;
}

void PID::set_y_min(RVector3D arg)
{
    y_min = arg;
}

void PID::set_y_max(RVector3D arg)
{
    y_max = arg;
}

void PID::set_data0(RVector3D arg)
{
    data0 = arg;
}
