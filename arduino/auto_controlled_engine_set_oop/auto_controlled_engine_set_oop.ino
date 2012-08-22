 //For gyroscope
#include <Wire.h>

#define DEBUG_NO_MOTORS
//#define DEBUG_NO_GYROSCOPE
//#define DEBUG_NO_ACCELEROMETER
#define DEBUG_SERIAL
#define DEBUG_SERIAL_HUMAN

const int infoLedPin = 13;
const int SERIAL_ACCURACY = 3;

double double_eps = 1E-2;
const double MPI = 3.141592653589793;

struct RVector3D
{
    RVector3D();
    
    double x, y, z;

    double module_sq();
    double module();
    void norm();
    void set_module();

    RVector3D operator*(double);
    RVector3D operator+(RVector3D);
    RVector3D operator-(RVector3D);
    RVector3D operator-=(RVector3D);
    RVector3D operator+=(RVector3D);
    RVector3D operator/=(double);
    RVector3D operator*=(double);

    double& value_by_axis_index(int index);

    enum print_mode {PRINT_INDEX, PRINT_TAB, PRINT_RAW, PRINT_TAB_2D, PRINT_RAW_2D};
    enum use_axis {USE_2D, USE_3D};
    void print_serial(print_mode, use_axis uaxis = USE_3D);

    void x_angle_inc(double w);
    void y_angle_inc(double w);
    void x_angle_dec(double w);
    void y_angle_dec(double w);
    
    const static double projection_eps = 0.1;
    RVector3D angle_from_projections(); //angle in [-MPI / 2; MPI / 2]
    RVector3D projections_from_angle(double a = 1); //angle in [-MPI; MPI]
};

RVector3D::RVector3D()
{
    x = 0;
    y = 0;
    z = 0;
}

void RVector3D::norm()
{
    double k = 1.0 / sqrt(module_sq());
    x *= k;
    y *= k;
    z *= k;
}

void RVector3D::x_angle_inc(double w)
{
    double old_y = y;
    y =     y * cos(w) - z * sin(w);
    z = old_y * sin(w) + z * cos(w);
}

void RVector3D::x_angle_dec(double w)
{
    x_angle_inc(-w);
}

void RVector3D::y_angle_inc(double w)
{
    double old_x = x;
    x =      x * cos(w) + z * sin(w);
    z = -old_x * sin(w) + z * cos(w);
}

void RVector3D::y_angle_dec(double w)
{
    y_angle_inc(-w);
}

void RVector3D::print_serial(print_mode mode, use_axis uaxis)
{
    unsigned int i;
    for(i = 0; i < 3; i++)
    {
        if(mode == PRINT_INDEX)
        {
            Serial.print(i);
            Serial.print("");
            Serial.print(value_by_axis_index(i), SERIAL_ACCURACY);
            Serial.print("; ");
        }
        else if(mode == PRINT_TAB)
        {
            Serial.print(value_by_axis_index(i), SERIAL_ACCURACY);
            Serial.print("\t");
        }
        else if(mode == PRINT_RAW)
        {
            double t = value_by_axis_index(i);
            if(t > 1) t = 1;
            else if(t < -1) t = -1;
            Serial.write((t + 1) / 2. * 255);
        }
        
        if(uaxis == USE_2D && i == 1) return;
    }
}

double RVector3D::module_sq()
{
    return x * x + y * y + z * z;
}

double RVector3D::module()
{
    return sqrt(module_sq());
}

RVector3D RVector3D::operator-(RVector3D c)
{
    RVector3D result;
    result.x = x - c.x;
    result.y = y - c.y;
    result.z = z - c.z;
    return(result);
}

RVector3D RVector3D::operator+(RVector3D c)
{
    RVector3D result;
    result.x = x + c.x;
    result.y = y + c.y;
    result.z = z + c.z;
    return(result);
}

RVector3D RVector3D::operator*(double c)
{
    RVector3D result;
    result.x = x * c;
    result.y = y * c;
    result.z = z * c;
    return(result);
}

RVector3D RVector3D::operator/=(double c)
{
    x /= c;
    y /= c;
    z /= c;
    return(*this);
}

RVector3D RVector3D::operator*=(double c)
{
    x *= c;
    y *= c;
    z *= c;
    return(*this);
}

RVector3D RVector3D::operator-=(RVector3D b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return(*this);
}

