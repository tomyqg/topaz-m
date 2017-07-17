#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // делаем ресайз

    qreal dpi = a.primaryScreen()->logicalDotsPerInch();

    w.resizeWidgets(w, 0.8);//(dpi/MainWindow::refDpi) - адаптивный вариант
    w.resize(1024,768);

    w.show();
    return a.exec();
}
