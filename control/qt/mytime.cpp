#include "mytime.h"
#include <ctime>
#include <sys/timeb.h>
#include <string>

using std::string;

mytime::mytime()
{
    old = getTimeMs();
    reset();
}

void mytime::setTime()
{
    old = getTimeMs();
    isset = true;
}

unsigned long long int mytime::getTimeMs()
{
    timeb* tp = new timeb;
    ftime(tp);

    return(tp->time * 1000 + tp->millitm);
}

void mytime::reset()
{
    isset = false;
}

bool mytime::isSet()
{
    return(isset);
}

unsigned long long int mytime::getTimeDifference()
{
    return(getTimeMs() - old);
}

string mytime::getTime()
{
    string result;

    time_t seconds = mytime::getSeconds();
    tm* timeinfo = localtime(&seconds);

    char time_buf[80];
    const char* format = "%Y.%m.%d_%H:%M:%S";
    strftime(time_buf, 80, format, timeinfo);

    result = time_buf;

    return(result);
}

time_t mytime::getSeconds()
{
    time_t seconds = time(NULL);

    return(seconds);
}
