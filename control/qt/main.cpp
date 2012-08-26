#include <QtGui/QApplication>
#include "quadro.h"
#include "serial.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Quadro w;
    w.show();
    
    return a.exec();
}
