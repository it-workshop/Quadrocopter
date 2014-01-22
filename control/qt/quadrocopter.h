#ifndef QUADROCOPTER_H
#define QUADROCOPTER_H

#include <serial.h>
#include <vect.h>
#include <vector>
#include "Definitions.h"

#define BN 60

using std::vector;

class quadrocopter: public serial
{
public:
    enum reaction_type_ {REACTION_NONE, REACTION_ANGULAR_VELOCITY, REACTION_ACCELERATION, REACTION_ANGLE};
private:
    number_vect_t power;
    vect angle, torque_corrected;
    vect gyroscope_readings, accelerometer_readings;

    vect torque_manual_correction;

    static const int MOTORS_N = 4;
    number_vect_t MOTORS[MOTORS_N];

    reaction_type_ reaction_type;

    number_vect_t read_time, write_time, loop_time;

    static const double g = 9.80665;

    number_vect_t connect_delay_arduino;

    vect PID_angle_Kp, PID_angle_Ki, PID_angle_Kd;
    vect PID_angle_MAXp, PID_angle_MAXi, PID_angle_MAXd;

    number_vect_t voltage;
    static const number_vect_t voltage_min = 8.0;
    static const number_vect_t voltage_max = 12.6;

    vect PID_P, PID_I, PID_D;

    bool newDataAvailable;

    number_vect_t copter_heading, joystick_heading;

    bool force_override;
    double force_override_value;

    void defaults();

    void read_data(); // read data from device

public:

    quadrocopter();

    number_vect_t get_power(); // returns power [0...1]
    vect get_torque_corrected(); // returns vect torque_corrected
    vect get_torque_manual_correction(); // returns vect torque_manual_correction
    int get_motors_n(); // returns MOTORS_N
    number_vect_t get_motor_power(int); // returns motor power [0...1]
    vect get_gyroscope_readings(); // returns gyroscope readings, values [0...1]
    vect get_accelerometer_readings(); // returns acceleromter readings, values [0...]
    vect get_angle(); //returns angle, values [0...1]

    void set_PID_angle_Kp_x(number_vect_t);
    void set_PID_angle_Ki_x(number_vect_t);
    void set_PID_angle_Kd_x(number_vect_t);

    void set_PID_angle_Kp_y(number_vect_t);
    void set_PID_angle_Ki_y(number_vect_t);
    void set_PID_angle_Kd_y(number_vect_t);

    void set_PID_angle_Kp_z(number_vect_t);
    void set_PID_angle_Ki_z(number_vect_t);
    void set_PID_angle_Kd_z(number_vect_t);

    void set_PID_angle_MAXp_x(number_vect_t);
    void set_PID_angle_MAXi_x(number_vect_t);
    void set_PID_angle_MAXd_x(number_vect_t);

    void set_PID_angle_MAXp_y(number_vect_t);
    void set_PID_angle_MAXi_y(number_vect_t);
    void set_PID_angle_MAXd_y(number_vect_t);

    void set_PID_angle_MAXp_z(number_vect_t);
    void set_PID_angle_MAXi_z(number_vect_t);
    void set_PID_angle_MAXd_z(number_vect_t);

    vect get_PID_angle_Kp();
    vect get_PID_angle_Ki();
    vect get_PID_angle_Kd();

    vect get_PID_angle_MAXp();
    vect get_PID_angle_MAXi();
    vect get_PID_angle_MAXd();

    // Live PID values from Arduino
    vect get_PID_P();
    vect get_PID_I();
    vect get_PID_D();

    void do_connect();
    void do_disconnect();

    number_vect_t get_read_time(); //last read time in sec
    number_vect_t get_write_time(); //last write time in sec
    number_vect_t get_loop_time(); //last loop time in sec

    number_vect_t get_voltage();
    number_vect_t get_voltage_percent();

    reaction_type_ get_reaction_type();
    void set_reaction_type(reaction_type_);

    void initiate_transmission(); // initiate transmission
    void write_data(); // write data to device

    void reset(); // set torque to (0, 0, 1)

    bool getNewDataAvailable();
    void resetNewDataAvailable();

    number_vect_t get_copter_heading();
    number_vect_t get_joystick_heading();

    void set_force_override(bool _do, double _value);

    virtual void on_rx();

    double getPower();
};

#endif // QUADROCOPTER_H
