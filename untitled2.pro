#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T11:30:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = untitled2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Downloads/QCustomPlot.tar/QCustomPlot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    ../Downloads/QCustomPlot.tar/QCustomPlot/qcustomplot.h

FORMS    += mainwindow.ui

DISTFILES += \
    logo.jpg

RESOURCES += \
    pics.qrc \
    img.qrc
