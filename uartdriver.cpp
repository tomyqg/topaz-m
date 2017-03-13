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
#include <uartdriver.h>

//#define comportname "/dev/ttyS1"
//#define uartsleep delay(50);

#define comportname "COM3"
#define uartsleep Sleep(50);

//serial.setPortName("/dev/ttyS1"); //usart1
//serial.setPortName("COM3"); //usart1

extern QString inputstr ;

double UartDriver::channelinputbuffer[] = {27.22,33.87,57.89,81.11};
double UartDriver::channeltempbuffer[] = {27.22,33.87,57.89,81.11};

void UartDriver::readuart()
{
    char arr[9] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D, '\n'};

    QByteArray requestData;
    // 01 03 00 00 00 0A C5 CD
    QByteArray ba(arr, 8);

    while (1)
    {
        QSerialPort serial;
        serial.setPortName(comportname); //usart1
        if (serial.open(QIODevice::ReadWrite))
        {
            //qDebug() << serial.portName() + " Opened";

            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::HardwareControl);

            serial.setRequestToSend(true);

            while (1)
            {

            }

            while (1)
            {
                serial.write(ba);
                while (serial.waitForBytesWritten(10))
                    ;

                uartsleep;

                while (serial.waitForReadyRead(10))
                    requestData = serial.readAll();

                //qDebug() << "recieve: " + requestData;

                char arr2[4] = {0x41, 0x3F, 0xD9, 0xAB};
                QByteArray tb(arr2, 4);


                QByteArray arr3;
                arr3.resize(4);

                arr3[0] = requestData.at(5);
                arr3[1] = requestData.at(6);
                arr3[2] = requestData.at(3);
                arr3[3] = requestData.at(4);

                float val1;
                QDataStream stream(arr3);
                stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                stream >> val1;

                //qDebug() << val1; // val = 0
            }
        }
    }
}

void UartDriver::writechannelvalue(int channel, double value)
{

    this->channelinputbuffer[channel-1] = value;
}

double UartDriver::readchannelvalue(int channelnumber)
{
    float val1;

    char arr[9] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D, '\n'};
    QByteArray requestData;
    QByteArray ba(arr, 8);

    {
        QSerialPort serial;

        serial.setPortName(comportname); //usart1
        if (serial.open(QIODevice::ReadWrite))
        {
            while (0)
            {
                SetRTS(1);delay(50);
                SetRTS(0);delay(10);
            }
            //            delay(500);
            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);

            SetRTS(1);
            delay(10);
            //uartsleep;
            serial.write(ba);
            while (serial.waitForBytesWritten(10))
                ;

            //while (serial.waitForBytesWritten(20))
            //  ;
            //            delay(10);
            SetRTS(0);
            uartsleep;

            while (serial.waitForReadyRead(10))
                requestData = serial.readAll();

            //qDebug() << "recieve: " + requestData;

            char arr2[4] = {0x41, 0x3F, 0xD9, 0xAB};
            QByteArray tb(arr2, 4);


            QByteArray arr3;
            arr3.resize(4);

            arr3[0] = requestData.at(5);
            arr3[1] = requestData.at(6);
            arr3[2] = requestData.at(3);
            arr3[3] = requestData.at(4);

            QDataStream stream(arr3);
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
            stream >> val1;

            //                process.startDetached("config-pin -a P8.07 hi");

        }
    }
    return val1;
}



float UartDriver::readchannel1value(int channelnumber)
{

    float val1;

    char arr[9] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D, '\n'};
    QByteArray requestData;
    QByteArray ba(arr, 8);

    //    while (1)
    {
        QSerialPort serial;
        serial.setPortName(comportname); //usart1
        if (serial.open(QIODevice::ReadWrite))
        {
            //qDebug() << serial.portName() + " Opened";

            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::HardwareControl);

            serial.setRequestToSend(true);

            //            while (1)
            {
                serial.write(ba);
                while (serial.waitForBytesWritten(10))
                    ;

                uartsleep;
                while (serial.waitForReadyRead(10))
                    requestData = serial.readAll();

                //qDebug() << "recieve: " + requestData;

                char arr2[4] = {0x41, 0x3F, 0xD9, 0xAB};
                QByteArray tb(arr2, 4);

                QByteArray arr3;
                arr3.resize(4);

                arr3[0] = requestData.at(5);
                arr3[1] = requestData.at(6);
                arr3[2] = requestData.at(3);
                arr3[3] = requestData.at(4);

                QDataStream stream(arr3);
                stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                stream >> val1;
            }
        }
    }
    return val1;
}

QByteArray UartDriver::ReadAllUartByteData()
{
    QSerialPort serial;
    QByteArray bytedata;
    serial.setPortName(comportname); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {

        //serial.waitForReadyRead(10);
        //qDebug() << serial.bytesAvailable();
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        while (serial.waitForReadyRead(10))
            bytedata.append( serial.readAll() );
    }

    return bytedata;
}

QString UartDriver::ReadAllUartStringData()
{
    QSerialPort serial;
    QByteArray bytedata;

    serial.setPortName(comportname); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        while (serial.waitForReadyRead(10))
            bytedata.append( serial.readAll() );
    }

    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(bytedata);

    return DataAsString;
}

QString UartDriver::readalluartports()
{
    QString a;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

        {
            QSerialPort serial;
            serial.setPort(info);
            a.append(info.portName());
        }
    }

    return a;
}

void UartDriver::writedata()
{
    char arr[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD};

    QByteArray ba(arr, 8);
    QByteArray bytedata;
    QSerialPort serial;

    serial.setPortName(comportname); //usart1
    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::SoftwareControl);

        serial.write(ba);

        while (serial.waitForBytesWritten(100))
            ;
        uartsleep;

        while (serial.waitForReadyRead(100))
            bytedata.append( serial.readAll() );
    }
    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(bytedata);
    serial.close();
}

void UartDriver::delay(int n)
{
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
        ;//        QCoreApplication::processEvents(QEventLoop::AllEvents,1);
}

void UartDriver::SetRTS(bool newstate)
{
    QFile filedir("/sys/class/gpio/gpio66/direction");

    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);

    outdir << "out";
    filedir.close();

    QFile file("/sys/class/gpio/gpio66/value");
    QTextStream out(&file);
    file.open(QIODevice::WriteOnly);

    out << newstate;
    file.close();
}

int UartDriver::GetXOR(QByteArray bytearray)
{
    unsigned int a = 0;

    foreach (unsigned char val, bytearray)
    {
        a = a^val;
    }
    //    qDebug() << a;
    return a;
}
QByteArray ModBus::ModBusMakeRequest(char deviceaddress,char functcode, char startaddress, char registercountforread)
{
    char arr[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0xCD};

    QByteArray ba(arr, 8);
    QByteArray tmp(arr, 7);
    QByteArray bytedata;
    QSerialPort serial;
    QByteArray requestdata;

    requestdata.append(deviceaddress);
    requestdata.append(functcode);
    requestdata.append(startaddress);
    requestdata.append(registercountforread);

    int crc;
    crc = GetXOR(requestdata);

    requestdata.append(crc);
    qDebug() << requestdata;

    serial.setPortName(comportname); //usart1

    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::SoftwareControl);

        serial.write(ba);

        while (serial.waitForBytesWritten(100))
            ;
        uartsleep;

        while (serial.waitForReadyRead(100))
            bytedata.append( serial.readAll() );
        //qDebug() << "bytesAvailable" + serial.bytesAvailable();
    }

    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(bytedata);
    serial.close();
    return 0;
}
