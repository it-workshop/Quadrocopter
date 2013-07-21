#include <QTimer>
#include <iostream>
#include <sstream>
#include <fstream>
#include "pid_test.h"
#include "ui_pid_test.h"

#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>

using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;

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

    //pid_angular_velocity.reset();

    pid_angular_velocity.setKpKiKd(10, 8, 0);
    pid_angular_velocity.setYMin(-30);
    pid_angular_velocity.setYMax(30);

    pid_angle.setKpKiKd(50, 0, 70);
    pid_angle.setYMin(-30);
    pid_angle.setYMax(30);

    timer_auto_interval = ui->dt->value();

    friction_coefficient = 0;

    settings_filename = "settings.txt";

    settings_read();

    on_comboBox_type_currentIndexChanged(ui->comboBox_type->currentIndex());
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

    ui->plot_val->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_val->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_val->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *plot_value_ = new QwtPlotCurve("value");
    plot_value_->attach(ui->plot_val);

    QwtPlotCurve *plot_x_ = new QwtPlotCurve("requested");
    plot_x_->attach(ui->plot_val);

    QwtPlotCurve *plot_P_ = new QwtPlotCurve("PID<sub>P</sub>");
    plot_P_->attach(ui->plot);

    QwtPlotCurve *plot_I_ = new QwtPlotCurve("PID<sub>I</sub>");
    plot_I_->attach(ui->plot);

    QwtPlotCurve *plot_D_ = new QwtPlotCurve("PID<sub>D</sub>");
    plot_D_->attach(ui->plot);

    ui->plot->setAxisScale(QwtPlot::yLeft, -10, 10);
    ui->plot_val->setAxisScale(QwtPlot::yLeft, -10, 10);

    // Set curve styles
    plot_value_->setPen(QPen(Qt::black));
    plot_x_->setPen(QPen(Qt::gray));
    plot_P_->setPen(QPen(Qt::green));
    plot_I_->setPen(QPen(Qt::blue));
    plot_D_->setPen(QPen(Qt::red));

    // Attach (don't copy) data.
    plot_value_->setRawData(plot_time, plot_value, plot_size);
    plot_x_->setRawData(plot_time, plot_x, plot_size);
    plot_P_->setRawData(plot_time, plot_P, plot_size);
    plot_I_->setRawData(plot_time, plot_I, plot_size);
    plot_D_->setRawData(plot_time, plot_D, plot_size);

    //ui->plot->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot->setAxisTitle(QwtPlot::yLeft, "PID components");

    QwtPlotMarker *zero1 = new QwtPlotMarker();
    zero1->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    zero1->setLineStyle(QwtPlotMarker::HLine);
    zero1->setYValue(0.0);
    zero1->attach(ui->plot);

    //ui->plot_val->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_val->setAxisTitle(QwtPlot::yLeft, "Value");

    QwtPlotMarker *zero2 = new QwtPlotMarker();
    zero2->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    zero2->setLineStyle(QwtPlotMarker::HLine);
    zero2->setYValue(0.0);
    zero2->attach(ui->plot_val);
}

void PID_test::plot_reset_data()
{
    for(int i = 0; i < plot_size; i++)
    {
        plot_time[i] = 0;

        plot_value[i] = 0;
        plot_x[i] = 0;
        plot_P[i] = 0;
        plot_I[i] = 0;
        plot_D[i] = 0;
    }
}

void PID_test::plot_update()
{
    int plot_current = plot_size - 1;

    long double dt_seconds = plot_mytime.getTimeDifference() / 1E3 * ui->scale->value();

    //shifting values
    for(int i = 0; i < plot_size - 1; i++)
    {
        plot_time[i] = plot_time[i + 1];

        plot_value[i] = plot_value[i + 1];
        plot_x[i] = plot_x[i + 1];
        plot_P[i] = plot_P[i + 1];
        plot_I[i] = plot_I[i + 1];
        plot_D[i] = plot_D[i + 1];
    }

    plot_time[plot_current] = plot_time[plot_current - 1] + dt_seconds;

    //for first run seconds
    for(int i = plot_size - 2; i >= 0; i--)
        if(plot_time[i] == 0) plot_time[i] = plot_time[i + 1] - dt_seconds;

    //gyro
    plot_value[plot_current] = value;
    plot_x[plot_current] = x;
    if(ui->comboBox_type->currentIndex() == 0)
    {
        plot_P[plot_current] = pid_angular_velocity.getLastPID()[0].x;
        plot_I[plot_current] = pid_angular_velocity.getLastPID()[1].x;
        plot_D[plot_current] = pid_angular_velocity.getLastPID()[2].x;
    }
    else
    {
        plot_P[plot_current] = pid_angle.getLastPID()[0].x;
        plot_I[plot_current] = pid_angle.getLastPID()[1].x;
        plot_D[plot_current] = pid_angle.getLastPID()[2].x;
    }

    ui->plot->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot->replot();

    ui->plot_val->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_val->replot();

    //for dt_seconds
    plot_mytime.setTime();

    ui->value->setValue(value * 100);
    ui->diff->setValue(-(ui->x->value() / 100. - value) * 1000);
}

