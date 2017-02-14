#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "dialog.h"
#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>
#include <channel1.h>

void MainWindow::WritetoFile()
{
    QFile filedir("/sys/class/gpio/gpio69/direction");

    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);

    outdir << "out";
    filedir.close();

    QFile file("/sys/class/gpio/gpio69/value");

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    if (1)
    {
        out << "1";
    }

    else
    {
        out << "0";
    }

    file.close();
    ui->pushButton_2->setText("настрйки сохранены");
}

void Options::WriteSystemOptionsToFile()
{
    QJsonObject systemoptions;

    QDateTime local(QDateTime::currentDateTime());

    systemoptions["Time"] = local.time().toString();
    systemoptions["Date"] = local.date().toString();
    systemoptions["Calibration"] = Options::calibrationprm;

    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);

    QFile file("/usr/systemoptions.txt");

    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file

    QTextStream out(&file);
    out << setstr;
    file.close();
}
