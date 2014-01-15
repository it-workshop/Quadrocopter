#include "pwm01.h"

uint32_t  pwm_duty = 32767;
uint32_t  pwm_freq1 = 500;

void setup() 
{
    Serial.begin(9600);
    Serial.setTimeout(4);
    
    // Set PWM Resolution
    pwm_set_resolution(16);  

    // Setup PWM Once (Up to two unique frequencies allowed
    //-----------------------------------------------------    
    pwm_setup( 6, pwm_freq1, 1);  // Pin 6 freq set to "pwm_freq1" on clock A
    pwm_setup( 7, pwm_freq1, 1);  // Pin 7 freq set to "pwm_freq2" on clock B
    pwm_setup( 8, pwm_freq1, 1);  // Pin 8 freq set to "pwm_freq2" on clock B
    pwm_setup( 9, pwm_freq1, 1);  // Pin 9 freq set to "pwm_freq2" on clock B
}

void loop() 
{  
    if(Serial.available() > 0)
    {
        pwm_duty = Serial.parseInt() * 256;
        Serial.print(pwm_duty / 256);
        Serial.println(" / 255");
        // Write PWM Duty Cycle Anytime After PWM Setup
        //-----------------------------------------------------    
        pwm_write_duty( 6, pwm_duty );  // 50% duty cycle on Pin 6
        pwm_write_duty( 7, pwm_duty );  // 50% duty cycle on Pin 7
        pwm_write_duty( 8, pwm_duty );  // 50% duty cycle on Pin 8
        pwm_write_duty( 9, pwm_duty );  // 50% duty cycle on Pin 9
    }
}
