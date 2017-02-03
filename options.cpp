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
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(Channel1TypeChange()) );
    connect(ui->buttonGroup_2, SIGNAL(buttonClicked(int)), this, SLOT(Channel2TypeChange()) );
    connect(ui->buttonGroup_3, SIGNAL(buttonClicked(int)), this, SLOT(Channel3TypeChange()) );
    connect(ui->buttonGroup_4, SIGNAL(buttonClicked(int)), this, SLOT(Channel4TypeChange()) );
}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
    applynewsettings();
    WriteOptionsToFile();
    this->close();
}

void Options::on_pushButton_2_clicked()
{
    this->close();
}

void Options::Channel1TypeChange()
{
    if (ui->ButonOtklChannel_1->isChecked())
    {
        options1.SetSignalType(1);
        ui->UnitsChannel_1->setText("None");
        ui->UnitsChannel_1->setEnabled(false);
        ui->VerhnPredelChannel_1->setEnabled(false);
        ui->NignPredelChannel_1->setEnabled(false);
        ui->NignPredIzmerChannel_1->setEnabled(false);
        ui->VerhnPredIzmerChannel_1->setEnabled(false);
        ui->PeriodIzmerChannel_1->setEnabled(false);
    }
    else
    {
        ui->UnitsChannel_1->setEnabled(true);
        ui->VerhnPredelChannel_1->setEnabled(true);
        ui->NignPredelChannel_1->setEnabled(true);
        ui->NignPredIzmerChannel_1->setEnabled(true);
        ui->VerhnPredIzmerChannel_1->setEnabled(true);
        ui->PeriodIzmerChannel_1->setEnabled(true);
    }
    
    if (ui->ButonTokChannel_1->isChecked())
    {
        ui->UnitsChannel_1->setText("мА");
        options1.SetSignalType(2);
        //qDebug() << "options1";
        //qDebug() << options1.GetSignalType();
    }
    
    if (ui->ButonNapryagenieChannel_1->isChecked())
    {
        options1.SetSignalType(3);
        ui->UnitsChannel_1->setText("В");
        //qDebug() << "options1";
        //qDebug() << options1.GetSignalType();
    }
    
    if (ui->ButonResistorChannel_1->isChecked())
    {
        options1.SetSignalType(4);
        ui->UnitsChannel_1->setText("Ом");
        //qDebug() << "options1";
        //qDebug() << options1.GetSignalType();
    }
    
    if (ui->ButonTermoparaChannel_1->isChecked())
    {
        options1.SetSignalType(5);
        ui->UnitsChannel_1->setText("мВ");
        //qDebug() << "options1";
        //qDebug() << options1.GetSignalType();
    }
    
    if (ui->ButonImpulseChannel_1->isChecked())
    {
        options1.SetSignalType(6);
        ui->UnitsChannel_1->setText("1");
        //qDebug() << "options1";
        //qDebug() << options1.GetSignalType();
    }
}

void Options::Channel2TypeChange()
{
    if (ui->ButonOtklChannel_2->isChecked())
    {
        options2.SetSignalType(1);
        //qDebug() << "options2";
        //qDebug() << options2.GetSignalType();
    }
    
    if (ui->ButonTokChannel_2->isChecked())
    {
        options2.SetSignalType(2);
        //qDebug() << "options2";
        //qDebug() << options2.GetSignalType();
    }
    
    if (ui->ButonNapryagenieChannel_2->isChecked())
    {
        options2.SetSignalType(3);
        //qDebug() << "options2";
        //qDebug() << options2.GetSignalType();
    }
    
    if (ui->ButonResistorChannel_2->isChecked())
    {
        options2.SetSignalType(4);
        //qDebug() << "options2";
        //qDebug() << options2.GetSignalType();
    }
    
    if (ui->ButonTermoparaChannel_2->isChecked())
    {
        options2.SetSignalType(5);
        //qDebug() << "options2";
        //qDebug() << options2.GetSignalType();
    }
    
    if (ui->ButonImpulseChannel_2->isChecked())
    {
        options2.SetSignalType(6);
        //qDebug() << "options2";
        //qDebug() << options2.GetSignalType();
    }
}

