#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>

#include <options.h>

#include <QDialog>

#include <mainwindow.h>

class Channel1Options: public QObject
{
    Q_OBJECT

private slots:

    int GetSignalType1();

private:

    Ui::Options *ui;
};

#endif // CHANNEL1_H
