#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "options.h"
#include "keyboard.h"
#include "Channels/channelOptions.h"
#include "uartdriver.h"

#include <QFile>
#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <mathresolver.h>
#include "defines.h"

extern QString inputstr;

double DataBuffer::channelinputbuffer[4];
bool DataBuffer::needtoupdatechannel[4] = {0,0,0,0};

QMutex *DataBuffer::channeldatamutex    = new QMutex();
QMutex *DataBuffer::channelupdatemutex  = new QMutex();

QByteArray copyarray;

quint16 DataBuffer::CalculateCRC16RTU(const QByteArray &array)
{
    static const quint16 wCRCTable[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040};

    quint8 nTemp;
    quint16 wCRCWord = 0xFFFF;

    int arraylengt  = array.length();

    if (arraylengt  == 0)
    {
        return 0; // если входящий байтмассив нулевой то сразу возвращаем и не считаем срс16
    }

    for (int i=0; i < arraylengt ;++i)
    {
        nTemp = (quint8)array.at(i) ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord ^= wCRCTable[nTemp];
    }
    //    qDebug() << wCRCWord;
    return wCRCWord;
}

void DataBuffer::writechannelvalue(int channel, double value)
{
    channeldatamutex->lock();
    channelinputbuffer[channel] = value;
    channeldatamutex->unlock();
}

double DataBuffer::readchannelvalue(int channel)
{
    channeldatamutex->lock();
    double res = channelinputbuffer[channel];
    channeldatamutex->unlock();
    return res;
}

bool DataBuffer::readupdatestatus(int channel)
{
    bool res;
    channelupdatemutex->lock();
    res = DataBuffer::needtoupdatechannel[channel];
    channelupdatemutex->unlock();
    return res;
}

void DataBuffer::writeupdatestatus(int channel, bool value)
{
    channelupdatemutex->lock();
    DataBuffer::needtoupdatechannel[channel] = value;
    channelupdatemutex->unlock();
}

QByteArray DataBuffer::ReadAllUartDataByteFormat()
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
        {
            if (serial.size() < 9)
                return 0;
            bytedata.append( serial.readAll() );
        }
    }
    return bytedata;
}

QString DataBuffer::ReadAllUartDataStringFormat()
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
        {
            if (serial.size() < 9) // защита от неправильных пакетов
                return 0;
            bytedata.append( serial.readAll() );
        }
    }
    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(bytedata);
    return DataAsString;
}

QString DataBuffer::ReadAllAvailableCOMPorts()
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


void ModBus::ConfigureDevices(QList<ModbusDeviceStruct> * devstructlist)
{
    ModbusDeviceStruct a ;
    for (int index = 0; index < devstructlist->length(); ++index) {
        a = devstructlist->at(index);
        ConfigureChannel(&a);
    }
}

void  ModBus::ConfigureChannel(ModbusDeviceStruct* devicestructure)
{
    // Задаем тип сигнала
    ModBusSetSingleRegisterUint16(devicestructure->ModbusNetworkAddress,devicestructure->SignalTypeAddress,devicestructure->SignalType);
    // Задаем тип фильтрации
    ModBusSetSingleRegisterUint16(devicestructure->ModbusNetworkAddress,devicestructure->FilterTypeAddress,devicestructure->FilterType);
    //  TransferFunction
    ModBusSetSingleRegisterUint16(devicestructure->ModbusNetworkAddress,devicestructure->TransferFunctionAddress,devicestructure->TransferFunction);
    //  transfer signal hi limit
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->TransferSignalHiLimitAddress,devicestructure->TransferSignalHiLimit);
    //  transfer lo limit
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->TransferSignalLowLimitAddress,devicestructure->TransferSignalLowLimit);
    //  transfer scale
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->TransferScaleHiLimitAddress,devicestructure->TransferScaleHiLimit);
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->TransferScaleLoLimitAddress,devicestructure->TransferScaleLoLimit);
    //  UserCalibration1
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration1GainAddress,devicestructure->UserCalibration1Gain);
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration1OffsetAddress,devicestructure->UserCalibration1Offset);
    ModBusSetSingleRegisterUint32(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration1DateAddress,devicestructure->UserCalibration1Date);
    //  UserCalibration2
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration2GainAddress,devicestructure->UserCalibration2Gain);
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration2OffsetAddress,devicestructure->UserCalibration2Offset);
    ModBusSetSingleRegisterUint32(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration2DateAddress,devicestructure->UserCalibration2Date);
    //  UserCalibration3
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration3GainAddress,devicestructure->UserCalibration3Gain);
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration3OffsetAddress,devicestructure->UserCalibration3Offset);
    ModBusSetSingleRegisterUint32(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration3DateAddress,devicestructure->UserCalibration3Date);
    //  UserCalibration4
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration4GainAddress,devicestructure->UserCalibration4Gain);
    ModBusSetSingleRegisterFloat(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration4OffsetAddress,devicestructure->UserCalibration4Offset);
    ModBusSetSingleRegisterUint32(devicestructure->ModbusNetworkAddress,devicestructure->UserCalibration4DateAddress,devicestructure->UserCalibration4Date);
}

