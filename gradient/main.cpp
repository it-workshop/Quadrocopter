//На вход подаются: x0, y0: координаты маячка, x1, y1: координаты квадрокоптера.
#include <iostream>
#include <math.h>
using namespace std;
#define eps 1e-10
#define step 1e-4
#define COORDS 2
#define double long double
int b[2] = {0, 0};
#define tempvectn template <int n> struct vect

template <int n> struct vect
{
	double v[n];
};

double dist2(double x1, double y1, double x2, double y2)
{
	return ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

template <int n>
double fv(vect<n> x)
{
	double res = 0;
	for (int i = 0; i < COORDS; i++)
		res += (x.v[i] - b[i])*(x.v[i] - b[i]);
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
	if (norm > 1e-8)
		res = vmul(res, 1.0/abs(res));
	else
		res = makedv<n>(-1);
	return res;
}

vect<COORDS> x;
vect<COORDS> tmp;
double d;
vect<COORDS> grad;

template <int n>
void FLY(vect<n> &x, vect<n> a, double d)
{
	x = vsum(x, vmul(a, d));
}

void GO()
{
	FLY(x, grad, d);
	d/=1.1;
	grad = getgrad(x);
}

int main()
{
	freopen("output.txt", "w", stdout);
	cin >> b[0] >> b[1];
	for (int i = 0; i < COORDS; i++)
		cin >> x.v[i];
	d = sqrt(dist2(b[0], b[1], x.v[0], x.v[1])) * 0.75;
	//cin >> d;
	grad = getgrad(x);
	int k = 0;
	double delta = 1;

	while (abs(delta) > 1e-8)
	{
		delta = 0;
		tmp = x;
		GO();
		k++;
	//	cout << k << ": (";
		for (int i = 0; i < COORDS-1; i++)
		{
			cout << x.v[i] << ", ";
			delta += (tmp.v[i]-x.v[i])*(tmp.v[i]-x.v[i]);
		}
		cout << x.v[COORDS-1] << /*") f = " << fv(x) <<*/ endl;
		delta += (tmp.v[COORDS-1]-x.v[COORDS-1])*(tmp.v[COORDS-1]-x.v[COORDS-1]);
	}

	for (int i = 0; i < COORDS-1; i++)
		cout << x.v[i] << ", ";
	cout << x.v[COORDS-1] <</* ") f = " << fv(x) << */endl;

	return 0;
}