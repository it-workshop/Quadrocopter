#include <iostream>

using std::cout;
using std::endl;

#define POWER 55
#define rampUp(x, y, step) {for(i = x; i <= y; i += step) {cout << POWER << " " << i << " 0 0" << endl;}}
#define rampDown(x, y, step) {for(i = x; i >= y; i -= step) {cout << POWER << " " << i << " 0 0" << endl;}}
#define repeat(x, times) {for(i = 0; i < times; i++) {cout << POWER << " " << x << " 0 0" << endl;}}

int main()
{
    double i;
    repeat(0, 100);

    rampUp(0, 45, 0.8);
    repeat(45, 10);

    rampDown(45, -45, 5);
    repeat(-45, 10);

    rampUp(-45, 45, 5);
    repeat(45, 10);

    rampDown(45, -45, 5);
    repeat(-45, 10);

    rampUp(-45, 45, 5);
    repeat(45, 10);

    rampDown(45, -45, 3);
    repeat(-45, 10);

    rampUp(-45, 45, 3);
    repeat(-45, 10);

    rampDown(45, 0, 7);
    repeat(0, 50);

    cout << "0 0 0 0" << endl;
}
