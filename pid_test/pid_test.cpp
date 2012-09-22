#include <QTimer>
#include "pid_test.h"
#include "ui_pid_test.h"

#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>

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

    mypid.set_KpKiKd(0, 0, 0);
    mypid.set_y_min(-1);
    mypid.set_y_max(1);

    timer_auto_interval = ui->dt->value();
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

    double dt_seconds = plot_mytime.get_time_difference() / 1E3;

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
    double dt = plot_mytime.get_time_difference() / 1.E3;
    double correction;

    if(!plot_mytime.is_set())
    {
        plot_mytime.set_time();
    }
    else
    {
        for(int i = 0; i < 10; i++)
        {
            correction = mypid.get_y(value, dt).x;

            value_speed += correction * dt;

            value += value_speed * dt;
        }

        plot_update();
    }
}

void PID_test::on_pushButton_reset_clicked()
{
    value = 0;
    value_speed = 0;
    plot_mytime.reset();
    plot_reset_data();
    mypid.set_KpKiKd(ui->Kp->value(), ui->Ki->value(), ui->Kd->value());
    mypid.set_data0(ui->x->value());
    mypid.reset();
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
    mypid.set_data0(arg1);
}

void PID_test::on_Kp_valueChanged(double arg1)
{
    mypid.set_Kp(arg1);
}

void PID_test::on_Ki_valueChanged(double arg1)
{
    mypid.set_Ki(arg1);
}

void PID_test::on_Kd_valueChanged(double arg1)
{
    mypid.set_Kd(arg1);
}

void PID_test::on_dt_valueChanged(int arg1)
{
    timer_auto_interval = arg1;
}
