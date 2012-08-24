#ifndef VECT_H
#define VECT_H

#include <vector>
#include <string>

using std::vector;
using std::string;
typedef long double number_vect_t;

class vect
{
public:
    number_vect_t x;
    number_vect_t y;
    number_vect_t z;

    vect();
    vect(number_vect_t x1, number_vect_t y1, number_vect_t z1);

    string print();
    string print2d();
    string print_tab();
    string print2d_tab();

    number_vect_t abs_2();
    number_vect_t abs();

    number_vect_t& value_by_axis_index(int i);

    vect operator+(vect t);
    vect operator-(vect t);
    vect operator^=(vect t);
    vect operator^(vect t);
    vect operator/(number_vect_t t);
    vect operator*=(number_vect_t t);
    vect operator+=(vect t);
    vect operator/=(vect t);
    vect operator/=(number_vect_t t);
    bool operator==(vect t);
    vect operator-=(vect t);
    vect operator*(number_vect_t t);
    number_vect_t operator*(vect t);

    vect angle_from_projections();
    vect projections_from_angle(double a = 1);
};

#endif // VECT_H
