#include "PID.h"

#ifndef PID_CPP
#define PID_CPP

template <typename T> T PID<T>::getY(T data, double dt)
{
    prepare(data, dt);
    iteration();
    return(y);
}

template <typename T> T PID<T>::getY(T data, double dt, T derivative)
{
    prepare(data, dt);
    eDerivative = derivative;
    iteration();
    return(y);
}

template <typename T> void PID<T>::prepare(T data, double dt)
{
    //difference between requested and current data
    e = data0 - data;

    //discrete derivative
    eDerivative = (e - ePrev) / dt;

    //discrete integral
    eIntegral += e * dt;
}

template <typename T> void PID<T>::iteration()
{
    ePrev = e;

    for(int i = 0; i < 3; i++)
    {
        if(e.valueByAxisIndex(i) < PMin.valueByAxisIndex(i))
            e.valueByAxisIndex(i) = PMin.valueByAxisIndex(i);

        if(e.valueByAxisIndex(i) > PMax.valueByAxisIndex(i))
            e.valueByAxisIndex(i) = PMax.valueByAxisIndex(i);

        if(eIntegral.valueByAxisIndex(i) < IMin.valueByAxisIndex(i))
            eIntegral.valueByAxisIndex(i) = IMin.valueByAxisIndex(i);

        if(eIntegral.valueByAxisIndex(i) > IMax.valueByAxisIndex(i))
            eIntegral.valueByAxisIndex(i) = IMax.valueByAxisIndex(i);

        if(eDerivative.valueByAxisIndex(i) < DMin.valueByAxisIndex(i))
            eDerivative.valueByAxisIndex(i) = DMin.valueByAxisIndex(i);

        if(eDerivative.valueByAxisIndex(i) > DMax.valueByAxisIndex(i))
            eDerivative.valueByAxisIndex(i) = DMax.valueByAxisIndex(i);
    }

    //correction
    P = e % Kp;
    I = eIntegral % Ki;
    D = eDerivative % Kd;

    y = P + I + D;

    for(int i = 0; i < 3; i++)
    {
        if(y.valueByAxisIndex(i) < yMin.valueByAxisIndex(i))
            y.valueByAxisIndex(i) = yMin.valueByAxisIndex(i);

        if(y.valueByAxisIndex(i) > yMax.valueByAxisIndex(i))
            y.valueByAxisIndex(i) = yMax.valueByAxisIndex(i);
    }

}

template <typename T> PID<T>::PID()
{
    eIntegral = T();
    data0 = T();
    ePrev = T();
    Kp = Ki = Kd = T();
    eDerivative = T();
}

template <typename T> T PID<T>::getKp()
{
    return(Kp);
}

template <typename T> T PID<T>::getKi()
{
    return(Ki);
}

template <typename T> T PID<T>::getKd()
{
    return(Kd);
}

template <typename T> void PID<T>::setKp_x(double arg)
{
    Kp.x = arg;
}

template <typename T> void PID<T>::setKi_x(double arg)
{
    Ki.x = arg;
}

template <typename T> void PID<T>::setKd_x(double arg)
{
    Kd.x = arg;
}

template <typename T> void PID<T>::setKp_y(double arg)
{
    Kp.y = arg;
}

template <typename T> void PID<T>::setKi_y(double arg)
{
    Ki.y = arg;
}

template <typename T> void PID<T>::setKd_y(double arg)
{
    Kd.y = arg;
}

#ifdef PID_USE_YAW
template <typename T> void PID<T>::setKp_z(double arg)
{
    Kp.z = arg;
}

template <typename T> void PID<T>::setKi_z(double arg)
{
    Ki.z = arg;
}

template <typename T> void PID<T>::setKd_z(double arg)
{
    Kd.z = arg;
}
#endif

template <typename T> void PID<T>::setPMinMax_x(double arg)
{
    PMin.x = -arg;
    PMax.x =  arg;
}

template <typename T> void PID<T>::setIMinMax_x(double arg)
{
    IMin.x = -arg;
    IMax.x =  arg;
}

template <typename T> void PID<T>::setDMinMax_x(double arg)
{
    DMin.x = -arg;
    DMax.x =  arg;
}

template <typename T> void PID<T>::setPMinMax_y(double arg)
{
    PMin.y = -arg;
    PMax.y =  arg;
}

template <typename T> void PID<T>::setIMinMax_y(double arg)
{
    IMin.y = -arg;
    IMax.y =  arg;
}

template <typename T> void PID<T>::setDMinMax_y(double arg)
{
    DMin.y = -arg;
    DMax.y =  arg;
}

#ifdef PID_USE_YAW
template <typename T> void PID<T>::setPMinMax_z(double arg)
{
    PMin.z = -arg;
    PMax.z =  arg;
}

template <typename T> void PID<T>::setIMinMax_z(double arg)
{
    IMin.z = -arg;
    IMax.z =  arg;
}

template <typename T> void PID<T>::setDMinMax_z(double arg)
{
    DMin.z = -arg;
    DMax.z =  arg;
}
#endif

template <typename T> double PID<T>::getPMax_x()
{
    return(PMax.x);
}

template <typename T> double PID<T>::getIMax_x()
{
    return(IMax.x);
}

template <typename T> double PID<T>::getDMax_x()
{
    return(DMax.x);
}

template <typename T> void PID<T>::setKp(double arg)
{
    Kp = arg;
}

template <typename T> void PID<T>::setKi(double arg)
{
    Ki = arg;
}

template <typename T> void PID<T>::setKd(double arg)
{
    Kd = arg;
}

template <typename T> void PID<T>::setKpKiKd(double nKp, double nKi, double nKd)
{
    Kp = nKp;
    Ki = nKi;
    Kd = nKd;
}

template <typename T> void PID<T>::setYMin(T arg)
{
    yMin = arg;
}

template <typename T> void PID<T>::setYMax(T arg)
{
    yMax = arg;
}

template <typename T> void PID<T>::setPMin(T arg)
{
    PMin = arg;
}

template <typename T> void PID<T>::setPMax(T arg)
{
    PMax = arg;
}

template <typename T> void PID<T>::setIMin(T arg)
{
    IMin = arg;
}

template <typename T> void PID<T>::setIMax(T arg)
{
    IMax = arg;
}

template <typename T> void PID<T>::setDMin(T arg)
{
    DMin = arg;
}

template <typename T> void PID<T>::setDMax(T arg)
{
    DMax = arg;
}

template <typename T> void PID<T>::setData0(T arg)
{
    data0 = arg;
}

template <typename T> T PID<T>::getData0()
{
    return(data0);
}

template <typename T> void PID<T>::reset()
{
    ePrev = data0;
    eIntegral = 0;
}

#endif
