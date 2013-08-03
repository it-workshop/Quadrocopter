#include "joystick.h"
#include <QDebug>
#include <iostream>

using std::cerr;
using std::endl;

joystick::joystick()
{
    maxwait = 500;
    rate = 9600;

    device = "ttyUSB0";

    connectDelayTime = 500;

    readBytesN = 7;

    defaults();
}

void joystick::defaults()
{
    data_default.x = 0;
    data_default.y = 0;
    data.x = 0;
    data.y = 0;
    power_switch = false;
    power_value = 0;
}

void joystick::initiate_transmission()
{
    if(!isoperational() || readError()) return;

    flush();

    swriteClear();
    swrite('r');
    swritePut();
}

void joystick::read_data()
{
    int t_high, t_low, t_int;

    vect t_vect;

    for(int i = 0; i < 2; i++) // 2 - axis count
    {
        t_high = sread();
        t_low = sread();
        t_int = (t_low & 0xff) + (t_high << 8);

        t_vect.value_by_axis_index(i) = t_int;
    }

    t_high = sread();
    t_low = sread();

    bool t_bool = sread();

    if(!readError())
    {
        data = t_vect;
        power_value = ((t_low & 0xff) + (t_high << 8));
        power_switch = t_bool;
    }
    else qDebug() << "error";
}

void joystick::do_disconnect()
{
    defaults();

    sclose();
}

void joystick::do_connect()
{
    defaults();

    sopen();
}

vect joystick::get_readings()
{
    vect data_formatted = data - data_default;

    for(int i = 0; i < 2; i++) // 2 - axis count
    {
        if(data_formatted.value_by_axis_index(i) > 0)
            data_formatted.value_by_axis_index(i) /= (MAX_VALUE - data_default.value_by_axis_index(i));
        else if(data_formatted.value_by_axis_index(i) < 0)
            data_formatted.value_by_axis_index(i) /= (data_default.value_by_axis_index(i) - MIN_VALUE);
    }

    //data_formatted.y *= -1;

    data_formatted *= 0.5;

    return(data_formatted);
}

void joystick::set_data_default()
{
    data_default = data;

    for(int i = 0; i < 2; i++) // 2 - axis count
    {
        if(data_default.value_by_axis_index(i) <= MIN_VALUE)
            data_default.value_by_axis_index(i) = MIN_VALUE + 1;

        else if(data_default.value_by_axis_index(i) >= MAX_VALUE)
            data_default.value_by_axis_index(i) = MAX_VALUE - 1;
    }
}

bool joystick::is_switched_on()
{
    return(power_switch);
}

number_vect_t joystick::get_power_value()
{
    number_vect_t x = get_power_value_raw(), y;
    if(x >= 0 && x <= 0.2)
    {
        y = x * 40 / 0.2;
    }
    else if(x >= 0.2 && x <= 0.8)
    {
        y = 40 + (x - 0.2) * 35 / 0.6;
    }
    else
    {
        y = 75 + (x - 0.8) * 25 / 0.2;
    }

    y /= 100;

    return(y);
}

number_vect_t joystick::get_power_value_raw()
{
    return(power_value / (MAX_POWER_VALUE * 1.0));
}

void joystick::on_rx()
{
    //qDebug() << "available: " << port->bytesAvailable();

    while(port->bytesAvailable() >= readBytesN)
    {
        //qDebug() << "calling read_data()";
        read_data();
    }
}
