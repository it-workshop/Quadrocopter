#include "PID.h"
#include "math.h"

double PID::getY(double data, double dt)
{
    prepare(data, dt);
    iteration();
    return(y);
}

double PID::getY(double data, double dt, double derivative)
{
    prepare(data, dt);
    eDerivative = derivative;
    iteration();
    return(y);
}

void PID::prepare(double data, double dt)
{
    //difference between requested and current data

    if(mode == DIFFERENCE_NORMAL)
        e = data0 - data;
    else if(mode == DIFFERENCE_ANGLE)
    {
        double e1, e2, e3;
        e1 = data0 - data;
        e2 = e1 - 2 * MPI;
        e3 = e1 + 2 * MPI;
        if(fabs(e2) < fabs(e1))
            e1 = e2;
        if(fabs(e3) < fabs(e1))
            e1 = e3;
        e = e1;
    }

    //discrete derivative
    eDerivative = (e - ePrev) / dt;

    //discrete integral
    eIntegral += e * dt;
}

#define LIMIT_CUT(x, xMin, xMax) {if((x) < (xMin)) {x = (xMin);} if((x) > (xMax)) {(x) = (xMax);} }

void PID::iteration()
{
    ePrev = e;

    LIMIT_CUT(e          , PMin / Kp, PMax / Kp);
    LIMIT_CUT(eIntegral  , IMin / Ki, IMax / Ki);
    LIMIT_CUT(eDerivative, DMin / Kd, DMax / Kd);

    //correction
    P = e * Kp;
    I = eIntegral * Ki;
    D = eDerivative * Kd;

    y = P + I + D;

    LIMIT_CUT(y, yMin, yMax);
}

PID::PID(pidMode nMode)
{
    eIntegral = 0;
    data0 = 0;
    ePrev = 0;
    Kp = Ki = Kd = 0;
    eDerivative = 0;
    PMin = PMax = 0;
    IMin = IMax = 0;
    DMin = DMax = 0;
    yMin = yMax = 0;
    P = I = D = 0;
    y = 0;
    mode = nMode;
}


void PID::setPMinMax(double arg)
{
    PMin = -arg;
    PMax =  arg;
}

void PID::setIMinMax(double arg)
{
    IMin = -arg;
    IMax =  arg;
}

void PID::setDMinMax(double arg)
{
    DMin = -arg;
    DMax =  arg;
}

void PID::setYMinYMax(double arg)
{
    yMin = -arg;
    yMax =  arg;
}

void PID::setKpKiKd(double nKp, double nKi, double nKd)
{
    Kp = nKp;
    Ki = nKi;
    Kd = nKd;
}


void PID::reset()
{
    ePrev = 0;
    eIntegral = 0;
}
