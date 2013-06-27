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
    vect t_torque_corrected = read_vect_byte(), t_angle = read_vect_byte(2),
            t_gyroscope_readings = read_vect_byte() * SERIAL_GYRO_COEFF,
            t_PID = read_vect_byte() / SERIAL_PID_COEFF,
            t_torque_correction = read_vect_byte();

    reaction_type_ t_reaction_type;

    number_vect_t t_motors[MOTORS_N], t_loop_time;

    for(int i = 0; i < MOTORS_N; i++)
        t_motors[i] = sread();

    t_loop_time = read_unsigned_int_3byte() / 1.E6;

    t_reaction_type = (reaction_type_) (sread() - '0');

    number_vect_t t_voltage = read_number_vect_t(0, 20, 2);

    if(!readError())
    {
        for(int i = 0; i < MOTORS_N; i++)
            MOTORS[i] = t_motors[i];

        torque_corrected = t_torque_corrected;
        angle = t_angle;
        gyroscope_readings = t_gyroscope_readings;
        //accelerometer_readings = t_accelerometer_readings * g;
        PID_P = t_PID.x;
        PID_I = t_PID.y;
        PID_D = t_PID.z;

        if(t_reaction_type == REACTION_ANGULAR_VELOCITY)
            torque_angular_velocity_correction = t_torque_correction;
        else if(t_reaction_type == REACTION_ACCELERATION)
            torque_acceleration_correction = t_torque_correction;
        else if(t_reaction_type == REACTION_ANGLE)
            torque_angle_correction = t_torque_correction;

        loop_time = t_loop_time;
        reaction_type = t_reaction_type;

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
    for(int i = 0; i < 3; i++) // 2 - axis count
        write_number_vect_t(-1, 1, torque_manual_correction.value_by_axis_index(i), 2);

    //send power
    swrite(power * 100); // in percents

    //send reaction type
    swrite('0' + reaction_type);

    write_number_vect_t(-1.5, 1.5, PID_angle_Kp, 2);
    write_number_vect_t(-1.5, 1.5, PID_angle_Ki, 2);
    write_number_vect_t(-1.5, 1.5, PID_angle_Kd, 2);

    write_number_vect_t(0, 5, PID_angle_MAXp, 2);
    write_number_vect_t(0, 5, PID_angle_MAXi, 2);
    write_number_vect_t(0, 5, PID_angle_MAXd, 2);

    write_number_vect_t(0, 100, accel_period, 2);
    write_number_vect_t(0, 100, angle_period, 2);
}
