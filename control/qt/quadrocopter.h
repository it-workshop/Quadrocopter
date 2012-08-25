#ifndef QUADROCOPTER_H
#define QUADROCOPTER_H

#include <serial.h>
#include <vect.h>
#include <vector>

using std::vector;

class quadrocopter: public serial
{
public:
    enum reaction_type_ {REACTION_NONE, REACTION_ANGULAR_VELOCITY, REACTION_ACCELERATION};
private:
    number_vect_t power;
    vect angle, throttle_corrected;
    vect throttle_rotation;
    vect gyroscope_readings, throttle_gyroscope_correction;
    vect accelerometer_readings, throttle_accelerometer_correction;
    vect acceleration;

    static const int MOTORS_N = 4;
    number_vect_t MOTORS[MOTORS_N];

    static const double serial_gyroscope_coefficient = 0.08;

    reaction_type_ reaction_type;

    number_vect_t read_time, write_time, loop_time;

    static const double g = 9.80665;

    number_vect_t connect_delay_arduino;

    void defaults();

public:

    quadrocopter();

    number_vect_t get_power(); // returns power [0...1]
    vect get_throttle_corrected(); // returns vect throttle_corrected
    vect get_throttle_rotation(); // returns vect throttle_rotation
    int get_motors_n(); // returns MOTORS_N
    number_vect_t get_motor_power(int); // returns motor power [0...1]
    vect get_gyroscope_readings(); // returns gyroscope readings, values [0...1]
    vect get_accelerometer_readings(); // returns acceleromter readings, values [0...]
    vect get_angle(); //returns angle, values [0...1]

    vect get_throttle_gyroscope_correction();
    vect get_throttle_accelerometer_correction();

    void set_power(number_vect_t);
    void set_throttle_rotation(vect);

    void connect();
    void disconnect();

    number_vect_t get_read_time(); //last read time in sec
    number_vect_t get_write_time(); //last write time in sec
    number_vect_t get_loop_time(); //last loop time in sec

    reaction_type_ get_reaction_type();
    void set_reaction_type(reaction_type_);

    void read_data(); // read data from device
    void write_data(); // write data to device
    void reset_throttle(); // set throttle to (0, 0, 1)
};

#endif // QUADROCOPTER_H
