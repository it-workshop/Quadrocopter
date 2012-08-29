#include "joystick.h"
#include <QDebug>

joystick::joystick()
{
    maxwait = 500;
    rate = 9600;

    device = "ttyUSB0";

    connect_delay_time = 500;

    read_bytes_N = 7;

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

void joystick::read_data_request()
{
    if(!isoperational() || read_error()) return;

    flush();

    swrite_clear();
    swrite('r');
    swrite_put();
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

    if(!read_error())
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

    data_formatted.y *= -1;

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
    return(power_value / (MAX_POWER_VALUE * 1.0));
}

void joystick::on_rx()
{
    //qDebug() << "available: " << port->bytesAvailable();

    while(port->bytesAvailable() >= read_bytes_N)
    {
        //qDebug() << "calling read_data()";
        read_data();
    }
}
