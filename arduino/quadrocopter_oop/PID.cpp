#include "PID.h"

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
    e = data0 - data;

    //discrete derivative
    eDerivative = (e - ePrev) / dt;

    //discrete integral
    eIntegral += e * dt;
}

void PID::iteration()
{
    ePrev = e;

    
	if(e < PMin)
		e = PMin;

	if(e > PMax)
		e = PMax;

	if(eIntegral < IMin)
		eIntegral = IMin;

	if(eIntegral > IMax)
		eIntegral = IMax;

	if(eDerivative < DMin)
		eDerivative = DMin;

	if(eDerivative > DMax)
		eDerivative = DMax;


    //correction
    P = e * Kp;
    I = eIntegral * Ki;
    D = eDerivative * Kd;

    y = P + I + D;


	if(y < yMin)
		y = yMin;

	if(y > yMax)
		y = yMax;


}

PID::PID()
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
