#include <QCoreApplication>
#include "qextserialport.h"
#include "../../../control/qt/serial.h"
#include <iostream>

serial Serial;

int read_int(){
     int t_high, t_low;
     t_high = Serial.sread();
     t_low = Serial.sread();
     return (t_low & 0xff) + (t_high << 8);
}

struct raw {
    double x, y, z;
    raw();
};

raw::raw(){
        x = y= z= 0;
}

const unsigned int N = 10000;

inline double sqr(double x){
        return x * x;
}

void initiate_transmission(){
    Serial.flush();

    Serial.swriteClear();
    Serial.swrite('r');
    Serial.swritePut();
}

int main(int argc, char *argv[])
{
    double k = 1./N;
    Serial.setDevice(argv[1]);
    Serial.sopen();
    std::cout << argv[1];
    raw measurements[N];
    raw mean, mean2;

    for (int i = 0; i < N; i++){
        initiate_transmission();
        measurements[i].x = read_int();
        measurements[i].y = read_int();
        measurements[i].z = read_int();
        if (!(i%100)) std::cout << "Measurement " << i << "\n";
        mean.x += k * measurements[i].x;
        mean.y += k * measurements[i].y;
        mean.z += k * measurements[i].z;
    }

    for (int i = 0; i < N; i++){
        mean2.x += k * sqr(measurements[i].x - mean.x);
        mean2.y += k * sqr(measurements[i].y - mean.y);
        mean2.z += k * sqr(measurements[i].z - mean.z);
    }

    float a, b, c, II;
    II = (mean2.x + mean2.y + mean2.z)/2.;
    a = (II - mean2.x)/II;
    b = (II - mean2.y)/II;
    c = (II - mean2.z)/II;

    std::cout << "Calibration done\n" \
         << "Mean: x: " << mean.x \
         << " y: " << mean.y \
         << " z: " << mean.z << "\n"\
         << "Axes: x: " << a \
         << " y: " << b \
         << " z: " << c << "\n";


}
