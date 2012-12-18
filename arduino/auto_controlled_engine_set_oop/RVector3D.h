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

    //vector product
    RVector3D operator^(RVector3D);

    double& valueByAxisIndex(int index);

    RVector3D operator%(RVector3D);

    void xAngleInc(double w);
    void yAngleInc(double w);
    void xAngleDec(double w);
    void yAngleDec(double w);
    void zAngleInc(double w);
    void zAngleDec(double w);
    
    void angleInc(RVector3D angularRotation);
    
    RVector3D angleFromProjections(); //angle in [-MPI / 2; MPI / 2]
    RVector3D projectionsFromAngle(double a = 1); //angle in [-MPI; MPI]
};

#endif