RVector3D RVector3D::operator+=(RVector3D b)
{
    x += b.x;
    y += b.y;
    z += b.z;
    return(*this);
}

double& RVector3D::value_by_axis_index(int index)
{
    switch(index)
    {
    case 0:
        return(x);
    case 1:
        return(y);
    case 2:
        return(z);
    }
}

RVector3D RVector3D::angle_from_projections()
{
    RVector3D result;
    
    result.x = acos(y / sqrt(pow(y, 2) + pow(z, 2))) - MPI / 2;
    result.y = acos(x / sqrt(pow(x, 2) + pow(z, 2))) - MPI / 2;
    result.z = 0;
    
    return(result);
}

RVector3D RVector3D::projections_from_angle(double a)
{
    RVector3D result;
    result.x = 0;
    result.y = 0;
    result.z = 0;

    if(fabs(x - MPI / 2) < double_eps || fabs(y - MPI / 2) < double_eps
     || fabs(x + MPI / 2) < double_eps || fabs(y + MPI / 2) < double_eps)
    {
        result.z = 0;

        if(fabs(y + MPI / 2) < double_eps) result.x = -1;
        if(fabs(y - MPI / 2) < double_eps) result.x = 1;

        if(fabs(x + MPI / 2) < double_eps) result.y = 1;
        if(fabs(x - MPI / 2) < double_eps) result.y = -1;
    }
    else
    {
        result.z = +a / sqrt(1 + pow(tan(x), 2) + pow(tan(y), 2));
        result.x = +result.z * tan(y);
        result.y = -result.z * tan(x);
    }

    if((fabs(y) - double_eps) >= MPI / 2 && (fabs(y) + double_eps) <= MPI) result.x *= -1;
    if((x + double_eps) <= -MPI / 2 || x - (double_eps) >= MPI / 2) result.y *= -1;

    result.x *= -1;
    result.y *= -1;

    return(result);

}

struct Motor
{
    enum EXTREMAL_SPEED 
    {
        MIN_SPEED = 100,
        MAX_SPEED = 254
    };
    int control_pin;
    int speedie;
    inline const int power();
    void makeSpeed(int percentage); 
};

