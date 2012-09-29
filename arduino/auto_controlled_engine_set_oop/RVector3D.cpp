#include "RVector3D.h"
#include "Definitions.h"

//arduino define
#ifndef HIGH
    #include "math.h"
#else
    #include "Arduino.h"
#endif

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

RVector3D::RVector3D(double xyz)
{
    x = xyz;
    y = xyz;
    z = xyz;
}

RVector3D RVector3D::operator =(double xyz)
{
    x = xyz;
    y = xyz;
    z = xyz;

    return(*this);
}

void RVector3D::norm()
{
    double k = 1.0 / sqrt(moduleSq());
    x *= k;
    y *= k;
    z *= k;
}

void RVector3D::xAngleInc(double w)
{
    double old_y = y;
    y =     y * cos(w) - z * sin(w);
    z = old_y * sin(w) + z * cos(w);
}

void RVector3D::xAngleDec(double w)
{
    xAngleInc(-w);
}

void RVector3D::yAngleInc(double w)
{
    double old_x = x;
    x =      x * cos(w) + z * sin(w);
    z = -old_x * sin(w) + z * cos(w);
}

void RVector3D::yAngleDec(double w)
{
    yAngleInc(-w);
}

void RVector3D::angleInc(RVector3D rotation)
{
    xAngleInc(rotation.x);
    yAngleInc(rotation.y);
}

double RVector3D::moduleSq()
{
    return x * x + y * y + z * z;
}

double RVector3D::module()
{
    return sqrt(moduleSq());
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

double& RVector3D::valueByAxisIndex(int index)
{
    switch(index)
    {
    case 0:
        return(x);
    case 1:
        return(y);
    case 2:
        return(z);
    default:
        return (x);
    }
}

RVector3D RVector3D::operator %(RVector3D b)
{
    RVector3D result = *this;
    result.x *= b.x;
    result.y *= b.y;
    result.z *= b.z;

    return(result);
}

RVector3D RVector3D::angleFromProjections()
{
    RVector3D result = RVector3D();

    result.x = atan2(y, z);
    //result.x = -(acos(y / sqrt(y*y + z*z)) - MPI / 2);
    result.y = - atan2(x, z);
    //result.y =   acos(x / sqrt(x*x + z*z)) - MPI / 2;
    result.z = 0;
    
    return(result);
}

RVector3D RVector3D::projectionsFromAngle(double a)
{
    RVector3D result = RVector3D();

    if (fabs(x - MPI / 2) < doubleEps || fabs(y - MPI / 2) < doubleEps
     || fabs(x + MPI / 2) < doubleEps || fabs(y + MPI / 2) < doubleEps)
    {
        result.z = 0;

        if (fabs(y + MPI / 2) < doubleEps) result.x = -1;
        if (fabs(y - MPI / 2) < doubleEps) result.x = 1;

        if (fabs(x + MPI / 2) < doubleEps) result.y = 1;
        if (fabs(x - MPI / 2) < doubleEps) result.y = -1;
    }
    else
    {
        result.z = +a / sqrt(1 + pow(tan(x), 2) + pow(tan(y), 2));
        result.x = +result.z * tan(y);
        result.y = -result.z * tan(x);
    }

    if ((fabs(y) - doubleEps) >= MPI / 2 && (fabs(y) + doubleEps) <= MPI) result.x *= -1;
    if ((x + doubleEps) <= -MPI / 2 || x - (doubleEps) >= MPI / 2) result.y *= -1;

    result.x *= -1;
    result.y *= -1;

    return(result);

}
