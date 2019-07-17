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
    enum {

    };

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
    int GetDempherValue();

    double GetCurrentChannelValue();
    double GetMaximumChannelValue();
    double GetMinimumChannelValue();
    double GetAverageChannelValue();
    double GetMaxplusMinChannelValue();
    double GetDempheredChannelValue();
    double GetValuePercent();
    double ConvertSignalToValue(double signal);
    double ConvertVisualValue(double signal);

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
    QVector<double> GetChannelTimeBuffer();

    void SetConfirmationNeed(bool confirmationstate);
    void SetSignalType(uint16_t newsignaltype);
    void SetCurSignalType(uint16_t newsignaltype);
//    void ReadSingleChannelOptionFromFile(int channel);
    void SetChannelName(QString newname);
    void SetLowerLimit(double newsignaltype);
    void SetHigherLimit(double newhigherlimit);
    void SetLowerMeasureLimit(double newlowermeaslimit);
    void SetHigherMeasureLimit(double newhighermeaslimit);
    void SetMeasurePeriod(double newmeasperiod);
    void SetUnitsName(QString newunit);
    void SetState1HighMessage(QString newmessage);
    void SetState1LowMessage(QString newmessage);
    void SetState2HighMessage(QString newmessage);
    void SetState2LowMessage(QString newmessage);
    void SetMathEquation(QString newmathstring);
    void SetMathematical(bool newstate);
    void SetCurrentChannelValue(double value);
    void SetDempher(int newdempher);
//    void SetDiapason(int newdiapason);
    void SetRegistrationType(int newdregistrationtype);
    void SetNormalColor(QColor newcolor);
    void SetMaximumColor(QColor newmaxcolor);
    void SetMinimumColor(QColor newmincolor);
    int getShema();
//    void setShema(int sh);
    bool IsChannelMathematical();
    void parserChannel(Transaction tr);
    void copyOptions(ChannelOptions *ch);
    void SetDiapasonShema(int newdiapason, int sh);
    void enableColdJunction(int en);
    int getStateColdJunction(void);
    void setShiftColdJunction(double shift);
    double getShiftColdJunction(void);
    void setVolueVoltageType(int type) { valueVoltage = type; }
    int getVoltageType(void) { return valueVoltage; }
    void setCapacity(int cap) { capacity = cap; }
    int getCapacity(void) { return capacity; }
    bool isNewValue();
    int getChannelStatus() { return inputData.chanStatus; }

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
        // Смещение компенсации холодного спая
        float chanFSRinternal;  //Vag: врменная ячейка для записи смещения х.с.
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
    double currentvalue;
    int demphervalue;
    int numChannel;
    int slot;
    int slotChannel;
    int valueVoltage;
    int capacity;
    bool newValue;      // признак наличия свежих данных

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
    bool enColdJunction;    //Включение компенсации холодного спая
    double shiftColdJunction;    //Величина компенсации холодного спая

    QVector<double> channelvaluesbuffer;
    QVector<double> dempheredvaluesbuffer;
    QVector<double> channelxbuffer;
    QVector<double> channelpercentbuffer;
    QVector<double> channeltimebuffer;

    QMutex * buffermutex;
    QTimer * timer;
    QTimer * timerUpdateParam;




public:

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

    enum ValueType {
        Value_Real = 0,
        Value_Procent = 1
    };
    Q_ENUM(ValueType)


//    QString getNameUnitByParam(void);
    QString getNameUnitByParam(uint16_t type, int diap);
    int optimalPrecision();

    void setAdditionalParametr1(uint8_t *param);
    void setAdditionalParametr2(uint8_t *param);
    void getAdditionalParametr1(uint8_t *param);
    void getAdditionalParametr2(uint8_t *param);
};
#endif // CHANNEL1_H