void ModBus::SetSingleCoil(char channel, uint16_t Address, bool newstate)
{
    ModBusSetSingleRegisterUint16(channel,Address,newstate*0xFF00); // если вкл то посылаем FF00 иначе посылаем 0x0000
}

double ModBus::ReadOnBoardVoltage()
{
    return ModBusGetInputRegister(ModBus::Board4AIAddress,ModBus::OnboardVoltageAddress,ModBus::OnboardVoltageLenght); //ModBus::ElmetroChannelAB2Address
}

double ModBus::ReadOnBoardTemperature()
{
    return ModBusGetInputRegister(ModBus::Board4AIAddress,ModBus::OnboardTempAddress,ModBus::OnboardVoltageLenght); //ModBus::ElmetroChannelAB2Address
}

double ModBus::ReadDataChannel(int channeladdress)
{
    return ModBusGetInputRegister(ModBus::Board4AIAddress,channeladdress,ModBus::DataChannelLenght); //ModBus::ElmetroChannelAB2Address
}

void ModBus::WriteDataChannel(int channeladdress, double data)
{
    ModBusSetSingleRegisterFloat(ModBus::Board4AIAddress,channeladdress,data);
}

double ModBus::ClickRelay(char channel)
{
    SetSingleCoil(channel,ModBus::ElmetroRelayAddress,1);
//    longsleep;
    SetSingleCoil(channel,ModBus::ElmetroRelayAddress,0);
//    longsleep;
    return 2;
}

float ModBus::ModBusGetInputRegister(char DeviceAdress,uint16_t Address,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterLenghtHi,RegisterLenghtLo,CRC16Hi,CRC16Lo;

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    RegisterLenghtHi = (int) ((Lenght & 0xFF00)>>8);
    RegisterLenghtLo = (int) (Lenght & 0x00FF);

    requestdata.append(DeviceAdress); // Адрес устройства
    requestdata.append(ModBus::ReadInputRegisters); // Функциональный код - читать регистр
    requestdata.append(AddressHi); // Адрес первого регистра Hi байт
    requestdata.append(AddressLo); //Адрес первого регистра Lo байт
    requestdata.append(RegisterLenghtHi); // Количество регистров Hi байт
    requestdata.append(RegisterLenghtLo); // Количество регистров Lo байт

    quint16 CRC16 = CalculateCRC16RTU(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo);
    requestdata.append(CRC16Hi);
    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response // после этой строки точно вылетает ассерт

    //    qDebug() << requestdata << "requestdata ";
    //    qDebug() << InputDataByteArray << "InputDataByteArray";
    //    qDebug() << requestdata << "requestdata ";

    if (InputDataByteArray.length()<3)
        return ModBus::ConnectionError;

    QByteArray InputDataByteArrayParsed;

    int InputDataByteArraylenght = InputDataByteArray.length();

    // разбираем пакет данных

    for (int byteindex = 0; byteindex < InputDataByteArraylenght; ++byteindex)
    {
        if ((InputDataByteArray.at(byteindex)  == DeviceAdress) && (InputDataByteArray.at(byteindex+1)  == ModBus::ReadInputRegisters)) // ищем когда нулевой байт равен адресу а следующий байт равен функции
        {
            InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex)); // адрес девайса
            InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex+1)); // функция
            InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex+2)); // количество байт далее

            quint8 baytdalee = InputDataByteArray.at(byteindex+2) ; // узнали сколько байт идет далее и записываем их в аррэй

            for (int var2 = 0; var2 < baytdalee; ++var2) {
                InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex+3+var2));
            }

            quint8 inpbytecrchibyteindex = byteindex+4+baytdalee;
            quint8 inpbytecrclobyteindex = inpbytecrchibyteindex-1;
            quint8 inpbytecrchibyte = (uint8_t)InputDataByteArray.at(inpbytecrchibyteindex);
            quint8 inpbytecrclobyte = (uint8_t)InputDataByteArray.at(inpbytecrclobyteindex);
            quint16 inputcrc16summ1 = ((uint16_t) (inpbytecrchibyte<<8))|( (uint16_t) inpbytecrclobyte );
            quint16 calculatedcrc16summ = CalculateCRC16RTU(InputDataByteArrayParsed);

            if (inputcrc16summ1 == calculatedcrc16summ)
            {
                QByteArray arraytofloat;
                float val;

                copyarray.clear();
                arraytofloat.append(InputDataByteArrayParsed.at(3));
                arraytofloat.append(InputDataByteArrayParsed.at(4));
                arraytofloat.append(InputDataByteArrayParsed.at(5));
                arraytofloat.append(InputDataByteArrayParsed.at(6));

                //convert hex to double
                QDataStream stream(arraytofloat);
                stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
                stream >> val;

                return val;
            }
            else
            {
                return ModBus::BadCRC;
            }
        }
    }
    return ModBus::ConnectionError;
}

