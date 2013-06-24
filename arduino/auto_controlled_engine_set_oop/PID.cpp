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
    //correction
    y = e % Kp + eIntegral % Ki + eDerivative % Kd;

    ePrev = e;

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

//template <typename T> void PID<T>::setKp(T arg)
//{
//    Kp = arg;
//}

//template <typename T> void PID<T>::setKi(T arg)
//{
//    Ki = arg;
//}

//template <typename T> void PID<T>::setKd(T arg)
//{
//    Kd = arg;
//}

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
