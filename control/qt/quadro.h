#ifndef QUADRO_H
#define QUADRO_H

#include <QMainWindow>
#include <serial.h>
#include <string>
#include <fstream>
#include <QTimer>
#include <vect.h>
#include <quadrocopter.h>
#include <vector>

#include "qextserialenumerator.h"

#include <qwt_plot_curve.h>

using std::string;
using std::ofstream;
using std::ifstream;
using std::vector;

namespace Ui {
    class Quadro;
}

class Quadro : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Quadro(QWidget *parent = 0);
    ~Quadro();
    
private slots:
    void timer_auto_update();
    void timer_reconnect_update();
    void timer_log_update();
    void timer_track_update();

    //quadro
    void on_actionQuadroConnect_triggered();
    void on_actionQuadroDisconnect_triggered();
    void on_actionQuadroReset_triggered();
    void on_quadro_device_textChanged(const QString &arg1);

    void on_reaction_type_currentIndexChanged(int index);

private:
    Ui::Quadro *ui;

    QextSerialEnumerator QeSEnumerator;

    static const double timer_auto_interval = 50;
    double timer_log_interval;
    static const double timer_reconnect_interval = 1000;
    double timer_track_interval;
    QTimer timer_auto, timer_reconnect, timer_log, timer_track;

    quadrocopter quadro;

    vect angular_velocity, angle, acceleration, torque, PID_P, PID_I, PID_D;
    vect joystick_readings, correction;

    double power, voltage, joystick_heading, joystick_power;
    int M[4];
    double read_time, write_time, loop_time;
    vector<int> log_lines;
    int log_line;

    // plot

    static const int plot_size = 200 + 1;
    double plot_time[plot_size],
        plot_gyro_x[plot_size], plot_gyro_y[plot_size], plot_gyro_z[plot_size],
        plot_joy_x[plot_size], plot_joy_y[plot_size],
        plot_angle_x[plot_size], plot_angle_y[plot_size], plot_angle_z[plot_size],
        plot_angle_accx[plot_size], plot_angle_accy[plot_size],
        plot_torque_x[plot_size], plot_torque_y[plot_size], plot_torque_z[plot_size], plot_force[plot_size],
        plot_acceleration_correction_x[plot_size], plot_acceleration_correction_y[plot_size], plot_acceleration_correction_z[plot_size],
        plot_angular_velocity_correction_x[plot_size], plot_angular_velocity_correction_y[plot_size], plot_angular_velocity_correction_z[plot_size],
        plot_angle_correction_x[plot_size], plot_angle_correction_y[plot_size], plot_angle_correction_z[plot_size], plot_voltage[plot_size],
        plot_PID_P_x[plot_size], plot_PID_I_x[plot_size], plot_PID_D_x[plot_size],
        plot_PID_P_y[plot_size], plot_PID_I_y[plot_size], plot_PID_D_y[plot_size],
        plot_PID_P_z[plot_size], plot_PID_I_z[plot_size], plot_PID_D_z[plot_size];

    static const long double PLOT_TORQUE_COEFF_XY = 4, PLOT_TORQUE_COEFF_Z = 1;

    QwtPlotCurve *plot_curve_angular_velocity_correction_x, *plot_curve_angular_velocity_correction_y, *plot_curve_angular_velocity_correction_z;
    QwtPlotCurve *plot_curve_acceleration_correction_x, *plot_curve_acceleration_correction_y, *plot_curve_acceleration_correction_z;
    QwtPlotCurve *plot_curve_angle_correction_x, *plot_curve_angle_correction_y, *plot_curve_angle_correction_z;
    QwtPlotCurve *plot_curve_voltage, *plot_curve_joy_x, *plot_curve_joy_y;

    // /plot

    mytime plot_mytime;

    int track_line, track_line_max;
    string track_filename;
    ifstream track_file;

    string log_filename;
    ifstream log_file;

    string save_filename;
    ofstream save_file;

    string settings_filename;
    ofstream settings_file;

    void quadro_connect();
    void quadro_disconnect();
    void quadro_fetch_data();

    void set_quadro_data();
    void set_auto(bool);

    void save_data();
    void save_open();
    void save_close();

    bool quadro_save_settings;

    void settings_data();
    void settings_open();
    void settings_close();
    void settings_read();

    void plot_update();
    void plot_init();
    void plot_reset_data();
    void plot_torque_and_force_update_legend();

    void interface_read();
    void interface_write();
    void interface_init();

private slots:
    void update_ports();
    void on_quadro_device_currentIndexChanged(const QString &arg1);
    void keyPressEvent(QKeyEvent*);
    void on_quadro_update_clicked();
    void on_PID_angle_Kp_x_valueChanged(double arg1);
    void on_PID_angle_Ki_x_valueChanged(double arg1);
    void on_PID_angle_Kd_x_valueChanged(double arg1);
    void on_PID_angle_Kp_y_valueChanged(double arg1);
    void on_PID_angle_Ki_y_valueChanged(double arg1);
    void on_PID_angle_Kd_y_valueChanged(double arg1);
    void on_PID_angle_MAXp_x_valueChanged(double arg1);
    void on_PID_angle_MAXi_x_valueChanged(double arg1);
    void on_PID_angle_MAXd_x_valueChanged(double arg1);
    void on_PID_angle_MAXp_y_valueChanged(double arg1);
    void on_PID_angle_MAXi_y_valueChanged(double arg1);
    void on_PID_angle_MAXd_y_valueChanged(double arg1);
    void on_quadro_autoupdate_triggered();
    void on_quadro_connect_clicked();
    void on_PID_angle_Kp_z_valueChanged(double arg1);
    void on_PID_angle_Ki_z_valueChanged(double arg1);
    void on_PID_angle_Kd_z_valueChanged(double arg1);
    void on_PID_angle_MAXp_z_valueChanged(double arg1);
    void on_PID_angle_MAXi_z_valueChanged(double arg1);
    void on_PID_angle_MAXd_z_valueChanged(double arg1);
    void on_force_checkbox_clicked();
    void on_force_valueChanged(double value);
    void on_stopButton_clicked();
    void on_log_browse_clicked();
    void on_log_start_clicked();
    void on_log_pause_clicked();
    void on_log_time_valueChanged(int arg1);
    void on_log_scroll_valueChanged(int value);
    void on_actionDebug_stderr_triggered(bool checked);
    void on_logfileaction_clicked();
    void on_angle_offset_x_valueChanged(double arg1);
    void on_angle_offset_y_valueChanged(double arg1);
    void on_angle_offset_z_valueChanged(double arg1);
    void on_track_browse_clicked();
    void on_track_start_clicked();
    void on_track_pause_clicked();
    void on_track_open_clicked();
};

#endif // QUADRO_H