float ModBus::ModBusGetHoldingRegister(char DeviceAdress,uint16_t Address,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterLenghtHi,RegisterLenghtLo,CRC16Hi,CRC16Lo;

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    RegisterLenghtHi = (int) ((Lenght & 0xFF00)>>8);
    RegisterLenghtLo = (int) (Lenght & 0x00FF);

    //    requestdata.append(DeviceAdress);
    requestdata.append(DeviceAdress); // Адрес устройства
    requestdata.append(ModBus::ReadHoldingRegisters); // Функциональный код - читать регистр
    requestdata.append(AddressHi); // Адрес первого регистра Hi байт
    requestdata.append(AddressLo); //Адрес первого регистра Lo байт
    requestdata.append(RegisterLenghtHi); // Количество регистров Hi байт
    requestdata.append(RegisterLenghtLo); // Количество регистров Lo байт

    //    qDebug() << requestdata;
    quint16 CRC16 = CalculateCRC16RTU(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo);
    requestdata.append(CRC16Hi);
    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response // после этой строки точно вылетает ассерт

    //    qDebug() << requestdata << "requestdata ";
    //    qDebug() << InputDataByteArray << "InputDataByteArray";
    //    qDebug() << requestdata << "requestdata ";

    if (InputDataByteArray.length()<3)
        return ModBus::ConnectionError;

    QByteArray InputDataByteArrayParsed;

    int InputDataByteArraylenght = InputDataByteArray.length();

    // разбираем пакет данных

    for (int byteindex = 0; byteindex < InputDataByteArraylenght; ++byteindex)
    {
        if ((InputDataByteArray.at(byteindex)  == DeviceAdress) && (InputDataByteArray.at(byteindex+1)  == ModBus::ReadHoldingRegisters)) // ищем когда нулевой байт равен адресу а следующий байт равен функции
        {
            InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex)); // адрес девайса
            InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex+1)); // функция
            InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex+2)); // количество байт далее

            quint8 baytdalee = InputDataByteArray.at(byteindex+2) ; // узнали сколько байт идет далее и записываем их в аррэй

            for (int var2 = 0; var2 < baytdalee; ++var2) {
                InputDataByteArrayParsed.append(InputDataByteArray.at(byteindex+3+var2));
            }

            quint8 inpbytecrchibyteindex = byteindex+4+baytdalee;
            quint8 inpbytecrclobyteindex = inpbytecrchibyteindex-1;
            quint8 inpbytecrchibyte = (uint8_t)InputDataByteArray.at(inpbytecrchibyteindex);
            quint8 inpbytecrclobyte = (uint8_t)InputDataByteArray.at(inpbytecrclobyteindex);
            quint16 inputcrc16summ1 = ((uint16_t) (inpbytecrchibyte<<8))|( (uint16_t) inpbytecrclobyte );
            quint16 calculatedcrc16summ = CalculateCRC16RTU(InputDataByteArrayParsed);

            if (inputcrc16summ1 == calculatedcrc16summ)
            {
                QByteArray arraytofloat;
                float val;

                copyarray.clear();
                arraytofloat.append(InputDataByteArrayParsed.at(3));
                arraytofloat.append(InputDataByteArrayParsed.at(4));
                arraytofloat.append(InputDataByteArrayParsed.at(5));
                arraytofloat.append(InputDataByteArrayParsed.at(6));

                //convert hex to double
                QDataStream stream(arraytofloat);
                stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
                stream >> val;
                return val;
            }
            else
            {
                return ModBus::BadCRC;
            }
        }
    }
    return ModBus::ConnectionError;
}

