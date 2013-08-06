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
    static const unsigned int BufferMax = 70; //maximal buffer size

    bool readError; //read error
    unsigned int readN;

    //buffers
    int bufferCount;
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

    void RVector3DWrite(RVector3D vector, RVector3DPrintMode mode = PRINT_TAB, RVector3DUseAxis uaxis = USE_3D);

    void writeNumber(double value);

    void receiveCommand(); //recieve command and store it into command
    char getCommand();
    void dropCommand();

    bool isSendAutomaticlyEnabled();
    void toggleSendAutomaticly();

    void waitForByte(); //wait MAXWAIT_U useconds for incoming byte
    bool getReadError();

    char read();

    void flush();

    unsigned int bytesAvailable();
};

#endif // MYSERIAL_H
