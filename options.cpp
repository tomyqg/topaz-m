#include "options.h"
#include "ui_options.h"

//#include "mainwindow.h"
//#include "ui_mainwindow.h"

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

#include "channel1.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("OPTIONS"));
    connect(ui->ButonOtklChannel_1, SIGNAL(pressed()), this, SLOT(checkboxchange()) );

    // параметры для каждого канала

//    ChannelOptions options1;
//    ChannelOptions options2;
//    QList<ChannelOptions> ChannelOptionsList;
//    ChannelOptions tempoptions1;
//    tempoptions1.SetUnitsName("Ampers321");
//    ChannelOptionsList.append(tempoptions1);
//    qDebug() << ChannelOptionsList[0].GetUnitsName();
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
    ui->Chanel1_Units->setText("newstring");
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
    QJsonObject channel1;
    QJsonObject channels;
    QJsonArray settings;
    
    //Channel1Options a;
    
    channel1["Type"] = GetSignalType();
    channel1["Units"] = GetSignalUnits();
    channel1["HigherLimit"] = 100;
    channel1["LowerLimit"] = 0;
    channel1["HigherMeasLimit"] = 20;
    channel1["LowerMeasLimit"] = 4;
    
    settings.append(channel1);
    
    channels["channel1"] = settings;
    channels["channel2"] = settings;
    
    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
    qDebug() << QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
    //    QFile file("/sys/class/gpio/gpio69/value");
    QFile file("C:/Work/options.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << setstr;
    file.close();
}



void Options::on_NPI_BOX_3_valueChanged(int arg1)
{
    options1.SetUnitsName("1111");
    options2.SetUnitsName("222");
    options3.SetUnitsName("333");
}

void Options::on_NPI_BOX_4_valueChanged(int arg1)
{

    qDebug() << options1.GetUnitsName();
    qDebug() << options2.GetUnitsName();
    qDebug() << options3.GetUnitsName();


    options1.SetUnitsName("444");
    options2.SetUnitsName("555");
    options3.SetUnitsName("666");
}
