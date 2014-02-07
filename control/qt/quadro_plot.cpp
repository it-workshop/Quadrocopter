#include "quadro.h"
#include "ui_quadro.h"
#include <sstream>
#include <string>
#include <iostream>
#include <mytime.h>

#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>

#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QtCore/QDebug>

using std::cout;
using std::cerr;
using std::endl;

using std::stringstream;
using std::string;

void Quadro::plot_init()
{
    plot_reset_data();

    ui->plot_gyro->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_gyro->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_gyro->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *gyro_x = new QwtPlotCurve("angular_velocity<sub>x</sub>");
    gyro_x->attach(ui->plot_gyro);

    QwtPlotCurve *gyro_y = new QwtPlotCurve("angular_velocity<sub>y</sub>");
    gyro_y->attach(ui->plot_gyro);

    QwtPlotCurve *gyro_z = new QwtPlotCurve("angular_velocity<sub>z</sub>");
    gyro_z->attach(ui->plot_gyro);

    ui->plot_gyro->setAxisScale(QwtPlot::yLeft, -100, 100);

    // Set curve styles
    gyro_x->setPen(QPen(Qt::red));
    gyro_y->setPen(QPen(Qt::green));
    gyro_z->setPen(QPen(Qt::blue));

    // Attach (don't copy) data.
    gyro_x->setRawData(plot_time, plot_gyro_x, plot_size);
    gyro_y->setRawData(plot_time, plot_gyro_y, plot_size);
    gyro_z->setRawData(plot_time, plot_gyro_z, plot_size);

    ui->plot_gyro->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_gyro->setAxisTitle(QwtPlot::yLeft, "Angular velocity [radians/s]");

    ui->plot_voltage->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_voltage->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_voltage->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    plot_curve_voltage = new QwtPlotCurve("voltage");
    plot_curve_voltage->attach(ui->plot_voltage);

    ui->plot_voltage->setAxisScale(QwtPlot::yLeft, 0, 15);

    // Set curve styles
    plot_curve_voltage->setPen(QPen(Qt::red));

    // Attach (don't copy) data.
    plot_curve_voltage->setRawData(plot_time, plot_voltage, plot_size);

    ui->plot_voltage->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_voltage->setAxisTitle(QwtPlot::yLeft, "Voltage [V]");

    ui->plot_joy->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_joy->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_joy->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *joy_x = new QwtPlotCurve("x");
    joy_x->attach(ui->plot_joy);

    QwtPlotCurve *joy_y = new QwtPlotCurve("y");
    joy_y->attach(ui->plot_joy);

    ui->plot_joy->setAxisScale(QwtPlot::yLeft, -0.5, 0.5);

    // Set curve styles
    joy_x->setPen(QPen(Qt::red));
    joy_y->setPen(QPen(Qt::green));

    // Attach (don't copy) data.
    joy_x->setRawData(plot_time, plot_joy_x, plot_size);
    joy_y->setRawData(plot_time, plot_joy_y, plot_size);

    ui->plot_joy->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_joy->setAxisTitle(QwtPlot::yLeft, "Angle [rad]");

    ui->plot_angle->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_angle->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_angle->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *angle_x = new QwtPlotCurve("angle<sub>x</sub>");
    angle_x->attach(ui->plot_angle);

    QwtPlotCurve *angle_y = new QwtPlotCurve("angle<sub>y</sub>");
    angle_y->attach(ui->plot_angle);

    QwtPlotCurve *angle_accx = new QwtPlotCurve("acc_data_angle<sub>x</sub>");
    angle_accx->attach(ui->plot_angle);

    QwtPlotCurve *angle_accy = new QwtPlotCurve("acc_data_angle<sub>y</sub>");
    angle_accy->attach(ui->plot_angle);

    ui->plot_angle->setAxisScale(QwtPlot::yLeft, -M_PI / 2 * 1.1, M_PI / 2 * 1.1);

    // Set curve styles
    angle_x->setPen(QPen(Qt::red));
    angle_y->setPen(QPen(Qt::green));

    angle_accx->setPen(QPen(Qt::blue));
    angle_accy->setPen(QPen(Qt::yellow));

    // Attach (don't copy) data.
    angle_x->setRawData(plot_time, plot_angle_x, plot_size);
    angle_y->setRawData(plot_time, plot_angle_y, plot_size);

    angle_accx->setRawData(plot_time, plot_angle_accx, plot_size);
    angle_accy->setRawData(plot_time, plot_angle_accy, plot_size);

    ui->plot_angle->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_angle->setAxisTitle(QwtPlot::yLeft, "Angle [radians]");

    ui->plot_torques_and_force->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_torques_and_force->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_torques_and_force->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *torque_x = new QwtPlotCurve("torque<sub>x</sub>");
    torque_x->attach(ui->plot_torques_and_force);

    QwtPlotCurve *torque_y = new QwtPlotCurve("torque<sub>y</sub>");
    torque_y->attach(ui->plot_torques_and_force);

    QwtPlotCurve *torque_z = new QwtPlotCurve("torque<sub>z</sub>");
    torque_z->attach(ui->plot_torques_and_force);

    QwtPlotCurve *force = new QwtPlotCurve("F<sub>ABCD</sub>");
    force->attach(ui->plot_torques_and_force);

    ui->plot_torques_and_force->setAxisScale(QwtPlot::yLeft, -1, 1);
    ui->plot_torques_and_force->setAxisScale(QwtPlot::yRight, -0.25, 0.25);

    // Set curve styles
    torque_x->setPen(QPen(Qt::red));
    torque_y->setPen(QPen(Qt::green));
    torque_z->setPen(QPen(Qt::blue));

    force->setPen(QPen(Qt::black));

    // Attach (don't copy) data.
    torque_x->setRawData(plot_time, plot_torque_x, plot_size);
    torque_y->setRawData(plot_time, plot_torque_y, plot_size);
    torque_z->setRawData(plot_time, plot_torque_z, plot_size);

    force->setRawData(plot_time, plot_force, plot_size);

    ui->plot_torques_and_force->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_torques_and_force->setAxisTitle(QwtPlot::yLeft, "Force [c2 * kg * m / s ^ 2]");
    ui->plot_torques_and_force->setAxisTitle(QwtPlot::yRight, "Torque [c1 * kg * m ^ 2 / s ^ 2]");
    ui->plot_torques_and_force->enableAxis(QwtPlot::yRight);

    ui->plot_PID_x->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_PID_x->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_PID_x->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    ui->plot_PID_y->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_PID_y->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_PID_y->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    ui->plot_PID_z->canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    ui->plot_PID_z->canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);
    ui->plot_PID_z->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    QwtPlotCurve *PID_P_x = new QwtPlotCurve("P");
    PID_P_x->attach(ui->plot_PID_x);

    QwtPlotCurve *PID_I_x = new QwtPlotCurve("I");
    PID_I_x->attach(ui->plot_PID_x);

    QwtPlotCurve *PID_D_x = new QwtPlotCurve("D");
    PID_D_x->attach(ui->plot_PID_x);

    // Insert new curves
    QwtPlotCurve *PID_P_y = new QwtPlotCurve("P");
    PID_P_y->attach(ui->plot_PID_y);

    QwtPlotCurve *PID_I_y = new QwtPlotCurve("I");
    PID_I_y->attach(ui->plot_PID_y);

    QwtPlotCurve *PID_D_y = new QwtPlotCurve("D");
    PID_D_y->attach(ui->plot_PID_y);

    // Insert new curves
    QwtPlotCurve *PID_P_z = new QwtPlotCurve("P");
    PID_P_z->attach(ui->plot_PID_z);

    QwtPlotCurve *PID_I_z = new QwtPlotCurve("I");
    PID_I_z->attach(ui->plot_PID_z);

    QwtPlotCurve *PID_D_z = new QwtPlotCurve("D");
    PID_D_z->attach(ui->plot_PID_z);

    ui->plot_PID_x->setAxisScale(QwtPlot::yLeft, -0.1, 0.1);
    ui->plot_PID_y->setAxisScale(QwtPlot::yLeft, -0.1, 0.1);
    ui->plot_PID_z->setAxisScale(QwtPlot::yLeft, -1, 1);

    // Set curve styles
    PID_P_x->setPen(QPen(Qt::green));
    PID_I_x->setPen(QPen(Qt::blue));
    PID_D_x->setPen(QPen(Qt::red));

    // Set curve styles
    PID_P_y->setPen(QPen(Qt::green));
    PID_I_y->setPen(QPen(Qt::blue));
    PID_D_y->setPen(QPen(Qt::red));

    // Set curve styles
    PID_P_z->setPen(QPen(Qt::green));
    PID_I_z->setPen(QPen(Qt::blue));
    PID_D_z->setPen(QPen(Qt::red));

    // Attach (don't copy) data.
    PID_P_x->setRawData(plot_time, plot_PID_P_x, plot_size);
    PID_I_x->setRawData(plot_time, plot_PID_I_x, plot_size);
    PID_D_x->setRawData(plot_time, plot_PID_D_x, plot_size);

    // Attach (don't copy) data.
    PID_P_y->setRawData(plot_time, plot_PID_P_y, plot_size);
    PID_I_y->setRawData(plot_time, plot_PID_I_y, plot_size);
    PID_D_y->setRawData(plot_time, plot_PID_D_y, plot_size);

    // Attach (don't copy) data.
    PID_P_z->setRawData(plot_time, plot_PID_P_z, plot_size);
    PID_I_z->setRawData(plot_time, plot_PID_I_z, plot_size);
    PID_D_z->setRawData(plot_time, plot_PID_D_z, plot_size);

    ui->plot_PID_x->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_PID_x->setAxisTitle(QwtPlot::yLeft, "Correction");

    ui->plot_PID_y->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_PID_y->setAxisTitle(QwtPlot::yLeft, "Correction");

    ui->plot_PID_z->setAxisTitle(QwtPlot::xBottom, "Time [s]");
    ui->plot_PID_z->setAxisTitle(QwtPlot::yLeft, "Correction");

    QwtPlotMarker *angle_zero = new QwtPlotMarker();
    angle_zero->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    angle_zero->setLineStyle(QwtPlotMarker::HLine);
    angle_zero->setYValue(0.0);
    angle_zero->attach(ui->plot_angle);

    QwtPlotMarker *torques_and_force_zero = new QwtPlotMarker();
    torques_and_force_zero->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    torques_and_force_zero->setLineStyle(QwtPlotMarker::HLine);
    torques_and_force_zero->setYValue(0.0);
    torques_and_force_zero->attach(ui->plot_torques_and_force);

    QwtPlotMarker *gyro_zero = new QwtPlotMarker();
    gyro_zero->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    gyro_zero->setLineStyle(QwtPlotMarker::HLine);
    gyro_zero->setYValue(0.0);
    gyro_zero->attach(ui->plot_gyro);

    QwtPlotMarker *joy_zero = new QwtPlotMarker();
    joy_zero->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    joy_zero->setLineStyle(QwtPlotMarker::HLine);
    joy_zero->setYValue(0.0);
    joy_zero->attach(ui->plot_joy);

    QwtPlotMarker *PID_zero_x = new QwtPlotMarker();
    PID_zero_x->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    PID_zero_x->setLineStyle(QwtPlotMarker::HLine);
    PID_zero_x->setYValue(0.0);
    PID_zero_x->attach(ui->plot_PID_x);

    QwtPlotMarker *PID_zero_y = new QwtPlotMarker();
    PID_zero_y->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    PID_zero_y->setLineStyle(QwtPlotMarker::HLine);
    PID_zero_y->setYValue(0.0);
    PID_zero_y->attach(ui->plot_PID_y);

    QwtPlotMarker *PID_zero_z = new QwtPlotMarker();
    PID_zero_z->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    PID_zero_z->setLineStyle(QwtPlotMarker::HLine);
    PID_zero_z->setYValue(0.0);
    PID_zero_z->attach(ui->plot_PID_z);
}

