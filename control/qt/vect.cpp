#include "vect.h"
#include <math.h>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

using std::stringstream;

const number_vect_t rad_deg = 180. / M_PI;

vect::vect()
{
    x = 0;
    y = 0;
    z = 0;
}

vect::vect(number_vect_t x1, number_vect_t y1, number_vect_t z1)
{
    x = x1;
    y = y1;
    z = z1;
}

number_vect_t& vect::value_by_axis_index(int i)
{
    switch(i)
    {
        case 0:
            return(x);
        case 1:
            return(y);
        case 2:
            return(z);
    };

    return(x);
}

string vect::print2d_tab()
{
    stringstream t_stream;
    t_stream.precision(2);

    t_stream << x << "\t" << y;

    return(t_stream.str());
}

string vect::print2d()
{
    stringstream t_stream;
    t_stream.precision(2);

    t_stream << "(" << x << ",\t" << y << ")";

    return(t_stream.str());
}

string vect::print()
{
    stringstream t_stream;
    t_stream.precision(2);

    t_stream << "(" << x << ",\t" << y << ",\t" << z << ")";

    return(t_stream.str());
}

string vect::print_tab()
{
    stringstream t_stream;
    t_stream.precision(2);

    t_stream << x << "\t" << y << "\t" << z;

    return(t_stream.str());
}

number_vect_t vect::abs_2()
{
    return((*this) * (*this));
}

number_vect_t vect::abs()
{
    return(sqrt(fabs(abs_2())));
}

vect vect::operator*=(number_vect_t t)
{
    x *= t;
    y *= t;
    z *= t;
    return(*this);
}

vect vect::operator+=(vect t)
{
    x += t.x;
    y += t.y;
    z += t.z;
    return(*this);
}

vect vect::operator /=(number_vect_t t)
{
    x /= t;
    y /= t;
    z /= t;
    return(*this);
}

bool vect::operator==(vect t)
{
    return(t.x == x && t.y == y && t.z == z);
}

vect vect::operator /=(vect t)
{
    x /= t.x;
    y /= t.y;
    z /= t.z;
    return(*this);
}

vect vect::operator+(vect t)
{
    vect t_vect = (*this);
    t_vect += t;
    return(t_vect);
}

vect vect::operator-(vect t)
{
    vect t_vect = (*this);
    t_vect -= t;
    return(t_vect);
}

vect vect::operator^=(vect t)
{
    vect n = (*this) ^ t;
    x = n.x;
    y = n.y;
    z = n.z;
    return(n);
}

vect vect::operator^(vect t)
{
    vect n(y * t.z - z * t.y, z * t.x - x * t.z, x * t.y - y * t.x);
    return(n);
}

vect vect::operator /(number_vect_t t)
{
    vect t_vect = (*this);
    t_vect /= t;
    return(t_vect);
}

vect vect::operator-=(vect t)
{
    x -= t.x;
    y -= t.y;
    z -= t.z;
    return(*this);
}

vect vect::operator*(number_vect_t t)
{
    vect t_vect = (*this);
    t_vect *= t;
    return(t_vect);
}

number_vect_t vect::operator*(vect t)
{
    return(x * t.x + y * t.y + z * t.z);
}
