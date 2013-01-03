#include "serial.h"
#include <iostream>
#include <map>
#include <mytime.h>

#include "qextserialport.h"
#include <QDebug>

using std::cerr;
using std::cout;
using std::endl;

using std::map;

serial::serial()
{
    rate_map[9600] = BAUD9600;
    rate_map[57600] = BAUD57600;
    rate_map[115200] = BAUD115200;

    port = new QextSerialPort;

    device = "";
    rate = 9600;
    maxwait = 100;

    serror = false;

    connect_delay_time = 0;
}

vect serial::read_vect_byte(unsigned int axis)
{
    vect result;
    for(unsigned int i = 0; i < axis; i++)
        result.value_by_axis_index(i) = read_number_vect_t(-10, 10, 2);
    if(read_error()) result = vect();

    return(result);
}

void serial::write_number_vect_t(number_vect_t min_value, number_vect_t max_value, number_vect_t value, unsigned int bytes)
{
    //cutting
    if(value > max_value) value = max_value;
    if(value < min_value) value = min_value;

    //mapping
    value -= min_value;
    value /= (max_value - min_value);

    //scaling to bytes
    value *= 1 << (8 * bytes);

    //bytes to send in one int
    unsigned long long t_int = value;

    //writing
    unsigned char t_char;
    for(int i = bytes - 1; i >= 0; i--)
    {
        t_char = t_int >> (8 * i);
        swrite(t_char);
    }
}

number_vect_t serial::read_number_vect_t(number_vect_t min_value, number_vect_t max_value, unsigned int bytes)
{
    unsigned long long t_int = 0;

    //reading
    unsigned char t_char;
    for(int i = bytes - 1; i >= 0; i--)
    {
        t_char = sread();
        t_int |= t_char << (8 * i);
    }

    number_vect_t value = t_int;

    //scaling from bytes
    value /= 1 << (8 * bytes);

    //mapping
    value *= (max_value - min_value);
    value += min_value;

    return(value);
}

unsigned int serial::read_unsigned_int_3byte()
{
    unsigned int res = 0;

    for(int i = 2; i >= 0; i--)
        res |= (sread()) << 8 * i;
    return(res);
}

void serial::sopen()
{
    if(port->isOpen())
    {
        sclose();
    }

    port->reset();

    port->setPortName(device.c_str());
    port->setQueryMode(port->EventDriven);

    port->setBaudRate(rate_map[rate]);

    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    if(port->open(QIODevice::ReadWrite) == true)
    {
        connect(port, SIGNAL(readyRead()), this, SLOT(on_rx()));
        connect_delay.set_time();
        flush();
        serror = false;

        qDebug() << "port " << port->portName() << "opened";

        if (!(port->lineStatus() & LS_DSR))
            qDebug() << "warning: device is not turned on";
    }
    else
    {
        disconnect(port, SIGNAL(readyRead()), this, SLOT(on_rx()));
        qDebug() << "device failed to open:" << port->errorString();
        serror = true;
    }
}

void serial::sclose()
{
    if(port->isOpen())
    {
        port->close();
        disconnect(port, SIGNAL(readyRead()), this, SLOT(on_rx()));
        read_error_reset();
        port->reset();
    }
}

bool serial::read_error()
{
    return(serror);
}

void serial::reopen()
{
    sclose();
    sopen();
}

void serial::read_error_reset()
{
    serror = false;
}

void serial::set_rate(unsigned int newrate)
{
    rate = newrate;
}

void serial::set_device(string newdevice)
{
    device = newdevice;
}

string serial::get_device()
{
    return(device);
}

void serial::swrite_clear()
{
    buffer.clear();
}

void serial::swrite(serial_t buf)
{
    buffer.push_back(buf);
}

void serial::swrite_put()
{
    char* c = new char[buffer.size()];

    //cerr << "writing [" << buffer.size() << "] = ";

    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        //cerr << buffer[i] << " ";
        c[i] = buffer[i];

    }

    port->write(c, buffer.size());

    delete c;

    //cerr << endl;
}

void serial::flush()
{
    port->flush();
}

serial_t serial::sread()
{
    unsigned char t_char = port->read(1)[0];
    serial_t t_serial_t = t_char;
    return(t_serial_t);
}

bool serial::isoperational()
{
    return(isconnected() && connect_delay.get_time_difference() >= connect_delay_time);
}

bool serial::iswriteable()
{
    return(isconnected() && !port->bytesToWrite());
}

bool serial::isconnected()
{
    //if(port->lastError() != 0)
    //    sclose();

    return(port->isOpen() && !read_error());
}

bool serial::iswaiting()
{
    return(isconnected() && connect_delay.get_time_difference() < connect_delay_time);
}

void serial::on_rx()
{
    port->readAll();
}
