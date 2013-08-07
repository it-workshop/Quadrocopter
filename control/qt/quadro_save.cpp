#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>

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
    stringstream ss;

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

    ss << quadro.get_PID_angularVelocity_Kp().z << " ";
    ss << quadro.get_PID_angularVelocity_Ki().z << " ";
    ss << quadro.get_PID_angularVelocity_Kd().z << " ";
    ss << quadro.get_PID_angularVelocity_MAXp().z << " ";
    ss << quadro.get_PID_angularVelocity_MAXi().z << " ";
    ss << quadro.get_PID_angularVelocity_MAXd().z << " ";

    //ss << quadro.getDevice() << " ";
    //ss << quadro.get_torque_manual_correction().x << " ";
    //ss << quadro.get_torque_manual_correction().y << " ";
    ss << ui->torque_manual_correction_x->value() << " ";
    ss << ui->torque_manual_correction_y->value() << " ";
    ss << ui->torque_manual_correction_z->value() << " ";
    ss << quadro.get_reaction_type() << " ";
    ss << ui->quadro_autoupdate->isChecked() << " ";

    //ss << joy.getDevice() << " ";

    settings_open();

    settings_file << ss.str();

    settings_close();
}

void Quadro::settings_read()
{
    ifstream settings_file(settings_filename.c_str());

    double t_double;
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

    settings_file >> t_double; quadro.set_PID_angularVelocity_Kp_z(t_double);
    settings_file >> t_double; quadro.set_PID_angularVelocity_Ki_z(t_double);
    settings_file >> t_double; quadro.set_PID_angularVelocity_Kd_z(t_double);

    settings_file >> t_double; quadro.set_PID_angularVelocity_MAXp_z(t_double);
    settings_file >> t_double; quadro.set_PID_angularVelocity_MAXi_z(t_double);
    settings_file >> t_double; quadro.set_PID_angularVelocity_MAXd_z(t_double);

//    string t_string;
//    settings_file >> t_string; quadro.setDevice(t_string);

    double t_double1;
    settings_file >> t_double;
    settings_file >> t_double1;
    quadro.set_torque_manual_correction(vect(t_double, t_double1, 0));

    settings_file >> t_double;
    quadro.set_joystick_heading(t_double * M_PI / 180.);

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
        save_file << "#seconds\tdatetime\tgyro_x\tgyro_y\tgyro_z\tacc_x\tacc_y\tacc_z\treact._t\tangle_x\tangle_y\t"
                  << "ca_x\tca_y\tca_z\tj_conn\tj_use\tj_x\tj_y\tj_power\tj_switch\tquad_conn\t"
                  << "cm_x\tcm_y\tcm_z\ttorq_x\ttorq_y\ttorq_z\tforce\tM_A\tM_B\tM_C\tM_D\t"
                  << "read_t\twrite_t\tloop_t\tav_kp\tav_ki\tav_kd\ta_kp\ta_ki\ta_kd\tTaccel\tTangle" << endl;
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

        t_ss << t_time.getSeconds() << "\t" << t_time.getTime() << "\t"
             << quadro.get_gyroscope_readings().print() << "\t"
             << quadro.get_accelerometer_readings().print() << "\t"
             << quadro.get_reaction_type() << "\t"
             << quadro.get_angle().print2d() << "\t"

             << joy.isoperational() << "\t"
             << ui->JoystickUse->isChecked() << "\t"
             << joy.get_readings().print2d() << "\t"
             << joy.get_power_value() << "\t"
             << joy.is_switched_on() << "\t"

             << quadro.isoperational() << "\t"
             << quadro.get_torque_manual_correction().print() << "\t"
             << quadro.get_torque_corrected().print() << "\t"
             << quadro.get_power() << "\t";

        for(i = 0; i < quadro.get_motors_n(); i++)
        {
            t_ss << quadro.get_motor_power(i) << "\t";
        }

        t_ss.precision(6);
        t_ss << quadro.get_read_time() << "\t"
             << quadro.get_write_time() << "\t"
             << quadro.get_loop_time() << "\t";

        t_ss << quadro.get_PID_angle_Kp().x << "\t";
        t_ss << quadro.get_PID_angle_Ki().x << "\t";
        t_ss << quadro.get_PID_angle_Kd().x << "\t";

        t_ss << quadro.get_PID_angle_Kp().y << "\t";
        t_ss << quadro.get_PID_angle_Ki().y << "\t";
        t_ss << quadro.get_PID_angle_Kd().y << "\t";

        save_file << t_ss.str() << endl;
    }
}
