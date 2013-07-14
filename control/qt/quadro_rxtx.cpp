#include "quadrocopter.h"
#include <math.h>
#include <iostream>
#include <mytime.h>
#include <vect.h>

#include "qextserialport.h"
#include <QDebug>

using std::cerr;
using std::endl;

void quadrocopter::on_rx()
{
    qDebug() << "available: " << port->bytesAvailable();

    while(port->bytesAvailable() >= readBytesN)
    {
        qDebug() << "calling read_data()";
        read_data();
        qDebug() << "read_data() ok";
    }
}

void quadrocopter::reset()
{
//    torque_manual_correction.x = 0;
//    torque_manual_correction.y = 0;
    swriteClear();
    swrite('n');
    swritePut();
    flush();
    busyBit = false;
}

void quadrocopter::initiate_transmission()
{
    readErrorReset();
    if(!isoperational() || busyBit) return;
    busyBit = true;
    qDebug() << "=== TRANSMISSION BEGINS ===";

    mytime write_timer;
    write_timer.setTime();

    flush();
    swriteClear();

    qDebug() << "=== TRANSMISSION: WRITE ===";
    swrite('p');
    write_data();

    swritePut();

    write_time = write_timer.getTimeDifference() / 1.E3;

    readTimer.setTime();
    qDebug() << "=== TRANSMISSION: READ_BEGIN ===";
}

void quadrocopter::read_data()
{
    qDebug() << "=== TRANSMISSION: READ_ACTUAL ===";
#ifndef DEBUG_NO_TX_ARDUINO
    vect t_torque_corrected = read_vect_byte();
    number_vect_t t_angle_x = read_number_vect_t(-10, 10, 2),
            t_angle_y = read_number_vect_t(-10, 10, 2);
    vect t_gyroscope_readings = read_vect_byte() * SERIAL_GYRO_COEFF,
            t_PID_x = read_vect_byte() / SERIAL_PID_COEFF,
            t_PID_y = read_vect_byte() / SERIAL_PID_COEFF;

    number_vect_t t_motors[MOTORS_N];

    for(int i = 0; i < MOTORS_N; i++)
        t_motors[i] = sread();

    number_vect_t t_voltage = read_number_vect_t(0, 20, 2);

    if(!readError())
    {
        for(int i = 0; i < MOTORS_N; i++)
            MOTORS[i] = t_motors[i];

        torque_corrected = t_torque_corrected;
        angle.x = t_angle_x;
        angle.y = t_angle_y;
        gyroscope_readings = t_gyroscope_readings;
        //accelerometer_readings = t_accelerometer_readings * g;
        PID_P.x = t_PID_x.x;
        PID_I.x = t_PID_x.y;
        PID_D.x = t_PID_x.z;

        PID_P.y = t_PID_y.x;
        PID_I.y = t_PID_y.y;
        PID_D.y = t_PID_y.z;

        voltage = t_voltage;
    }
//    char x[BN + 1];
//    x[BN] = 0;
//    int ef = 0;
//    for(int i = 0; i < BN; i++)
//    {
//        x[i] = sread();
//        if(x[i] != 33 + i)
//        {
//            qDebug() << "ef at i=" << i;
//            ef = 1;
//        }
//    }

//    qDebug() << "read" << endl << x;
    if(!readError()) newDataAvailable = true;
#else
    sread();
#endif

    read_time = readTimer.getTimeDifference() / 1.E3;

    if(readError()) qDebug() << " !!! READ_ERROR !!!";
    //if(ef) qDebug() << " !!! CMP_ERROR !!!";
    if(!(/*ef || */readError()))
        busyBit = false;

    qDebug() << "=== END OF TRANSMISSION ===" << endl;
}

void quadrocopter::write_data()
{
//    for(int i = 0; i < BN; i++)
//    {
//        swrite(i + 33);
//    }
    if(power > 1) power = 1;
    else if(power < 0) power = 0;

    //send torque_manual_correction
    for(int i = 0; i < 2; i++) // 2 - axis count
        write_number_vect_t(-1, 1, torque_manual_correction.value_by_axis_index(i), 2);

    //send power
    swrite(power * 100); // in percents

    //send reaction type
    swrite('0' + reaction_type);

    write_number_vect_t(-1.5, 1.5, PID_angle_Kp.x, 2);
    write_number_vect_t(-1.5, 1.5, PID_angle_Ki.x, 2);
    write_number_vect_t(-1.5, 1.5, PID_angle_Kd.x, 2);

    write_number_vect_t(0, 5, PID_angle_MAXp.x, 2);
    write_number_vect_t(0, 5, PID_angle_MAXi.x, 2);
    write_number_vect_t(0, 5, PID_angle_MAXd.x, 2);

    write_number_vect_t(-1.5, 1.5, PID_angle_Kp.y, 2);
    write_number_vect_t(-1.5, 1.5, PID_angle_Ki.y, 2);
    write_number_vect_t(-1.5, 1.5, PID_angle_Kd.y, 2);

    write_number_vect_t(0, 5, PID_angle_MAXp.y, 2);
    write_number_vect_t(0, 5, PID_angle_MAXi.y, 2);
    write_number_vect_t(0, 5, PID_angle_MAXd.y, 2);
}
