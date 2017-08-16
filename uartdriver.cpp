#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "channel1.h"
#include "uartdriver.h"

#include <QFile>
#include <QThread>
#include <QDebug>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <mathresolver.h>
#include "defines.h"

extern QString inputstr;

double UartDriver::channelinputbuffer[4];
double UartDriver::channeltempbuffer[4];
bool UartDriver::needtoupdatechannel[4] = {0,0,0,0};

QByteArray copyarray;

quint16 UartDriver::Calculate_crc16_modbus(const QByteArray &array)
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

void UartDriver::writechannelvalue(int channel, double value)
{
    this->channelinputbuffer[channel-1] = value;
}

double UartDriver::readchannelvalue(int channel)
{
    return this->channelinputbuffer[channel-1];
}

QByteArray UartDriver::ReadAllUartDataByteFormat()
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

QString UartDriver::ReadAllUartDataStringFormat()
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

QString UartDriver::ReadAllAvailableCOMPorts()
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

void UartDriver::DelayMsec(int n)
{
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
        ;
}

void UartDriver::SetRTS(bool newstate)
{
    newstate;
    // если плата MYD то она автоматом делает направление линии. то есть RTS пин дергать не нужно.
#ifdef MYD
    return;
#endif

#ifdef LinuxBoard
    QFile file(GetPathToRTSPinValue());
    QTextStream out(&file);
    file.open(QIODevice::WriteOnly);
    out << newstate;
    file.close();
#endif
}

void  UartDriver::SetRTSPinDirection()
{
    // если плата MYD то она автоматом делает направление линии. то есть RTS пин дергать не нужно.
#ifdef MYD
    return;
#endif

#ifdef LinuxBoard
    QFile filedir(GetPathToRTSPinDirection());
    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);
    outdir << "out";
    filedir.close();
#endif
}

void  ModBus::ConfigureChannel(ModbusDeviceStruct* devstruct)
{

}

void ModBus::SetSingleCoil(char channel, uint16_t Address, bool newstate)
{
    switch (newstate) {
    case 0:
        ModBusSetRegisterUint16(channel,ModBus::WriteSingleCoil,Address,0x0000);
        break;

    case 1:
        ModBusSetRegisterUint16(channel,ModBus::WriteSingleCoil,Address,0xFF00);
        break;

    default:
        break;
    }
}

double ModBus::ReadOnBoardVoltage()
{
    return ModBusGetRegister(ModBus::Board4AIAddress,ModBus::ReadInputRegisters,ModBus::OnboardVoltageAddress,ModBus::OnboardVoltageLenght); //ModBus::ElmetroChannelAB2Address
}

double ModBus::ReadOnBoardTemperature()
{
    return ModBusGetRegister(ModBus::Board4AIAddress,ModBus::ReadInputRegisters,ModBus::OnboardTempAddress,ModBus::OnboardVoltageLenght); //ModBus::ElmetroChannelAB2Address
}

double ModBus::ReadDataChannel(int channeladdress)
{
    return ModBusGetRegister(ModBus::Board4AIAddress,ModBus::ReadInputRegisters,channeladdress,ModBus::DataChannelLenght); //ModBus::ElmetroChannelAB2Address
}

void ModBus::WriteDataChannel(int channeladdress, double data)
{
    ModBusSetRegisterFloat(ModBus::Board4AIAddress,ModBus::WriteMultipleRegisters,channeladdress,data);
}

double ModBus::ClickRelay(char channel)
{
    SetSingleCoil(channel,ModBus::ElmetroRelayAddress,1);
    longsleep;
    SetSingleCoil(channel,ModBus::ElmetroRelayAddress,0);
    longsleep;
    return 2;
}

