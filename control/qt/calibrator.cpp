#include "calibrator.h"

Calibrator::Calibrator(quadrocopter* quadro_, char axis, char coeff)
{
    quadro = quadro_;

    _axis = axis;
    _coeff = coeff;

    if (axis == 'X' && coeff == 'P') {
        get = &Calibrator::getKpX;
        set = &Calibrator::setKpX;
        max = quadro->get_PID_angle_MAXp().x;
    }
    if (axis == 'X' && coeff == 'I') {
        get = &Calibrator::getKiX;
        set = &Calibrator::setKiX;
        max = quadro->get_PID_angle_MAXi().x;
    }
    if (axis == 'X' && coeff == 'D') {
        get = &Calibrator::getKdX;
        set = &Calibrator::setKdX;
        max = quadro->get_PID_angle_MAXd().x;
    }
    if (axis == 'Y' && coeff == 'P') {
        get = &Calibrator::getKpY;
        set = &Calibrator::setKpY;
        max = quadro->get_PID_angle_MAXp().y;
    }
    if (axis == 'Y' && coeff == 'I') {
        get = &Calibrator::getKiY;
        set = &Calibrator::setKiY;
        max = quadro->get_PID_angle_MAXi().y;
    }
    if (axis == 'Y' && coeff == 'D') {
        get = &Calibrator::getKiY;
        set = &Calibrator::setKiY;
        max = quadro->get_PID_angle_MAXd().y;
    }

    direction = 1;
    k = (this->*get)();
}

Calibrator::~Calibrator() {

}


double Calibrator::getKpX() {
    return quadro->get_PID_angle_Kp().x;
}

double Calibrator::getKiX() {
    return quadro->get_PID_angle_Ki().x;
}

double Calibrator::getKdX() {
    return quadro->get_PID_angle_Kd().x;
}

double Calibrator::getKpY() {
    return quadro->get_PID_angle_Kp().y;
}

double Calibrator::getKiY() {
    return quadro->get_PID_angle_Ki().y;
}

double Calibrator::getKdY() {
    return quadro->get_PID_angle_Kd().y;
}

void Calibrator::setKpX(double k) {
    quadro->set_PID_angle_Kp_x(k);
}

void Calibrator::setKiX(double k) {
    quadro->set_PID_angle_Kp_x(k);
}

void Calibrator::setKdX(double k) {
    quadro->set_PID_angle_Kp_x(k);
}

void Calibrator::setKpY(double k) {
    quadro->set_PID_angle_Kp_y(k);
}

void Calibrator::setKiY(double k) {
    quadro->set_PID_angle_Kp_y(k);
}

void Calibrator::setKdY(double k) {
    quadro->set_PID_angle_Kp_y(k);
}

void Calibrator::iteration() {
    if (!ready) {
        if (active) {
            // Quadrocopter is active
            angleOld = angle;
            angle = angleNew;
            angleNew = (this->*get)();

            if (angle > angleOld && angle > angleNew) qual = 1 / angle;
            if (angle < angleOld && angle < angleNew) qual = - 1 / angle;

            if (time.getTimeDifference() > calibTime) {
                // Stopping
                quadro->set_power(0);

                if (qual < qualPrev) {
                    if (total_attempts == 0) direction = - direction;
                    else {
                        ready = true;
                        (this->*set)(kPrev);
                    }
                }

                total_attempts++;
                active = false;

                time.setTime();
            }
        }
        else {
            // Quadrocopter is off
            if (time.getTimeDifference() > waitTime) {
                // Starting
                active = true;
                quadro->set_power(calibPower);
                kPrev = k;
                k += step * direction;
                (this->*set)(k);
                qualPrev = qual;
            }
        }
    }
}

char Calibrator::axis() { return _axis; }
char Calibrator::coeff() { return _coeff; }
