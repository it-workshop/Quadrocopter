#include <QCoreApplication>
#include "qextserialport.h"
#include "../../../control/qt/serial.h"
#include <fstream>
#include <iostream>

serial Serial;

int16_t read_int(){
     int16_t t_high, t_low, t_int;
     t_high = Serial.sread();
     t_low = Serial.sread();
     t_int = (t_low & 0xff) + (t_high << 8);
     return t_int;
}

struct raw {
    double x, y, z;
    raw();
    bool operator==(raw& oth){
           return x == oth.x && y == oth.y && z == oth.z;
    }
    bool operator!=(raw& oth){
           return x != oth.x || y != oth.y || z != oth.z;
    }
};

raw::raw(){
        x = y= z= 0;
}

const unsigned int N = 7000;

inline double sqr(double x){
        return x * x;
}

void initiate_transmission(){
    Serial.swriteClear();
    Serial.swrite('r');
    Serial.swritePut();
}

using std::cin;
using std::cout;

int main(int argc, char *argv[])
{
    double k = 1./N;
    Serial.setDevice(argv[1]);
    Serial.setRate(115200);
    Serial.sopen();

    std::cout << "Device: " << argv[1] << "\n";
    std::ofstream raw_csv ("../raw.csv", std::ofstream::out);
    raw m[N]; //measurements
    raw mean, mean2;

    char c;
    cout << "Do continue? [y/n]?: ";
    cin >> c;
    if(c == 'n') return(0);
    


    for (int i = 0; i < N; i++){
        std::cerr << "#";
        initiate_transmission();
        while(Serial.bytesAvailable()<6) continue;

        m[i].x = read_int();
        m[i].y = read_int();
        m[i].z = read_int();
        if (i==0 || (m[i-1]!=m[i])) {
            raw_csv << m[i].x << ',' << m[i].y << ',' << m[i].z << '\n';
        }
        /*if (!(i%100))*/ std::cout << "Measurement " << i << ":\n" \
                                   << m[i].x << ',' << m[i].y << ',' << m[i].z << '\n';

    }

    raw_csv.close();

    std::cout << "Data prepared for calibration.\n" \
         << "Now you should proccess your raw.csv with elipsoid.ipy";


}
