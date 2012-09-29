#include "PID.h"
#include "RVector3D.h"


RVector3D PID::getY(RVector3D data, double dt)
{
    //difference between requested and current data
    RVector3D e = data0 - data;

    //discrete derivative
    RVector3D e_derivative = (e - ePrev) / dt;

    //discrete integral
    eIntegral += e * dt;

    //correction
    RVector3D y = e % Kp + eIntegral % Ki + e_derivative % Kd;

    ePrev = e;

    for(int i = 0; i < 3; i++)
    {
        if(y.valueByAxisIndex(i) < yMin.valueByAxisIndex(i))
            y.valueByAxisIndex(i) = yMin.valueByAxisIndex(i);

        if(y.valueByAxisIndex(i) > yMax.valueByAxisIndex(i))
            y.valueByAxisIndex(i) = yMax.valueByAxisIndex(i);
    }

    return(y);
}

PID::PID()
{
    eIntegral = RVector3D();
    data0 = RVector3D();
    ePrev = RVector3D();
}

RVector3D PID::getKp()
{
    return(Kp);
}

RVector3D PID::getKi()
{
    return(Ki);
}

RVector3D PID::getKd()
{
    return(Kd);
}

void PID::setKp(RVector3D arg)
{
    Kp = arg;
}

void PID::setKi(RVector3D arg)
{
    Ki = arg;
}

void PID::setKd(RVector3D arg)
{
    Kd = arg;
}

void PID::setKp(double arg)
{
    Kp = arg;
}

void PID::setKi(double arg)
{
    Ki = arg;
}

void PID::setKpKiKd(double nKp, double nKi, double nKd)
{
    Kp = nKp;
    Ki = nKi;
    Kd = nKd;
}

void PID::setKd(double arg)
{
    Kd = arg;
}

void PID::setYMin(RVector3D arg)
{
    yMin = arg;
}

void PID::setYMax(RVector3D arg)
{
    yMax = arg;
}

void PID::setData0(RVector3D arg)
{
    data0 = arg;
}

RVector3D PID::getData0()
{
    return(data0);
}

void PID::reset()
{
    ePrev = data0;
    eIntegral = 0;
}
