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
         qcustomplot.cpp \
    updategraph.cpp \
    keyboard.cpp \
    eventfilter.cpp \
    uartdriver.cpp \
    readoptionsfromfile.cpp \
    messages.cpp \
    writetofile.cpp \
    metrologicalcalc.cpp \
    mathresolver.cpp \
    options_gui.cpp \
    options_logic.cpp \
    channel.cpp \
    mainwindow_gui.cpp \
    mainwindow_logic.cpp \
    painting.cpp

HEADERS  += mainwindow.h \
         qcustomplot.h \
    updategraph.h \
    options.h \
    channel1.h\
    channel1.h \
    keyboard.h \
    messages.h \
    uartdriver.h \
    metrologicalcalc.h \
    mathresolver.h

FORMS    += mainwindow.ui \
    options.ui \
    keyboard.ui \
    messages.ui
#target.path  = $$[QT_INSTALL_EXAMPLES]/quick/demos
target.path  = /usr/test
INSTALLS    += target
DISTFILES +=
