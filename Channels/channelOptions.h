#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>
#include <QColor>
#include <QMutex>
#include <QVector>
#include "../mathresolver.h"
#include "../ustavka.h"
#include "transaction.h"

#define BASE_CHANNELS_OFFSET 32768

class ChannelOptions: public QObject
{
    Q_OBJECT
public:

    // constructor
    explicit ChannelOptions();
    ~ChannelOptions();
public:
    bool enable;        // флаг включения и отключения канала (для сохранения настроек)
    int getNum() { return numChannel; }
    void setNum(int i) { numChannel = i; }
    void setSlot(int sl) { slot = sl; }
    int getSlot() { return slot; }
    void setSlotChannel(int ch) { slotChannel = ch; }
    int getSlotChannel() { return slotChannel; }
    uint16_t GetSignalType();
    uint16_t GetCurSignalType();
    int GetDiapason();
    int GetRegistrationType();
    double GetLowerLimit();
    double GetHigherLimit();
    double GetLowerMeasureLimit();
    double GetHigherMeasureLimit();
    double GetMeasurePeriod();
    double GetState1Value();
    double GetState2Value();
    int GetDempherValue();

    double GetCurrentChannelValue();
    double GetMaximumChannelValue();
    double GetMinimumChannelValue();
    double GetAverageChannelValue();
    double GetMaxplusMinChannelValue();
    double GetDempheredChannelValue();
    double GetValuePercent();
    double ConvertSignalToValue(double signal);

    QString GetUnitsName();
    QString GetState1HighMessage();
    QString GetState1LowMessage();
    QString GetState2HighMessage();
    QString GetState2LowMessage();
    QString GetChannelName();
    QString GetMathString();
    QColor GetStateDependentColor();
    QColor GetNormalColor();
    QColor GetMaximumColor();
    QColor GetMinimumColor();
    bool GetConfirmationNeed();
    bool MaximumNow();
    bool MinimumNow();
    QVector<double> GetChannelValuesBuffer();
    QVector<double> GetChannelXBuffer();

    void SetConfirmationNeed(bool confirmationstate);
    void SetSignalType(uint16_t newsignaltype);
    void SetCurSignalType(uint16_t newsignaltype);
    void ReadSingleChannelOptionFromFile(int channel);
    void SetChannelName(QString newname);
    void SetLowerLimit(double newsignaltype);
    void SetHigherLimit(double newhigherlimit);
    void SetLowerMeasureLimit(double newlowermeaslimit);
    void SetHigherMeasureLimit(double newhighermeaslimit);
    void SetMeasurePeriod(double newmeasperiod);
    void SetUnitsName(QString newunit);
    void SetState1Value(double newvalue);
    void SetState2Value(double newvalue);
    void SetState1HighMessage(QString newmessage);
    void SetState1LowMessage(QString newmessage);
    void SetState2HighMessage(QString newmessage);
    void SetState2LowMessage(QString newmessage);
    void SetMathEquation(QString newmathstring);
    void SetMathematical(bool newstate);
    void SetCurrentChannelValue(double value);
    void SetDempher(int newdempher);
    void SetDiapason(int newdiapason);
    void SetRegistrationType(int newdregistrationtype);
    void SetNormalColor(QColor newcolor);
    void SetMaximumColor(QColor newmaxcolor);
    void SetMinimumColor(QColor newmincolor);
    int getShema();
    void setShema(int sh);


    bool IsChannelMathematical();

    void parserChannel(Transaction tr);

signals:
    void updateSignal(int index);
    void sendToWorker(Transaction tr);

public slots:

private slots:
    void timerSlot();
    void updateParam();


    // приватные переменные настроек канала
private:

    //структура данных и параметров канала в плате 4AI
    typedef struct
    {
        // Parameter: Data (Measure/Generate)  : chanData : NV Address: 28
        float chanData;
        // Parameter: Data flags : chanDataFlags : NV Address: 28
        uint16_t chanDataFlags;
        // Parameter: Status : chanStatus : NV Address: 28
        uint16_t chanStatus;
        // Parameter: ќшибки канала.  : chanError : NV Address: 28
        uint16_t chanError;
        // Parameter: Quantity  : chanQuantity : NV Address: 28
        uint32_t chanQuantity;
        // Parameter: Uptime  : chanUptime : NV Address: 28
        uint32_t chanUptime;
        // Parameter: Raw Data : chanRawData : NV Address: 28
        float chanRawData;
        // Parameter: Signal Type : chanSignalType : NV Address: 28
        uint16_t chanSignalType;
        // Parameter: Additional parameter1 : chanAdditionalParameter1 : NV Address: 30
        uint8_t chanAdditionalParameter1[12];
        // Parameter: Additional parameter2 : chanAdditionalParameter2 : NV Address: 42
        uint8_t chanAdditionalParameter2[12];
    } tChanParams;

    tChanParams inputData;  //полученные значения
    tChanParams outputData; //значения для записи

