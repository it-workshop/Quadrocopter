#include "SerialFunctions.h"
#include "RVector3D.h"
#include "MotorController.h"
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Arduino.h"
#include "TimerCount.h"

#ifdef DEBUG_SERIAL

extern unsigned int last_dt, i;
extern double t_double, angle_alpha;
extern char c, reaction_type_names[][8];
extern MotorController* MController;
extern int reaction_type;
extern long double dt;

extern RVector3D accel_data, gyro_data;
extern RVector3D angle_rotation, acceleration_rotation, angular_velocity_rotation;
extern RVector3D throttle_manual_rotation, throttle_corrected, throttle_scaled;
extern RVector3D angle;

const double serial_gyroscope_coefficient = 0.08;
    
int serial_type;

bool serial_read_error = false;

#ifdef DEBUG_SERIAL_HUMAN
    unsigned int serial_auto_count = 0;
    unsigned int serial_auto_send = 0;
#endif

int serial_buffer_count = 0;
uint8_t serial_buffer[SERIAL_BUFFER_MAX];

void serial_buffer_init()
{
    serial_buffer_count = 0;
    for(int i = 0; i < SERIAL_BUFFER_MAX; i++)
        serial_buffer[i] = 0;
}

void serial_buffer_write()
{
    Serial.write(serial_buffer, serial_buffer_count);
}

void serial_buffer_add(uint8_t t_char)
{
    serial_buffer[serial_buffer_count++] = t_char;
}

void write_double(double min_value, double max_value, double value, unsigned int bytes)
{
    //cutting
    if(value > max_value) value = max_value;
    if(value < min_value) value = min_value;
    
    //mapping
    value -= min_value;
    value /= (max_value - min_value);

    //scaling to bytes
    value *= pow(2, 8 * bytes);

    //bytes to send in one int
    unsigned long t_int = value;

    //writing
    unsigned char t_char;
    for(int i = bytes - 1; i >= 0; i--)
    {
        t_char = (t_int >> (8 * i));
        Serial.write((unsigned int) t_char);
    }
}

void read_double(double min_value, double max_value, double& value, unsigned int bytes)
{
    unsigned long long t_int = 0;

    //reading
    unsigned int t_int_curr;
    for(int i = bytes - 1; i >= 0; i--)
    {
        serial_wait_for_byte();
        if(serial_read_error) break;
        
        t_int_curr = Serial.read();
        t_int_curr = t_int_curr << (8 * i);
        
        t_int |= t_int_curr;
    }

    if(serial_read_error) return;

    value = t_int;

    //scaling from bytes
    value /= pow(2, 8 * bytes);

    //mapping
    value *= (max_value - min_value);
    value += min_value;
}

void serial_wait_for_byte()
{
    TimerCount t_count;
    t_count.set_time();
    
    while(Serial.available() <= 0 && t_count.get_time_difference() < SERIAL_MAXWAIT_U) {}
    
    if(t_count.get_time_difference() >= SERIAL_MAXWAIT_U) serial_read_error = true;
}

void write_RVector3D(RVector3D vect, RVector3D_print_mode mode, RVector3D_use_axis uaxis)
{
    unsigned int i;
    for(i = 0; i < 3; i++)
    {
        if(mode == PRINT_INDEX)
        {
            Serial.print(i);
            Serial.print("");
            Serial.print(vect.value_by_axis_index(i), SERIAL_ACCURACY);
            Serial.print("; ");
        }
        else if(mode == PRINT_TAB)
        {
            Serial.print(vect.value_by_axis_index(i), SERIAL_ACCURACY);
            Serial.print("\t");
        }
        else if(mode == PRINT_RAW)
        {
            double t = vect.value_by_axis_index(i);
            if(t > 1) t = 1;
            else if(t < -1) t = -1;
            serial_buffer_add((t + 1) / 2. * 255);
        }
        
        if (uaxis == USE_2D && i == 1) return;
    }
}

