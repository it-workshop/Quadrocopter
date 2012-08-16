#include "quadrocopter.h"
#include <math.h>
#include <iostream>
#include <mytime.h>
#include <vect.h>

using std::cerr;
using std::endl;

quadrocopter::quadrocopter()
{
    tty_fd = -1;
    rate = 115200;
    maxwait = 500;

    defaults();
}

void quadrocopter::read_data()
{
    mytime t_time;
    t_time.set_time();

    if(!isconnected() || read_error()) return;

    flush();

    swrite('p');

    vect t_throttle_corrected = read_vect_byte(), t_angle = read_vect_byte(2),
            t_gyroscope_readings = read_vect_byte(), t_accelerometer_readings = read_vect_byte(),
            t_throttle_gyroscope_correction = read_vect_byte(2), t_throttle_accelerometer_correction = read_vect_byte(3);

    number_vect_t t_motors[MOTORS_N], t_loop_time;

    for(int i = 0; i < MOTORS_N; i++)
        t_motors[i] = sread();

    t_loop_time = read_unsigned_int_3byte() / 1.E6;

    if(!read_error())
    {
        for(int i = 0; i < MOTORS_N; i++)
            MOTORS[i] = t_motors[i];

        throttle_corrected = t_throttle_corrected;
        angle = t_angle;
        gyroscope_readings = t_gyroscope_readings / serial_gyroscope_coefficient;
        accelerometer_readings = t_accelerometer_readings;
        throttle_gyroscope_correction = t_throttle_gyroscope_correction;
        throttle_accelerometer_correction = t_throttle_accelerometer_correction;
        loop_time = t_loop_time;
    }


    read_time = t_time.get_time_difference() / 1.E3;

    /*mytime t_time, t_curr_time;
    t_time.set_time();

    const unsigned int N = 19;
    char* a = new char[N + 1];
    for(unsigned int i = 0; i <= N; i++) a[i] = 0;

    cerr << "started" << endl;

    for(unsigned int i = 0; i < N; i++)
    {
        t_curr_time.set_time();
        while(read(tty_fd, &(a[i]), 1) <= 0) usleep(1000);
        cerr << " got [" << a[i] << "] time=" << t_curr_time.get_time_difference() << endl;
    }

    cerr << "str=" << a << " time=" << t_time.get_time_difference() << endl;*/
}

void quadrocopter::write_data()
{
    mytime t_time;
    t_time.set_time();

    if(!isconnected() || read_error()) return;

    flush();

    if(power > 1) power = 1;
    else if(power < 0) power = 0;

    swrite('m');
    swrite(power * 100); // in percents

    swrite('i');

    unsigned int t_int, t_high, t_low;

    for(int i = 0; i < 2; i++) // 2 - axis count
    {
        t_int = fabs((throttle_rotation.value_by_axis_index(i) + 1) * 65535 / 2);
        t_high = t_int >> 8;
        t_low = t_int & 0xff;

        swrite(t_high);
        swrite(t_low);
    }

    write_time = t_time.get_time_difference() / 1.E3;
}

void quadrocopter::defaults()
{
    throttle_rotation = vect();
    throttle_corrected = vect();
    gyroscope_readings = vect();
    accelerometer_readings = vect();
    throttle_gyroscope_correction = vect();
    throttle_accelerometer_correction = vect();

    angle = vect();
    power = 0;
    for(int i = 0; i < get_motors_n(); i++) MOTORS[i] = 0;

    read_time = 0;
    write_time = 0;
    loop_time = 0;
}

void quadrocopter::connect()
{
    defaults();

    sopen();
    connected = true;
}

void quadrocopter::disconnect()
{
    defaults();
    power = 0;

    write_data();

    sclose();

    connected = false;
    read_error_reset();
}

number_vect_t quadrocopter::get_read_time()
{
    return(read_time);
}

void quadrocopter::reset_throttle()
{
    throttle_rotation.x = 0;
    throttle_rotation.y = 0;

    swrite('n');

    /*if(isconnected())
        write_data();*/
}


vect quadrocopter::get_throttle_corrected()
{
    return(throttle_corrected);
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

vect quadrocopter::get_throttle_gyroscope_correction()
{
    return(throttle_gyroscope_correction);
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

void quadrocopter::set_throttle_rotation(vect n_throttle_rotation)
{
    throttle_rotation = n_throttle_rotation;
}

vect quadrocopter::get_throttle_rotation()
{
    return(throttle_rotation);
}

number_vect_t quadrocopter::get_power()
{
    return(power);
}

vect quadrocopter::get_throttle_accelerometer_correction()
{
    return(throttle_accelerometer_correction);
}

number_vect_t quadrocopter::get_write_time()
{
    return(write_time);
}

number_vect_t quadrocopter::get_loop_time()
{
    return(loop_time);
}
