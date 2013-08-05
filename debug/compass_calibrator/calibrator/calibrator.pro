#-------------------------------------------------
#
# Project created by QtCreator 2013-08-05T23:46:45
#
#-------------------------------------------------

QT       += core
QT       -= gui


INCLUDEPATH += /usr/include /usr/local/include /usr/include/QtExtSerialPort ../../../control/qt

CONFIG += extserialport

LIBS += -L/usr/lib/i386-linux-gnu/ -lqextserialport

TARGET = calibrator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../../../control/qt/serial.cpp \
    ../../../control/qt/vect.cpp \
    ../../../control/qt/mytime.cpp

HEADERS  +=  ../../../control/qt/serial.h \
    ../../../control/qt/vect.h \
    ../../../control/qt/mytime.h