void serial_process_write()
{    
     #ifdef DEBUG_SERIAL_HUMAN
        if((c == 'g' && serial_type == SERIAL_DEFAULT) || (serial_auto_send && serial_auto_count == SERIAL_AUTO_COUNT_M))
        {
            Serial.print(accel_data.module());
            Serial.print("\t");
            write_RVector3D(accel_data, PRINT_TAB);
            write_RVector3D(gyro_data, PRINT_TAB);
            
            write_RVector3D(throttle_manual_rotation, PRINT_TAB);
            write_RVector3D(throttle_scaled, PRINT_TAB);
    
            Serial.print(MController->get_throttle_abs(), SERIAL_ACCURACY);
    
            Serial.print("\t");
            
            for(unsigned int i = 0; i < 4; i++)
            {
                Serial.print(MController->speedGet(throttle_scaled, i));
                Serial.print("\t");
            }
    
            write_RVector3D(angle, PRINT_TAB, USE_2D);
            
            Serial.print(last_dt / 1.E3);
            
            Serial.print("\t");
            
            Serial.print("R:");
            Serial.print(reaction_type_names[reaction_type]);
            
            Serial.print("\n");
            
            serial_auto_count = 0;
            
            serial_type = SERIAL_WAITING;
        }
        else serial_auto_count++;
    #endif
    
    if(serial_type == SERIAL_DEFAULT)
    {
        if(c == 'p')
        {
            #ifdef DEBUG_SERIAL_HUMAN
                serial_auto_send = 0;
            #endif
            
            serial_type = SERIAL_WAITING;
            
            // reading
            
            // throttle_manual_rotation
            for(int i = 0; i < 2; i++)
                read_double(-1, 1, throttle_manual_rotation.value_by_axis_index(i), 2);
            
            //throttle_abs
            serial_wait_for_byte();
            if(serial_read_error) return;
            
            c = Serial.read();
            
            MController->set_throttle_abs(c / 100.);
            
            //reaction_type
            serial_wait_for_byte();
            if(serial_read_error) return;
            
            c = Serial.read();
            
            reaction_type = c - '0';
            
            //PID angle coefficients
            read_double(-1.5, 1.5, MController->angle_Kp, 2);
            read_double(-1.5, 1.5, MController->angle_Ki, 2);
            read_double(-1.5, 1.5, MController->angle_Kd, 2);
            
            //PID angular velocity coefficients
            read_double(-1.5, 1.5, MController->angular_velocity_Kp, 2);
            read_double(-1.5, 1.5, MController->angular_velocity_Ki, 2);
            read_double(-1.5, 1.5, MController->angular_velocity_Kd, 2);
            
            serial_buffer_init();
            
            // writing 26 bytes
            
            write_RVector3D(throttle_corrected, PRINT_RAW);
            write_RVector3D(angle, PRINT_RAW, USE_2D);
            
            write_RVector3D((gyro_data * serial_gyroscope_coefficient), PRINT_RAW);
            write_RVector3D(accel_data, PRINT_RAW);
            write_RVector3D(angular_velocity_rotation, PRINT_RAW, USE_2D);
            write_RVector3D(acceleration_rotation, PRINT_RAW);
            write_RVector3D(angle_rotation, PRINT_RAW, USE_2D);
            
            //motors
            for (i = 0; i < 4; i++)
                serial_buffer_add(MController->speedGet(throttle_scaled, i));
                
            //dt
            for (int si = 2; si >= 0; si--)
                serial_buffer_add((last_dt & (0xff << 8 * si)) >> (8 * si));
                
            //reaction type
            serial_buffer_add(reaction_type + '0');
            
            serial_buffer_write();
        }
    }
}

void serial_process_read()
{
    if(serial_type == SERIAL_DEFAULT)
    {
        if(c == 'n')
        {
            angle = RVector3D();
            throttle_manual_rotation = RVector3D(0, 0, 1);
            
            serial_type = SERIAL_WAITING;
        }
        #ifdef DEBUG_SERIAL_HUMAN
            else if(c == '+' && MController->get_throttle_abs() + SERIAL_THROTTLE_STEP <= 1)
            {
                MController->set_throttle_abs(MController->get_throttle_abs() + SERIAL_THROTTLE_STEP);
                
                serial_type = SERIAL_WAITING;
            }
                
            else if(c == '-' && MController->get_throttle_abs() - SERIAL_THROTTLE_STEP >= 0)
            {
                MController->set_throttle_abs(MController->get_throttle_abs() - SERIAL_THROTTLE_STEP);
                
                serial_type = SERIAL_WAITING;
            }

            else if(c >= '0' && c <= '9')
            {
                MController->set_throttle_abs((c - '0') / 10.0);
                
                serial_type = SERIAL_WAITING;
            }
            
            else if(c == 'a')
            {
                throttle_manual_rotation.y += SERIAL_ANGLE_STEP;
                
                serial_type = SERIAL_WAITING;
            }
            else if(c == 'd')
            {
                throttle_manual_rotation.y -= SERIAL_ANGLE_STEP;
                
                serial_type = SERIAL_WAITING;
            }
            else if(c == 'w')
            {
                throttle_manual_rotation.x -= SERIAL_ANGLE_STEP;
                
                serial_type = SERIAL_WAITING;
            }
            else if(c == 's')
            {
                throttle_manual_rotation.x += SERIAL_ANGLE_STEP;
                
                serial_type = SERIAL_WAITING;
            }
            
            else if(c == 't')
            {
                serial_auto_send = !serial_auto_send;
                serial_auto_count = 0;
                
                serial_type = SERIAL_WAITING;
            }
        #endif
    }
}

void serial_get_command()
{
    if(Serial.available() > 0)
    {
        c = Serial.read();
        serial_type = SERIAL_DEFAULT;
        
        serial_read_error = false;
        
        digitalWrite(infoLedPin, HIGH);
    }
    else
    {
        serial_type = SERIAL_WAITING;
        digitalWrite(infoLedPin, LOW);
    }
}

#endif
