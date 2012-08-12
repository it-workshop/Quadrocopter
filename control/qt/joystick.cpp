#include "joystick.h"

joystick::joystick()
{
    tty_fd = -1;
    maxwait = 500;
    rate = 9600;

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

void joystick::read_data()
{
    if(!isconnected() || read_error()) return;

    flush();

    int t_high, t_low, t_int;

    swrite('r');

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
}

void joystick::disconnect()
{
    defaults();

    sclose();

    connected = false;
}

void joystick::connect()
{
    defaults();

    sopen();

    connected = true;
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
    read_data();
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
