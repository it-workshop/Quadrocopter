#include "RVector3D.h"
#include "Definitions.h"
#include "Arduino.h"

RVector3D::RVector3D()
{
    x = 0;
    y = 0;
    z = 0;
}

RVector3D::RVector3D(double x_0, double y_0, double z_0)
{
    x = x_0;
    y = y_0;
    z = z_0;
}

void RVector3D::norm()
{
    double k = 1.0 / sqrt(module_sq());
    x *= k;
    y *= k;
    z *= k;
}

void RVector3D::x_angle_inc(double w)
{
    double old_y = y;
    y =     y * cos(w) - z * sin(w);
    z = old_y * sin(w) + z * cos(w);
}

void RVector3D::x_angle_dec(double w)
{
    x_angle_inc(-w);
}

void RVector3D::y_angle_inc(double w)
{
    double old_x = x;
    x =      x * cos(w) + z * sin(w);
    z = -old_x * sin(w) + z * cos(w);
}

void RVector3D::y_angle_dec(double w)
{
    y_angle_inc(-w);
}

void RVector3D::angle_inc(RVector3D rotation)
{
    x_angle_inc(rotation.x);
    y_angle_inc(rotation.y);
}

double RVector3D::module_sq()
{
    return x * x + y * y + z * z;
}

double RVector3D::module()
{
    return sqrt(module_sq());
}

RVector3D RVector3D::operator-(RVector3D c)
{
    RVector3D result;
    result.x = x - c.x;
    result.y = y - c.y;
    result.z = z - c.z;
    return(result);
}

RVector3D RVector3D::operator+(RVector3D c)
{
    RVector3D result;
    result.x = x + c.x;
    result.y = y + c.y;
    result.z = z + c.z;
    return(result);
}

RVector3D RVector3D::operator*(double c)
{
    RVector3D result;
    result.x = x * c;
    result.y = y * c;
    result.z = z * c;
    return(result);
}

RVector3D RVector3D::operator/(double c)
{
    RVector3D result;
    result.x = x / c;
    result.y = y / c;
    result.z = z / c;
    return(result);
}

RVector3D RVector3D::operator/=(double c)
{
    x /= c;
    y /= c;
    z /= c;
    return(*this);
}

RVector3D RVector3D::operator*=(double c)
{
    x *= c;
    y *= c;
    z *= c;
    return(*this);
}

RVector3D RVector3D::operator-=(RVector3D b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return(*this);
}

RVector3D RVector3D::operator+=(RVector3D b)
{
    x += b.x;
    y += b.y;
    z += b.z;
    return(*this);
}

double& RVector3D::value_by_axis_index(int index)
{
    switch(index)
    {
    case 0:
        return(x);
    case 1:
        return(y);
    case 2:
        return(z);
    }
}

RVector3D RVector3D::angle_from_projections()
{
    RVector3D result = RVector3D();
    
    result.x = -(acos(y / sqrt(y*y + z*z)) - MPI / 2);
    result.y =   acos(x / sqrt(x*x + z*z)) - MPI / 2;
    result.z = 0;
    
    return(result);
}

RVector3D RVector3D::projections_from_angle(double a)
{
    RVector3D result = RVector3D();

    if (fabs(x - MPI / 2) < double_eps || fabs(y - MPI / 2) < double_eps
     || fabs(x + MPI / 2) < double_eps || fabs(y + MPI / 2) < double_eps)
    {
        result.z = 0;

        if (fabs(y + MPI / 2) < double_eps) result.x = -1;
        if (fabs(y - MPI / 2) < double_eps) result.x = 1;

        if (fabs(x + MPI / 2) < double_eps) result.y = 1;
        if (fabs(x - MPI / 2) < double_eps) result.y = -1;
    }
    else
    {
        result.z = +a / sqrt(1 + pow(tan(x), 2) + pow(tan(y), 2));
        result.x = +result.z * tan(y);
        result.y = -result.z * tan(x);
    }

    if ((fabs(y) - double_eps) >= MPI / 2 && (fabs(y) + double_eps) <= MPI) result.x *= -1;
    if ((x + double_eps) <= -MPI / 2 || x - (double_eps) >= MPI / 2) result.y *= -1;

    result.x *= -1;
    result.y *= -1;

    return(result);

}
