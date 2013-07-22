#-------------------------------------------------
#
# Project created by QtCreator 2012-09-22T19:01:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pid_test
TEMPLATE = app

INCLUDEPATH += /usr/include /usr/local/include /usr/include/qwt-qt4 ../control/qt ../arduino/quadrocopter_oop/

LIBS += -L/usr/local/lib -lqwt-qt4
LIBS += -L/usr/local/lib -lQtSvg

CONFIG += qwt

SOURCES += main.cpp\
        pid_test.cpp \
    ../control/qt/mytime.cpp \
    ../arduino/quadrocopter_oop/PID.cpp \
    ../arduino/quadrocopter_oop/RVector3D.cpp

HEADERS  += pid_test.h \
    ../control/qt/mytime.h \
    ../arduino/quadrocopter_oop/PID.h \
    ../arduino/quadrocopter_oop/RVector3D.h

FORMS    += pid_test.ui
