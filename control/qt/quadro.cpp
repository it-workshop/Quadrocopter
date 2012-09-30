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

    if(joy.isoperational() && ui->JoystickUse->isChecked())
    {
        t_correction = joy.get_readings();

        t_power = joy.get_power_value();
        if(!joy.is_switched_on()) t_power = 0;
    }
    else
    {
        t_correction = vect(ui->torque_manual_correction_x->value(), ui->torque_manual_correction_y->value(), 0);

        t_power = ui->power->value() / 10.;
    }

    quadro.set_power(t_power);
    quadro.set_reaction_type((quadrocopter::reaction_type_) ui->reaction_type->currentIndex());
    quadro.set_joystick_correction(t_correction);
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
    joy.read_error_reset();
    joy.do_connect();

    if(joy.isoperational())
    {
        joy.read_data_request();
        joy.set_data_default();

        interface_write();
    }
}

void Quadro::quadro_connect()
{
    plot_reset_data();
    plot_mytime.reset();

    quadro.read_error_reset();
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
            quadro.read_error_reset();
            quadro_connect();
            interface_write();
        }
        if(!joy.isoperational() && ui->joy_reconnect->isChecked())
        {
            joy.read_error_reset();
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

        if(quadro.read_error())
        {
            quadro_disconnect();
            quadro.read_error_reset();
        }
        if(joy.read_error())
        {
            joy_disconnect();
            joy.read_error_reset();
        }

        save_data();

        set_quadro_data();

        if(quadro.iswriteable())
            quadro.read_data_request();

        if(joy.isoperational())
            joy.read_data_request();

        interface_write();

        if(quadro.isoperational())
        {
            if(!plot_mytime.is_set()) plot_mytime.set_time();
            else plot_update();
        }

        allowed = true;
    }
}
