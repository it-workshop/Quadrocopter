#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>

using std::cout;
using std::cerr;
using std::endl;

/*
  Protocol description (PC <=> DEVICE):
   => 'p' to request status:
    <= throttle x
    <= throttle y
    <= throttle z
    <= gyro x
    <= gyro y
    <= gyro z
    <= motor A
    <= motor B
    <= motor C
    <= motor D
  */

using std::stringstream;
using std::string;

Quadro::Quadro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Quadro)
{
    mytime t_time;

    ui->setupUi(this);

    interface_read();

    connect(&timer_auto, SIGNAL(timeout()), this, SLOT(timer_auto_update()));
    connect(&timer_reconnect, SIGNAL(timeout()), this, SLOT(timer_reconnect_update()));

    save = true;
    save_filename = "../log/quadro_";
    save_filename.append(t_time.get_time());
    save_filename.append(".txt");
    ui->save_filename->setText(save_filename.c_str());

    mode_auto = false;

    quadro_reconnect = false;
    joy_reconnect = false;

    save_open();

    joy_disconnect();
    quadro_disconnect();

    timer_reconnect.start(timer_reconnect_interval);
}

Quadro::~Quadro()
{
    delete ui;
}

void Quadro::interface_read()
{
    stringstream t_ss;
    int t_int;

    quadro.set_device(ui->device->currentText().toAscii().data());
    joy.set_device(ui->joystick_device->text().toAscii().data());

    t_ss << (ui->speed->currentText().toAscii().data());
    t_ss >> t_int;
    quadro.set_rate(t_int);

    power_interface = ui->power->value() / 10.;

    joystick_use = ui->joystick_use->isChecked();

    save = ui->save->isChecked();
    save_filename = ui->save_filename->text().toAscii().data();

    quadro_reconnect = ui->quadro_reconnect->isChecked();
    joy_reconnect = ui->joy_reconnect->isChecked();

    quadro.set_reaction_type((quadrocopter::reaction_type_) ui->reaction_type->currentIndex());
}

void Quadro::interface_write()
{
    stringstream t_ss, t_ss1, t_ss2;
    if(quadro.isconnected())
    {
        ui->throttle->setText(quadro.get_throttle_corrected().print().c_str());
        ui->gyro->setText(quadro.get_gyroscope_readings().print().c_str());
        ui->accel->setText(quadro.get_accelerometer_readings().print().c_str());
        ui->angle->setText(quadro.get_angle().print2d().c_str());
        ui->throttle_accelerometer_correction->setText(quadro.get_throttle_accelerometer_correction().print().c_str());
        ui->throttle_gyroscope_correction->setText(quadro.get_throttle_gyroscope_correction().print2d().c_str());

        t_ss2 << quadro.get_read_time() * 1E3 << " ms / " << quadro.get_write_time() * 1E3 << " ms / "
              << quadro.get_loop_time() * 1E6 << " us";
        ui->readwrite_time->setText(t_ss2.str().c_str());

        t_ss << "(";

        for(int i = 0; i < quadro.get_motors_n(); i++)
        {
            t_ss << quadro.get_motor_power(i);
            if(i != quadro.get_motors_n() - 1) t_ss << "\t";
        }

        t_ss << ")";

        ui->motors->setText(t_ss.str().c_str());

        ui->reaction_type->setCurrentIndex(quadro.get_reaction_type());

    }

    if(joy.isconnected())
    {
        t_ss1 << (joy.is_switched_on() ? "online" : "offline") << "\t";
        t_ss1 << "p=" << joy.get_power_value() << "\t";
        t_ss1 << joy.get_readings().print2d();

        ui->throttle_joystick->clear();
        ui->throttle_joystick->setText(t_ss1.str().c_str());
    }

    ui->quadro_reconnect->setChecked(quadro_reconnect);
    ui->joy_reconnect->setChecked(joy_reconnect);

    if(joy.isconnected())
    {
        ui->joystick_device->setAutoFillBackground(true);
        ui->joystick_device->setStyleSheet("background-color: rgb(100, 255, 100);");
        ui->joystick_device->setReadOnly(1);
    }
    else
    {
        ui->joystick_device->setReadOnly(0);
        ui->joystick_device->setAutoFillBackground(true);
        ui->joystick_device->setStyleSheet("background-color: rgb(255, 100, 100);");
    }

    if(quadro.isconnected())
    {
        ui->device->setEditable(0);
        ui->device->setAutoFillBackground(true);
        ui->device->setStyleSheet("background-color: rgb(100, 255, 100);");
    }
    else
    {
        ui->device->setEditable(1);
        ui->device->setAutoFillBackground(true);
        ui->device->setStyleSheet("background-color: rgb(255, 100, 100);");
    }
}

