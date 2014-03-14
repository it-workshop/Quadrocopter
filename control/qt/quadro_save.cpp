#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>
#include <QDateTime>

using std::endl;

using std::stringstream;
using std::string;
using std::ifstream;

void Quadro::save_close()
{
    save_file.close();
}

void Quadro::settings_data()
{
    if(!quadro_save_settings)
        return;
    stringstream ss;

    ss << ui->angle_offset_x->value() << " ";
    ss << ui->angle_offset_y->value() << " ";
    ss << ui->angle_offset_z->value() << " ";

    ss << quadro.get_PID_angle_Kp().x << " ";
    ss << quadro.get_PID_angle_Ki().x << " ";
    ss << quadro.get_PID_angle_Kd().x << " ";
    ss << quadro.get_PID_angle_MAXp().x << " ";
    ss << quadro.get_PID_angle_MAXi().x << " ";
    ss << quadro.get_PID_angle_MAXd().x << " ";

    ss << quadro.get_PID_angle_Kp().y << " ";
    ss << quadro.get_PID_angle_Ki().y << " ";
    ss << quadro.get_PID_angle_Kd().y << " ";
    ss << quadro.get_PID_angle_MAXp().y << " ";
    ss << quadro.get_PID_angle_MAXi().y << " ";
    ss << quadro.get_PID_angle_MAXd().y << " ";

    ss << quadro.get_PID_angle_Kp().z << " ";
    ss << quadro.get_PID_angle_Ki().z << " ";
    ss << quadro.get_PID_angle_Kd().z << " ";
    ss << quadro.get_PID_angle_MAXp().z << " ";
    ss << quadro.get_PID_angle_MAXi().z << " ";
    ss << quadro.get_PID_angle_MAXd().z << " ";

    //ss << quadro.getDevice() << " ";
    //ss << quadro.get_torque_manual_correction().x << " ";
    //ss << quadro.get_torque_manual_correction().y << " ";
    ss << quadro.get_reaction_type() << " ";
    ss << ui->quadro_autoupdate->isChecked() << " ";

    settings_open();

    settings_file << ss.str();

    settings_close();
}

void Quadro::settings_read()
{
    ifstream settings_file(settings_filename.c_str());

    double t_double;

    settings_file >> t_double; ui->angle_offset_x->setValue(t_double);
    quadro.set_angle_offset_x(t_double * M_PI / 180);
    settings_file >> t_double; ui->angle_offset_y->setValue(t_double);
    quadro.set_angle_offset_y(t_double * M_PI / 180);
    settings_file >> t_double; ui->angle_offset_z->setValue(t_double);
    quadro.set_angle_offset_z(t_double * M_PI / 180);

    settings_file >> t_double; quadro.set_PID_angle_Kp_x(t_double);
    settings_file >> t_double; quadro.set_PID_angle_Ki_x(t_double);
    settings_file >> t_double; quadro.set_PID_angle_Kd_x(t_double);

    settings_file >> t_double; quadro.set_PID_angle_MAXp_x(t_double);
    settings_file >> t_double; quadro.set_PID_angle_MAXi_x(t_double);
    settings_file >> t_double; quadro.set_PID_angle_MAXd_x(t_double);

    settings_file >> t_double; quadro.set_PID_angle_Kp_y(t_double);
    settings_file >> t_double; quadro.set_PID_angle_Ki_y(t_double);
    settings_file >> t_double; quadro.set_PID_angle_Kd_y(t_double);

    settings_file >> t_double; quadro.set_PID_angle_MAXp_y(t_double);
    settings_file >> t_double; quadro.set_PID_angle_MAXi_y(t_double);
    settings_file >> t_double; quadro.set_PID_angle_MAXd_y(t_double);

    settings_file >> t_double; quadro.set_PID_angle_Kp_z(t_double);
    settings_file >> t_double; quadro.set_PID_angle_Ki_z(t_double);
    settings_file >> t_double; quadro.set_PID_angle_Kd_z(t_double);

    settings_file >> t_double; quadro.set_PID_angle_MAXp_z(t_double);
    settings_file >> t_double; quadro.set_PID_angle_MAXi_z(t_double);
    settings_file >> t_double; quadro.set_PID_angle_MAXd_z(t_double);

//    string t_string;
//    settings_file >> t_string; quadro.setDevice(t_string);

    int t_int;
    settings_file >> t_int; quadro.set_reaction_type((quadrocopter::reaction_type_) t_int);
    settings_file >> t_int; ui->quadro_autoupdate->setChecked(t_int);

    settings_file.close();
}

