#include "mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "stackedoptions.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>


//By Andrey Shmelev e-mail: andrshmel@gmail.com +79227112858

int main(int argc, char *argv[])
{
#ifndef Q_OS_WIN32
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //    QTextCodec::setCodecForTr(codec);
    //    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
#endif

    QApplication a(argc, argv);

    QTranslator translator;
    MainWindow w;

    //    qApp->closeAllWindows();
    //делаем ресайз
    w.resizeWindow(w,w.GetWindowWidthPixels(),w.GetWindowHeightPixels());
    w.show();


    return a.exec();
}
