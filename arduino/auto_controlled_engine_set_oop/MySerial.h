#include "RVector3D.h"
#include "Arduino.h"
#include "InfoLED.h"

#ifndef MYSERIAL_H
#define MYSERIAL_H

class MySerial
{
private:
    //constants
    const char* Accuracy; //precision
    static const unsigned int MaxwaitU = 100000; //maximal wait time in useconds
    static const unsigned int BufferMax = 300; //maximal buffer size

    bool readError = false; //read error

    //buffers
    int bufferCount = 0;
    uint8_t buffer[BufferMax];
    char command;
    bool commandAvailable;

    bool sendAutomaticly;

    InfoLED led;

public:
    MySerial();

    void bufferInit();
    void bufferWrite();
    void bufferAdd(uint8_t tChar);
    void bufferAdd(char* tArr);
    void bufferWriteN();

    //read-write methods
    void readDouble(double minValue, double maxValue, double& value, unsigned int bytes);
    void writeDouble(double minValue, double maxValue, double value, unsigned int bytes);

    //read-write methods for RVector3D
    enum RVector3DPrintMode {PRINT_TAB, PRINT_RAW}; //print type: TAB (decimal) or RAW (binary)
    enum RVector3DUseAxis {USE_2D, USE_3D}; //axis count

    void RVector3D_write(RVector3D vector, RVector3DPrintMode mode = PRINT_TAB, RVector3DUseAxis uaxis = USE_3D);

    void receiveCommand(); //recieve command and store it into command
    bool isSendAutomaticlyEnabled();
    void toggleSendAutomaticly();

    void waitForByte(); //wait MAXWAIT_U useconds for incoming byte

    char getCommand();
    void dropCommand();

    bool getReadError();

    char read();
};

#endif // MYSERIAL_H
