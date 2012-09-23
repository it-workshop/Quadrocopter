#include <QTimer>
#include <iostream>
#include "pid_test.h"
#include "ui_pid_test.h"

#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>

using std::cerr;
using std::endl;

PID_test::PID_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PID_test)
{
    ui->setupUi(this);

    connect(&timer_auto, SIGNAL(timeout()), this, SLOT(timer_auto_update()));

    plot_init();

    //timer_auto.start(timer_auto_interval);

    value = 0;
    x = 0;

    pid_angular_velocity.set_KpKiKd(10, 8, 0);
    pid_angular_velocity.set_y_min(-30);
    pid_angular_velocity.set_y_max(30);

    pid_angle.set_KpKiKd(50, 0, 70);
    pid_angle.set_y_min(-30);
    pid_angle.set_y_max(30);

    timer_auto_interval = ui->dt->value();

    friction_coefficient = 0;

    on_comboBox_type_currentIndexChanged(0);
}

PID_test::~PID_test()
{
    delete ui;
}

void PID_test::plot_init()
{
    plot_reset_data();

    ui->plot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *plot_value_ = new QwtPlotCurve("value");
    plot_value_->attach(ui->plot);

    QwtPlotCurve *plot_x_ = new QwtPlotCurve("requested");
    plot_x_->attach(ui->plot);

    ui->plot->setAxisScale(QwtPlot::yLeft, -10, 10);

    // Set curve styles
    plot_value_->setPen(QPen(Qt::red));
    plot_x_->setPen(QPen(Qt::green));

    // Attach (don't copy) data.
    plot_value_->setRawData(plot_time, plot_value, plot_size);
    plot_x_->setRawData(plot_time, plot_x, plot_size);

    ui->plot->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot->setAxisTitle(QwtPlot::yLeft, "");
}

void PID_test::plot_reset_data()
{
    for(int i = 0; i < plot_size; i++)
    {
        plot_time[i] = 0;

        plot_value[i] = 0;
        plot_x[i] = 0;
    }
}

void PID_test::plot_update()
{
    int plot_current = plot_size - 1;

    long double dt_seconds = plot_mytime.get_time_difference() / 1E3 * TIME_SCALE;

    //shifting values
    for(int i = 0; i < plot_size - 1; i++)
    {
        plot_time[i] = plot_time[i + 1];

        plot_value[i] = plot_value[i + 1];
        plot_x[i] = plot_x[i + 1];
    }

    plot_time[plot_current] = plot_time[plot_current - 1] + dt_seconds;

    //for first run seconds
    for(int i = plot_size - 2; i >= 0; i--)
        if(plot_time[i] == 0) plot_time[i] = plot_time[i + 1] - dt_seconds;

    //gyro
    plot_value[plot_current] = value;
    plot_x[plot_current] = x;

    ui->plot->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot->replot();

    //for dt_seconds
    plot_mytime.set_time();
}

void PID_test::timer_auto_update()
{
    //calculating value
    long double dt = plot_mytime.get_time_difference() / 1.E3;
    double correction;
    long double value_speed_;

    if(!plot_mytime.is_set())
    {
        plot_mytime.set_time();
    }
    else if(dt)
    {
        for(int i = 0; i < TIME_SCALE; i++)
        {
            value_speed_ = (value - prev_value) / dt;

            if(ui->comboBox_type->currentIndex() == 0)
                correction = pid_angular_velocity.get_y(value_speed_, dt).x;
            else
                correction = pid_angle.get_y(value, dt).x;

            //assuming that the acceleration (correction) is constant
            value += value_speed * dt + correction * dt * dt / 2;

            //motors force
            value_speed += correction * dt;

            //Fdt=dp
            //Fdt/m=dv
            //value_speed *= dt;

            value_speed += -friction_coefficient * dt * value_speed;
            //cerr << "velocity = " << value_speed << " deltav = " << dt * value_speed << endl;
        }
        plot_update();
    }

    prev_value = value;
}

void PID_test::on_pushButton_reset_clicked()
{
    value = 0;
    value_speed = 0;
    plot_mytime.reset();
    plot_reset_data();
    timer_auto_interval = ui->dt->value();
    if(ui->comboBox_type->currentIndex() == 0)
    {
        pid_angular_velocity.set_KpKiKd(ui->Kp->value(), ui->Ki->value(), ui->Kd->value());
        pid_angular_velocity.set_data0(0);
        pid_angular_velocity.reset();
    }
    else
    {
        pid_angle.set_KpKiKd(ui->Kp->value(), ui->Ki->value(), ui->Kd->value());
        pid_angle.set_data0(ui->x->value());
        pid_angle.reset();
    }
}

void PID_test::on_pushButton_pause_clicked()
{
    if(timer_auto.isActive())
    {
        timer_auto.stop();
        ui->pushButton_pause->setText("Start");
    }
    else
    {
        plot_reset_data();
        plot_mytime.reset();
        ui->pushButton_pause->setText("Pause");
        timer_auto.start(timer_auto_interval);
    }
}

void PID_test::on_x_valueChanged(double arg1)
{
    x = arg1;
    if(ui->comboBox_type->currentIndex() == 1)
        pid_angle.set_data0(arg1);
}

void PID_test::on_Kp_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        pid_angular_velocity.set_Kp(arg1);
    else
        pid_angle.set_Kp(arg1);
}

void PID_test::on_Ki_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        pid_angular_velocity.set_Ki(arg1);
    else
        pid_angle.set_Ki(arg1);
}

void PID_test::on_Kd_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        pid_angular_velocity.set_Kd(arg1);
    else
        pid_angle.set_Kd(arg1);
}

void PID_test::on_dt_valueChanged(int arg1)
{
    timer_auto_interval = arg1;
}

void PID_test::on_comboBox_type_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->Kp->setValue(pid_angular_velocity.get_Kp().x);
        ui->Ki->setValue(pid_angular_velocity.get_Ki().x);
        ui->Kd->setValue(pid_angular_velocity.get_Kd().x);
        ui->x->setValue(pid_angular_velocity.get_data0().x);
    }
    else
    {
        ui->Kp->setValue(pid_angle.get_Kp().x);
        ui->Ki->setValue(pid_angle.get_Ki().x);
        ui->Kd->setValue(pid_angle.get_Kd().x);
        ui->x->setValue(pid_angle.get_data0().x);
    }
}

void PID_test::on_pushButton_wind_clicked()
{
    value_speed += ui->spinBox_wind->value();
}

void PID_test::on_doubleSpinBox_friction_valueChanged(double arg1)
{
    friction_coefficient = arg1;
}
