#include "PIDCalibrator.h"

PIDCalibrator::PIDCalibrator(PIDCoeffs _K, PIDCoeffs _E)
{
    K = _K;
    E = _E;
}

PIDCoeffs PIDCalibrator::getNewCoeffs()
{

}

PIDCoeffs PIDCalibrator::getCoeffs()
{
    return(K);
}

void PIDCalibrator::setReaction(PIDResult _R)
{
    R = _R;
}


void PIDCalibrator::iteration()
{
    if()
}