inline const int Motor::power()
{
    return (double) 100 * (speedie - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
}

void Motor::makeSpeed(int percentage)
{
    if (percentage >= 100) speedie = MAX_SPEED;
    else if (percentage <=   0) speedie = MIN_SPEED;
    else speedie = (double) 0.01 * percentage * (MAX_SPEED - MIN_SPEED) + MIN_SPEED;
    
    analogWrite(control_pin, speedie);
}

class MotorController
{
private:

#ifdef DEBUG_NO_MOTORS
    static const int INIT_TIMEOUT = 0; // ms
#else
    static const int INIT_TIMEOUT = 8000; // ms
#endif

    static const int DEF_SPEED_STEP = 400;

    double throttle_abs;
    
    RVector3D accelerometer_xi;
    
    static const double gyroscope_coefficient = -0.375;
    
    static const double joystick_coefficient = 0.2;
    
    static const double min_speed_percent = 10;

    enum SIGN
    {
        ZERO = 0,
        PLUS = 1,
        MINUS = -1
    };

    enum MOTORS
    {
        A, B, C, D, N_MOTORS
    };

    Motor motors_[N_MOTORS];

    bool use_motors[N_MOTORS];

    inline const SIGN x_sign(int i)
    {
        switch (i)
        {
        case A:
            return MINUS;
        case B:
            return ZERO;
        case C:
            return PLUS;
        case D:
            return ZERO;
        default:
            return ZERO;
        }
    }

    inline const SIGN y_sign(int i)
    {
        switch (i)
        {
        case A:
            return ZERO;
        case B:
            return PLUS;
        case C:
            return ZERO;
        case D:
            return MINUS;
        default:
            return ZERO;
        }
    }

public:
    MotorController(const int motor_control_pins[N_MOTORS]);
    ~MotorController();

    void speedChange(RVector3D throttle_vec);
    double speedGet(RVector3D throttle_vec, int motor);
    //void linearSpeedInc(int inc_percent, int speed_step_time);

    inline RVector3D get_joystick_throttle(RVector3D joystick_data)
    {
        RVector3D throttle;
        
        throttle.x = 0;
        throttle.y = 0;
        throttle.z = 1;
        
        throttle.x_angle_inc(joystick_data.x * joystick_coefficient);
        throttle.y_angle_inc(joystick_data.y * joystick_coefficient);
        
        return(throttle);
    }

    RVector3D get_accelerometer_correction(RVector3D angle, RVector3D accel_data)
    {
        RVector3D g = angle.projections_from_angle();
        RVector3D a = accel_data - g;
        
        RVector3D rotation = a.angle_from_projections();
        RVector3D moment_of_force;
        moment_of_force.x = rotation.x * accelerometer_xi.x;
        moment_of_force.y = rotation.y * accelerometer_xi.y;
        
        RVector3D throttle_new;
        throttle_new.z = 1 / sqrt(1 + pow(moment_of_force.x / 2, 2) + pow(moment_of_force.y / 2, 2));
        throttle_new.x =  moment_of_force.y / 2 * throttle_new.z;
        throttle_new.y = -moment_of_force.x / 2 * throttle_new.z;
        
        return(throttle_new);
    }

    inline RVector3D get_gyroscope_correction(RVector3D gyro_data)
    {
        RVector3D correction = gyro_data * gyroscope_coefficient;
        correction.x *= -1;
        
        double t_double;
        t_double = correction.x;
        correction.x = correction.y;
        correction.y = t_double;
        
        return(correction);
    }

    inline double get_throttle_abs()
    {
        return(throttle_abs);
    }

    inline void set_throttle_abs(double a)
    {
        throttle_abs = a;
    }
};

double MotorController::speedGet(RVector3D throttle_vec, int motor)
{
    // This comes from the Cubic Vector Model
    double res = 100 * ( throttle_vec.module_sq() + x_sign(motor) * throttle_vec.x + y_sign(motor) * throttle_vec.y ) / throttle_vec.z;
    
    //it is necessary because the motor controller starts a motor with greater speed than needed
    if(res <= min_speed_percent && throttle_vec.module_sq() != 0) res = min_speed_percent;
    
    return(res);
}

void MotorController::speedChange(RVector3D throttle_vec)
{
    for (int i = 0; i < N_MOTORS; i++)
        motors_[i].makeSpeed(use_motors[i] ? speedGet(throttle_vec, i) : 0);
}

MotorController::MotorController(const int motor_control_pins[N_MOTORS])
{
    accelerometer_xi.x = 1;
    accelerometer_xi.y = 1;
    
    use_motors[A] = 1;
    use_motors[B] = 0;
    use_motors[C] = 1;
    use_motors[D] = 0;
    
    for (int i = 0; i < N_MOTORS; i++)
    {
        pinMode(motor_control_pins[i], OUTPUT);
        motors_[i].control_pin = motor_control_pins[i];
        motors_[i].makeSpeed(0);
    }

    // Ожидание инициализации конроллов моторов + примерно три писка
    delay(INIT_TIMEOUT);

    RVector3D start_throttle;
    start_throttle.x = 0;
    start_throttle.y = 0;
    start_throttle.z = 0;

    throttle_abs = start_throttle.module();

    speedChange(start_throttle);
}

/*
// Увеличить обороты двигателя на inc_percent (за время, кратное speed_step_time``)
void MotorController::linearSpeedInc(int inc_percent, int speed_step_time)
{
    linearSpeedChange(power() + inc_percent, speed_step_time);
}
*/

class Accelerometer
{
private:
    static const unsigned int AXIS = 3, AVG_N = 20;
    double ACCURACY = 1E-2;
    
    static const double adc_aref = 5, adc_maxvalue = 1023;
    double map_a[AXIS], map_b[AXIS];
    
    int ports[AXIS];
    RVector3D defaults;

public:
    Accelerometer(int new_ports[AXIS])
    {
        map_a[0] = 0.8616664408; map_b[0] = 1.5204146893;
        map_a[1] = 0.892143084;  map_b[1] = 1.8134632093;
        map_a[2] = 0.8861390819; map_b[2] = 1.5457698227;
        
        defaults.x = 0;
        defaults.y = 0;
        defaults.z = 0;
        
        unsigned int i;
        for(i = 0; i < AXIS; i++)
        {
            ports[i] = new_ports[i];
        }
        
//        set_defaults();
    }

    RVector3D get_readings(); //in m/s^2 divided by g
    RVector3D get_raw_readings(); //in volts

    void set_defaults()
    {
        defaults = get_readings();
    }
};

RVector3D Accelerometer::get_raw_readings()
{
    unsigned int i;
    RVector3D result;
    result *= 0;
    
    for(i = 0; i < AXIS; i++)
    {
        #ifdef DEBUG_NO_ACCELEROMETER
            result.value_by_axis_index(i) = 0;
        #else
            result.value_by_axis_index(i) = analogRead(ports[i]) * adc_aref / adc_maxvalue;
        #endif
    }
    
    return(result);
}

RVector3D Accelerometer::get_readings()
{
    unsigned int i;
    RVector3D result;
    result.x = 0;
    result.y = 0;
    result.z = 0;
    
    for(i = 0; i < AVG_N; i++)
        result += get_raw_readings();
    
    result /= AVG_N;

    result -= defaults;

    for(i = 0; i < AXIS; i++)
    {
        result.value_by_axis_index(i) = (result.value_by_axis_index(i) - map_b[i]) / map_a[i];

        if(fabs(result.value_by_axis_index(i)) < ACCURACY)
            result.value_by_axis_index(i) = 0;
    }

    return(result);
}

class Gyroscope
{
private:
    // ITG3200 Register Defines
    const static unsigned char WHO = 0x00;
    const static unsigned char SMPL = 0x15;
    const static unsigned char DLPF = 0x16;
    const static unsigned char INT_C = 0x17;
    const static unsigned char INT_S = 0x1A;
    const static unsigned char TMP_H = 0x1B;
    const static unsigned char TMP_L = 0x1C;
    const static unsigned char GX_H = 0x1D;
    const static unsigned char GX_L = 0x1E;
    const static unsigned char GY_H = 0x1F;
    const static unsigned char GY_L = 0x20;
    const static unsigned char GZ_H = 0x21;
    const static unsigned char GZ_L = 0x22;
    const static unsigned char PWR_M = 0x3E;
    const static int GYRO_ADDRESS = 0x68;

    static const double ACCURACY = 1E-1; // in radians / sec.
    static const int AXIS = 3;
    static const double lsb_per_deg_per_sec = 14.375;

public:
    Gyroscope();

    char ITG3200Readbyte(unsigned char address);

    int ITG3200Read(unsigned char addressh, unsigned char addressl);

    RVector3D get_raw_readings();
    RVector3D get_readings();
};

Gyroscope::Gyroscope()
{
    Wire.begin();
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(0x3E);
    Wire.write(0x80);  //send a reset to the device
    Wire.endTransmission(); //end transmission
    
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(0x15);
    Wire.write(0x00);   //sample rate divider
    Wire.endTransmission(); //end transmission
 
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(0x16);
    Wire.write(0x18); // ±2000 degrees/s (default value)
    Wire.endTransmission(); //end transmission
}

char Gyroscope::ITG3200Readbyte(unsigned char address)
{
    char data;
 
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    
    Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        data = Wire.read();
    }
    
    return data;
    
    Wire.endTransmission();
}

