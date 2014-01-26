#include "quadrocopter.h"
#include <math.h>
#include <iostream>
#include <mytime.h>
#include <vect.h>

#include "qextserialport.h"
#include <QDebug>

using std::cerr;
using std::endl;

quadrocopter::quadrocopter()
{
    debug_stderr = true;

    rate = 115200;
    maxwait = 500;

    //device = "rfcomm0";
    device = "ttyACM0";

    connectDelayTime = 1000;

    //connect_delay_arduino = 9000;
    connect_delay_arduino = 5000;

    PID_angle_Kp = 0;
    PID_angle_Ki = 0;
    PID_angle_Kd = 0;

    //see arduino code
#ifdef DEBUG_NO_TX_ARDUINO
    readBytesN = 1;
#else
    readBytesN = 28;
    #ifdef PID_USE_YAW
        readBytesN += 3;
    #endif
    #ifdef PID_USE_YAW_ANGLE
        readBytesN += 3;
    #endif
    #ifdef USE_COMPASS
        readBytesN += 2;
    #endif
#endif

    if(debug_stderr)
        qDebug() << "readBytesN=" << readBytesN;

    //readBytesN = BN;

    busyBit = false;

    newDataAvailable = false;

    force_override = 0;
    force_override_value = 0;

    defaults();
}

void quadrocopter::defaults()
{
    busyBit = false;
    torque_manual_correction = vect();
    torque_corrected = vect();
    gyroscope_readings = vect();
    accelerometer_readings = vect();

    angle = vect();
    power = 0;
    for(int i = 0; i < get_motors_n(); i++) MOTORS[i] = 0;

    read_time = 0;
    write_time = 0;
    loop_time = 0;

    reaction_type = REACTION_NONE;

    voltage = 0;

    //wait for arduino to load
    connectDelayTime = 5500;
}

void quadrocopter::do_connect()
{
    defaults();

    sopen();
}

void quadrocopter::do_disconnect()
{
    defaults();
    power = 0;

    write_data();

    sclose();

    readErrorReset();
}

number_vect_t quadrocopter::get_read_time()
{
    return(read_time);
}

vect quadrocopter::get_PID_P()
{
    return(PID_P);
}

vect quadrocopter::get_PID_I()
{
    return(PID_I);
}

vect quadrocopter::get_PID_D()
{
    return(PID_D);
}

bool quadrocopter::getNewDataAvailable()
{
    return(newDataAvailable);
}

void quadrocopter::resetNewDataAvailable()
{
    newDataAvailable = false;
}

number_vect_t quadrocopter::get_copter_heading()
{
#ifdef USE_COMPASS
    return(copter_heading);
#else
    return(0);
#endif
}

number_vect_t quadrocopter::get_joystick_heading()
{
    return(joystick_heading);
}

void quadrocopter::set_force_override(bool _do, double _value)
{
    force_override = _do;
    force_override_value = _value / 100.;
    //cerr << "set do=" << force_override << " value=" << force_override_value << endl;
}

vect quadrocopter::get_torque_corrected()
{
    return(torque_corrected);
}

vect quadrocopter::get_gyroscope_readings()
{
    return(gyroscope_readings);
}

vect quadrocopter::get_accelerometer_readings()
{
    return(accelerometer_readings);
}

vect quadrocopter::get_angle()
{
    return(angle);
}

number_vect_t quadrocopter::get_motor_power(int i)
{
    return(MOTORS[i]);
}

int quadrocopter::get_motors_n()
{
    return(MOTORS_N);
}

void quadrocopter::set_PID_angle_Kp_x(number_vect_t t)
{
    PID_angle_Kp.x = t;
}

void quadrocopter::set_PID_angle_Ki_x(number_vect_t t)
{
    PID_angle_Ki.x = t;
}

void quadrocopter::set_PID_angle_Kd_x(number_vect_t t)
{
    PID_angle_Kd.x = t;
}

void quadrocopter::set_PID_angle_Kp_y(number_vect_t t)
{
    PID_angle_Kp.y = t;
}

void quadrocopter::set_PID_angle_Ki_y(number_vect_t t)
{
    PID_angle_Ki.y = t;
}

void quadrocopter::set_PID_angle_Kd_y(number_vect_t t)
{
    PID_angle_Kd.y = t;
}

void quadrocopter::set_PID_angle_Kp_z(number_vect_t t)
{
    PID_angle_Kp.z = t;
}

void quadrocopter::set_PID_angle_Ki_z(number_vect_t t)
{
    PID_angle_Ki.z = t;
}

void quadrocopter::set_PID_angle_Kd_z(number_vect_t t)
{
    PID_angle_Kd.z = t;
}

void quadrocopter::set_PID_angle_MAXp_x(number_vect_t t)
{
    PID_angle_MAXp.x = t;
}

void quadrocopter::set_PID_angle_MAXi_x(number_vect_t t)
{
    PID_angle_MAXi.x = t;
}

void quadrocopter::set_PID_angle_MAXd_x(number_vect_t t)
{
    PID_angle_MAXd.x = t;
}

void quadrocopter::set_PID_angle_MAXp_y(number_vect_t t)
{
    PID_angle_MAXp.y = t;
}

void quadrocopter::set_PID_angle_MAXi_y(number_vect_t t)
{
    PID_angle_MAXi.y = t;
}

void quadrocopter::set_PID_angle_MAXd_y(number_vect_t t)
{
    PID_angle_MAXd.y = t;
}

void quadrocopter::set_PID_angle_MAXp_z(number_vect_t t)
{
    PID_angle_MAXp.z = t;
}

void quadrocopter::set_PID_angle_MAXi_z(number_vect_t t)
{
    PID_angle_MAXi.z = t;
}

void quadrocopter::set_PID_angle_MAXd_z(number_vect_t t)
{
    PID_angle_MAXd.z = t;
}

vect quadrocopter::get_PID_angle_Kp()
{
    return(PID_angle_Kp);
}

vect quadrocopter::get_PID_angle_Ki()
{
    return(PID_angle_Ki);
}

vect quadrocopter::get_PID_angle_Kd()
{
    return(PID_angle_Kd);
}

vect quadrocopter::get_torque_manual_correction()
{
    return(torque_manual_correction);
}

number_vect_t quadrocopter::get_power()
{
    return(power);
}

number_vect_t quadrocopter::get_write_time()
{
    return(write_time);
}

number_vect_t quadrocopter::get_loop_time()
{
    return(loop_time);
}

number_vect_t quadrocopter::get_voltage()
{
    return(voltage);
}

number_vect_t quadrocopter::get_voltage_percent()
{
    if(voltage <= voltage_min) return 0;
    else if(voltage >= voltage_max) return 100;
    else return((voltage - voltage_min) / (voltage_max - voltage_min) * 100);
}

quadrocopter::reaction_type_ quadrocopter::get_reaction_type()
{
    return(reaction_type);
}

void quadrocopter::set_reaction_type(quadrocopter::reaction_type_ n_reaction_type)
{
    reaction_type = n_reaction_type;
}


vect quadrocopter::get_PID_angle_MAXp()
{
    return(PID_angle_MAXp);
}

vect quadrocopter::get_PID_angle_MAXi()
{
    return(PID_angle_MAXi);
}

vect quadrocopter::get_PID_angle_MAXd()
{
    return(PID_angle_MAXd);
}