void ModBus::ModBusSetSingleRegisterFloat(char DeviceAdress,uint16_t Address,float Value)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterCountHi,RegisterCountLo,CRC16Hi,CRC16Lo;

    RegisterCountHi =  (int) ((0x02 & 0xFF00)>>8);
    RegisterCountLo = (int) (0x02 & 0x00FF);

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);

    requestdata.append(DeviceAdress); //Адрес устройства
    requestdata.append(ModBus::WriteMultipleRegisters); //Функциональный код
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(RegisterCountHi); // длина hi
    requestdata.append(RegisterCountLo); // длина Lo
    requestdata.append(0x04); // количество байт 4 - фиксированное т.к. посылаем только 1 значение флоата... затем их можно будет посылать несколько но это потом

    // раскладываем флоат в массив
    QByteArray floatarray(reinterpret_cast<const char*>(&Value), sizeof(Value));

    // флоат интерпретируется задом наперед поэтому такая вот рокировочка
    requestdata.append(floatarray.at(3)); // вставляем флоат в запрос
    requestdata.append(floatarray.at(2));
    requestdata.append(floatarray.at(1));
    requestdata.append(floatarray.at(0));

    quint16 CRC16 = CalculateCRC16RTU(requestdata); // считаем срс исходящего пакета
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo); // подставляем в конец контрольную сумму
    requestdata.append(CRC16Hi);

    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response

    //    qDebug() << requestdata << "requestdata";
    //    qDebug() << InputDataByteArray << "InputDataByteArray";
}

void ModBus::ModBusSetSingleRegisterUint16(char DeviceAdress,uint16_t Address,uint16_t Value)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterValueHi,RegisterValueLo,CRC16Hi,CRC16Lo;
    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    RegisterValueHi = (int) ((Value & 0xFF00)>>8);
    RegisterValueLo = (int) (Value & 0x00FF);

    //requestdata.append(DeviceAdress);
    requestdata.append(DeviceAdress); //Адрес устройства
    requestdata.append(ModBus::WriteSingleRegister); //Функциональный код
    requestdata.append(AddressHi); //	Адрес первого регистра Hi байт
    requestdata.append(AddressLo); // Адрес первого регистра Lo байт
    requestdata.append(RegisterValueHi); //Значение Hi байт
    requestdata.append(RegisterValueLo); //Значение Lo байт

    quint16 CRC16 = CalculateCRC16RTU(requestdata); // считаем срс исходящего пакета
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo); // подставляем в конец контрольную сумму
    requestdata.append(CRC16Hi);
    UartWriteData(requestdata); // делаем запрос

    if(InputDataByteArray == requestdata)
    {
        //qDebug() << "InputDataByteArray = requestdata";
    }
}

void ModBus::ModBusSetSingleRegisterUint32(char DeviceAdress,uint16_t Address,uint32_t Value)
{
    char RegisterValueMegaHi, RegisterValueVeryHi, RegisterValueHi,RegisterValueLo;
    RegisterValueMegaHi= (int) ((Value & 0xFF000000)>>24);
    RegisterValueVeryHi = (int) ((Value & 0xFF0000)>>16);
    RegisterValueHi = (int) ((Value & 0xFF00)>>8);
    RegisterValueLo = (int) (Value & 0x00FF);
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterCountHi,RegisterCountLo,CRC16Hi,CRC16Lo;

    RegisterCountHi =  (int) ((0x02 & 0xFF00)>>8);
    RegisterCountLo = (int) (0x02 & 0x00FF);

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);

    requestdata.append(DeviceAdress); //Адрес устройства
    requestdata.append(ModBus::WriteMultipleRegisters); //Функциональный код
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(RegisterCountHi); // длина hi
    requestdata.append(RegisterCountLo); // длина Lo
    requestdata.append(0x04); // количество байт 4 - фиксированное т.к. посылаем только 1 значение флоата... затем их можно будет посылать несколько но это потом

    requestdata.append(RegisterValueMegaHi); // вставляем uint32
    requestdata.append(RegisterValueVeryHi);
    requestdata.append(RegisterValueHi);
    requestdata.append(RegisterValueLo);

    quint16 CRC16 = CalculateCRC16RTU(requestdata); // считаем срс исходящего пакета
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo); // подставляем в конец контрольную сумму
    requestdata.append(CRC16Hi);
    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response
    //qDebug() << requestdata << "requestdata uint32";
    //qDebug() << InputDataByteArray << "InputDataByteArray uint32";
}


