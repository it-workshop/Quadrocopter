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

    connect(&timer_auto, SIGNAL(timeout()), this, SLOT(timer_auto_update()));
    connect(&timer_reconnect, SIGNAL(timeout()), this, SLOT(timer_reconnect_update()));

    ui->LogSave_data->setChecked(true);
    save_filename = "../log/quadro_";
    save_filename.append(t_time.get_time());
    save_filename.append(".txt");

    ui->quadro_reconnect->setChecked(false);
    ui->joy_reconnect->setChecked(false);

    ui->joystick_device->setText(joy.get_device().c_str());
    ui->device->setEditText(quadro.get_device().c_str());

    save_open();

    joy_disconnect();
    quadro_disconnect();

    timer_reconnect.start(timer_reconnect_interval);
    timer_auto.start(timer_auto_interval);
}

Quadro::~Quadro()
{
    delete ui;
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
    if(ui->LogSave_data->isChecked())
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
             << ui->JoystickUse->isChecked() << "\t"
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

        t_ss.precision(6);
        t_ss << quadro.get_read_time() << "\t"
             << quadro.get_write_time() << "\t"
             << quadro.get_loop_time();

        //cout << t_ss.str() << endl;
        save_file << t_ss.str() << endl;
    }
}

void Quadro::set_quadro_data()
{
    number_vect_t t_power;
    vect t_rotation;

    if(joy.isconnected() && ui->JoystickUse->isChecked())
    {
        t_rotation = joy.get_readings();

        t_power = joy.get_power_value();
        if(!joy.is_switched_on()) t_power = 0;
    }
    else
    {
        t_rotation = vect(ui->anglex->value(), ui->angley->value(), 0);

        t_power = ui->power->value() / 10.;
    }

    quadro.set_power(t_power);
    quadro.set_reaction_type((quadrocopter::reaction_type_) ui->reaction_type->currentIndex());
    quadro.set_throttle_rotation(t_rotation);
}

void Quadro::quadro_disconnect()
{
    quadro.disconnect();

    interface_write();
}

void Quadro::joy_disconnect()
{
    ui->power->setValue(0);

    joy.disconnect();

    interface_write();
}

void Quadro::joy_connect()
{
    joy.read_error_reset();
    joy.connect();

    if(joy.isconnected())
    {
        joy.read_data();
        joy.set_data_default();

        interface_write();
    }
}

void Quadro::quadro_connect()
{
    quadro.read_error_reset();
    quadro.connect();

    interface_write();
}

void Quadro::timer_reconnect_update()
{
    static bool allowed = true;
    if(allowed)
    {
        allowed = false;
        if(!quadro.isconnected() && ui->quadro_reconnect->isChecked())
        {
            quadro.read_error_reset();
            quadro_connect();
            interface_write();
        }
        if(!joy.isconnected() && ui->joy_reconnect->isChecked())
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

        quadro.read_data();
        joy.read_data();

        set_quadro_data();
        quadro.write_data();

        interface_write();

        allowed = true;
    }
}

void Quadro::save_close()
{
    save_file.close();
}

void Quadro::save_open()
{
    if(ui->LogSave_data->isChecked())
    {
        save_file.open(save_filename.c_str(), std::ios_base::app);
        save_file << "#seconds\t\tdatetime\t\tgyro_x\tgyro_y\tgyro_z\tacc_x\tacc_y\tacc_z\treact._t\tangle_x\tangle_y\t"
                  << "gyr_c_x\tgyr_c_y\tacc_c_x\tacc_c_y\tacc_c_z\tj_conn\tj_use\tj_x\tj_y\tj_power\tj_switch\tquad_conn\trot_x\trot_y\tthr_x\t"
                  << "thr_y\tthr_z\tpower\tM_A\tM_B\tM_C\tM_D\tread_t\twrite_t\tloop_t" << endl;
    }
}


void Quadro::on_actionQuadroConnect_triggered()
{
    quadro_connect();
}

void Quadro::on_actionQuadroDisconnect_triggered()
{
    ui->quadro_reconnect->setChecked(false);
    quadro_disconnect();
}

void Quadro::on_actionQuadroReset_triggered()
{
    quadro.reset_throttle();
}

void Quadro::on_actionJoystickConnect_triggered()
{
    joy_connect();
}

void Quadro::on_actionJoystickDisconnect_triggered()
{
    ui->joy_reconnect->setChecked(false);
    joy_disconnect();
}

void Quadro::on_actionJoystickCalibrate_zero_triggered()
{
    if(joy.isconnected())
    {
        joy.read_data();
        joy.set_data_default();
    }
}

void Quadro::on_device_editTextChanged(const QString &arg1)
{
    quadro.set_device(arg1.toAscii().data());
}

void Quadro::on_joystick_device_textChanged(const QString &arg1)
{
    joy.set_device(arg1.toAscii().data());
}

void Quadro::on_reaction_type_currentIndexChanged(int index)
{
    quadro.set_reaction_type((quadrocopter::reaction_type_) index);
}

void Quadro::on_JoystickUse_toggled()
{
    if(joy.isconnected())
    {
        joy.set_data_default();

        ui->power->setValue(0);
    }
}
