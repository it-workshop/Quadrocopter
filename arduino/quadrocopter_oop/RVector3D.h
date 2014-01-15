#ifndef RVECTOR3D_H
#define RVECTOR3D_H

class RVector3D
{
public:
//private:
    double x, y, z;

public:
    RVector3D();
    RVector3D(double x_0, double y_0, double z_0);
    RVector3D(double xyz);

    RVector3D operator=(double);
    void parseFloat(float*);

    //length squared
    double moduleSq();

    //length
    double module();

    //make length equal 1
    RVector3D normalize();

    RVector3D operator+(RVector3D);
    RVector3D operator-(RVector3D);

    RVector3D operator+=(RVector3D);
    RVector3D operator-=(RVector3D);
    
    RVector3D operator*(double);
    RVector3D operator/(double);
    
    RVector3D operator*=(double);
    RVector3D operator/=(double);

    //cross product
    RVector3D operator^(RVector3D);

    double& valueByAxisIndex(int index);

    RVector3D operator%(RVector3D);
};

#endif
