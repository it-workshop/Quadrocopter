//На вход подаются: x0, y0: координаты маячка, x1, y1: координаты квадрокоптера.
#ifndef gradient.h
//#define gradient.h

//#include <iostream>
#include <math.h>
using namespace std;
#define eps 1e-10
#define step 1e-4
//#define double long double
//#define tempvectn template <int n> struct vect
template <int n> struct vect
{
	double v[n];
};

template <int n>
double dist2(vect<n> a, vect<n> b)
{
	double res = 0;
	for (int i = 0; i < n; i++)
		res += (a.v[i] - b.v[i])*(a.v[i] - b.v[i]);
	return res;
}

template <int n>
vect<n> makev(double x[n])
{
	vect<n> res;
	for (int i = 0; i < n; i++)
		res.v[i] = x[i];
	return res;
}

template <int n>
vect<n> makedv(int k)
{
	vect<n> res = {0};
	memset(res.v, 0, sizeof(res.v));
	if (k > -1)
		res.v[k] = 1;
	return res;
}

template <int n>
vect<n> vsum(vect<n> a, vect<n> b)
{
	vect<n> res;
	for (int i = 0; i < n; i++)
		res.v[i] = a.v[i] + b.v[i];
	return res;
}

template <int n>
vect<n> vmul(vect<n> a, double b)
{
	vect<n> res;
	for (int i = 0; i < n; i++)
		res.v[i] = a.v[i] * b;
	return res;
}

template <int n>
double abs(vect<n> x)
{
	double res = 0;
	for (int i = 0; i < n; i++)
		res += x.v[i] * x.v[i];
	return sqrt(res);
}

template <int n>
vect<n> getgrad(vect<n> x)
{
	vect<n> res;
	double fx = fv(x);

	for (int i = 0; i < n; i++)
	{
		res.v[i] = -(fv(vsum(x, vmul(makedv<n>(i), step))) - fx);
	}
	double norm = abs(res);
	if (norm > eps/1000)
		res = vmul(res, 1.0/abs(res));
	else
		res = makedv<n>(-1);
	return res;
}

template <int n>
void GO(void FLY(vect<n> &x, vect<n> a, double d))
{
	FLY(x, grad, d);
	d/=1.1;
	grad = getgrad(x);
}

#endif