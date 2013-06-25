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

    PID_angular_velocity_Kp = 0;
    PID_angular_velocity_Ki = 0;
    PID_angular_velocity_Kd = 0;

    //see arduino code
#ifdef DEBUG_NO_TX_ARDUINO
    readBytesN = 1;
#else
    readBytesN = 38;
#endif
    //readBytesN = BN;

    joystick_coefficient = 0.5;

    accel_period = 0;
    angle_period = 0;

    busyBit = false;

    defaults();
}



void quadrocopter::defaults()
{
    busyBit = false;
    torque_manual_correction = vect();
    torque_corrected = vect();
    gyroscope_readings = vect();
    accelerometer_readings = vect();
    torque_angular_velocity_correction = vect();
    torque_acceleration_correction = vect();

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

void quadrocopter::set_accel_period(double n_period)
{
    accel_period = n_period;
}

void quadrocopter::set_angle_period(double n_period)
{
    angle_period = n_period;
}

double quadrocopter::get_accel_period()
{
    return(accel_period);
}

double quadrocopter::get_angle_period()
{
    return(angle_period);
}

number_vect_t quadrocopter::get_PID_P()
{
    return(PID_P);
}

number_vect_t quadrocopter::get_PID_I()
{
    return(PID_I);
}

number_vect_t quadrocopter::get_PID_D()
{
    return(PID_D);
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

vect quadrocopter::get_torque_angular_velocity_correction()
{
    return(torque_angular_velocity_correction);
}

number_vect_t quadrocopter::get_motor_power(int i)
{
    return(MOTORS[i]);
}

int quadrocopter::get_motors_n()
{
    return(MOTORS_N);
}

void quadrocopter::set_power(number_vect_t n_power)
{
    if(n_power > 1) n_power = 1;
    else if(n_power < 0) n_power = 0;
    power = n_power;
}

void quadrocopter::set_joystick_correction(vect n_joystick_correction)
{
    set_torque_manual_correction(n_joystick_correction * joystick_coefficient);
}

void quadrocopter::set_torque_manual_correction(vect n_torque_manual_correction)
{
    torque_manual_correction = n_torque_manual_correction;
}

void quadrocopter::set_PID_angle_Kp(number_vect_t t_Kp)
{
    PID_angle_Kp = t_Kp;
}

void quadrocopter::set_PID_angle_Ki(number_vect_t t_Ki)
{
    PID_angle_Ki = t_Ki;
}

void quadrocopter::set_PID_angle_Kd(number_vect_t t_Kd)
{
    PID_angle_Kd = t_Kd;
}

void quadrocopter::set_PID_angle_MAXp(number_vect_t arg)
{
    PID_angle_MAXp = arg;
}

void quadrocopter::set_PID_angle_MAXi(number_vect_t arg)
{
    PID_angle_MAXi = arg;
}

void quadrocopter::set_PID_angle_MAXd(number_vect_t arg)
{
    PID_angle_MAXd = arg;
}

void quadrocopter::set_PID_angular_velocity_Kp(number_vect_t t_Kp)
{
    PID_angular_velocity_Kp = t_Kp;
}

void quadrocopter::set_PID_angular_velocity_Ki(number_vect_t t_Ki)
{
    PID_angular_velocity_Ki = t_Ki;
}

void quadrocopter::set_PID_angular_velocity_Kd(number_vect_t t_Kd)
{
    PID_angular_velocity_Kd = t_Kd;
}

number_vect_t quadrocopter::get_PID_angle_Kp()
{
    return(PID_angle_Kp);
}

number_vect_t quadrocopter::get_PID_angle_Ki()
{
    return(PID_angle_Ki);
}

number_vect_t quadrocopter::get_PID_angle_Kd()
{
    return(PID_angle_Kd);
}

number_vect_t quadrocopter::get_PID_angle_MAXp()
{
    return(PID_angle_MAXp);
}

number_vect_t quadrocopter::get_PID_angle_MAXi()
{
    return(PID_angle_MAXi);
}

number_vect_t quadrocopter::get_PID_angle_MAXd()
{
    return(PID_angle_MAXd);
}

number_vect_t quadrocopter::get_PID_angular_velocity_Kp()
{
    return(PID_angular_velocity_Kp);
}

number_vect_t quadrocopter::get_PID_angular_velocity_Ki()
{
    return(PID_angular_velocity_Ki);
}

number_vect_t quadrocopter::get_PID_angular_velocity_Kd()
{
    return(PID_angular_velocity_Kd);
}

vect quadrocopter::get_torque_manual_correction()
{
    return(torque_manual_correction);
}

number_vect_t quadrocopter::get_power()
{
    return(power);
}

vect quadrocopter::get_torque_acceleration_correction()
{
    return(torque_acceleration_correction);
}

vect quadrocopter::get_torque_angle_correction()
{
    return(torque_angle_correction);
}

vect quadrocopter::get_torque_automatic_correction()
{
    switch(reaction_type)
    {
        case REACTION_ANGULAR_VELOCITY:
            return(get_torque_angular_velocity_correction());
            break;
        case REACTION_ACCELERATION:
            return(get_torque_acceleration_correction());
            break;
        case REACTION_ANGLE:
            return(get_torque_angle_correction());
            break;
        default:
            return(vect());
            break;
    }
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
