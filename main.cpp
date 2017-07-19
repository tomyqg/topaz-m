#include "mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;

//    Options w;

        keyboard w;

    // делаем ресайз

    qreal dpi = a.primaryScreen()->logicalDotsPerInch();

    w.resize(1000,800);
    w.resizeWidgets(w, 0.4);//(dpi/MainWindow::refDpi) - адаптивный вариант
//    w.resize(333,200);
    w.show();
    return a.exec();
}
