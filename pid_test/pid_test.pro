#-------------------------------------------------
#
# Project created by QtCreator 2012-09-22T19:01:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pid_test
TEMPLATE = app

INCLUDEPATH += /usr/include /usr/local/include /usr/include/qwt-qt4 /usr/include/qt4/QtExtSerialPort ../control/qt ../arduino/auto_controlled_engine_set_oop/

LIBS += -L/usr/local/lib -lqwt-qt4
LIBS += -L/usr/local/lib -lQtSvg

CONFIG += extserialport qwt

# magick?
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lqextserialport-1.2


SOURCES += main.cpp\
        pid_test.cpp \
    ../control/qt/mytime.cpp \
    ../arduino/auto_controlled_engine_set_oop/PID.cpp \
    ../arduino/auto_controlled_engine_set_oop/RVector3D.cpp

HEADERS  += pid_test.h \
    ../control/qt/mytime.h \
    ../arduino/auto_controlled_engine_set_oop/PID.h \
    ../arduino/auto_controlled_engine_set_oop/RVector3D.h

FORMS    += pid_test.ui
