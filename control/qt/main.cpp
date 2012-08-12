#include <QtGui/QApplication>
#include "quadro.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Quadro w;
    w.show();
    
    return a.exec();
}
