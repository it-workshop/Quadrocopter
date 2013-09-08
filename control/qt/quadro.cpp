#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>

#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>

#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QtCore/QDebug>

using std::endl;

using std::stringstream;
using std::string;

Quadro::Quadro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Quadro)
{
    ui->setupUi(this);

    connect(&timer_auto, SIGNAL(timeout()), this, SLOT(timer_auto_update()));
    connect(&timer_reconnect, SIGNAL(timeout()), this, SLOT(timer_reconnect_update()));
    connect(&timer_log, SIGNAL(timeout()), this, SLOT(timer_log_update()));

    //only works on Windows and OSX
    /*QeSEnumerator.setUpNotifications();

    connect(&QeSEnumerator, SIGNAL(deviceDiscovered()), this, SLOT(update_ports()));
    connect(&QeSEnumerator, SIGNAL(deviceRemoved()), this, SLOT(update_ports()));*/

    interface_init();
    update_ports();
    save_open();

    joy_disconnect();
    quadro_disconnect();

    timer_reconnect.start(timer_reconnect_interval);
    timer_auto.start(timer_auto_interval);

    quadro_save_settings = true;

    plot_init();
}

Quadro::~Quadro()
{
    delete ui;
}

void Quadro::set_quadro_data()
{
    number_vect_t t_power;
    vect t_correction;

    t_correction = vect(ui->torque_manual_correction_x->value(), ui->torque_manual_correction_y->value(), 0);

    if(joy.isoperational() && ui->joystick_use->isChecked())
    {
        t_correction += joy.get_readings() * quadro.get_joystick_coefficient();

        t_power = joy.get_power_value();
        if(!joy.is_switched_on()) t_power = 0;

        quadro.set_joystick_heading(joy.get_heading());
    }
    else
    {
        t_power = ui->power->value();
        quadro.set_joystick_heading(ui->torque_manual_correction_z->value() / 180. * M_PI);
    }

    quadro.set_power(t_power);
    quadro.set_reaction_type((quadrocopter::reaction_type_) ui->reaction_type->currentIndex());
    quadro.set_torque_manual_correction(t_correction);
}

void Quadro::quadro_disconnect()
{
    quadro.do_disconnect();

    interface_write();
}

void Quadro::joy_disconnect()
{
    ui->power->setValue(0);

    joy.do_disconnect();

    interface_write();
}

void Quadro::joy_connect()
{
    joy.readErrorReset();
    joy.do_connect();

    if(joy.isoperational())
    {
        joy.initiate_transmission();
        joy.set_data_default();

        interface_write();
    }
}

void Quadro::quadro_fetch_data()
{
    angular_velocity = quadro.get_gyroscope_readings();
    angle = quadro.get_angle();
    angle.z = quadro.get_copter_heading();
    /*vect angular_velocity, angle, acceleration, torque, power, voltage, PID_P, PID_I, PID_D;
    vect joystick_readings, correction, joystick_power, joystick_heading, joystick_power;
    int M1, M2, M3, M4;
    double read_time, write_time, loop_time;*/
    torque = quadro.get_torque_corrected();
    power = quadro.get_power();
    voltage = quadro.get_voltage();
    PID_P = quadro.get_PID_P();
    PID_I = quadro.get_PID_I();
    PID_D = quadro.get_PID_D();

    for(int i = 0; i < quadro.get_motors_n(); i++)
        M[i] = quadro.get_motor_power(i);

    read_time = quadro.get_read_time();
    write_time = quadro.get_write_time();
    loop_time = quadro.get_loop_time();
}

void Quadro::joy_fetch_data()
{
    joystick_readings = joy.get_readings();
    joystick_power = joy.get_power_value();
    joystick_heading = joy.get_heading();
}

void Quadro::quadro_connect()
{
    plot_reset_data();
    plot_mytime.reset();

    quadro.readErrorReset();
    quadro.do_connect();

    interface_write();
}

void Quadro::timer_reconnect_update()
{
    update_ports();

    static bool allowed = true;
    if(allowed)
    {
        allowed = false;
        if(!quadro.isoperational() && ui->quadro_reconnect->isChecked())
        {
            quadro.readErrorReset();
            quadro_connect();
            interface_write();
        }
        if(!joy.isoperational() && ui->joy_reconnect->isChecked())
        {
            joy.readErrorReset();
            joy_connect();
            interface_write();
        }
        allowed = true;
    }
}

void Quadro::timer_auto_update()
{
    static bool allowed = true;

    if(allowed)
    {
        allowed = false;

        if(quadro.readError())
        {
            quadro_disconnect();
            quadro.readErrorReset();
        }
        if(joy.readError())
        {
            joy_disconnect();
            joy.readErrorReset();
        }

        save_data();

        set_quadro_data();

        if(quadro.iswriteable() && ui->quadro_autoupdate->isChecked())
            quadro.initiate_transmission();

        if(joy.isoperational())
            joy.initiate_transmission();

        interface_write();

        if(quadro.getNewDataAvailable())
        {
            if(!plot_mytime.isSet()) plot_mytime.setTime();
            else
            {
                plot_update();
                joy_fetch_data();
                quadro_fetch_data();
            }
            quadro.resetNewDataAvailable();
        }

        allowed = true;
    }
}
