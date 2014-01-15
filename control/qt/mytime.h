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

    void setTime();
    unsigned long long int getTimeDifference();
    unsigned long long int getTimeMs();

    void reset();
    bool isSet();

    time_t getSeconds();
    string getTime();

private:
    unsigned long long int old;
    bool isset;
};

#endif // MYTIME_H
