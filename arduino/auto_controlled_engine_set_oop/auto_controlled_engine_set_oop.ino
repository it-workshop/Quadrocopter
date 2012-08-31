#include <Wire.h>

#include "Definitions.h"

#include "RVector3D.h"
#include "TimerCount.h"
#include "SerialFunctions.h"

#include "Accelerometer.h"
#include "Gyroscope.h"

#include "Motor.h"
#include "MotorController.h"

MotorController* MController;
Accelerometer* Accel;
Gyroscope* Gyro;
TimerCount* TCount;

//reaction type (different types of processing sensors' data)
enum reaction_type_ {REACTION_NONE, REACTION_ANGULAR_VELOCITY, REACTION_ACCELERATION, REACTION_ANGLE};
char reaction_type_names[][8] = {"none", "ang_vel", "accel", "angle"};
reaction_type_ reaction_type = REACTION_NONE;

// distance from gyroscope to the accelerometer in meters
RVector3D gyro_to_acc = RVector3D(-1.9E-2, -1.7E-2, 2.1E-2);

// torque manual correction
RVector3D torque_manual_correction = RVector3D(0, 0, 0);

// torque default value
const RVector3D torque_default = RVector3D(0, 0, 0);

// angle between Earth's coordinate and ours
RVector3D angle;

// period for low-pass filter for accelerometer
double angle_period = 4;

// gravitational acceleration
const double g = 9.80665;

// corrections
RVector3D angular_velocity_correction, acceleration_correction, angle_correction;

void setup()
{
    Serial.begin(115200);
    
    pinMode(infoLedPin, OUTPUT);

    // init the global objects
    int motor_control_pins[4] = {3, 9, 10, 11};
    int accelerometer_pins[3] = {A0, A1, A2};
    
    MController = new MotorController(motor_control_pins);
    MController->set_force(0);
    
    Accel = new Accelerometer(accelerometer_pins);
    Gyro = new Gyroscope();
    
    TCount = new TimerCount;
    
    #ifdef DEBUG_SERIAL
        serial_type = SERIAL_WAITING;
    #endif
}    

/*
   Global Variables
*/
unsigned int last_dt = 0, i = 0;
double angle_alpha = 0;
long double dt = 0;

char c = 0;

// sensors' data
RVector3D accel_data, gyro_data;

RVector3D torque_corrected;

// for angular acceleration
RVector3D gyro_prev_data = RVector3D();

void loop()
{ 
    // data from sensors
    accel_data = Accel->get_readings();
    gyro_data = Gyro->get_readings();

    // on second loop() iteration
    if (TCount->get_time_isset())
    {
        // calculating dt from previous iteration
        last_dt = TCount->get_time_difference();
        dt = TCount->get_time_difference();
        dt /= 1.E6;
        
        // angular acceleration creates unwanted linear acceleration
        RVector3D angular_acceleration = (gyro_data - gyro_prev_data) / dt;
        accel_data.x -= (angular_acceleration.y * gyro_to_acc.z - angular_acceleration.z * gyro_to_acc.y) / g;
        accel_data.y -= (angular_acceleration.z * gyro_to_acc.x - angular_acceleration.x * gyro_to_acc.z) / g;
        accel_data.z -= (angular_acceleration.x * gyro_to_acc.y - angular_acceleration.y * gyro_to_acc.x) / g;
       
        // angle from accel_data
        RVector3D accel_angle = accel_data.angle_from_projections();
        
        // alpha coefficient for low-pass filter
        angle_alpha = dt / (dt + angle_period / (2 * MPI));
  
        // low-pass filter      
        angle.x = (angle.x + gyro_data.x * dt) * (1 - angle_alpha) + accel_angle.x * angle_alpha;
        angle.y = (angle.y + gyro_data.y * dt) * (1 - angle_alpha) + accel_angle.y * angle_alpha;
        
        // sometimes some stuff happen
        for(i = 0; i < 2; i++)
            if (!(angle.value_by_axis_index(i) >= -MPI && angle.value_by_axis_index(i) <= MPI))
                angle.value_by_axis_index(i) = 0;
    }
    TCount->set_time();
    
    // calculating correction (all methods)
    acceleration_correction = MController->get_acceleration_correction(angle, accel_data);
    angular_velocity_correction = MController->get_angular_velocity_correction(gyro_data, dt);
    angle_correction = MController->get_angle_correction(angle, dt);

    #ifdef DEBUG_SERIAL
    
        // reading command, if available
        serial_get_command();
        
        // processing read commands (to arduino)
        serial_process_read();
        
    #endif

    // resulting vector
    torque_corrected = torque_default;
    torque_corrected += torque_manual_correction;
    
    RVector3D t_correction = RVector3D(0, 0, 0);
    
    // applying corrections
    #ifndef DEBUG_NO_GYROSCOPE
    
        // reaction on angular velocity from gyroscope
        if (reaction_type == REACTION_ANGULAR_VELOCITY)
        {
            torque_corrected += angular_velocity_correction;
            t_correction = angular_velocity_correction;
        }
            
        #ifndef DEBUG_NO_ACCELEROMETER
        
            // reaction on acceleration
            if (reaction_type == REACTION_ACCELERATION)
                torque_corrected += acceleration_correction;
                
            // reaction on angle
            else if (reaction_type == REACTION_ANGLE)
            {
                torque_corrected += angle_correction;
                t_correction = angle_correction;
            }
                
        #endif
    #endif
    
    #ifdef DEBUG_SERIAL
    
        //processing write commands (from arduino)
        serial_process_write();
        
    #endif

    // setting speed
    MController->set_torque(torque_corrected);
    
/*    double motors_speed[4];
    
    for(int i = 0; i < 4; i++)
        motors_speed[i] = MController->get_force();

    motors_speed[3] += t_correction.x;
    motors_speed[1] -= t_correction.x;

    motors_speed[2] += t_correction.y;
    motors_speed[0] -= t_correction.y;

    MController->speedChangeRaw(motors_speed);*/

    // for angular acceleration
    
    gyro_prev_data = gyro_data;
}

