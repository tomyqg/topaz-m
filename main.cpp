#include "mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "stackedoptions.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>


//By Andrey Shmelev e-mail: andrshmel@gmail.com +79227112858

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;

    //translator.load(QApplication::applicationName() + "_" + QLocale::system().name());
    qDebug() << QApplication::applicationName() + "_" + QLocale::system().name();

    translator.load("untitled2_en_EN");
    qApp->installTranslator(&translator);

    MainWindow w;

    //делаем ресайз
    w.resizeWindow(w,w.GetWindowWidthPixels(),w.GetWindowHeightPixels());
    w.show();
    return a.exec();
}
