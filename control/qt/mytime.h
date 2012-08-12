#ifndef MYTIME_H
#define MYTIME_H

#include <ctime>
#include <string>
#include <sys/timeb.h>

using std::string;

class mytime
{
public:
    mytime();

    void set_time();
    unsigned long long int get_time_difference();
    unsigned long long int get_time_ms();

    const time_t get_seconds();
    const string get_time();

private:
    unsigned long long int old;
};

#endif // MYTIME_H
