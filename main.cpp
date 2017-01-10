#include "mainwindow.h"
#include <QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

QObject::connect(&a, SIGNAL(focusChanged(QWidget*,QWidget*)), &w, SLOT(textupdate()));
//QObject::connect(&a, SIGNAL(focusChanged(QWidget*,QWidget*)), &w, SLOT(focusChanged(QWidget* old, QWidget* now)));
    w.show();
    return a.exec();


}
