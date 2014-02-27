#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>
#include <QFileDialog>
#include <QDebug>

using std::cout;
using std::endl;

using std::stringstream;
using std::string;
using std::ifstream;
using std::getline;

void Quadro::on_log_browse_clicked()
{
    log_filename = QFileDialog::getOpenFileName(this,
             tr("Open log file"), "../log", tr("Quadro log (*.txt)")).toAscii().data();

    qDebug() << "Opening" << log_filename.c_str();

    ui->log_file->setText(log_filename.c_str());

    log_file.close();
    log_file.open(log_filename.c_str());

    string s;

    int last_pos = log_file.tellg();
    while(getline(log_file, s))
    {
        log_lines.push_back(last_pos);
        qDebug() << log_file.tellg();
        last_pos = log_file.tellg();
    }

    log_file.close();
    log_file.open(log_filename.c_str());

    log_line = 10;

    qDebug() << "lines=" << log_lines.size();

    getline(log_file, s);
    qDebug() << "Header line: " << s.c_str();
    log_file.seekg(log_lines[1]);
    getline(log_file, s);
    qDebug() << "First line: " << s.c_str();
}

void Quadro::timer_log_update()
{
    static bool allowed = true;

    double t_double;
    long long seconds;
    string date, t_str;
    int t_int;

    if(allowed)
    {
        allowed = false;

        if(!plot_mytime.isSet()) plot_mytime.setTime();

        qDebug() << "line=" << log_lines[log_line] << " " << log_line;

        log_file.clear();
        log_file.seekg(log_lines[log_line]);

        log_file >> seconds >> date >> t_int;

        log_file >> angular_velocity.value_by_axis_index(0) >>
                    angular_velocity.value_by_axis_index(1) >>
                    angular_velocity.value_by_axis_index(2);

        log_file >> t_int;

        log_file >> angle.value_by_axis_index(0) >>
                    angle.value_by_axis_index(1) >>
                    angle.value_by_axis_index(2);

        log_file >> torque.value_by_axis_index(0) >>
                    torque.value_by_axis_index(1) >>
                    torque.value_by_axis_index(2);

        log_file >> power >> voltage;

        log_file >> PID_P.value_by_axis_index(0) >>
                    PID_P.value_by_axis_index(1) >>
                    PID_P.value_by_axis_index(2);

        log_file >> PID_I.value_by_axis_index(0) >>
                    PID_I.value_by_axis_index(1) >>
                    PID_I.value_by_axis_index(2);

        log_file >> PID_D.value_by_axis_index(0) >>
                    PID_D.value_by_axis_index(1) >>
                    PID_D.value_by_axis_index(2);

        log_file >> angle.value_by_axis_index(2);

        log_file >> t_double >> t_double >> t_double;
        log_file >> t_double >> t_double >> t_double;
        log_file >> t_double >> t_double >> t_double;

        log_file >> t_double >> t_double >> t_double;
        log_file >> t_double >> t_double >> t_double;
        log_file >> t_double >> t_double >> t_double;

        log_file >> joystick_readings.value_by_axis_index(0);
        log_file >> joystick_readings.value_by_axis_index(1);

        //log_file >> t_double >> t_double >> t_double;

        log_file >> joystick_heading >> joystick_power;

        log_file >> M[0] >> M[1] >> M[2] >> M[3];

        log_file >> read_time >> write_time >> loop_time;


        ui->current_time->setText(date.c_str());

        interface_write();
        plot_update();

        log_line++;
        ui->log_scroll->setValue(log_line * 100/ log_lines.size());

        allowed = true;
    }
}

void Quadro::on_log_start_clicked()
{
    quadro_save_settings = false;
    ui->LogSave_data->setChecked(false);
    ui->LogSave_data->setCheckable(false);
    timer_log_interval = ui->log_time->value();
    plot_reset_data();
    plot_mytime.reset();
    timer_log.start(timer_log_interval);
}

void Quadro::on_log_pause_clicked()
{
    timer_log.stop();
    ui->LogSave_data->setChecked(true);
    ui->LogSave_data->setCheckable(true);
    quadro_save_settings = true;
    plot_mytime.reset();
}

void Quadro::on_log_time_valueChanged(int arg1)
{
    on_log_start_clicked();
}

void Quadro::on_log_scroll_valueChanged(int value)
{
    log_line = log_lines.size() * value / 100;
    if(log_line < 10) log_line = 10;
}
