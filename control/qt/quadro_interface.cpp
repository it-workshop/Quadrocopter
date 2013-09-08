#include "quadro.h"
#include "ui_quadro.h"
#include "mytime.h"
#include <sstream>
#include <string>

using std::stringstream;
using std::string;

void Quadro::interface_init()
{
    mytime t_time;

    ui->LogSave_data->setChecked(true);
    save_filename = "../log/quadro_";
    save_filename.append(t_time.getTime());
    save_filename.append(".txt");

    settings_filename = "settings.txt";

    settings_read();

    ui->quadro_reconnect->setChecked(false);
    ui->joy_reconnect->setChecked(false);

    ui->joystick_device->setEditText(joy.getDevice().c_str());
    ui->quadro_device->setEditText(quadro.getDevice().c_str());

    ui->quadro_device->setAutoFillBackground(true);
    ui->joystick_device->setAutoFillBackground(true);

    ui->PID_angle_Kp_x->setValue(quadro.get_PID_angle_Kp().x);
    ui->PID_angle_Ki_x->setValue(quadro.get_PID_angle_Ki().x);
    ui->PID_angle_Kd_x->setValue(quadro.get_PID_angle_Kd().x);

    ui->PID_angle_Kp_y->setValue(quadro.get_PID_angle_Kp().y);
    ui->PID_angle_Ki_y->setValue(quadro.get_PID_angle_Ki().y);
    ui->PID_angle_Kd_y->setValue(quadro.get_PID_angle_Kd().y);

    ui->PID_angle_Kp_z->setValue(quadro.get_PID_angle_Kp().z);
    ui->PID_angle_Ki_z->setValue(quadro.get_PID_angle_Ki().z);
    ui->PID_angle_Kd_z->setValue(quadro.get_PID_angle_Kd().z);

    ui->PID_angle_MAXp_x->setValue(quadro.get_PID_angle_MAXp().x);
    ui->PID_angle_MAXi_x->setValue(quadro.get_PID_angle_MAXi().x);
    ui->PID_angle_MAXd_x->setValue(quadro.get_PID_angle_MAXd().x);

    ui->PID_angle_MAXp_y->setValue(quadro.get_PID_angle_MAXp().y);
    ui->PID_angle_MAXi_y->setValue(quadro.get_PID_angle_MAXi().y);
    ui->PID_angle_MAXd_y->setValue(quadro.get_PID_angle_MAXd().y);

    ui->PID_angle_MAXp_z->setValue(quadro.get_PID_angle_MAXp().z);
    ui->PID_angle_MAXi_z->setValue(quadro.get_PID_angle_MAXi().z);
    ui->PID_angle_MAXd_z->setValue(quadro.get_PID_angle_MAXd().z);

#if !(defined(PID_USE_YAW) || defined(PID_USE_YAW_ANGLE))
    ui->PID_angle_Kp_z->setEnabled(false);
    ui->PID_angle_Ki_z->setEnabled(false);
    ui->PID_angle_Kd_z->setEnabled(false);

    ui->PID_angle_MAXp_z->setEnabled(false);
    ui->PID_angle_MAXi_z->setEnabled(false);
    ui->PID_angle_MAXd_z->setEnabled(false);
#endif

    ui->power->setValue(0);
    ui->power->setScalePosition(QwtSlider::TopScale);
    ui->power->setFocusPolicy(Qt::ClickFocus);

    ui->power->setScale(0, 1, 0.1);
    ui->power->setRange(0, 1, 0.01, 0);

    ui->torque_manual_correction_x->setValue(quadro.get_torque_manual_correction().x);
    ui->torque_manual_correction_y->setValue(quadro.get_torque_manual_correction().y);
    ui->torque_manual_correction_z->setValue(quadro.get_joystick_heading() * 180. / M_PI);

    ui->reaction_type->setCurrentIndex(quadro.get_reaction_type());

    setFocus();
}

