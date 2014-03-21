#include <iostream>

using std::cout;
using std::endl;

int main()
{
    double i;
    for(i = 0; i <= 45; i += 0.8)
    {
        cout << "36 " << i << " " << 0 << " " << 0 << endl;
    }

    for(i = 0; i < 10; i++)
    {
        cout << "36 45 0 0" << endl;
    }

    
    for(i = 45; i >= -45; i -= 6)
    {
        cout << "36 " << i << " " << 0 << " " << 0 << endl;
    }

    for(i = 0; i < 5; i++)
    {
        cout << "36 -45 0 0" << endl;
    }

    for(i = -45; i >= 45; i += 6)
    {
        cout << "36 " << i << " " << 0 << " " << 0 << endl;
    }

    cout << "0 0 0 0" << endl;
}