int Gyroscope::ITG3200Read(unsigned char addressh, unsigned char addressl)
{
    long int data, t_data;

    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(addressh);
    Wire.endTransmission();
    
    Wire.requestFrom(GYRO_ADDRESS, 1);
    if(Wire.available() > 0)
    {
        t_data = Wire.read();
        data |= t_data << 8;
    }
    
    Wire.beginTransmission(GYRO_ADDRESS);
    Wire.write(addressl);
    Wire.endTransmission();
    
    if(Wire.available() > 0)
    {
        data |= Wire.read();
    }
    
    return data;
}

RVector3D Gyroscope::get_readings()
{
    RVector3D result = get_raw_readings();

    int i;

    for(i = 0; i < AXIS; i++)
    {
        result.value_by_axis_index(i) *= MPI / (180 * lsb_per_deg_per_sec);
        
        if(fabs(result.value_by_axis_index(i)) < ACCURACY)
            result.value_by_axis_index(i) = 0;
    }

    return(result);
}

RVector3D Gyroscope::get_raw_readings()
{
    RVector3D result;
    
    #ifdef DEBUG_NO_GYROSCOPE
    
        result.x = 0;
        result.y = 0;
        result.z = 0;
    
    #else

        result.x = ITG3200Read(GX_H, GX_L);
        result.y = ITG3200Read(GY_H, GY_L);
        result.z = ITG3200Read(GZ_H, GZ_L);
        
    #endif
    
    return(result);
}

