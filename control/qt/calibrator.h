#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include"mytime.h"
#include"quadrocopter.h"

class Calibrator
{
private:
    // Current calibration info
    double qual, qualPrev;
    double k, kPrev;
    double angleOld, angle, angleNew;
    double (Calibrator::*get)();
    void (Calibrator::*set)(double);
    double max;
    double direction;
    int total_attempts;

    mytime time;

    quadrocopter* quadro;

    bool active;
    // true - quadcrocopter is working with test parameters
    // false - quadrocoper is temporarily off

    // Constants (perhaps, the will become variables set with interface)
    static const double calibPower = 0.4;
    static const unsigned int calibTime = 30000;
    static const unsigned int waitTime = 5000;
    static const double step = 0.2;

    char _axis;
    char _coeff;

    double getKpX(); double getKpY();
    double getKiX(); double getKiY();
    double getKdX(); double getKdY();

    void setKpX(double); void setKpY(double);
    void setKiX(double); void setKiY(double);
    void setKdX(double); void setKdY(double);

public:
    Calibrator(quadrocopter* quadro, char axis, char coeff);
    ~Calibrator();
    void iteration();

    // Calibration is over
    bool ready;

    char axis();
    char coeff();
};

#endif // CALIBRATOR_H
