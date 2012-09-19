#include "MySerial.h"
#include "TimerCount.h"
#include "Arduino.h"

MySerial::MySerial()
{
    Serial.begin(115200);

    led = InfoLED(13);
    commandAvailable = false;
    readError = false;
}

void MySerial::bufferInit()
{
    bufferCount = 0;
    for(int i = 0; i < BufferMax; i++)
        buffer[i] = 0;
}

void MySerial::bufferWrite()
{
    Serial.write(buffer, bufferCount);
}

void MySerial::bufferAdd(uint8_t t_char)
{
    buffer[bufferCount++] = t_char;
}

void MySerial::writeDouble(double min_value, double max_value, double value, unsigned int bytes)
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
    for(int i = bytes - 1; i >= 0; i--)
        bufferAdd(t_int >> (8 * i));
}

void MySerial::readDouble(double min_value, double max_value, double& value, unsigned int bytes)
{
    unsigned long long t_int = 0;

    //reading
    unsigned int t_int_curr;
    for(int i = bytes - 1; i >= 0; i--)
    {
        waitForByte();
        if(readError) break;

        t_int_curr = Serial.read();
        t_int_curr = t_int_curr << (8 * i);

        t_int |= t_int_curr;
    }

    if(readError) return;

    value = t_int;

    //scaling from bytes
    value /= pow(2, 8 * bytes);

    //mapping
    value *= (max_value - min_value);
    value += min_value;
}

void MySerial::waitForByte()
{
    TimerCount t_count;
    t_count.setTime();

    while(Serial.available() <= 0 && t_count.getTimeDifference() < MaxwaitU) {}

    if(t_count.getTimeDifference() >= MaxwaitU) readError = true;
}

char MySerial::getCommand()
{
    if(commandAvailable) return(command);
    else return(0);
}

void MySerial::dropCommand()
{
    commandAvailable = false;
}

bool MySerial::getReadError()
{
    return(readError);
}

char MySerial::read()
{
    return(Serial.read());
}

void MySerial::RVector3D_write(RVector3D vect, RVector3DPrintMode mode, RVector3DUseAxis uaxis)
{
    unsigned int i;
    for(i = 0; i < 3; i++)
    {
        if(mode == PRINT_TAB)
        {
            Serial.print(vect.value_by_axis_index(i), Accuracy);
            Serial.print("\t");
        }
        else if(mode == PRINT_RAW)
            writeDouble(-1, 1, vect.value_by_axis_index(i), 1);

        if (uaxis == USE_2D && i == 1) return;
    }
}

void MySerial::receiveCommand()
{
    if(Serial.available() > 0)
    {
        command = Serial.read();
        commandAvailable = true;

        readError = false;

        led.setOn();
    }
    else
    {
        led.setOff();
        commandAvailable = false;
    }
}
