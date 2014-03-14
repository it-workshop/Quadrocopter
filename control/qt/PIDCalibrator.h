#ifndef PIDCALIBRATOR_H
#define PIDCALIBRATOR_H

#include <vector>

typedef struct
{
    double P;
    double I;
    double D;
} PIDCoeffs;

class quadro;

class PIDCalibrator
{
friend class quadro;
private:
    PIDCoeffs K;
    PIDCoeffs E;
    map<PIDCoeffs, double> time;
    PIDCoeffs Kprev;
    PIDResult R;

    double x;
    double xD;

    double xE;
    double xDE;

public:
    PIDCalibrator();

    void iteration();
};

#endif // PIDCALIBRATOR_H
