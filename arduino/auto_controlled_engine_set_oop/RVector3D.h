#ifndef RVECTOR3D_H
#define RVECTOR3D_H

struct RVector3D
{
    RVector3D();
    RVector3D(double x_0, double y_0, double z_0);

    double x, y, z;

    double module_sq();
    double module();
    void norm();
    void set_module();

    RVector3D operator+(RVector3D);
    RVector3D operator-(RVector3D);

    RVector3D operator+=(RVector3D);
    RVector3D operator-=(RVector3D);
    
    RVector3D operator*(double);
    RVector3D operator/(double);
    
    RVector3D operator*=(double);
    RVector3D operator/=(double);

    double& value_by_axis_index(int index);

    enum print_mode {PRINT_INDEX, PRINT_TAB, PRINT_RAW, PRINT_TAB_2D, PRINT_RAW_2D};
    enum use_axis {USE_2D, USE_3D};
    void print_serial(print_mode, use_axis uaxis = USE_3D);

    void x_angle_inc(double w);
    void y_angle_inc(double w);
    void x_angle_dec(double w);
    void y_angle_dec(double w);
    
    void angle_inc(RVector3D);
    
    RVector3D angle_from_projections(); //angle in [-MPI / 2; MPI / 2]
    RVector3D projections_from_angle(double a = 1); //angle in [-MPI; MPI]
};

#endif