float ModBus::ModBusGetRegister(char DeviceAdress,char Function,uint16_t Address,uint16_t Lenght)
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
    requestdata.append(Function); // Функциональный код
    requestdata.append(AddressHi); // Адрес первого регистра Hi байт
    requestdata.append(AddressLo); //Адрес первого регистра Lo байт
    requestdata.append(RegisterLenghtHi); // Количество регистров Hi байт
    requestdata.append(RegisterLenghtLo); // Количество регистров Lo байт

    //    qDebug() << requestdata;
    quint16 CRC16 = Calculate_crc16_modbus(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo);
    requestdata.append(CRC16Hi);
    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response // после этой строки точно вылетает ассерт

    //    qDebug() << requestdata << "requestdata ";
    if (InputDataByteArray.length()<3)
        return ModBus::ConnectionError;

    QByteArray InputDataByteArrayParsed;

    int InputDataByteArraylenght = InputDataByteArray.length();

    // разбираем пакет данных

    for (int byteindex = 0; byteindex < InputDataByteArraylenght; ++byteindex)
    {
        if ((InputDataByteArray.at(byteindex)  == DeviceAdress) && (InputDataByteArray.at(byteindex+1)  == Function)) // ищем когда нулевой байт равен адресу а следующий байт равен функции
        {
            //            InputDataByteArrayParsed.clear(); // если нашли такую последовательность то пошли дальше обрабатывать данные

            //if ( (Function>=0x01)|| (Function<=0x04) ) // если запрос на чтение 01 (0x01)	Чтение DO 02 (0x02)	Чтение DI 03 (0x03)	Чтение AO 04 (0x04)	Чтение AI
            if (  (Function==0x04) ) // если запрос на Чтение AO 04 (0x04)	Чтение AI
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

                quint16 calculatedcrc16summ = Calculate_crc16_modbus(InputDataByteArrayParsed);

                if (inputcrc16summ1 == calculatedcrc16summ)
                {
                    //                    SetConnectFailure(0);
                    QByteArray arraytofloat;
                    float val;

                    copyarray.clear();

                    arraytofloat.append(InputDataByteArrayParsed.at(3));
                    arraytofloat.append(InputDataByteArrayParsed.at(4));
                    arraytofloat.append(InputDataByteArrayParsed.at(5));
                    arraytofloat.append(InputDataByteArrayParsed.at(6));

                    //                    arraytofloat.append(0x01);
                    //                    arraytofloat.append(0x02);
                    //                    arraytofloat.append(0xFC);
                    //                    arraytofloat.append(0x19);

                    copyarray.append(InputDataByteArrayParsed.at(3));
                    copyarray.append(InputDataByteArrayParsed.at(4));
                    copyarray.append(InputDataByteArrayParsed.at(5));
                    copyarray.append(InputDataByteArrayParsed.at(6));

                    //                    qDebug() << arraytofloat << "arraytofloat ";
                    //                    qDebug() << arraytofloat.at(0) << arraytofloat.at(1) << arraytofloat.at(2) << arraytofloat.at(3) << arraytofloat.at(4) ;

                    //qDebug() << copyarray << "copyarray ";
                    //qDebug() << arraytofloat.at(1) << "arraytofloat.at(0) ";
                    //qDebug() << copyarray.at(1) << "copyarray.at(0) ";

                    //convert hex to double
                    QDataStream stream(arraytofloat);
                    stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
                    stream >> val;
                    return val;
                }
                else
                {
                    //                    qDebug() << "BadCRC";
                    return ModBus::BadCRC;
                }
            }
        }
    }
    return ModBus::ConnectionError;
}

void ModBus::ModBusSetRegisterFloat(char DeviceAdress,char Function,uint16_t Address,float Value)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterCountHi,RegisterCountLo,CRC16Hi,CRC16Lo;

    RegisterCountHi =  (int) ((0x02 & 0xFF00)>>8);
    RegisterCountLo = (int) (0x02 & 0x00FF);

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);

    requestdata.append(DeviceAdress); //Адрес устройства
    requestdata.append(Function); //Функциональный код
    requestdata.append(AddressHi); //	Адрес первого регистра Hi байт
    requestdata.append(AddressLo); // Адрес первого регистра  hi

    requestdata.append(RegisterCountHi); // длина hi
    requestdata.append(RegisterCountLo); // длина Lo

    requestdata.append(0x04); // количество байт

    // записываем 88.88
    requestdata.append(0x42);
    requestdata.append(0xB1);
    requestdata.append(0xC2);
    requestdata.append(0x8F);

    quint16 CRC16 = Calculate_crc16_modbus(requestdata); // считаем срс исходящего пакета
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);

    requestdata.append(CRC16Lo); // Контрольная сумма CRC
    requestdata.append(CRC16Hi); // Контрольная сумма CRC

    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response

    qDebug() << requestdata << "requestdata ";
    qDebug() << InputDataByteArray << "InputDataByteArray";
}

