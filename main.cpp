#include "mainwindow.h"
#include <QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //MyThread *thread= new MyThread();


    w.show();


    return a.exec();
}
