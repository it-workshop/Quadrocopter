#ifndef QUADRO_H
#define QUADRO_H

#include <QMainWindow>
#include <serial.h>
#include <string>
#include <fstream>
#include <QTimer>
#include <vect.h>
#include <quadrocopter.h>
#include <joystick.h>

#include "qextserialenumerator.h"

#include <qwt_plot_curve.h>

using std::string;
using std::ofstream;

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

    //quadro
    void on_actionQuadroConnect_triggered();
    void on_actionQuadroDisconnect_triggered();
    void on_actionQuadroReset_triggered();
    void on_quadro_device_textChanged(const QString &arg1);

    void on_reaction_type_currentIndexChanged(int index);

    //joy
    void on_actionJoystickConnect_triggered();
    void on_actionJoystickDisconnect_triggered();
    void on_actionJoystickCalibrate_zero_triggered();
    void on_joystick_device_textChanged(const QString &arg1);

private:
    Ui::Quadro *ui;

    QextSerialEnumerator QeSEnumerator;

    static const double timer_auto_interval = 50;
    static const double timer_reconnect_interval = 1000;
    QTimer timer_auto, timer_reconnect;

    quadrocopter quadro;
    joystick joy;

    // plot

    static const int plot_size = 200 + 1;
    double plot_time[plot_size],
        plot_gyro_x[plot_size], plot_gyro_y[plot_size], plot_gyro_z[plot_size],
        plot_acc_x[plot_size], plot_acc_y[plot_size], plot_acc_z[plot_size],
        plot_angle_x[plot_size], plot_angle_y[plot_size],
        plot_angle_accx[plot_size], plot_angle_accy[plot_size],
        plot_torque_x[plot_size], plot_torque_y[plot_size], plot_torque_z[plot_size], plot_force[plot_size],
        plot_acceleration_correction_x[plot_size], plot_acceleration_correction_y[plot_size], plot_acceleration_correction_z[plot_size],
        plot_angular_velocity_correction_x[plot_size], plot_angular_velocity_correction_y[plot_size], plot_angular_velocity_correction_z[plot_size],
        plot_angle_correction_x[plot_size], plot_angle_correction_y[plot_size], plot_angle_correction_z[plot_size], plot_voltage[plot_size],
        plot_PID_P_x[plot_size], plot_PID_I_x[plot_size], plot_PID_D_x[plot_size],
        plot_PID_P_y[plot_size], plot_PID_I_y[plot_size], plot_PID_D_y[plot_size];

    QwtPlotCurve *plot_curve_angular_velocity_correction_x, *plot_curve_angular_velocity_correction_y, *plot_curve_angular_velocity_correction_z;
    QwtPlotCurve *plot_curve_acceleration_correction_x, *plot_curve_acceleration_correction_y, *plot_curve_acceleration_correction_z;
    QwtPlotCurve *plot_curve_angle_correction_x, *plot_curve_angle_correction_y, *plot_curve_angle_correction_z;
    QwtPlotCurve *plot_curve_voltage;

    // /plot

    mytime plot_mytime;

    string save_filename;
    ofstream save_file;

    string settings_filename;
    ofstream settings_file;

    void quadro_connect();
    void quadro_disconnect();
    void joy_disconnect();
    void joy_connect();

    void set_quadro_data();
    void set_auto(bool);

    void save_data();
    void save_open();
    void save_close();

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
    void on_joystick_device_currentIndexChanged(const QString &arg1);
    void on_quadro_device_currentIndexChanged(const QString &arg1);
    void keyPressEvent(QKeyEvent*);
    void on_setAngle_clicked();
    void on_torque_manual_reset_clicked();
    void on_quadro_update_clicked();
    void on_torque_manual_correction_x_valueChanged(double arg1);
    void on_torque_manual_correction_y_valueChanged(double arg1);
    void on_PID_angle_Kp_x_valueChanged(double arg1);
    void on_PID_angle_Ki_x_valueChanged(double arg1);
    void on_PID_angle_Kd_x_valueChanged(double arg1);
    void on_PID_angle_Kp_y_valueChanged(double arg1);
    void on_PID_angle_Ki_y_valueChanged(double arg1);
    void on_PID_angle_Kd_y_valueChanged(double arg1);
    void on_PID_angle_Kp_z_valueChanged(double arg1);
    void on_PID_angle_Ki_z_valueChanged(double arg1);
    void on_PID_angle_Kd_z_valueChanged(double arg1);
    void on_PID_angle_MAXp_x_valueChanged(double arg1);
    void on_PID_angle_MAXi_x_valueChanged(double arg1);
    void on_PID_angle_MAXd_x_valueChanged(double arg1);
    void on_PID_angle_MAXp_y_valueChanged(double arg1);
    void on_PID_angle_MAXi_y_valueChanged(double arg1);
    void on_PID_angle_MAXd_y_valueChanged(double arg1);
    void on_PID_angle_MAXp_z_valueChanged(double arg1);
    void on_PID_angle_MAXi_z_valueChanged(double arg1);
    void on_PID_angle_MAXd_z_valueChanged(double arg1);
    void on_joystick_connect_clicked();
    void on_joystick_use_clicked();
    void on_joystick_calibrate_clicked();
    void on_quadro_autoupdate_triggered();
    void on_quadro_connect_clicked();
};

#endif // QUADRO_H
