#include "mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //делаем ресайз
    w.resizeWindow(w,w.GetWindowWidthPixels(),w.GetWindowHeightPixels());
    //w.resizeWindow(w,1280,800);//(dpi/MainWindow::refDpi) - адаптивный вариант
    w.show();
    return a.exec();
}
