#ifndef SERIAL_H
#define SERIAL_H

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <map>
#include <vect.h>

using std::string;
using std::map;

class quadrocopter;
class joystick;

class serial
{
friend class quadrocopter;
friend class joystick;

private:
    enum debug_type {DEBUG_NONE, DEBUG_ERROR, DEBUG_ALL};

    debug_type debug_level;

    struct termios tio;
    int tty_fd;

    unsigned int rate;
    string device;

    unsigned int maxwait;
    bool serror;
    bool connected;

    map<int, int> rate_map;

public:
    serial();

    void sopen();
    void sclose();
    void reopen();

    void set_maxwait(unsigned int);
    void set_rate(unsigned int);
    void set_device(string);

    string get_device();

    void swrite(short unsigned int);
    short unsigned int sread();
    void flush();

    bool read_error();
    void read_error_reset();

    bool isconnected();

    vect read_vect_byte(unsigned int axis = 3);
    unsigned int read_unsigned_int_3byte();
};

#endif // SERIAL_H
