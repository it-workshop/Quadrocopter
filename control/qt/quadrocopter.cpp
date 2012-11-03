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

    connect_delay_time = 500;

    //connect_delay_arduino = 9000;
    connect_delay_arduino = 2000;

    PID_angle_Kp = 0.16;
    PID_angle_Ki = 0;
    PID_angle_Kd = 0.03;

    PID_angular_velocity_Kp = 0.08;
    PID_angular_velocity_Ki = 0;
    PID_angular_velocity_Kd = 0;

    //see arduino code
    read_bytes_N = 38;

    joystick_coefficient = 0.5;

    defaults();
}

void quadrocopter::read_data_request()
{
    if(!isoperational() || read_error()) return;

    mytime write_timer;
    write_timer.set_time();

    flush();
    swrite_clear();

    swrite('p');
    write_data();

    swrite_put();

    write_time = write_timer.get_time_difference() / 1.E3;

    read_timer.set_time();
}

void quadrocopter::read_data()
{
    vect t_torque_corrected = read_vect_byte(), t_angle = read_vect_byte(2),
            t_gyroscope_readings = read_vect_byte(), t_accelerometer_readings = read_vect_byte(),
            t_torque_correction = read_vect_byte();

    reaction_type_ t_reaction_type;

    number_vect_t t_motors[MOTORS_N], t_loop_time;

    for(int i = 0; i < MOTORS_N; i++)
        t_motors[i] = sread();

    t_loop_time = read_unsigned_int_3byte() / 1.E6;

    t_reaction_type = (reaction_type_) (sread() - '0');

    number_vect_t t_voltage = read_number_vect_t(0, 20, 2);

    if(!read_error())
    {
        for(int i = 0; i < MOTORS_N; i++)
            MOTORS[i] = t_motors[i];

        torque_corrected = t_torque_corrected;
        angle = t_angle;
        gyroscope_readings = t_gyroscope_readings;
        accelerometer_readings = t_accelerometer_readings * g;

        if(t_reaction_type == REACTION_ANGULAR_VELOCITY)
            torque_angular_velocity_correction = t_torque_correction;
        else if(t_reaction_type == REACTION_ACCELERATION)
            torque_acceleration_correction = t_torque_correction;
        else if(t_reaction_type == REACTION_ANGLE)
            torque_angle_correction = t_torque_correction;

        loop_time = t_loop_time;
        reaction_type = t_reaction_type;

        voltage = t_voltage;

        read_time = read_timer.get_time_difference() / 1.E3;
    }
}

void quadrocopter::write_data()
{
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

    write_number_vect_t(-1.5, 1.5, PID_angular_velocity_Kp, 2);
    write_number_vect_t(-1.5, 1.5, PID_angular_velocity_Ki, 2);
    write_number_vect_t(-1.5, 1.5, PID_angular_velocity_Kd, 2);

    write_number_vect_t(0, 100, gyro_period, 2);
    write_number_vect_t(0, 100, accel_period, 2);
    write_number_vect_t(0, 100, angle_period, 2);
}

void quadrocopter::defaults()
{
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
    connect_delay_time = !device.substr(0, 6).compare("ttyACM") ? connect_delay_arduino : 500;
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

    read_error_reset();
}

number_vect_t quadrocopter::get_read_time()
{
    return(read_time);
}

void quadrocopter::reset()
{
    torque_manual_correction.x = 0;
    torque_manual_correction.y = 0;

    swrite_clear();
    swrite('n');
    swrite_put();
}

void quadrocopter::set_gyro_period(double n_period)
{
    gyro_period = n_period;
}

void quadrocopter::set_accel_period(double n_period)
{
    accel_period = n_period;
}

void quadrocopter::set_angle_period(double n_period)
{
    angle_period = n_period;
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

void quadrocopter::on_rx()
{
    //qDebug() << "available: " << port->bytesAvailable();

    while(port->bytesAvailable() >= read_bytes_N)
    {
        //qDebug() << "calling read_data()";
        read_data();
    }
}
