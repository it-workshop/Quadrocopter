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

void Quadro::on_quadro_device_textChanged(const QString &arg1)
{
    quadro.setDevice(arg1.toAscii().data());
    settings_data();
}

void Quadro::on_reaction_type_currentIndexChanged(int index)
{
    quadro.set_reaction_type((quadrocopter::reaction_type_) index);
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

void Quadro::on_quadro_update_clicked()
{
    if(quadro.iswriteable())
        quadro.initiate_transmission();
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


void Quadro::on_PID_angle_Kp_z_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kp_z(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Ki_z_valueChanged(double arg1)
{
    quadro.set_PID_angle_Ki_z(arg1);
    settings_data();
}

void Quadro::on_PID_angle_Kd_z_valueChanged(double arg1)
{
    quadro.set_PID_angle_Kd_z(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXp_z_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXp_z(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXi_z_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXi_z(arg1);
    settings_data();
}

void Quadro::on_PID_angle_MAXd_z_valueChanged(double arg1)
{
    quadro.set_PID_angle_MAXd_z(arg1);
    settings_data();
}

void Quadro::on_force_checkbox_clicked()
{
    quadro.set_force_override(ui->force_checkbox->isChecked(), ui->force->value());
    settings_data();
}

void Quadro::on_force_valueChanged(double value)
{
    quadro.set_force_override(ui->force_checkbox->isChecked(), ui->force->value());
    settings_data();
}

void Quadro::on_stopButton_clicked()
{
    ui->force->setValue(0);
    ui->force_checkbox->setChecked(1);
    quadro.set_force_override(ui->force_checkbox->isChecked(), ui->force->value());
    settings_data();
}

void Quadro::on_actionDebug_stderr_triggered(bool checked)
{
    quadro.set_debug_stderr(checked);
}

void Quadro::on_logfileaction_clicked()
{
    if(save_file.is_open())
    {
        //close it
        ui->logfileaction->setText("Open");
        ui->logfileaction->setStyleSheet("");
        save_close();
    }
    else
    {
        mytime t_time;
        save_filename = "../log/quadro_";
        save_filename.append(t_time.getTime());
        save_filename.append("_");
        save_filename.append(ui->flightname->text().toAscii().data());
        save_filename.append(".txt");
        save_open();
        ui->logfileaction->setText("Close");
        ui->logfileaction->setStyleSheet("background-color: rgb(100, 255, 100);");
    }
}

void Quadro::on_angle_offset_x_valueChanged(double arg1)
{
    quadro.set_angle_offset_x(arg1 * M_PI / 180);
}

void Quadro::on_angle_offset_y_valueChanged(double arg1)
{
    quadro.set_angle_offset_y(arg1 * M_PI / 180);
}

void Quadro::on_angle_offset_z_valueChanged(double arg1)
{
    quadro.set_angle_offset_z(arg1 * M_PI / 180);
}