void Quadro::plot_reset_data()
{
    for(int i = 0; i < plot_size; i++)
    {
        plot_time[i] = 0;

        plot_gyro_x[i] = 0;
        plot_gyro_y[i] = 0;
        plot_gyro_z[i] = 0;

        plot_joy_x[i] = 0;
        plot_joy_y[i] = 0;

        plot_angle_x[i] = 0;
        plot_angle_y[i] = 0;

        plot_angle_accx[i] = 0;
        plot_angle_accy[i] = 0;

        plot_torque_x[i] = 0;
        plot_torque_y[i] = 0;
        plot_torque_z[i] = 0;

        plot_force[i] = 0;

        plot_acceleration_correction_x[i] = 0;
        plot_acceleration_correction_y[i] = 0;
        plot_acceleration_correction_z[i] = 0;

        plot_angular_velocity_correction_x[i] = 0;
        plot_angular_velocity_correction_y[i] = 0;
        plot_angular_velocity_correction_z[i] = 0;

        plot_angle_correction_x[i] = 0;
        plot_angle_correction_y[i] = 0;
        plot_angle_correction_z[i] = 0;

        plot_voltage[i] = 0;

        plot_PID_P_x[i] = 0;
        plot_PID_I_x[i] = 0;
        plot_PID_D_x[i] = 0;

        plot_PID_P_y[i] = 0;
        plot_PID_I_y[i] = 0;
        plot_PID_D_y[i] = 0;

        plot_PID_P_z[i] = 0;
        plot_PID_I_z[i] = 0;
        plot_PID_D_z[i] = 0;
    }
}