void ModBus::ModBusSetRegisterUint16(char DeviceAdress,char Function,uint16_t Address,uint16_t Value)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,RegisterValueHi,RegisterValueLo,CRC16Hi,CRC16Lo;
    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    RegisterValueHi = (int) ((Value & 0xFF00)>>8);
    RegisterValueLo = (int) (Value & 0x00FF);

    //    requestdata.append(DeviceAdress);
    requestdata.append(DeviceAdress); //Адрес устройства
    requestdata.append(Function); //Функциональный код
    requestdata.append(AddressHi); //	Адрес первого регистра Hi байт
    requestdata.append(AddressLo); // Адрес первого регистра Lo байт
    requestdata.append(RegisterValueHi); //Значение Hi байт
    requestdata.append(RegisterValueLo); //Значение Lo байт

    //    quint16 CRC16 = Calculate_crc16_modbus(requestdata); // считаем срс исходящего пакета
    //    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    //    CRC16Lo = (int) (CRC16 & 0x00FF);

    //    requestdata.append(CRC16Lo); // Контрольная сумма CRC
    //    requestdata.append(CRC16Hi); // Контрольная сумма CRC

    //    InputDataByteArray = UartWriteData(requestdata); // make request and recieve response // после этой строки точно вылетает ассерт

    return;
}

QByteArray ModBus::ModBusMakeRequest(char DeviceAdress,char Function,uint16_t Address,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,LenghtHi,LenghtLo,CRC16Hi,CRC16Lo;

    AddressHi = (int) ((Address & 0xFF00)>>8);
    AddressLo = (int) (Address & 0x00FF);
    LenghtHi = (int) ((Lenght & 0xFF00)>>8);
    LenghtLo = (int) (Lenght & 0x00FF);

    requestdata.append(DeviceAdress);
    requestdata.append(Function);
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(LenghtHi);
    requestdata.append(LenghtLo);

    quint16 CRC16 = Calculate_crc16_modbus(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);
    requestdata.append(CRC16Hi);
    requestdata.append(CRC16Lo);

    InputDataByteArray  = UartWriteData(requestdata);

    QByteArray InputDataByteArrayNoCRCnew = InputDataByteArray;
    InputDataByteArrayNoCRCnew.remove(InputDataByteArray.length()-2,2);
    quint8 inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);
    quint8 inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
    quint16 inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo );

    quint16 crc = Calculate_crc16_modbus(InputDataByteArrayNoCRCnew);

    if (inpcrc == crc)
    {
        return InputDataByteArray;// qDebug() << "CRC GOOD";
    }
    else
    {
        return 0;// qDebug() << "CRC BAD";
    }
    return 0;
}

