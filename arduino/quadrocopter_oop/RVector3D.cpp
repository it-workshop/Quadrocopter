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

void RVector3D::parseFloat(float * a)
{
    x = a[0];
    y = a[1];
    z = a[2];
}

double RVector3D::moduleSq()
{
    return x * x + y * y + z * z;
}

double RVector3D::module()
{
    return sqrt(moduleSq());
}

RVector3D RVector3D::normalize()
{
    return(*this / module());
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

RVector3D RVector3D::operator^(RVector3D t)
{
    /*
     i  j  k
     x  y  z
     tx ty tz

     ytz-zty;ztx-xtz;xty-ytx
     */
    RVector3D n(y * t.z - z * t.y, z * t.x - x * t.z, x * t.y - y * t.x);
    return(n);
}
