#include "serial.h"
#include <iostream>
#include <map>
#include <mytime.h>

using std::cerr;
using std::endl;

using std::map;

serial::serial()
{
    rate_map[9600] = B9600;
    rate_map[57600] = B57600;
    rate_map[115200] = B115200;

    tty_fd = -1;
    device = (char*)"/dev/ttyACM0";
    rate = 9600;
    maxwait = 100;

    connected = false;

    debug_level = DEBUG_ERROR;
}

vect serial::read_vect_byte(unsigned int axis)
{
    vect result;
    for(unsigned int i = 0; i < axis; i++)
        result.value_by_axis_index(i) = sread() / 255. * 2 - 1;

    if(read_error()) result = vect();

    return(result);
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
    if(tty_fd == -1)
    {
        mytime t_time;

        tty_fd = open(device.c_str(), O_RDWR | O_NONBLOCK| O_NOCTTY | O_NDELAY);

        if(tty_fd == -1 && debug_level >= DEBUG_ERROR)
            cerr << t_time.get_time() << " " << device << "@" << rate << ": error opening" << endl;

        if(tcgetattr(tty_fd, &tio) < 0)
        {
            sclose();
            if(debug_level >= DEBUG_ERROR)
                cerr << t_time.get_time() << " " << device << "@" << rate << ": couldn't get term attributes" << endl;
        }

        cfsetispeed(&tio, rate_map[rate]);
        cfsetospeed(&tio, rate_map[rate]);

        // 8N1
        tio.c_cflag &= ~PARENB;
        tio.c_cflag &= ~CSTOPB;
        tio.c_cflag &= ~CSIZE;
        tio.c_cflag |= CS8;
        // no flow control
        tio.c_cflag &= ~CRTSCTS;

        tio.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
        tio.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
        tio.c_iflag |= IGNPAR;

        tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
        tio.c_oflag &= ~OPOST; // make raw

        // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
        tio.c_cc[VMIN]  = 0;
        tio.c_cc[VTIME] = 0;

        if(tcsetattr(tty_fd, TCSANOW, &tio) < 0) {
            sclose();
            if(debug_level >= DEBUG_ERROR)
                cerr << t_time.get_time() << " " << device << "@" << rate << ": couldn't set term attributes" << endl;
        }

        if(debug_level >= DEBUG_ALL)
            cerr << "device opened" << endl;
    }
}

void serial::sclose()
{
    if(tty_fd != -1)
    {
        close(tty_fd);
        tty_fd = -1;
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

void serial::swrite(short unsigned int buf)
{
    mytime t_time;

    if(debug_level >= DEBUG_ALL)
        cerr << t_time.get_time() << " " << device << "@" << rate << ": write [" << (char) buf << "]" << endl;

    if(!read_error())
    {
        if(debug_level >= DEBUG_ALL)
            cerr << t_time.get_time() << " " << device << "@" << rate << ": write called" << endl;
        int res = write(tty_fd, &buf, 1);
        if(debug_level >= DEBUG_ALL)
            cerr << t_time.get_time() << " " << device << "@" << rate << ": write ok" << endl;

        //waits
        if(res >= 0)
        {
            if(debug_level >= DEBUG_ALL)
                cerr << t_time.get_time() << " " << device << "@" << rate << ": drain" << endl;
            tcdrain(tty_fd);
            if(debug_level >= DEBUG_ALL)
                cerr << t_time.get_time() << " " << device << "@" << rate << ": drain ok" << endl;
        }
        else if(read_error() && debug_level >= DEBUG_ERROR) cerr << t_time.get_time() << " " << device << "@" << rate << ": write error" << endl;
    }
    else if(debug_level >= DEBUG_ERROR)
        cerr << t_time.get_time() << " " << device << "@" << rate << ": prev error" << endl;
}

void serial::flush()
{
    mytime t_time;

    if(debug_level >= DEBUG_ALL)
        cerr << t_time.get_time() << " " << device << "@" << rate << ": flush" << endl;

    tcflush(tty_fd, TCIOFLUSH);
}

short unsigned int serial::sread()
{
    short unsigned int a = 0;

    mytime t_time;

    if(debug_level >= DEBUG_ALL)
        cerr << t_time.get_time() << " " << device << "@" << rate << ": read" << endl;

    if(!read_error())
    {
        t_time.set_time();
        while((read(tty_fd, &a, 1) <= 0) && (t_time.get_time_difference() < maxwait)) usleep(1000);
        if(t_time.get_time_difference() >= maxwait) serror = true;
    }

    if(read_error() && debug_level >= DEBUG_ERROR) cerr << t_time.get_time() << " " << device << "@" << rate << ": read error" << endl;

    else if(debug_level >= DEBUG_ALL)
        cerr << t_time.get_time() << " " << device << "@" << rate << ": read [" << a << "] time=" << t_time.get_time_difference() << endl;

    return(a);
}

bool serial::isconnected()
{
    return(connected);
}
