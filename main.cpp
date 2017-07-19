#include "mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    Options w;

//        keyboard w;

    // делаем ресайз

    qreal dpi = a.primaryScreen()->logicalDotsPerInch();

    w.resizeWidgets(w, 0.8);//(dpi/MainWindow::refDpi) - адаптивный вариант
    w.show();
    return a.exec();
}
