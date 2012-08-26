#include "Definitions.h"

#ifndef SERIALFUNCTIONS_H
#define SERIALFUNCTIONS_H

#ifdef DEBUG_SERIAL

    #define SERIAL_DEFAULT 0
    #define SERIAL_WAITING 1
    
    #define SERIAL_AUTO_COUNT_M 10        
    
    //for wasd and digits
    #define SERIAL_ANGLE_STEP 0.024543692 // PI / 128
    #define SERIAL_THROTTLE_STEP 0.1
    
    extern int serial_type;
    
    double read_double(double min_value, double max_value, unsigned int bytes);
    void write_double(double min_value, double max_value, double value, unsigned int bytes);
    
    void serial_process_read();
    void serial_process_write();
    void serial_get_command();

#endif

#endif
