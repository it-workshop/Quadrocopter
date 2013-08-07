#include "quadro.h"
#include "QKeyEvent"
#include "ui_quadro.h"

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
    quadro.reset();
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
    if(joy.isoperational())
    {
        joy.initiate_transmission();
        joy.set_data_default();
        settings_data();
    }
}

void Quadro::on_quadro_device_textChanged(const QString &arg1)
{
    quadro.setDevice(arg1.toAscii().data());
    settings_data();
}

void Quadro::on_joystick_device_textChanged(const QString &arg1)
{
    joy.setDevice(arg1.toAscii().data());
    settings_data();
}

void Quadro::on_reaction_type_currentIndexChanged(int index)
{
    quadro.set_reaction_type((quadrocopter::reaction_type_) index);
    settings_data();
}

void Quadro::on_joystick_device_currentIndexChanged(const QString &arg1)
{
    joy.setDevice(arg1.toAscii().data());
    settings_data();
}

void Quadro::on_quadro_device_currentIndexChanged(const QString &arg1)
{
    quadro.setDevice(arg1.toAscii().data());
    settings_data();
}

void Quadro::keyPressEvent(QKeyEvent *a)
{
    if(a->key() == Qt::Key_0)
    {
        //ui->power->setValue(0);
    }
}

void Quadro::on_setAngle_clicked()
{
    ui->torque_manual_correction_x->setValue(quadro.get_angle().x);
    ui->torque_manual_correction_y->setValue(quadro.get_angle().y);
    ui->torque_manual_correction_z->setValue(quadro.get_copter_heading() * 180 / M_PI);
    settings_data();
}

void Quadro::on_torque_manual_reset_clicked()
{
    ui->torque_manual_correction_x->setValue(0);
    ui->torque_manual_correction_y->setValue(0);
    ui->torque_manual_correction_z->setValue(90);
    settings_data();
}

void Quadro::on_quadro_update_clicked()
{
    if(quadro.iswriteable())
        quadro.initiate_transmission();
}

void Quadro::on_torque_manual_correction_x_valueChanged(double arg1)
{
    quadro.set_torque_manual_correction(vect(arg1, quadro.get_torque_manual_correction().y, 0));
    settings_data();
}

void Quadro::on_torque_manual_correction_y_valueChanged(double arg1)
{
    quadro.set_torque_manual_correction(vect(quadro.get_torque_manual_correction().x, arg1, 0));
    settings_data();
}

void Quadro::on_torque_manual_correction_z_valueChanged(double arg1)
{
    settings_data();
}

void Quadro::on_PID_angle_Kp_x_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kp_x(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Ki_x_valueChanged(double arg1)
{
    quadro.set_PID_angle_Ki_x(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Kd_x_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kd_x(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Kp_y_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kp_y(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Ki_y_valueChanged(double arg1)
{
    quadro.set_PID_angle_Ki_y(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Kd_y_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kd_y(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXp_x_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXp_x(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXi_x_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXi_x(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXd_x_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXd_x(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXp_y_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXp_y(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXi_y_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXi_y(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXd_y_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXd_y(arg1);
    settings_data();
}

void Quadro::on_joystick_connect_clicked()
{
    if(joy.isconnected())
    {
        ui->joy_reconnect->setChecked(false);
        joy_disconnect();
    }
    else
    {
        joy_connect();
    }
}

void Quadro::on_joystick_use_clicked()
{
    if(joy.isoperational())
    {
        joy.set_data_default();

        ui->power->setValue(0);
    }
}

void Quadro::on_joystick_calibrate_clicked()
{
    if(joy.isoperational())
    {
        joy.initiate_transmission();
        joy.set_data_default();
        settings_data();
    }
}

void Quadro::on_quadro_autoupdate_triggered()
{
    settings_data();
}

void Quadro::on_quadro_connect_clicked()
{
    if(quadro.isconnected())
    {
        ui->quadro_reconnect->setChecked(false);
        quadro_disconnect();
    }
    else
        quadro_connect();
}


void Quadro::on_PID_angularVelocity_Kp_z_valueChanged(double arg1)
{
    quadro.set_PID_angularVelocity_Kp_z(arg1);
    settings_data();
}

void Quadro::on_PID_angularVelocity_Ki_z_valueChanged(double arg1)
{
    quadro.set_PID_angularVelocity_Ki_z(arg1);
    settings_data();
}

void Quadro::on_PID_angularVelocity_Kd_z_valueChanged(double arg1)
{
    quadro.set_PID_angularVelocity_Kd_z(arg1);
    settings_data();
}

void Quadro::on_PID_angularVelocity_MAXp_z_valueChanged(double arg1)
{
    quadro.set_PID_angularVelocity_MAXp_z(arg1);
    settings_data();
}

void Quadro::on_PID_angularVelocity_MAXi_z_valueChanged(double arg1)
{
    quadro.set_PID_angularVelocity_MAXi_z(arg1);
    settings_data();
}

void Quadro::on_PID_angularVelocity_MAXd_z_valueChanged(double arg1)
{
    quadro.set_PID_angularVelocity_MAXd_z(arg1);
    settings_data();
}