    bool MathematicalState;
    uint16_t signaltype;
    uint16_t cursignaltype;
    double lowerlimit;
    double higherlimit;
    double lowermeasurelimit;
    double highermeasurelimit;
    double measureperiod;
    double state1value;
    double state2value;
    double currentvalue;
    int demphervalue;
    int numChannel;
    int slot;
    int slotChannel;

    QString unitsname;
    QString state1highmessage;
    QString state1lowmessage;
    QString state2highmessage;
    QString state2lowmessage;
    QString channelname;
    QString mathequationstring;

    QColor normalcolor; // цвет канала на графике норм
    QColor maximumcolor; // цвет канала на графике макс
    QColor minimumcolor; // цвет канала на графике мин
    bool needConfirmationchannel;
    int diapason;           //диапазон - тип датчика
    int shema;              //схема подключения датчика
    int registrationtype;

    QVector<double> channelvaluesbuffer;
    QVector<double> dempheredvaluesbuffer;
    QVector<double> channelxbuffer;
    QVector<double> channelpercentbuffer;

    QMutex * buffermutex;
    QTimer * timer;
    QTimer * timerUpdateParam;


public:

//    enum SignalType {
//        NoMeasure= 0,
//        CurrentMeasure= 1,
//        VoltageMeasure= 2,
////        ResistanceMeasure= 3,
//        TermoCoupleMeasure= 3,
//        TermoResistanceMeasure= 4,
//        DiscretMeasure= 6,
//        ImpulseCounterMeasure= 7,
//        FrequencyMeasure= 8
//    };
//    Q_ENUM(SignalType)

//    enum SupportedSignals {
//        SupportedSignalError= 0,
//        SupportedSignalCurrent = 1,
//        SupportedSignalVoltage = 2,
//        SupportedSignalResistance = 4,
//        SupportedSignalTermoCouple = 8,
//        SupportedSignalTermoResistance = 16,
//        SupportedSignalDiscret = 32,
//        SupportedSignalImpulseCounter = 64,
//        SupportedSignalFrequency = 128
//    };
//    Q_ENUM(SupportedSignals)

//    enum Status {
//        StatusOff = 0,
//        StatusOn = 1,
//        StatusConfigWait = 2,
//        StatusConfigWorking = 3,
//        StatusConfigCompleted = 4,
//        StatusWaitForUpdateData = 5,
//        StatusDataWorking = 6,
//        StatusDataCompleted = 7
//    };
//    Q_ENUM(Status)

//    enum ModBusErrors {
//        ConnectionError = -9998 ,
//        BadCRC = -9999 ,
//    };
//    Q_ENUM(ModBusErrors)

//    enum UartErrors {
//        NoResponse = 0x01 ,
//        CrcError = 0x02
//    };
//    Q_ENUM(UartErrors)

//    enum DeviceAdress {
//        MainDeviceAddress = 0x01 ,
//        Board4AIAddress = 0x01 ,
//        UniversalChannel1 = 0x01 ,
//        UniversalChannel2 = 0x02 ,
//        UniversalChannel3 = 0x04 ,
//        UniversalChannel4 = 0x08
//    };
//    Q_ENUM(DeviceAdress)

//    enum DeviceType{
//        DeviceType4AI = 1,
//        DeviceType8RP = 5
//    };
//    Q_ENUM(DeviceType)

//    enum RegisterAdress {
//        TemperetureAdress     = 0x0000,
//        VoltageAdressHi       = 0x02,
//        CurrentAdressHi       = 0x04,
//        ResistanceAdressHi    = 0x08,
//        FrequencyAdressHi     = 0x10,
//        TemperetureAdressLo   = 0x00, // возможно, не нужно
//        VoltageAdressLo       = 0x00,
//        CurrentAdressLo       = 0x00,
//        ResistanceAdressLo    = 0x00,
//        FrequencyAdressLo     = 0x00,
//        ElmetroRelayAddress = 0x01,
//        ElmetroChannelAB1Address = 0x00,
//        ElmetroChannelAB2Address = 0x02,
//        ElmetroChannelAB3Address = 0x04,
//        ElmetroChannelAB4Address = 0x06
//    };
//    Q_ENUM(RegisterAdress)

//    enum RegisterCount{
//        TemperetureRegLenght = 0x000A,
//        VoltageRegCountHi = 0x01,
//        CurrentRegCountHi = 0x01,
//        ResistanceRegCountHi = 0x01,
//        FrequencyRegCountHi = 0x01,
//        TemperetureRegCountLo = 0x00,
//        VoltageRegCountLo = 0x00,
//        CurrentRegCountLo = 0x00,
//        ResistanceRegCountLo = 0x00,
//        FrequencyRegCountLo = 0x00
//    };
//    Q_ENUM(RegisterCount)

//    enum Function {
//        ReadCoils = 0x01,
//        ReadDiscreteInputs = 0x02,
//        ReadHoldingRegisters = 0x03,
//        ReadInputRegisters = 0x04,
//        WriteSingleCoil = 0x05,
//        WriteSingleRegister = 0x06,
//        ReadExceptionStatus = 0x07,
//        DiagnosticRequest = 0x08,
//        GetCommEventCounter = 0x0B,
//        GetCommEventLog = 0x0C,
//        WriteMultipleRegisters = 0x10
//    };
//    Q_ENUM(Function)