QByteArray ModBus::ModBusMakeRequest(char DeviceAdress,char Function,uint16_t Address,uint16_t AddressBias,uint16_t Lenght)
{
    QByteArray requestdata;
    QByteArray InputDataByteArray;

    char AddressHi,AddressLo,LenghtHi,LenghtLo,CRC16Hi,CRC16Lo;

    AddressHi = (int) (((Address + AddressBias) & 0xFF00)>>8);
    AddressLo = (int) ((Address + AddressBias) & 0x00FF);
    LenghtHi = (int) ((Lenght & 0xFF00)>>8);
    LenghtLo = (int) (Lenght & 0x00FF);

    requestdata.append(DeviceAdress);
    requestdata.append(Function);
    requestdata.append(AddressHi);
    requestdata.append(AddressLo);
    requestdata.append(LenghtHi);
    requestdata.append(LenghtLo);

    quint16 CRC16 = Calculate_crc16_modbus(requestdata);
    CRC16Hi = (int) ((CRC16 & 0xFF00)>>8);
    CRC16Lo = (int) (CRC16 & 0x00FF);
    requestdata.append(CRC16Hi);
    requestdata.append(CRC16Lo);
    InputDataByteArray  = UartWriteData(requestdata);

    QByteArray InputDataByteArrayNoCRCnew = InputDataByteArray;
    InputDataByteArrayNoCRCnew.remove(InputDataByteArray.length()-2,2);
    quint8 inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);
    quint8 inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
    quint16  inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo );
    quint16  crc = Calculate_crc16_modbus(InputDataByteArrayNoCRCnew);

    if (inpcrc == crc)
    {
        return InputDataByteArray;
    }
    else
    {
        return 0;
    }
    return 0;
}

QByteArray UartDriver::UartWriteData(QByteArray data)
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

        SetRTS(1);
        serial.write(data);
        while (serial.waitForBytesWritten(10))
            ;
        SetRTS(0);
        uartsleep;

        while (serial.waitForReadyRead(10))
        {
            InputDataByteArray = serial.readAll();
        }

        int lengggh = InputDataByteArray.length() ;
        int lastindex = lengggh - 1;
        int lastitem = InputDataByteArray.at(lastindex);

        if ((lastitem==-1) && (lengggh>9)) //(lengggh>9)&&
        {
            InputDataByteArray.remove(lastindex,1);
        }

        if (InputDataByteArray.length() < 2) // проверка если длина пакета меньше двух, иначе вываливается ассерт
        {
            lengggh  = lengggh ;
            return 0;
        }

        QByteArray InputDataByteArrayNoCRC = InputDataByteArray;
        InputDataByteArrayNoCRC.remove(InputDataByteArray.length()-2,2);

        uint8_t inpcrchi = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-1);// до этой строки вроде все нормально работает, ассерт не вылетает // ассерт вываливается на этой строке, сука
        uint8_t inpcrclo = (uint8_t)InputDataByteArray.at(InputDataByteArray.length()-2);
        uint16_t inpcrc = ((uint16_t) (inpcrchi<<8))|( (uint16_t) inpcrclo);

        uint16_t crc = Calculate_crc16_modbus(InputDataByteArrayNoCRC);

        if (inpcrc == crc)
        {
            //qDebug() << "Crc ok,return";
            return InputDataByteArray;
        }
        else
        {
            //qDebug() << "Crc Bad";
            return 0;
        }
    }
    else
    {
        //qDebug() << "Not Open";
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
    }

    uint16_t address = ModBus::SignalTypeAddress + channelbias;
    return (uint16_t) ModBusGetRegister(channel,ModBus::ReadInputRegisters,address,ModBus::DataChannelLenght);
}

void ModBus::SetChannelAdditionalParametr(uint16_t channel, uint16_t additionalparametr)
{
    uint8_t channelbias;

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
    }

    uint16_t address = ModBus::AdditionalParameterAddress + channelbias;
    ModBusSetRegisterUint16(channel,ModBus::WriteSingleCoil,address,additionalparametr);
}

void ModBus::SetChannelSignalType(uint16_t channel, uint16_t signaltype)
{
    uint8_t channelbias;

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
    }
    uint16_t address = ModBus::SignalTypeAddress + channelbias;
    ModBusSetRegisterUint16(channel,ModBus::WriteSingleCoil,address,signaltype);
    //return (uint16_t) ModBusGetRegister(channel,ModBus::ReadInputRegisters,address,ModBus::DataChannelLenght);
}


double ModBus::DataChannelRead (char channel)
{
    return ModBusGetRegister(channel,ModBus::ReadInputRegisters,ModBus::DataChannel1,ModBus::DataChannelLenght);
}
