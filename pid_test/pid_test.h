#ifndef PID_TEST_H
#define PID_TEST_H

#include <QMainWindow>
#include "qtimer.h"
#include "mytime.h"

#define DESKTOP

#include <string>
#include "PID.h"
#include "RVector3D.h"

using std::string;

namespace Ui {
class PID_test;
}

class PID_test : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PID_test(QWidget *parent = 0);
    ~PID_test();
    
private:
    Ui::PID_test *ui;

    //plot
    static const int plot_size = 200 + 1;
    double plot_value[plot_size], plot_x[plot_size], plot_time[plot_size],
    plot_P[plot_size], plot_I[plot_size], plot_D[plot_size];

    //values
    double value, prev_value, value_speed;

    //requested value
    double x;

    double friction_coefficient;

    QTimer timer_auto;

    double timer_auto_interval;
    mytime plot_mytime;

    void plot_init();
    void plot_reset_data();
    void plot_update();

    PID<RVector3D> pid_angular_velocity, pid_angle;

    void settings_read();
    void settings_write();

    string settings_filename;

    static const double MIN_CORRECTION = 0.9;

private slots:
    void timer_auto_update();

    void on_pushButton_reset_clicked();
    void on_pushButton_pause_clicked();
    void on_Kp_valueChanged(double arg1);
    void on_Ki_valueChanged(double arg1);
    void on_Kd_valueChanged(double arg1);
    void on_dt_valueChanged(int arg1);
    void on_comboBox_type_currentIndexChanged(int index);
    void on_pushButton_wind_clicked();
    void on_doubleSpinBox_friction_valueChanged(double arg1);
    void on_Kp_2_valueChanged(double arg1);
    void on_Ki_2_valueChanged(double arg1);
    void on_Kd_2_valueChanged(double arg1);
    void on_spinBox_wind_valueChanged(int arg1);
    void on_scale_valueChanged(int arg1);
    void on_x_valueChanged(int value);
    void on_payload_valueChanged(int value);
};

#endif // PID_TEST_H
