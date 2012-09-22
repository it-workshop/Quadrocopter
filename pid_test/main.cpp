#include <QApplication>
#include "pid_test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PID_test w;
    w.show();
    
    return a.exec();
}