void PID_test::timer_auto_update()
{
    //calculating value
    long double dt = plot_mytime.getTimeDifference() / 1.E3;
    double correction;
    long double value_speed_;

    if(!plot_mytime.isSet())
    {
        plot_mytime.setTime();
    }
    else if(dt)
    {
        for(int i = 0; i < ui->scale->value(); i++)
        {
            value_speed_ = (value - prev_value) / dt;

            if(ui->comboBox_type->currentIndex() == 0)
                correction = pid_angular_velocity.getY(value_speed_, dt).x;
            else
                correction = pid_angle.getY(value, dt).x;

            if(fabs(correction) < ui->mincorr->value())
                correction = 0;

            //payload is an additional force
            correction += ui->payload->value();

            //cerr << "corr=" << correction << endl;

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
        pid_angular_velocity.setKpKiKd(ui->Kp->value(), ui->Ki->value(), ui->Kd->value());
        pid_angular_velocity.setData0(0);
        pid_angular_velocity.reset();
    }
    else
    {
        pid_angle.setKpKiKd(ui->Kp->value(), ui->Ki->value(), ui->Kd->value());
        pid_angle.setData0(ui->x->value() / 100.);
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

void PID_test::on_x_valueChanged(int value)
{
    x = value;
    x /= 100;
    if(ui->comboBox_type->currentIndex() == 1)
        pid_angle.setData0(x);
    settings_write();
}

void PID_test::on_Kp_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        pid_angular_velocity.setKp(arg1);
    else
        pid_angle.setKp(arg1);
    settings_write();
}

void PID_test::on_Ki_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        pid_angular_velocity.setKi(arg1);
    else
        pid_angle.setKi(arg1);
    settings_write();
}

void PID_test::on_Kd_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        pid_angular_velocity.setKd(arg1);
    else
        pid_angle.setKd(arg1);
    settings_write();
}

void PID_test::on_dt_valueChanged(int arg1)
{
    timer_auto_interval = arg1;
    settings_write();
}

void PID_test::on_comboBox_type_currentIndexChanged(int index)
{
    if(index == 0)
    {
        ui->Kp->setValue(pid_angular_velocity.getKp().x);
        ui->Ki->setValue(pid_angular_velocity.getKi().x);
        ui->Kd->setValue(pid_angular_velocity.getKd().x);
        ui->x->setValue(pid_angular_velocity.getData0().x * 100.);

        ui->Kp_2->setValue(pid_angular_velocity.getPMax_x());
        ui->Ki_2->setValue(pid_angular_velocity.getIMax_x());
        ui->Kd_2->setValue(pid_angular_velocity.getDMax_x());
    }
    else
    {
        ui->Kp->setValue(pid_angle.getKp().x);
        ui->Ki->setValue(pid_angle.getKi().x);
        ui->Kd->setValue(pid_angle.getKd().x);
        ui->x->setValue(pid_angle.getData0().x * 100.);
        ui->Kp_2->setValue(pid_angle.getPMax_x());
        ui->Ki_2->setValue(pid_angle.getIMax_x());
        ui->Kd_2->setValue(pid_angle.getDMax_x());
    }
    settings_write();
}

void PID_test::on_pushButton_wind_clicked()
{
    value_speed += ui->spinBox_wind->value();
    settings_write();
}

void PID_test::on_doubleSpinBox_friction_valueChanged(double arg1)
{
    friction_coefficient = arg1;
    settings_write();
}

void PID_test::on_Kp_2_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        //av
        pid_angular_velocity.setPMinMax_x(arg1);
    else
        pid_angle.setPMinMax_x(arg1);
    settings_write();
}

