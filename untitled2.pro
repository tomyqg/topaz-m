#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T23:24:55
#
#-------------------------------------------------

QT += core gui script
QT += serialport
QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++11
TARGET = untitled2

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
         ../../qcustomplot.cpp \
    updategraph.cpp \
    dialog.cpp \
    options.cpp \
    digitalinputs.cpp \
    options_setting_class.cpp \
    channel1.cpp

HEADERS  += mainwindow.h \
         ../../qcustomplot.h \
    updategraph.h \
    dialog.h \
    options.h \
    digitalinputs.h \
    options_setting_class.h \
    channel1.h

FORMS    += mainwindow.ui \
    dialog.ui \
    options.ui \
    digitalinputs.ui

DISTFILES +=