    /* Параметры устройства для группы G00 (выходные данные канала):
     * Для модуля типа 4AI при доступе по команде 03 (0x03)
     * Read Holding Registers и 04 (0x04) Read Input Registers
     * (формат команд аналогичный).
     * */
//    enum G00_Group
//    {
//        G00Bias = 0,
//        DataChannel1 = G00Bias + 2,
//        DataChannel2 = G00Bias + 6,
//        DataChannel3 = G00Bias + 10,
//        DataChannel4 = G00Bias + 14,
//        DataChannelLenght = 2
//    };
//    Q_ENUM(G00_Group)

    /* Доступ к параметрам данного раздела осуществляется
     * с использованием команды 03 (0x03)
     * Read Holding Registers
     * */

//    enum MeasureTypeVoltage{
//        Voltage1VoltNoBreakControl = 1,
//        Voltage1VoltWithBreakControl = 2,
//        Voltage100mVoltNoBreakControl = 3,
//        Voltage100mVoltWithBreakControl = 4,
//        Voltage5VoltNoBreakControl = 5,
//        Voltage5VoltWithBreakControl = 6,
//        Voltage10VoltNoBreakControl = 7,
//        Voltage10VoltWithBreakControl = 8
//    };
//    Q_ENUM(MeasureTypeVoltage)


//    enum MeasureTypeWireMode{
//        Wire3NoBreakControl = 3,
//        Wire3WithBreakControl = 4,
//        Wire4NoBreakControl = 5,
//        Wire4WithBreakControl = 6
//    };
//    Q_ENUM(MeasureTypeWireMode)

//    enum TermoCoupleType{
//        R = 1,  //(ТПП 13)
//        S = 2,  //(ТПП 10)
//        B = 3,  //(ТПР)
//        J = 4,  //(ТЖК)
//        T = 5,  //(ТМКн)
//        E = 6,  //(ТХКн)
//        K = 7,  //(ТХА)
//        N = 8,  //(ТНН)
//        A1 = 9, //(ТВР)
//        A2 = 10,//(ТВР)
//        A3 = 11,//(ТВР)
//        L = 12, //(ТХК)
//        M = 14  //(ТМК)
//    };
//    Q_ENUM(TermoCoupleType)

//    enum TermoResistorType{
//        Pt1dot3910 = 1,
//        Pt1dot3850  = 2,
//        Cu1dot4280 = 3,
//        Cu1dot4260  = 4,
//        Ni1dot6170 = 5,
//        m5m3m = 6,
//        PP46P = 7
//    };
//    Q_ENUM(TermoResistorType)

    enum SignalTypeSelect{
        MeasureOff = 0,
        MeasureCurrent = 1,
        MeasureVoltage = 2,
        MeasureResistance = 3,
        MeasureTC = 4,
        MeasureTermoResistance = 5,
        MeasureDiscrette = 6,
        MeasureImpulseCount = 7,
        MeasureImpulseFrequency = 8
    };
    Q_ENUM(SignalTypeSelect)

    enum FilterTypeSelect{
        FilterOff = 0,
        FilterMediana = 1,
        FilterAverage = 2
    };
    Q_ENUM(FilterTypeSelect)

    enum TransmissionFunction{
        LinearTransmission = 0,
        QuadroTransmission = 1,
        SQRTTransmission = 2
    };
    Q_ENUM(TransmissionFunction)

    enum CurrentDiapason{
        Current4_20mA ,
        Current0_20mA ,
        Current0_5mA ,
        Current0_20mA_sqrt ,
        Current4_20mA_sqrt ,
        Current_20_20mA
    };
    Q_ENUM(CurrentDiapason)


    enum VoltageDiapason{
        Voltage0_100mV ,
        Voltage0_10V ,
        Voltage0_5V ,
        Voltage1_5V ,
        Voltage150_150mV ,
        Voltage1_1V ,
        Voltage10_10V ,
        Voltage30_30V ,
        Voltage0_1V_sqrt ,
        Voltage0_10V_sqrt ,
        Voltage1_5V_sqrt
    };
    Q_ENUM(VoltageDiapason)

    enum ParamOffset{
        chanData = 0,
        chanDataFlags = 2,
        chanStatus = 3,
        chanError = 4,
        chanQuantity = 5,
        chanUptime = 7,
        chanRawData = 9,
        chanRawDataFlags = 11,
        chanSupportedSignals = 12,
        chanSignalType = 13,

        chanTransferSignalLowLim = 31,
        chanTransferSignalHighLim = 33,
        chanTransferScaleLowLim = 35,
        chanTransferScaleHighLim = 37,
        chanBadGoodComm = 39,
        chanReleyHi = 42,
        chanReleyLow = 43,

        chanCjValue = 90,
        chanResultCjValue = 92,

    };
    Q_ENUM(ParamOffset)

};
#endif // CHANNEL1_H
