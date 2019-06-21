#ifndef CFREQCHANNEL_H
#define CFREQCHANNEL_H

#include <QObject>
#include <QtCore>

#include "transaction.h"

class cFreqChannel : public QObject
{
    Q_OBJECT
public:
    explicit cFreqChannel(QObject *parent = 0);

    bool enable;

    int getNum() { return num; }
    void setNum(int i) { num = i; }
    void setSlot(int sl) { slot = sl; }
    int getSlot() { return slot; }
    void setSlotChannel(int ch) { slotChannel = ch; }
    int getSlotChannel() { return slotChannel; }
    uint16_t GetSignalType() { return outputData.digitSignalType; }
    uint16_t GetCurSignalType() { return inputData.digitSignalType; }
    float GetImpulseDuration() { return outputData.impulseDuration; }
    float GetCurImpulseDuration() { return inputData.impulseDuration; }
    void setImpulseDuration( float duration);
    double GetDempheredChannelValue();
    void SetCurrentChannelValue(double value);
    void SetSignalType(uint16_t newsignaltype);
    double GetCurrentChannelValue();
    QString GetChannelName() { return name; }
    void setName(QString n) { name = n; }
    void parserChannel(Transaction tr);
    void SetHigherLimit(double newlimit) { this->higherlimit=newlimit; }
    void SetLowerLimit(double newlimit) { this->lowerlimit=newlimit; }
    void SetHigherMeasureLimit(double newlimit) { this->highermeasurelimit=newlimit; }
    void SetLowerMeasureLimit(double newlimit) { this->lowermeasurelimit=newlimit; }
    double GetHigherLimit() { return this->higherlimit; }
    double GetHigherMeasureLimit() { return this->highermeasurelimit; }
    double GetLowerLimit() { return this->lowerlimit; }
    double GetLowerMeasureLimit() { return this->lowermeasurelimit; }
    void setUnit(QString u) { unit = u; }
    QString getUnit() { return unit; }
    void SetMeasurePeriod(double mp);
    double getMeasurePeriod() { return measureperiod; }
    void setDempher (double d) { demphervalue = d; }
    double getDempher() { return demphervalue; }
    QVector<double> GetChannelValuesBuffer();
    QVector<double> GetChannelTimeBuffer();
    double GetValuePercent();
    float getImpulseWeight() { return weight; }
    void setImpulseWeight(float w) { weight = w; }
private:
    QString name;       //название канала
    QString unit;       //единица измерения
    float weight;       //вес импульса
    int num;            //порядковый номер
    int slot;           //слот канала
    int slotChannel;    //канал в слоте
    //структура данных и параметров канала в плате 6DI6RO
    typedef struct
    {
        // Parameter: Out freq chan : chan0MeasureFreq : NV Address: 1000
        float measureFreq;
        // Parameter: Out  imp chan : chan0Impulse : NV Address: 1000
        float impulse;
        // Parameter: Buffer impulse : chan0ImpulseBuff : NV Address: 1004
        float impulseBuff;
        // Parameter: SignalType(Digital) : chan0DigitSignalType : NV Address: 1004
        uint16_t digitSignalType;
        // Parameter: Additional parameter : chan0DigitalAdditionalParameter : NV Address: 1004
        uint8_t digitalAdditionalParameter[12];
        // Parameter: TimeDamping : chan0DigitDamping : NV Address: 1004
        float impulseDuration;
    } tChanParams;
    tChanParams inputData;  //полученные значения
    tChanParams outputData; //значения для записи
    QVector<double> channelvaluesbuffer;
    QVector<double> dempheredvaluesbuffer;
    QVector<double> channelxbuffer;
    QVector<double> channelpercentbuffer;
    QVector<double> channeltimebuffer;
    double lowerlimit;
    double higherlimit;
    double lowermeasurelimit;
    double highermeasurelimit;
    double measureperiod;
    double currentvalue;
    double demphervalue;
    QMutex * buffermutex;
    QTimer * timer;
    QTimer * timerUpdateParam;
    QStringList listStr;

signals:
    void sendToWorker(Transaction tr);

public slots:
    void slotResetImpulsBuffer();

private slots:
    void timerSlot();
    void updateParam();
};

#endif // CFREQCHANNEL_H
