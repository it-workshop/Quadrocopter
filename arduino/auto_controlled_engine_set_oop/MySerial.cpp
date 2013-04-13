#include "MySerial.h"
#include "stdio.h"
#include "TimerCount.h"
#include "Arduino.h"

MySerial::MySerial()
{
    Serial.begin(115200);

    led = InfoLED(5);
    commandAvailable = false;
    readError = false;

    Accuracy = "%f";

    sendAutomaticly = false;
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

void MySerial::bufferAdd(uint8_t tChar)
{
    buffer[bufferCount++] = tChar;
}

void MySerial::bufferAdd(char *tArr)
{
    for(unsigned i = 0; tArr[i]; i++)
        bufferAdd((uint8_t) tArr[i]);
}

void MySerial::bufferWriteN()
{
    uint8_t buffer1[BufferMax + 1];
    buffer1[0] = bufferCount;

    for(int i = 0; i < bufferCount; i++)
        buffer1[i + 1] = buffer[i];

    Serial.write(buffer1, bufferCount + 1);
}

void MySerial::writeDouble(double minValue, double max_value, double value, unsigned int bytes)
{
    //cutting
    if(value > max_value) value = max_value;
    if(value < minValue) value = minValue;

    //mapping
    value -= minValue;
    value /= (max_value - minValue);

    //scaling to bytes
    value *= pow(2, 8 * bytes);

    //bytes to send in one int
    unsigned long t_int = value;

    //writing
    for(int i = bytes - 1; i >= 0; i--)
        bufferAdd(t_int >> (8 * i));
}

void MySerial::readDouble(double minValue, double maxValue, double& value, unsigned int bytes)
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
    value *= (maxValue - minValue);
    value += minValue;
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

void MySerial::flush()
{
    while(Serial.available())
        Serial.read();
}

unsigned int MySerial::bytesAvailable()
{
    return(Serial.available());
}

void MySerial::RVector3DWrite(RVector3D vect, RVector3DPrintMode mode, RVector3DUseAxis uaxis)
{
    unsigned int i;
    char x[BufferMax];
    for(i = 0; i < 3; i++)
    {
        if(mode == PRINT_TAB)
        {
            sprintf(x, "%d", (int) vect.valueByAxisIndex(i));
            bufferAdd(x);
            bufferAdd('\t');
        }
        else if(mode == PRINT_RAW)
            writeDouble(-10, 10, vect.valueByAxisIndex(i), 2);

        if(uaxis == USE_2D && i == 1) return;
    }
}

void MySerial::writeNumber(double value)
{
    char x[BufferMax];
    sprintf(x, "%d", (int) value);
    bufferAdd(x);
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
        if(!sendAutomaticly) led.setOff();
        commandAvailable = false;
    }
}

bool MySerial::isSendAutomaticlyEnabled()
{
    return(sendAutomaticly);
}

void MySerial::toggleSendAutomaticly()
{
    sendAutomaticly ^= 1;
    led.setState(sendAutomaticly);
}