void Quadro::save_data()
{
    if(save)
    {
        stringstream t_ss;
        int i;
        mytime t_time;

        t_ss.precision(2);

        //seconds time
        //gyro[3] accel[3]
        //reaction_type
        //angle[2] gyroscope_correction[2] accelerometer_correction[3]
        //joystick_connected joystick_use joystick_readings[2] joystick_power joystick_power_switch
        //quadro_connected throttle_rotation[2] throttle_corrected[3] power motors[4]
        //mode_auto
        //read_time_sec write_time_sec loop_time_sec
        //*quadro_device@quadro_speed
        //*joystick_device@joystick_speed
        t_ss << t_time.get_seconds() << "\t" << t_time.get_time() << "\t"
             << quadro.get_gyroscope_readings().print_tab() << "\t"
             << quadro.get_accelerometer_readings().print_tab() << "\t"
             << quadro.get_reaction_type() << "\t"
             << quadro.get_angle().print2d_tab() << "\t"

             << quadro.get_throttle_gyroscope_correction().print2d_tab() << "\t"
             << quadro.get_throttle_accelerometer_correction().print_tab() << "\t"

             << joy.isconnected() << "\t"
             << joystick_use << "\t"
             << joy.get_readings().print2d_tab() << "\t"
             << joy.get_power_value() << "\t"
             << joy.is_switched_on() << "\t"

             << quadro.isconnected() << "\t"
             << quadro.get_throttle_rotation().print2d_tab() << "\t"
             << quadro.get_throttle_corrected().print_tab() << "\t"
             << quadro.get_power() << "\t";

        for(i = 0; i < quadro.get_motors_n(); i++)
        {
            t_ss << quadro.get_motor_power(i) << "\t";
            //if(i != quadro.get_motors_n() - 1) t_ss << "\t";
        }

        t_ss << mode_auto << "\t";

        t_ss.precision(6);
        t_ss << quadro.get_read_time() << "\t"
             << quadro.get_write_time() << "\t"
             << quadro.get_loop_time();

        //cout << t_ss.str() << endl;
        save_file << t_ss.str() << endl;
    }
}

void Quadro::on_button_connect_clicked()
{
    interface_read();
    quadro_connect();
}

void Quadro::on_button_disconnect_clicked()
{
    quadro_reconnect = false;
    quadro_disconnect();
}

void Quadro::on_button_auto_clicked()
{
    mode_auto = mode_auto ? 0 : 1;
    set_auto(mode_auto);
}

void Quadro::set_quadro_data()
{
    number_vect_t t_power;

    if(joy.isconnected() && joystick_use)
        quadro.set_throttle_rotation(joy.get_readings());

    if(joy.isconnected() && joystick_use)
    {
        t_power = joy.get_power_value();
        if(!joy.is_switched_on()) t_power = 0;
    }
    else
        t_power = power_interface;

    quadro.set_power(t_power);
    quadro.set_reaction_type((quadrocopter::reaction_type_) ui->reaction_type->currentIndex());
}

void Quadro::set_auto(bool t)
{
    mode_auto = t;
    if(mode_auto)
    {
        ui->label_auto->setText("Enabled");
        timer_auto.start(timer_auto_interval);
    }
    else
    {
        ui->label_auto->setText("Disabled");
        timer_auto.stop();
    }
}

void Quadro::joy_disconnect()
{
    power_interface = 0;
    ui->power->setValue(0);

    joy.disconnect();

    //joy_reconnect = false;

    interface_write();
    save_data();
}

void Quadro::joy_connect()
{
    interface_read();
    joy.read_error_reset();
    joy.connect();

    joy.read_data();
    joy.set_data_default();

    interface_write();
    save_data();
}

void Quadro::quadro_disconnect()
{
    quadro.disconnect();

    save_data();

    //quadro_reconnect = false;

    interface_write();
    save_data();
}

void Quadro::quadro_connect()
{
    quadro.read_error_reset();
    quadro.connect();

    interface_write();
    save_data();
}

void Quadro::timer_reconnect_update()
{
    static bool allowed = true;
    if(allowed)
    {
        allowed = false;
        if(!quadro.isconnected() && quadro_reconnect)
        {
            quadro.read_error_reset();
            quadro_connect();
            interface_write();
        }
        if(!joy.isconnected() && joy_reconnect)
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
            quadro.read_error_reset();
            quadro_disconnect();
        }
        if(joy.read_error())
        {
            joy.read_error_reset();
            joy_disconnect();
        }

        if(mode_auto)
        {
            //mytime t_time;
            //t_time.set_time();

            interface_read();

            save_data();

            quadro.read_data();
            joy.read_data();

            //cerr << "read: " << t_time.get_time_difference() << "\t";
            //t_time.set_time();

            set_quadro_data();
            quadro.write_data();

            interface_write();

            //cerr << "write: " << t_time.get_time_difference() << endl;
        }

        allowed = true;
    }
}

void Quadro::on_button_joystick_disconnect_clicked()
{
    joy_reconnect = false;
    joy_disconnect();
}

void Quadro::on_button_joystick_connect_clicked()
{
    joy_connect();
}

void Quadro::on_joystick_use_clicked()
{
    joy.set_data_default();

    ui->power->setValue(0);
    power_interface = 0;

    save_data();
}

void Quadro::on_reset_clicked()
{
    quadro.reset_throttle();
}

void Quadro::on_button_joystick_read_defaults_clicked()
{
    joy.read_error_reset();
    joy.read_data();
    joy.set_data_default();

    save_data();
}

void Quadro::save_close()
{
    save_file.close();
}

void Quadro::save_open()
{
    if(save)
    {
        save_file.open(save_filename.c_str(), std::ios_base::app);
        save_file << "#seconds\t\tdatetime\t\tgyro_x\tgyro_y\tgyro_z\tacc_x\tacc_y\tacc_z\treact._t\tangle_x\tangle_y\t"
                  << "gyr_c_x\tgyr_c_y\tacc_c_x\tacc_c_y\tacc_c_z\tj_conn\tj_use\tj_x\tj_y\tj_power\tj_switch\tquad_conn\trot_x\trot_y\tthr_x\t"
                  << "thr_y\tthr_z\tpower\tM_A\tM_B\tM_C\tM_D\tauto\tread_t\twrite_t\tloop_t" << endl;
    }
}

void Quadro::on_save_toggled()
{
    interface_read();

    save_close();

    if(save)
        save_open();
}
