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



void Options::readsystemoptionsfromfile()
{
    QFile infile("/usr/systemoptions.txt");

    //    QFile infile("C:/Work/options.txt");

    infile.open(QIODevice::ReadOnly);

    QTextStream in(&infile);
    QString sss = in.readLine();

    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());

    QJsonObject json = doc.object();

    //QJsonArray array = json["channels"].toArray();

    Options::calibrationprm = json["Calibration"].toString();

    QProcess process;

    process.startDetached("sudo date --set " + json["Date"].toString());
    process.startDetached("sudo date --set " + json["Time"].toString()); // max freq on

    infile.close();
}

void Options::readoptionsfromfile()
{
    QFile infile("/usr/options.txt");

//        QFile infile("C:/Work/options.txt");

    infile.open(QIODevice::ReadOnly);

    QTextStream in(&infile);
    QString sss = in.readLine();

    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());

    QJsonObject json = doc.object();

    QJsonArray array = json["channels"].toArray();

    /*
    for (int x = 0; x < 2; ++x) {

        ch  = array.at(x).toObject();
    }*/

    QJsonObject ch1 = array.at(0).toObject();

    options1.SetHigherLimit(ch1.value("HigherLimit").toDouble());
    options1.SetLowerLimit(ch1.value("LowerLimit").toDouble());
    options1.SetHigherMeasureLimit(ch1.value("HigherMeasLimit").toDouble());
    options1.SetLowerMeasureLimit(ch1.value("LowerMeasLimit").toDouble());
    options1.SetSignalType(ch1.value("Type").toDouble());
    options1.SetUnitsName(ch1.value("Units").toString());

    QJsonObject ch2 = array.at(1).toObject();

    options2.SetHigherLimit(ch2.value("HigherLimit").toDouble());
    options2.SetLowerLimit(ch2.value("LowerLimit").toDouble());
    options2.SetHigherMeasureLimit(ch2.value("HigherMeasLimit").toDouble());
    options2.SetLowerMeasureLimit(ch2.value("LowerMeasLimit").toDouble());
    options2.SetSignalType(ch2.value("Type").toDouble());
    options2.SetUnitsName(ch2.value("Units").toString());

    QJsonObject ch3 = array.at(2).toObject();

    options3.SetHigherLimit(ch3.value("HigherLimit").toDouble());
    options3.SetLowerLimit(ch3.value("LowerLimit").toDouble());
    options3.SetHigherMeasureLimit(ch3.value("HigherMeasLimit").toDouble());
    options3.SetLowerMeasureLimit(ch3.value("LowerMeasLimit").toDouble());
    options3.SetSignalType(ch3.value("Type").toDouble());
    options3.SetUnitsName(ch3.value("Units").toString());

    QJsonObject ch4 = array.at(3).toObject();

    options4.SetHigherLimit(ch4.value("HigherLimit").toDouble());
    options4.SetLowerLimit(ch4.value("LowerLimit").toDouble());
    options4.SetHigherMeasureLimit(ch4.value("HigherMeasLimit").toDouble());
    options4.SetLowerMeasureLimit(ch4.value("LowerMeasLimit").toDouble());
    options4.SetSignalType(ch4.value("Type").toDouble());
    options4.SetUnitsName(ch4.value("Units").toString());

    //    qDebug() << json;
    //    qDebug() << ch1;
    //    qDebug() << ch2;
    //    qDebug() << ch3;
    //    qDebug() << ch4;

    infile.close();
}