void Quadro::interface_write()
{
    stringstream t_ss, t_ss1, t_ss2, t_ss3, t_ss4;
    if(quadro.isoperational() || timer_log.isActive())
    {
        ui->voltage->setValue(quadro.get_voltage_percent());
        t_ss.precision(2);
        t_ss4 << voltage << "V";
        ui->voltage->setFormat(t_ss4.str().c_str());

        ui->torque->setText(torque.print().c_str());
        ui->gyro->setText(angular_velocity.print().c_str());

        t_ss3 << (angle * 180 / M_PI).print2d().c_str() << "\t"
              << angle.z * 180 / M_PI;

        ui->angle->setText(t_ss3.str().c_str());

        t_ss2 << read_time * 1E3 << " ms / " << write_time * 1E3 << " ms / "
              << loop_time * 1E6 << " us";
        ui->readwrite_time->setText(t_ss2.str().c_str());

        for(int i = 0; i < quadro.get_motors_n(); i++)
        {
            t_ss << M[i];
            if(i != quadro.get_motors_n() - 1) t_ss << "\t";
        }

        ui->motors->setText(t_ss.str().c_str());
        ui->CompassCopter->setOrigin(-angle.z * 180. / M_PI);
    }

    if(joy.isoperational())
    {
        ui->joystick_power->setValue(joystick_power * 100);
        if(joy.is_switched_on())
            ui->label_joystick_power->setStyleSheet("background-color: rgb(100, 255, 100);");
        else
            ui->label_joystick_power->setStyleSheet("background-color: rgb(255, 100, 100);");

        //t_ss1 << (joy.is_switched_on() ? "online" : "offline") << "\t";
        //t_ss1 << "p=" << joy.get_power_value() << "\t";
        t_ss1 << joystick_readings.print2d();
        t_ss1.precision(3);
        t_ss1 << "\t" << joystick_heading * 180 / M_PI;

        ui->joystick_data->clear();
        ui->joystick_data->setText(t_ss1.str().c_str());

        ui->CompassJoystick->setOrigin(joystick_heading * 180. / M_PI);
    }

    if(joy.isoperational())
    {
        ui->joystick_device->setStyleSheet("background-color: rgb(100, 255, 100);");
        //ui->joystick_device->setReadOnly(1);
    }
    else if(joy.iswaiting())
    {
        ui->joystick_device->setStyleSheet("background-color: rgb(255, 216, 0);");
        //ui->joystick_device->setReadOnly(1);
    }
    else
    {
        //ui->joystick_device->setReadOnly(0);
        ui->joystick_device->setStyleSheet("background-color: rgb(255, 100, 100);");
    }

    if(quadro.isoperational())
    {
        //ui->quadro_device->setEditable(0);
        ui->quadro_device->setStyleSheet("background-color: rgb(100, 255, 100);");
    }
    else if(quadro.iswaiting())
    {
        //ui->quadro_device->setEditable(0);
        ui->quadro_device->setStyleSheet("background-color: rgb(255, 216, 0);");
    }
    else
    {
        //ui->quadro_device->setEditable(1);
        ui->quadro_device->setStyleSheet("background-color: rgb(255, 100, 100);");
    }

    if(joy.isconnected())
        ui->joystick_connect->setText("Disconnect");
    else
        ui->joystick_connect->setText("Connect");

    if(quadro.isconnected())
        ui->quadro_connect->setText("Disconnect");
    else
        ui->quadro_connect->setText("Connect");
}

void Quadro::update_ports()
{
    QString quadro_device_prev_name = ui->quadro_device->currentText();
    QString joystick_device_prev_name = ui->joystick_device->currentText();

    ui->quadro_device->clear();
    ui->joystick_device->clear();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

    foreach (QextPortInfo info, ports) {

        ui->quadro_device->addItem(info.portName);
        ui->joystick_device->addItem(info.portName);

        if(!info.portName.compare(quadro_device_prev_name))
            ui->quadro_device->setCurrentIndex(ui->quadro_device->count() - 1);

        if(!info.portName.compare(joystick_device_prev_name))
            ui->joystick_device->setCurrentIndex(ui->joystick_device->count() - 1);
    }
}