void Options::Channel3TypeChange()
{
    if (ui->ButonOtklChannel_3->isChecked())
    {
        options3.SetSignalType(1);
    }

    if (ui->ButonTokChannel_3->isChecked())
    {
        options3.SetSignalType(2);
    }

    if (ui->ButonNapryagenieChannel_3->isChecked())
    {
        options3.SetSignalType(3);
    }

    if (ui->ButonResistorChannel_3->isChecked())
    {
        options3.SetSignalType(4);
    }

    if (ui->ButonTermoparaChannel_3->isChecked())
    {
        options3.SetSignalType(5);
    }

    if (ui->ButonImpulseChannel_3->isChecked())
    {
        options3.SetSignalType(6);
    }
}

void Options::Channel4TypeChange()
{
    if (ui->ButonOtklChannel_4->isChecked())
    {
        options4.SetSignalType(1);
    }

    if (ui->ButonTokChannel_4->isChecked())
    {
        options4.SetSignalType(2);
    }

    if (ui->ButonNapryagenieChannel_4->isChecked())
    {
        options4.SetSignalType(3);
    }

    if (ui->ButonResistorChannel_4->isChecked())
    {
        options4.SetSignalType(4);
    }

    if (ui->ButonTermoparaChannel_4->isChecked())
    {
        options4.SetSignalType(5);
    }

    if (ui->ButonImpulseChannel_4->isChecked())
    {
        options4.SetSignalType(6);
    }
}

void Options::on_radioButton_2_clicked()
{
}

void Options::on_radioButton_3_clicked()
{
}

QString Options::GetSignalUnits()
{
    return "Signal units";
}

void Options::applynewsettings()
{
    Channel1TypeChange();
    options1.SetUnitsName(ui->UnitsChannel_1->text());
    options1.SetHigherLimit(ui->VerhnPredelChannel_1->value());
    options1.SetLowerLimit(ui->NignPredelChannel_1->value());
    options1.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_1->value());
    options1.SetLowerMeasureLimit(ui->NignPredIzmerChannel_1->value());

    Channel2TypeChange();
    options2.SetUnitsName(ui->UnitsChannel_2->text());
    options2.SetHigherLimit(ui->VerhnPredelChannel_2->value());
    options2.SetLowerLimit(ui->NignPredelChannel_2->value());
    options2.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_2->value());
    options2.SetLowerMeasureLimit(ui->NignPredIzmerChannel_2->value());
}

void Options::WriteOptionsToFile()
{
    QJsonObject channel1;
    QJsonObject channel2;
    QJsonObject channel3;
    QJsonObject channel4;
    QJsonObject channels;
    QJsonArray settings;
    
    channel1["Type"] = options1.GetSignalType();
    channel1["Units"] = options1.GetUnitsName();
    channel1["HigherLimit"] = options1.GetHigherLimit();
    channel1["LowerLimit"] = options1.GetLowerLimit();
    channel1["HigherMeasLimit"] = options1.GetHigherMeasureLimit();
    channel1["LowerMeasLimit"] = options1.GetLowerMeasureLimit();
    
    settings.append(channel1);
    
    channel2["Type"] = options2.GetSignalType();
    channel2["Units"] = options2.GetUnitsName();
    channel2["HigherLimit"] = options2.GetHigherLimit();
    channel2["LowerLimit"] = options2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options2.GetLowerMeasureLimit();
    

    channel2["Type"] = options2.GetSignalType();
    channel2["Units"] = options2.GetUnitsName();
    channel2["HigherLimit"] = options2.GetHigherLimit();
    channel2["LowerLimit"] = options2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options2.GetLowerMeasureLimit();


    settings.append(channel2);
    
    channels["count"] = 2;
    channels["channels"] = settings;
    
    //    channels["channel2"] = settings;
    
    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
    //qDebug() << QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
//    QFile file("/usr/options.txt");
    QFile file("C:/Work/options.txt");
    file.open(QIODevice::ReadWrite);
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
    //qDebug() << options1.GetUnitsName();
    //qDebug() << options2.GetUnitsName();
    //qDebug() << options3.GetUnitsName();
    
    options1.SetUnitsName("444");
    options2.SetUnitsName("555");
    options3.SetUnitsName("666");
}

void Options::on_UnitsChannel_1_editingFinished()
{
    options1.SetUnitsName(ui->UnitsChannel_1->text());
    //qDebug() << options1.GetUnitsName();
}
