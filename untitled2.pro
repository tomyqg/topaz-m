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
    painting.cpp \
    QTModbusRTU/qmodbusclient.cpp \
    QTModbusRTU/qmodbusdataunit.cpp \
    QTModbusRTU/qmodbusdevice.cpp \
    QTModbusRTU/qmodbusdeviceidentification.cpp \
    QTModbusRTU/qmodbuspdu.cpp \
    QTModbusRTU/qmodbusreply.cpp \
    QTModbusRTU/qmodbusrtuserialmaster.cpp \
    QTModbusRTU/qmodbusrtuserialslave.cpp \
    QTModbusRTU/qmodbusserver.cpp

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
    mathresolver.h \
    QTModbusRTU/qmodbus_symbols_p.h \
    QTModbusRTU/qmodbusadu_p.h \
    QTModbusRTU/qmodbusclient.h \
    QTModbusRTU/qmodbusclient_p.h \
    QTModbusRTU/qmodbuscommevent_p.h \
    QTModbusRTU/qmodbusdataunit.h \
    QTModbusRTU/qmodbusdevice.h \
    QTModbusRTU/qmodbusdevice_p.h \
    QTModbusRTU/qmodbusdeviceidentification.h \
    QTModbusRTU/qmodbuspdu.h \
    QTModbusRTU/qmodbusreply.h \
    QTModbusRTU/qmodbusrtuserialmaster.h \
    QTModbusRTU/qmodbusrtuserialmaster_p.h \
    QTModbusRTU/qmodbusrtuserialslave.h \
    QTModbusRTU/qmodbusrtuserialslave_p.h \
    QTModbusRTU/qmodbusserver.h \
    QTModbusRTU/qmodbusserver_p.h

FORMS    += mainwindow.ui \
    options.ui \
    keyboard.ui \
    messages.ui
#target.path  = $$[QT_INSTALL_EXAMPLES]/quick/demos
target.path  = /opt
INSTALLS    += target
DISTFILES += \
    ../build-untitled2-MYD-Release/Makefile
