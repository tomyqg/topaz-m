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
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QtTest/QTestEvent>
#include <QtTest/QTestEventList>
#include <QtTest/QTest>
#include <QPoint>
#include <QEvent>
#include <QMouseEvent>
#include <QFocusEvent>

#include "options.h"
#include "ui_options.h"
#include "channel1.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("OPTIONS"));
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(checkboxchange()) );
}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
    WriteOptionsToFile();
    savesettings();
    this->close();
}

void Options::on_pushButton_2_clicked()
{
    this->close();
}

void Options::checkboxchange()
{
    if (ui->ButonOtklChannel_1->isChecked())
        qDebug() << "ButonOtklChannel_1";

    if (ui->ButonNapryagenieChannel_1->isChecked())
        qDebug() << "ButonNapryagenieChannel_1";

    if (ui->ButonTokChannel_1->isChecked())
        qDebug() << "ButonTokChannel_1";


}

void Options::on_radioButton_2_clicked()
{
}

void Options::on_radioButton_3_clicked()
{
}

int Options::GetSignalType()
{
    /*
    if ( ui->UnChan1RadButOtkl->isChecked() )
        return 1;
    if ( ui->UnChan1RadButTok->isChecked() )
        return 2;
    if ( ui->UnChan1RadButNapryagenie->isChecked() )
        return 3;
    if ( ui->UnChan1RadButResistor->isChecked() )
        return 4;
    if ( ui->UnChan1RadButTPara->isChecked() )
        return 5;
    if ( ui->UnChan1RadButTPara->isChecked() )
        return 6;*/
    
    return 0;
}

QString Options::GetSignalUnits()
{
    return "Signal units";
}


void Options::savesettings()
{
}

void Options::WriteOptionsToFile()
{
    QJsonObject channel;
    QJsonObject channels;
    QJsonArray settings;
    
    channel["Type"] = GetSignalType();
    channel["Units"] = GetSignalUnits();
    channel["HigherLimit"] = 100;
    channel["LowerLimit"] = 0;
    channel["HigherMeasLimit"] = 20;
    channel["LowerMeasLimit"] = 4;
    
    settings.append(channel);
    
    channels["channel1"] = settings;
    channels["channel2"] = settings;
    
    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
    qDebug() << QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
    //    QFile file("/usr/options.txt");
    QFile file("C:/Work/options.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << setstr;
    file.close();
}

void Options::on_VerhnPredIzmerChannel_1_valueChanged(int arg1)
{
    options1.SetUnitsName("1111");
    options2.SetUnitsName("222");
    options3.SetUnitsName("333");
}

void Options::on_PeriodIzmerChannel_1_valueChanged(int arg1)
{
    qDebug() << options1.GetUnitsName();
    qDebug() << options2.GetUnitsName();
    qDebug() << options3.GetUnitsName();

    options1.SetUnitsName("444");
    options2.SetUnitsName("555");
    options3.SetUnitsName("666");
}

void Options::on_UnitsChannel_1_editingFinished()
{
    options1.SetUnitsName(ui->UnitsChannel_1->text());
    qDebug() << options1.GetUnitsName();
}