class TimerCount
{
    private:
        unsigned long time;
        bool time_isset;
    public:
        TimerCount()
        {
            time_isset = false;
        }
        
        void set_time()
        {
            time = micros();
            time_isset = true;
        }
        
        unsigned long get_time_difference()
        {
            return(micros() - time);
        }
        
        bool get_time_isset()
        {
            return(time_isset);
        }
};

MotorController* MController;
Accelerometer* Accel;
Gyroscope* Gyro;
TimerCount* TCount;

enum reaction_type_ {REACTION_NONE, REACTION_ANGULAR_VELOCITY, REACTION_ACCELERATION};
char reaction_type_names[][8] = {"none", "ang_vel", "accel"};
reaction_type_ reaction_type = REACTION_ANGULAR_VELOCITY;

RVector3D throttle;

RVector3D angle;
const double angle_period = 7.5;

#ifdef DEBUG_SERIAL
    const double serial_gyroscope_coefficient = 0.08;

    enum serial_type_ {SERIAL_DEFAULT, SERIAL_RESTORE};
        
    serial_type_ serial_type = SERIAL_DEFAULT;

    #ifdef DEBUG_SERIAL_HUMAN
        unsigned int serial_auto_count = 0, serial_auto_count_M = 10;
        unsigned int serial_auto_send = 0;
        
        //for wasd and digits
        const double angle_step = 0.024543692; // PI / 128
        const double throttle_step = 0.1;
    #endif
#endif

void setup()
{
    Serial.begin(115200);
    
    pinMode(infoLedPin, OUTPUT);

    // init the global objects
    int motor_control_pins[4] = {3, 9, 10, 11};
    int accelerometer_pins[3] = {A0, A1, A2};
    
    MController = new MotorController(motor_control_pins);
    MController->set_throttle_abs(0);
    
    Accel = new Accelerometer(accelerometer_pins);
    Gyro = new Gyroscope();
    
    TCount = new TimerCount;
    
    throttle.x = 0;
    throttle.y = 0;
    throttle.z = 1;
    
    angle.x = 0;
    angle.y = 0;
    angle.z = 0;
}