void Quadro::plot_update()
{
    int plot_current = plot_size - 1;

    /*if(plot_time[plot_current] == 0)
    {
        plot_mytime.set_time();
        usleep(1000);
    }*/

    number_vect_t dt_seconds = plot_mytime.getTimeDifference() / 1E3;

    //shifting values
    for(int i = 0; i < plot_size - 1; i++)
    {
        plot_time[i] = plot_time[i + 1];

        plot_gyro_x[i] = plot_gyro_x[i + 1];
        plot_gyro_y[i] = plot_gyro_y[i + 1];
        plot_gyro_z[i] = plot_gyro_z[i + 1];

        plot_joy_x[i] = plot_joy_x[i + 1];
        plot_joy_y[i] = plot_joy_y[i + 1];

        plot_angle_x[i] = plot_angle_x[i + 1];
        plot_angle_y[i] = plot_angle_y[i + 1];

        plot_angle_accx[i] = plot_angle_accx[i + 1];
        plot_angle_accy[i] = plot_angle_accy[i + 1];

        plot_torque_x[i] = plot_torque_x[i + 1];
        plot_torque_y[i] = plot_torque_y[i + 1];
        plot_torque_z[i] = plot_torque_z[i + 1];
        plot_force[i] = plot_force[i + 1];

        plot_voltage[i] = plot_voltage[i + 1];

        plot_PID_P_x[i] = plot_PID_P_x[i + 1];
        plot_PID_I_x[i] = plot_PID_I_x[i + 1];
        plot_PID_D_x[i] = plot_PID_D_x[i + 1];

        plot_PID_P_y[i] = plot_PID_P_y[i + 1];
        plot_PID_I_y[i] = plot_PID_I_y[i + 1];
        plot_PID_D_y[i] = plot_PID_D_y[i + 1];

        plot_PID_P_z[i] = plot_PID_P_z[i + 1];
        plot_PID_I_z[i] = plot_PID_I_z[i + 1];
        plot_PID_D_z[i] = plot_PID_D_z[i + 1];
    }

    plot_time[plot_current] = plot_time[plot_current - 1] + dt_seconds;

    //for first run seconds
    for(int i = plot_size - 2; i >= 0; i--)
        if(plot_time[i] == 0) plot_time[i] = plot_time[i + 1] - dt_seconds;

    //gyro
    plot_gyro_x[plot_current] = angular_velocity.x;
    plot_gyro_y[plot_current] = angular_velocity.y;
    plot_gyro_z[plot_current] = angular_velocity.z;

    ui->plot_gyro->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_gyro->replot();

    //acc
    plot_joy_x[plot_current] = joystick_readings.x;
    plot_joy_y[plot_current] = joystick_readings.y;

    ui->plot_joy->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_joy->replot();

    //torque
    plot_torque_x[plot_current] = torque.x * PLOT_TORQUE_COEFF_XY;
    plot_torque_y[plot_current] = torque.y * PLOT_TORQUE_COEFF_XY;
    plot_torque_z[plot_current] = torque.z * PLOT_TORQUE_COEFF_Z;
    plot_force[plot_current] = power;

    ui->plot_torques_and_force->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_torques_and_force->replot();

    //angle
    plot_angle_x[plot_current] = angle.x;
    plot_angle_y[plot_current] = angle.y;

    plot_angle_accx[plot_current] = quadro.get_accelerometer_readings().angle_from_projections().x;
    plot_angle_accy[plot_current] = quadro.get_accelerometer_readings().angle_from_projections().y;

    ui->plot_angle->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_angle->replot();

    //voltage
    plot_voltage[plot_current] = voltage;

    ui->plot_voltage->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_voltage->replot();

    //PID
    plot_PID_P_x[plot_current] = PID_P.x;
    plot_PID_I_x[plot_current] = PID_I.x;
    plot_PID_D_x[plot_current] = PID_D.x;

    plot_PID_P_y[plot_current] = PID_P.y;
    plot_PID_I_y[plot_current] = PID_I.y;
    plot_PID_D_y[plot_current] = PID_D.y;

    plot_PID_P_z[plot_current] = PID_P.z;
    plot_PID_I_z[plot_current] = PID_I.z;
    plot_PID_D_z[plot_current] = PID_D.z;

    ui->plot_PID_x->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_PID_x->replot();

    ui->plot_PID_y->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_PID_y->replot();

    ui->plot_PID_z->setAxisScale(QwtPlot::xBottom, plot_time[0], plot_time[plot_current]);
    ui->plot_PID_z->replot();

    //for dt_seconds
    plot_mytime.setTime();
}
