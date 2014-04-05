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
    debug_stderr = 1;
    if(debug_stderr)
        qDebug() << "available: " << port->bytesAvailable();

    while(port->bytesAvailable() >= readBytesN)
    {
        if(debug_stderr)
            qDebug() << "calling read_data()";
        read_data();
        if(debug_stderr)
            qDebug() << "read_data() ok";
    }
}

double quadrocopter::getPower()
{
    return(power);
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
    if(debug_stderr)
        qDebug() << "=== TRANSMISSION BEGINS ===";

    mytime write_timer;
    write_timer.setTime();

    flush();
    swriteClear();

    if(debug_stderr)
        qDebug() << "=== TRANSMISSION: WRITE ===";
    swrite('p');
    write_data();

    swritePut();

    write_time = write_timer.getTimeDifference() / 1.E3;

    readTimer.setTime();
    if(debug_stderr)
        qDebug() << "=== TRANSMISSION: READ_BEGIN ===";
}

void quadrocopter::read_data()
{
    if(debug_stderr)
        qDebug() << "=== TRANSMISSION: READ_ACTUAL ===";
#ifndef DEBUG_NO_TX_ARDUINO
    vect t_torque_corrected, t_gyroscope_readings;
    number_vect_t t_angle_x, t_angle_y, t_motors[MOTORS_N];
    number_vect_t t_voltage;
    vect t_PID_x, t_PID_y;
#if defined(PID_USE_YAW) || defined(PID_USE_YAW_ANGLE)
    vect t_PID_z;
#endif
#ifdef USE_COMPASS
    number_vect_t t_copter_heading, t_joystick_heading;
#endif
    number_vect_t t_power, t_angle0_x, t_angle0_y;

    t_torque_corrected.x = read_number_vect_t(-0.5, 0.5, 1);
    t_torque_corrected.y = read_number_vect_t(-0.5, 0.5, 1);
    t_torque_corrected.z = read_number_vect_t(-0.5, 0.5, 1);

    t_angle_x = read_number_vect_t(-10, 10, 2);
    t_angle_y = read_number_vect_t(-10, 10, 2);

    t_gyroscope_readings.x = read_number_vect_t(-100, 100, 1);
    t_gyroscope_readings.y = read_number_vect_t(-100, 100, 1);
    t_gyroscope_readings.z = read_number_vect_t(-100, 100, 1);

    t_PID_x.x = read_number_vect_t(-0.1, 0.1, 1);
    t_PID_x.y = read_number_vect_t(-0.1, 0.1, 1);
    t_PID_x.z = read_number_vect_t(-0.1, 0.1, 1);

    t_PID_y.x = read_number_vect_t(-0.1, 0.1, 1);
    t_PID_y.y = read_number_vect_t(-0.1, 0.1, 1);
    t_PID_y.z = read_number_vect_t(-0.1, 0.1, 1);

#if defined(PID_USE_YAW) || defined(PID_USE_YAW_ANGLE)
    t_PID_z.x = read_number_vect_t(-1, 1, 1);
    t_PID_z.y = read_number_vect_t(-1, 1, 1);
    t_PID_z.z = read_number_vect_t(-1, 1, 1);
#endif

#ifdef USE_COMPASS
    t_copter_heading = read_number_vect_t(-4, 4, 2);
    t_joystick_heading = read_number_vect_t(-4, 4, 2);
#endif

    t_power = read_number_vect_t(0, 105, 1) / 100.;

    t_angle0_x = read_number_vect_t(-2, 2, 2);
    t_angle0_y = read_number_vect_t(-2, 2, 2);

    for(int i = 0; i < MOTORS_N; i++)
        t_motors[i] = sread();

    t_voltage = read_number_vect_t(0, 20, 1);

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

#if defined(PID_USE_YAW) || defined(PID_USE_YAW_ANGLE)
        PID_P.z = t_PID_z.x;
        PID_I.z = t_PID_z.y;
        PID_D.z = t_PID_z.z;
#endif

#ifdef USE_COMPASS
        copter_heading = t_copter_heading;
        joystick_heading = t_joystick_heading;
#endif

        copter_angle0.x = t_angle0_x;
        copter_angle0.y = t_angle0_y;

        power = t_power;

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

    if(debug_stderr)
        qDebug() << "=== END OF TRANSMISSION ===" << endl;
}

void quadrocopter::write_data()
{
    swrite(force_override_value * 100); // +1
    //write_number_vect_t
    swrite(force_override); // +1

    //send reaction type
    swrite('0' + reaction_type);

    // +6
    write_number_vect_t(-M_PI, M_PI, angle_offset.x, 2);
    write_number_vect_t(-M_PI, M_PI, angle_offset.y, 2);
    write_number_vect_t(-M_PI, M_PI, angle_offset.z, 2);

    // PID
    write_number_vect_t(0, 0.5, PID_angle_Kp.x, 1);
    write_number_vect_t(0, 0.5, PID_angle_Ki.x, 1);
    write_number_vect_t(0, 0.5, PID_angle_Kd.x, 1);

    write_number_vect_t(0, 0.5, PID_angle_MAXp.x, 1);
    write_number_vect_t(0, 0.5, PID_angle_MAXi.x, 1);
    write_number_vect_t(0, 0.5, PID_angle_MAXd.x, 1);

    write_number_vect_t(0, 0.5, PID_angle_Kp.y, 1);
    write_number_vect_t(0, 0.5, PID_angle_Ki.y, 1);
    write_number_vect_t(0, 0.5, PID_angle_Kd.y, 1);

    write_number_vect_t(0, 0.5, PID_angle_MAXp.y, 1);
    write_number_vect_t(0, 0.5, PID_angle_MAXi.y, 1);
    write_number_vect_t(0, 0.5, PID_angle_MAXd.y, 1);

#if defined(PID_USE_YAW) || defined(PID_USE_YAW_ANGLE)
    write_number_vect_t(0, 0.5, PID_angle_Kp.z, 1);
    write_number_vect_t(0, 0.5, PID_angle_Ki.z, 1);
    write_number_vect_t(0, 0.5, PID_angle_Kd.z, 1);

    write_number_vect_t(0, 1, PID_angle_MAXp.z, 1);
    write_number_vect_t(0, 1, PID_angle_MAXi.z, 1);
    write_number_vect_t(0, 1, PID_angle_MAXd.z, 1);
#endif
}
