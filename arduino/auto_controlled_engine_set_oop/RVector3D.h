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
    double module_sq();

    //length
    double module();

    //make length equal 1
    void norm();

    RVector3D operator+(RVector3D);
    RVector3D operator-(RVector3D);

    RVector3D operator+=(RVector3D);
    RVector3D operator-=(RVector3D);
    
    RVector3D operator*(double);
    RVector3D operator/(double);
    
    RVector3D operator*=(double);
    RVector3D operator/=(double);

    double& value_by_axis_index(int index);

    RVector3D operator%(RVector3D);

    void x_angle_inc(double w);
    void y_angle_inc(double w);
    void x_angle_dec(double w);
    void y_angle_dec(double w);
    
    void angle_inc(RVector3D);
    
    RVector3D angle_from_projections(); //angle in [-MPI / 2; MPI / 2]
    RVector3D projections_from_angle(double a = 1); //angle in [-MPI; MPI]
};

#endif
