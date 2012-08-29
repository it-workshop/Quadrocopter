#include "Definitions.h"
#include "Arduino.h"
#include "RVector3D.h"

#ifndef SERIALFUNCTIONS_H
#define SERIALFUNCTIONS_H

#ifdef DEBUG_SERIAL

    #define SERIAL_DEFAULT 0
    #define SERIAL_WAITING 1
    
    #define SERIAL_AUTO_COUNT_M 10        
    
    //for wasd and digits
    #define SERIAL_ANGLE_STEP 0.024543692 // PI / 128
    #define SERIAL_THROTTLE_STEP 0.1
    
    #define SERIAL_MAXWAIT_U 100000
    
    #define SERIAL_BUFFER_MAX 100
    void serial_buffer_init();
    void serial_buffer_write();
    void serial_buffer_add(uint8_t t_char);
    
    extern int serial_type;
    
    void read_double(double min_value, double max_value, double& value, unsigned int bytes);
    void write_double(double min_value, double max_value, double value, unsigned int bytes);
    
    enum RVector3D_print_mode {PRINT_INDEX, PRINT_TAB, PRINT_RAW, PRINT_TAB_2D, PRINT_RAW_2D};
    enum RVector3D_use_axis {USE_2D, USE_3D};
    
    void write_RVector3D(RVector3D vector, RVector3D_print_mode mode = PRINT_TAB, RVector3D_use_axis uaxis = USE_3D);
    
    void serial_process_read();
    void serial_process_write();
    void serial_get_command();
    
    void serial_wait_for_byte();

#endif

#endif
