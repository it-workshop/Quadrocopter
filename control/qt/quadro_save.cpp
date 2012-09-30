#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>

using std::endl;

using std::stringstream;
using std::string;

void Quadro::save_close()
{
    save_file.close();
}

void Quadro::save_open()
{
    if(ui->LogSave_data->isChecked())
    {
        save_file.open(save_filename.c_str(), std::ios_base::app);
        save_file << "#seconds\tdatetime\tgyro_x\tgyro_y\tgyro_z\tacc_x\tacc_y\tacc_z\treact._t\tangle_x\tangle_y\t"
                  << "ca_x\tca_y\tca_z\tj_conn\tj_use\tj_x\tj_y\tj_power\tj_switch\tquad_conn\t"
                  << "cm_x\tcm_y\tcm_z\ttorq_x\ttorq_y\ttorq_z\tforce\tM_A\tM_B\tM_C\tM_D\t"
                  << "read_t\twrite_t\tloop_t\tav_kp\tav_ki\tav_kd\ta_kp\ta_ki\ta_kd" << endl;
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

        t_ss << t_time.get_seconds() << "\t" << t_time.get_time() << "\t"
             << quadro.get_gyroscope_readings().print() << "\t"
             << quadro.get_accelerometer_readings().print() << "\t"
             << quadro.get_reaction_type() << "\t"
             << quadro.get_angle().print2d() << "\t"

             << quadro.get_torque_automatic_correction().print() << "\t"

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

        t_ss << quadro.get_PID_angular_velocity_Kp() << "\t";
        t_ss << quadro.get_PID_angular_velocity_Ki() << "\t";
        t_ss << quadro.get_PID_angular_velocity_Kd() << "\t";

        t_ss << quadro.get_PID_angle_Kp() << "\t";
        t_ss << quadro.get_PID_angle_Ki() << "\t";
        t_ss << quadro.get_PID_angle_Kd();

        save_file << t_ss.str() << endl;
    }
}
