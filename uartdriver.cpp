#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "dialog.h"
#include "channel1.h"
#include "uartdriver.h"

#include <QFile>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPixmap>

#define BeagleBone

#ifdef BeagleBone
#define comportname "/dev/ttyS1"
#define uartsleep delay(50);
QString pathtofile = "/usr/";
#endif

#ifndef BeagleBone
#define comportname "COM3"
#define uartsleep Sleep(50);
QString pathtofile = "C:/Work/";
#endif

extern QString inputstr;

double UartDriver::channelinputbuffer[4];// = {27.22,33.87,57.89,81.11};
double UartDriver::channeltempbuffer[4];// = {27.22,33.87,57.89,81.11};

void UartDriver::readuart()
{
    char arr[8] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D};

    QByteArray requestData;
    QByteArray ba(arr, 8);
    QByteArray arr3;
    float val1;
    QDataStream stream(arr3);

    while (1)
    {
        QSerialPort serial;
        serial.setPortName(comportname); //usart1
        if (serial.open(QIODevice::ReadWrite))
        {
            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::HardwareControl);
            serial.setRequestToSend(true);

            while (1)
            {
                serial.write(ba);
                while (serial.waitForBytesWritten(10))
                    ;

                uartsleep;

                while (serial.waitForReadyRead(10))
                    requestData = serial.readAll();
                arr3.resize(4);
                arr3[0] = requestData.at(5);
                arr3[1] = requestData.at(6);
                arr3[2] = requestData.at(3);
                arr3[3] = requestData.at(4);

                stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                stream >> val1;
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

            serial.setBaudRate(QSerialPort::Baud9600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            serial.setFlowControl(QSerialPort::NoFlowControl);

            SetRTS(1);
            delay(10);
            serial.write(ba);
            while (serial.waitForBytesWritten(10))
                ;

            SetRTS(0);
            uartsleep;

            while (serial.waitForReadyRead(10))
                requestData = serial.readAll();

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
    return val1;
}

QByteArray UartDriver::ReadAllUartByteData()
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

#ifdef BeagleBone
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
#endif
}

int UartDriver::GetXOR(QByteArray bytearray)
{
    unsigned int a = 0;
    foreach (unsigned char val, bytearray)
    {
        a = a^val;
    }
    return a;
}

QByteArray ModBus::ModBusMakeRequest(char DeviceAdress,
                                     char Function,
                                     char StartingAddressHi,
                                     char StartingAddressLo,
                                     char QuantityofInputRegHi,
                                     char QuantityofInputRegLo)
{
    QByteArray requestdata;
    int crc;
    float val1;

    requestdata.append(DeviceAdress);
    requestdata.append(Function);
    requestdata.append(StartingAddressHi);
    requestdata.append(StartingAddressLo);
    requestdata.append(QuantityofInputRegHi);
    requestdata.append(QuantityofInputRegLo);
    crc = GetXOR(requestdata);
    requestdata.append(crc);

    char arr[8] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D};
    QByteArray requestData;
    QByteArray trimmeddata;
    QByteArray ba(arr, 8);

    QSerialPort serial;
    serial.setPortName(comportname); //usart1

    if (serial.open(QIODevice::ReadWrite))
    {
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

        SetRTS(0);
        uartsleep;

        while (serial.waitForReadyRead(10))
            requestData = serial.readAll();

        QByteArray arr3;
        arr3.resize(4);

        //int inddd = requestData.at(requestData.length()-2);

        trimmeddata = requestData;
        trimmeddata.remove(requestData.length() - 2,2);
        int inputcrc = GetXOR(trimmeddata);
        int inputcrcall = GetXOR(requestData);
        int crcfromMB = requestData.at(requestData.length() - 1);

        uartsleep;
        //qDebug() << requestData;
        //qDebug() << requestData.length();
        //qDebug() << requestData.at(5);

        //qDebug() << trimmeddata;
        //qDebug() << trimmeddata.length();
        //qDebug() << inputcrcall;
        //qDebug() << inputcrc;

        if (inputcrc == inputcrcall)
            //qDebug() << "crc VALID";

            arr3[0] = requestData.at(5);
        arr3[1] = requestData.at(6);
        arr3[2] = requestData.at(3);
        arr3[3] = requestData.at(4);

        QDataStream stream(arr3);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> val1;
    }

    return requestData;
}

double ModBus::ReadTemperature(char channel)
{
    QByteArray arr;
    QByteArray RequestRespose;
    float val;
    RequestRespose = ModBusMakeRequest(channel,
                                       ModBus::ReadHoldingRegisters,
                                       ModBus::TemperetureAdressHi,
                                       ModBus::TemperetureAdressLo,
                                       ModBus::TemperetureRegCountHi,
                                       ModBus::TemperetureRegCountLo);
    arr.resize(4);
    arr[0] = RequestRespose.at(5);
    arr[1] = RequestRespose.at(6);
    arr[2] = RequestRespose.at(3);
    arr[3] = RequestRespose.at(4);
    //convert hex to double
    QDataStream stream(arr);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream >> val;
    return val;
}

double ModBus::ReadVoltage(char channel)
{
    QByteArray arr;
    QByteArray RequestRespose;
    float val;
    RequestRespose = ModBusMakeRequest(channel,
                                       ModBus::ReadHoldingRegisters,
                                       ModBus::VoltageAdressHi,
                                       ModBus::VoltageAdressLo,
                                       ModBus::VoltageRegCountHi,
                                       ModBus::VoltageRegCountLo);
    arr.resize(4);
    arr[0] = RequestRespose.at(5);
    arr[1] = RequestRespose.at(6);
    arr[2] = RequestRespose.at(3);
    arr[3] = RequestRespose.at(4);

    //convert hex to double
    QDataStream stream(arr);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream >> val;
    return val;
}
