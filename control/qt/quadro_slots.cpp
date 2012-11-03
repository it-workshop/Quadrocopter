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
        joy.read_data_request();
        joy.set_data_default();
    }
}

void Quadro::on_quadro_device_textChanged(const QString &arg1)
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
    if(joy.isoperational())
    {
        joy.set_data_default();

        ui->power->setValue(0);
    }
}

void Quadro::on_PID_angle_Kp_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kp(arg1);
}

void Quadro::on_PID_angle_Ki_valueChanged(double arg1)
{
    quadro.set_PID_angle_Ki(arg1);
}

void Quadro::on_PID_angle_Kd_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kd(arg1);
}

void Quadro::on_PID_angular_velocity_Kp_valueChanged(double arg1)
{
    quadro.set_PID_angular_velocity_Kp(arg1);
}

void Quadro::on_PID_angular_velocity_Ki_valueChanged(double arg1)
{
    quadro.set_PID_angular_velocity_Ki(arg1);
}

void Quadro::on_PID_angular_velocity_Kd_valueChanged(double arg1)
{
    quadro.set_PID_angular_velocity_Kd(arg1);
}

void Quadro::on_joystick_device_currentIndexChanged(const QString &arg1)
{
    joy.set_device(arg1.toAscii().data());
}

void Quadro::on_quadro_device_currentIndexChanged(const QString &arg1)
{
    quadro.set_device(arg1.toAscii().data());
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
}

void Quadro::on_gyroscope_period_valueChanged(double arg1)
{
    quadro.set_gyro_period(arg1);
}

void Quadro::on_accel_period_valueChanged(double arg1)
{
    quadro.set_accel_period(arg1);
}

void Quadro::on_angle_period_valueChanged(double arg1)
{
    quadro.set_angle_period(arg1);
}