void PID_test::on_Ki_2_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        //av
        pid_angular_velocity.setIMinMax_x(arg1);
    else
        pid_angle.setIMinMax_x(arg1);
    settings_write();
}

void PID_test::on_Kd_2_valueChanged(double arg1)
{
    if(ui->comboBox_type->currentIndex() == 0)
        //av
        pid_angular_velocity.setDMinMax_x(arg1);
    else
        pid_angle.setDMinMax_x(arg1);
    settings_write();
}

void PID_test::settings_write()
{
    stringstream ss;
    ofstream settings_file;

    settings_file.open(settings_filename.c_str());

    ss << ui->comboBox_type->currentIndex() << "\t";

    ss << pid_angular_velocity.getKp().x << "\t";
    ss << pid_angular_velocity.getKi().x << "\t";
    ss << pid_angular_velocity.getKd().x << "\t";

    ss << pid_angular_velocity.getPMax_x() << "\t";
    ss << pid_angular_velocity.getIMax_x() << "\t";
    ss << pid_angular_velocity.getDMax_x() << "\t";

    ss << pid_angular_velocity.getData0().x << "\t";

    ss << pid_angle.getKp().x << "\t";
    ss << pid_angle.getKi().x << "\t";
    ss << pid_angle.getKd().x << "\t";

    ss << pid_angle.getPMax_x() << "\t";
    ss << pid_angle.getIMax_x() << "\t";
    ss << pid_angle.getDMax_x() << "\t";

    ss << pid_angle.getData0().x << "\t";

    ss << ui->dt->value() << "\t";
    ss << ui->spinBox_wind->value() << "\t";
    ss << ui->doubleSpinBox_friction->value() << "\t";

    ss << ui->scale->value() << "\t";
    ss << ui->payload->value() << "\t";
    ss << ui->mincorr->value() << "\t";

    settings_file << ss.str();

    settings_file.close();
}

void PID_test::settings_read()
{
    ifstream settings_file(settings_filename.c_str());

    double p1, i1, d1, mp1, mi1, md1, x1;
    double p2, i2, d2, mp2, mi2, md2, x2;
    double t_double;
    int t_int;

    settings_file >> t_int; ui->comboBox_type->setCurrentIndex(t_int);
    settings_file >> p1 >> i1 >> d1 >> mp1 >> mi1 >> md1 >> x1;
    settings_file >> p2 >> i2 >> d2 >> mp2 >> mi2 >> md2 >> x2;

    pid_angular_velocity.setKpKiKd(p1, i1, d1);
    pid_angle.setKpKiKd(p2, i2, d2);

    pid_angular_velocity.setPMinMax_x(mp1);
    pid_angular_velocity.setIMinMax_x(mi1);
    pid_angular_velocity.setDMinMax_x(md1);

    pid_angle.setPMinMax_x(mp2);
    pid_angle.setIMinMax_x(mi2);
    pid_angle.setDMinMax_x(md2);

    pid_angular_velocity.setData0(x1);
    pid_angle.setData0(x2);

    if(t_int == 0)
    {
        ui->Kp->setValue(p1);
        ui->Ki->setValue(i1);
        ui->Kd->setValue(d1);

        ui->Kp_2->setValue(mp1);
        ui->Ki_2->setValue(mi1);
        ui->Kd_2->setValue(md1);

        ui->x->setValue(x1 * 100.);
    }
    else
    {
        ui->Kp->setValue(p2);
        ui->Ki->setValue(i2);
        ui->Kd->setValue(d2);

        ui->Kp_2->setValue(mp2);
        ui->Ki_2->setValue(mi2);
        ui->Kd_2->setValue(md2);

        ui->x->setValue(x2 * 100.);
    }

    settings_file >> t_int; ui->dt->setValue(t_int);
    settings_file >> t_int; ui->spinBox_wind->setValue(t_int);
    settings_file >> t_double; ui->doubleSpinBox_friction->setValue(t_double);
    settings_file >> t_int; ui->scale->setValue(t_int);
    settings_file >> t_int; ui->payload->setValue(t_int);
    settings_file >> t_int; ui->mincorr->setValue(t_int);

    settings_file.close();
}

void PID_test::on_spinBox_wind_valueChanged(int arg1)
{
    settings_write();
}

void PID_test::on_scale_valueChanged(int arg1)
{
    settings_write();
}

void PID_test::on_payload_valueChanged(int value)
{
    settings_write();
}

void PID_test::on_mincorr_valueChanged(int value)
{
    settings_write();
}
