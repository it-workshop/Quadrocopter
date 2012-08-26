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
    void on_device_editTextChanged(const QString &arg1);

    void on_reaction_type_currentIndexChanged(int index);

    //joy
    void on_actionJoystickConnect_triggered();
    void on_actionJoystickDisconnect_triggered();
    void on_actionJoystickCalibrate_zero_triggered();
    void on_joystick_device_textChanged(const QString &arg1);
    void on_JoystickUse_toggled();

    void on_PID_angle_Kp_valueChanged(double arg1);

    void on_PID_angle_Ki_valueChanged(double arg1);

    void on_PID_angle_Kd_valueChanged(double arg1);

private:
    Ui::Quadro *ui;

    static const double timer_auto_interval = 180;
    static const double timer_reconnect_interval = 1000;
    QTimer timer_auto, timer_reconnect;

    quadrocopter quadro;
    joystick joy;

    static const int plot_size = 200 + 1;
    double plot_time[plot_size],
        plot_gyro_x[plot_size], plot_gyro_y[plot_size], plot_gyro_z[plot_size],
        plot_acc_x[plot_size], plot_acc_y[plot_size], plot_acc_z[plot_size],
        plot_angle_x[plot_size], plot_angle_y[plot_size],
        plot_angle_accx[plot_size], plot_angle_accy[plot_size],
        plot_throttle_x[plot_size], plot_throttle_y[plot_size], plot_throttle_z[plot_size], plot_throttle[plot_size],
        plot_acc_rotation_x[plot_size], plot_acc_rotation_y[plot_size],
        plot_gyro_rotation_x[plot_size], plot_gyro_rotation_y[plot_size],
        plot_angle_rotation_x[plot_size], plot_angle_rotation_y[plot_size];

    mytime plot_mytime;

    string save_filename;
    ofstream save_file;

    void quadro_connect();
    void quadro_disconnect();
    void joy_disconnect();
    void joy_connect();

    void set_quadro_data();
    void set_auto(bool);

    void save_data();
    void save_open();
    void save_close();

    void plot_update();
    void plot_init();
    void plot_reset_data();

    void interface_read();
    void interface_write();
};

#endif // QUADRO_H
