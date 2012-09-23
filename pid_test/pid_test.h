#ifndef PID_TEST_H
#define PID_TEST_H

#include <QMainWindow>
#include "qtimer.h"
#include "mytime.h"

#define DESKTOP

#include "PID.h"

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
    static const int TIME_SCALE = 10;
    double plot_value[plot_size], plot_x[plot_size], plot_time[plot_size];

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

    PID pid_angular_velocity, pid_angle;

private slots:
    void timer_auto_update();

    void on_pushButton_reset_clicked();
    void on_pushButton_pause_clicked();
    void on_x_valueChanged(double arg1);
    void on_Kp_valueChanged(double arg1);
    void on_Ki_valueChanged(double arg1);
    void on_Kd_valueChanged(double arg1);
    void on_dt_valueChanged(int arg1);
    void on_comboBox_type_currentIndexChanged(int index);
    void on_pushButton_wind_clicked();
    void on_doubleSpinBox_friction_valueChanged(double arg1);
};

#endif // PID_TEST_H