QByteArray DataBuffer::UartWriteData(QByteArray data)
{
    QByteArray InputDataByteArray;
    QSerialPort serial;
    serial.setPortName(comportname); //usart1

    if (serial.open(QIODevice::ReadWrite))
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        serial.write(data);
        while (serial.waitForBytesWritten(10))
            ;

        while (serial.waitForReadyRead(10))
        {
            InputDataByteArray = serial.readAll();
        }

        if (InputDataByteArray.length() < 2) // проверка если длина пакета меньше двух, иначе вываливается ассерт
        {
            return 0;
        }

        int inpbytelenght = InputDataByteArray.length() ;
        int lastitemindex = inpbytelenght - 1;
        int lastitem = InputDataByteArray.at(lastitemindex);

        //TODO нужно поправить  - не всегда последний символ 0xff  режется потому что длина иногда может быть >9

        if ((lastitem==-1) && (inpbytelenght>9)) // если последний символ прилетел 0xFF
        {
            InputDataByteArray.remove(lastitemindex,1); // убираем последний символ 0xff
        }

        QByteArray InputDataByteArrayNoCRC = InputDataByteArray;
        InputDataByteArrayNoCRC.remove(InputDataByteArray.length()-2,2);

        uint8_t inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);// до этой строки вроде все нормально работает, ассерт не вылетает // ассерт вываливается на этой строке, сука
        uint8_t inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
        uint16_t inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo);

        uint16_t crc = CalculateCRC16RTU(InputDataByteArrayNoCRC);

        //        qDebug() << data << "data";
        //qDebug() << InputDataByteArray << "InputDataByteArray";

        if (inpcrc == crc) // если срс совпало то возвращаем  байт массив
        {
            return InputDataByteArray;
        }

        // если срс не совпало и все-таки последний символ == 0xff
        else if (lastitem==-1)
        {
            // повторяем то же самое но уже с удаленным последним символом 0xFF
            InputDataByteArray.remove(lastitemindex,1);
            InputDataByteArrayNoCRC.remove(InputDataByteArray.length()-2,2);

            inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);// до этой строки вроде все нормально работает, ассерт не вылетает // ассерт вываливается на этой строке, сука
            inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
            inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo);
            crc = CalculateCRC16RTU(InputDataByteArrayNoCRC);
            if (inpcrc == crc) // если срс совпало то возвращаем  байт массив
            {
                //                qDebug() << "вот щас нормально";
                //                qDebug() << InputDataByteArray << "InputDataByteArray вот щас нормально";
                return InputDataByteArray;
            }
        }
    }
    return 0;
}

uint16_t ModBus::GetChannelSignalType(uint8_t channel)
{
    int channelbias;

    switch (channel) {
    case ModBus::DataChannel1:
        channelbias = ModBus::Channel1AddressBias;
        break;
    case ModBus::DataChannel2:
        channelbias = ModBus::Channel2AddressBias;
        break;
    case ModBus::DataChannel3:
        channelbias = ModBus::Channel3AddressBias;
        break;
    case ModBus::DataChannel4:
        channelbias = ModBus::Channel4AddressBias;
        break;
    default:
        channelbias=0;
    }

    uint16_t address = ModBus::SignalTypeAddress + channelbias;
    return (uint16_t) ModBusGetInputRegister(channel,address,ModBus::DataChannelLenght);
}

void ModBus::SetChannelAdditionalParametr(uint16_t channel, uint16_t additionalparametr)
{
    uint16_t channelbias;

    // avoid warnings
    additionalparametr = channelbias+1;

    switch (channel) {
    case ModBus::DataChannel1:
        channelbias = ModBus::Channel1AddressBias;
        break;
    case ModBus::DataChannel2:
        channelbias = ModBus::Channel2AddressBias;
        break;
    case ModBus::DataChannel3:
        channelbias = ModBus::Channel3AddressBias;
        break;
    case ModBus::DataChannel4:
        channelbias = ModBus::Channel4AddressBias;
        break;
    default:
        channelbias=0;
    }
}

void ModBus::SetChannelSignalType(uint16_t channel, uint16_t signaltype)
{
    channel = signaltype;

    //    uint8_t channelbias;
    //    switch (channel) {
    //    case ModBus::DataChannel1:
    //        channelbias = ModBus::Channel1AddressBias;
    //        break;
    //    case ModBus::DataChannel2:
    //        channelbias = ModBus::Channel2AddressBias;
    //        break;
    //    case ModBus::DataChannel3:
    //        channelbias = ModBus::Channel3AddressBias;
    //        break;
    //    case ModBus::DataChannel4:
    //        channelbias = ModBus::Channel4AddressBias;
    //        break;
    //    }

    //uint16_t address = ModBus::SignalTypeAddress + channelbias;
}

double ModBus::DataChannelRead (char channel)
{
    return ModBusGetInputRegister(channel,ModBus::DataChannel1,ModBus::DataChannelLenght);
}
