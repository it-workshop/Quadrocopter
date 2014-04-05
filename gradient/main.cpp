//На вход подаются: x0, y0: координаты маячка, x1, y1: координаты квадрокоптера.
#define COORDS 2
#include "gradient.h"
#include "jason_output.h"
#include <iostream>
#include <iomanip>
#include <vector>
//#include <string>
using namespace std;
int dots = 0;
vect<COORDS> x;
vect<COORDS> tmp;
vect<COORDS> b;
vect<COORDS> grad;
vect<2> base;
double d = 0;

#define polygon vector< vect<2> >

/*template <int n>
double fv(vect<n> x)
{

	double res = 0;
	if (dist2(x, b) > 10)
		for (int i = 0; i < COORDS; i++)
			res += (x.v[i] - (b.v[i]+1))*(x.v[i] - (b.v[i])+1);
	else for (int i = 0; i < COORDS; i++)
			res += (x.v[i] - b.v[i])*(x.v[i] - b.v[i]);
	return res;
}*/
double xm = 0;
double ym = 0;

double fv(vect<2> a)
{
	double x =(a.v[0] - b.v[0])*100;
/*		if (abs(x) > xm)
			xm = abs(x);*/
	double y =(a.v[1] - b.v[1])*100;
		/*if (abs(y) > ym)
			ym = abs(y);*/
	if (dist2(a, b) > 1e-5)
		return cos(x*x+y*y-3*atan(y/(abs(x)+0.1)))/(x*x+y*y+1);
				
	else
		return (x*x+y*y);
}

template <int n>
void FLY(vect<n> &x, vect<n> a, double d) //Полёт. X - текущее положение, 
{
	x = vsum(x, vmul(a, d));
}

//string OUTPUT;
void calc_line(vect<2> from, vect<2> to)
{
	NEW_POINT(base);
	NEW_POINT(from);
	b = to;
	x = from;

	d = sqrt(dist2(b, x)) * 0.75;
	grad = getgrad(x);
	int k = 2;
	double delta = 1;
	while ((abs(delta) > 1e-8) && (d > 1e-10))
	{
		delta = 0;
		tmp = x;
		GO(FLY<2>);
		k++;
		for (int i = 0; i < COORDS-1; i++)
		{
			delta += (tmp.v[i]-x.v[i])*(tmp.v[i]-x.v[i]);
		}

		NEW_POINT(x);
		/*if (k % 4 == 0)
			cout << endl << "    ";*/

		delta += (tmp.v[COORDS-1]-x.v[COORDS-1])*(tmp.v[COORDS-1]-x.v[COORDS-1]);
	}

	LAST_POINT(x);
}

vect<2> MakePoint(double x, double y)
{
	vect<2> res;
	res.v[0] = x;
	res.v[1] = y;
	return res;
}

vect<2> GetRandomPoint(polygon p, int mask)
{
	int k = 0;
	double x = 0;
	double y = 0;
	for (int i = 0; i < p.size(); i++)
	{
		if (i && mask)
		{
			x+=p[i].v[0];
			y+=p[i].v[1];
			k++;
		}
	}
	return MakePoint(x/k, y/k);
}

polygon p[10];
void initP0()
{
	p[0].push_back(MakePoint(43.355209, 42.443719));
	p[0].push_back(MakePoint(43.355574, 42.446220));
	p[0].push_back(MakePoint(43.356483, 42.445382));
	p[0].push_back(MakePoint(43.356247, 42.443557));

	p[1].push_back(MakePoint(43.354135, 42.431240));
	p[1].push_back(MakePoint(43.356224, 42.426909));
	p[1].push_back(MakePoint(43.348484, 42.414491));
	p[1].push_back(MakePoint(43.342689, 42.416100));
	//SORT;
}

void init()
{
	base.v[0] = 43.355299;
	base.v[1] = 42.452171;
	initP0();
}

int main()
{
//	freopen("input.txt", "r", stdin);
	vect<2> from, to;
	double x0, y0, x1, y1;
	int N, M;
	init();
	freopen("output.txt", "w", stdout);
	cout.precision(8);
	cin >> N;
	for (int z = 0; z < N; z++)
	{
		int pnum = rand() % 2; //Какой полигон выбираем?
		to = GetRandomPoint(p[pnum], rand());
		//cin >> to.v[0] >> to.v[1]; //Куда летим
		SINGLE_POINT(to);
		NEW_GROUP();
		cin >> M;
		for (int zz = 0; zz < M; zz++)
		{
			//cin >> x1 >> y1; //Откуда летим
			from = p[pnum][zz];
			NEW_LINE();
			calc_line(from, to);
			if (zz < M-1)
				END_LINE();
			else
				END_LAST_LINE();
		}
		END_GROUP();
	}
	cout << endl << endl << xm << endl << ym;
	return 0;
}