void loop()
{
    static unsigned int last_dt, i;
    static double t_double, angle_alpha;
    static long double dt;
    
    RVector3D accel_data = Accel->get_readings();
    RVector3D gyro_data = Gyro->get_readings();

    if(TCount->get_time_isset())
    {
        last_dt = TCount->get_time_difference();
        dt = TCount->get_time_difference();
        dt /= 1.E6;
        
        angle_alpha = dt / (dt + angle_period / (2 * MPI));
        
        RVector3D accel_angle = accel_data.angle_from_projections();
  
        angle.x = angle.x * (1 - angle_alpha) + accel_angle.x * angle_alpha + gyro_data.x * dt;
        angle.y = angle.y * (1 - angle_alpha) + accel_angle.y * angle_alpha + gyro_data.y * dt;
    }
    TCount->set_time();
    
    RVector3D accel_correction = MController->get_accelerometer_correction(angle, accel_data);
    RVector3D gyro_correction = MController->get_gyroscope_correction(gyro_data);

#ifdef DEBUG_SERIAL
    static char c;
    static unsigned int t_low, t_high, t_int;
    static RVector3D throttle_tmp;
    
    c = 0;
    
    if(Serial.available() > 0)
    {
        c = Serial.read();
        
        if(serial_type == SERIAL_DEFAULT)
        {
            if(c == 'n')
            {
                angle.x = 0;
                angle.y = 0;
                angle.z = 0;
                
                throttle.x = 0;
                throttle.y = 0;
                throttle.z = 1;
            }
            else if(c == 'i')
            {                
                serial_type = SERIAL_RESTORE;
                
                int serial_writing;
                
                for(serial_writing = 0; serial_writing < 4; serial_writing++)
                {
                    while(Serial.available() <= 0);
                    c = Serial.read();
                    
                    //high byte
                    if(serial_writing % 2 == 0) t_high = c;
                    else
                    {
                        //recieved low byte;
                        t_low = c;
                        t_int = (t_high << 8) + (t_low & 0xff);
                        
                        t_double = ((double) t_int) / 65535. * 2. - 1;
                        
                        throttle_tmp.value_by_axis_index(serial_writing / 2) = t_double;   
                    }
                }

                throttle = MController->get_joystick_throttle(throttle_tmp);
                
                while(Serial.available() <= 0);
                
                c = Serial.read();
                
                MController->set_throttle_abs(c / 100.);
                
                while(Serial.available() <= 0);
                
                c = Serial.read();
                
                reaction_type = (reaction_type_) (c - '0');
                
            }
            #ifdef DEBUG_SERIAL_HUMAN
                else if(c == '+' && MController->get_throttle_abs() + throttle_step <= 1)
                    MController->set_throttle_abs(MController->get_throttle_abs() + throttle_step);
                    
                else if(c == '-' && MController->get_throttle_abs() - throttle_step >= 0)
                    MController->set_throttle_abs(MController->get_throttle_abs() - throttle_step);
                    
                else if(c >= '0' && c <= '9') MController->set_throttle_abs((c - '0') / 10.0);
                
                else if(c == 'a')
                    throttle.y_angle_inc(angle_step);
                else if(c == 'd')
                    throttle.y_angle_dec(angle_step);
                else if(c == 'w')
                    throttle.x_angle_dec(angle_step);
                else if(c == 's')
                    throttle.x_angle_inc(angle_step);
                else if(c == 't')
                {
                    serial_auto_send = !serial_auto_send;
                    serial_auto_count = 0;
                }
            #endif
        }
    }
#endif

    RVector3D throttle_corrected = throttle;
    
    #ifndef DEBUG_NO_GYROSCOPE
        if(reaction_type == REACTION_ANGULAR_VELOCITY)
            throttle_corrected += gyro_correction;
            
        #ifndef DEBUG_NO_ACCELEROMETER
        
            if(reaction_type == REACTION_ACCELERATION)
                throttle_corrected = accel_correction;
                
        #endif
    #endif
    
    throttle_corrected /= throttle_corrected.module();
    throttle_corrected *= MController->get_throttle_abs();
    
#ifdef DEBUG_SERIAL
    if(serial_type == SERIAL_DEFAULT)
    {
        #ifdef DEBUG_SERIAL_HUMAN
            if(c == 'g' || (serial_auto_send && serial_auto_count == serial_auto_count_M))
            {
                Serial.print(accel_data.module());
                Serial.print("\t");
                accel_data.print_serial(RVector3D::PRINT_TAB);
                
                gyro_data.print_serial(RVector3D::PRINT_TAB);
                throttle.print_serial(RVector3D::PRINT_TAB);
                throttle_corrected.print_serial(RVector3D::PRINT_TAB);
        
                Serial.print(MController->get_throttle_abs(), SERIAL_ACCURACY);
        
                Serial.print("\t");
                
                for(unsigned int i = 0; i < 4; i++)
                {
                    Serial.print(MController->speedGet(throttle_corrected, i));
                    Serial.print("\t");
                }
        
                angle.print_serial(RVector3D::PRINT_TAB, RVector3D::USE_2D);
                
                Serial.print(last_dt / 1.E3);
                
                Serial.print("\t");
                
                Serial.print("R:");
                Serial.print(reaction_type_names[reaction_type]);
                
                Serial.print("\n");
                
                serial_auto_count = 0;
            }
            else serial_auto_count++;
        #endif
        
        if(c == 'p')
        {
            //24 bytes
            throttle_corrected.print_serial(RVector3D::PRINT_RAW);
            angle.print_serial(RVector3D::PRINT_RAW, RVector3D::USE_2D);
            
            (gyro_data * serial_gyroscope_coefficient).print_serial(RVector3D::PRINT_RAW);
            accel_data.print_serial(RVector3D::PRINT_RAW);
            gyro_correction.print_serial(RVector3D::PRINT_RAW, RVector3D::USE_2D);
            accel_correction.print_serial(RVector3D::PRINT_RAW);
            
            for(i = 0; i < 4; i++)
                Serial.write((int) MController->speedGet(throttle_corrected, i));
                
            for(int si = 2; si >= 0; si--)
                Serial.write((last_dt & (0xff << 8 * si)) >> (8 * si));
                
            Serial.write(reaction_type + '0');
        }
    }
#endif

    MController->speedChange(throttle_corrected);
    
    #ifdef DEBUG_SERIAL
        if(serial_type == SERIAL_RESTORE) serial_type = SERIAL_DEFAULT;
    #endif
}