void Quadro::settings_open()
{
    settings_file.open(settings_filename.c_str());
}

void Quadro::settings_close()
{
    settings_file.close();
}

void Quadro::save_open()
{
    if(ui->LogSave_data->isChecked())
    {
        save_file.open(save_filename.c_str(), std::ios_base::app);
        save_file << "#milliseconds\tdatetime\tqptr_op\tgyro_x\tgyro_y\tgyro_z\treact_t\tangle_x\tangle_y\tangle_z\t"
                  << "trq_x\ttrq_y\ttrq_z\tc_power\tvoltage\tPID_P_x\tPID_P_y\tPID_P_z\tPID_I_x\tPID_I_y\tPID_I_z\t"
                  << "PID_D_x\tPID_D_y\tPID_D_z\tq_head\tKp_x\tKp_y\tKp_z\tKi_x\tKi_y\tKi_z\tKd_x\tKd_y\tKd_z\t"
                  << "MaxP_x\tMaxP_y\tMaxP_z\tMaxI_x\tMaxI_y\tMaxI_z\tMaxD_x\tMaxD_y\tMaxD_z\tjoy_x\tjoy_y\tjoy_hdn\t"
                  << "j_power\t\tM1\tM2\tM3\tM4\tread_time\twrite_time\tloop_time\toffx\toffy\toffz" << endl;
    }
}

void Quadro::save_data()
{
    if(ui->LogSave_data->isChecked() && quadro.isoperational() && save_file.is_open())
    {
        stringstream t_ss;
        int i;
        mytime t_time;

        t_ss.precision(3);

        t_ss << (QDateTime::currentMSecsSinceEpoch()) << "\t" << t_time.getTime() << "\t"
             << quadro.isoperational() << "\t"
             << quadro.get_gyroscope_readings().print() << "\t"
             << quadro.get_reaction_type() << "\t"
             << quadro.get_angle().print() << "\t"
             << quadro.get_torque_corrected().print() << "\t"
             << quadro.get_power() << "\t"
             << quadro.get_voltage() << "\t"
             << quadro.get_PID_P().print() << "\t"
             << quadro.get_PID_I().print() << "\t"
             << quadro.get_PID_D().print() << "\t"
             << quadro.get_copter_heading() << "\t"
             << quadro.get_PID_angle_Kp().print() << "\t"
             << quadro.get_PID_angle_Ki().print() << "\t"
             << quadro.get_PID_angle_Kd().print() << "\t"
             << quadro.get_PID_angle_MAXp().print() << "\t"
             << quadro.get_PID_angle_MAXi().print() << "\t"
             << quadro.get_PID_angle_MAXd().print() << "\t"

             << quadro.get_torque_manual_correction().x << "\t"
             << quadro.get_torque_manual_correction().y << "\t"
             << quadro.get_joystick_heading() << "\t"
             << quadro.get_power() << "\t";

        for(i = 0; i < quadro.get_motors_n(); i++)
        {
            t_ss << quadro.get_motor_power(i) << "\t";
        }

        t_ss.precision(6);
        t_ss << quadro.get_read_time() << "\t"
             << quadro.get_write_time() << "\t"
             << quadro.get_loop_time() << "\t";

        save_file << t_ss.str();

        save_file << ui->angle_offset_x->value() << "\t";
        save_file << ui->angle_offset_y->value() << "\t";
        save_file << ui->angle_offset_z->value() << "\t";

        save_file << endl;
    }
}
