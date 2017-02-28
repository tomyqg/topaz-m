#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>

class ChannelOptions
{

public:
    explicit ChannelOptions();

public:

    QString GetUnitsName();

    double GetSignalType();
    double GetLowerLimit();
    double GetHigherLimit();
    double GetLowerMeasureLimit();
    double GetHigherMeasureLimit();
    double GetMeasurePeriod();
    double GetState1Value();
    double GetState2Value();

    QString GetState1HighMessage();
    QString GetState1LowMessage();
    QString GetState2HighMessage();
    QString GetState2LowMessage();
    QString GetChannelName();

    void SetChannelName(QString newname);
    void SetSignalType(double newsignaltype);
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

    bool IsHighState1Setted();
    bool IsLowState1Setted();
    bool IsHighState2Setted();
    bool IsLowState2Setted();

    void readoptionsfromfile(int channel);


    bool HighState1Setted ;
    bool LowState1Setted ;
    bool HighState2Setted;
    bool LowState2Setted ;


    // приватные переменные настроек канала

private:

    double signaltype;
    double lowerlimit;
    double higherlimit;
    double lowermeasurelimit;
    double highermeasurelimit;
    double measureperiod;
    double state1value;
    double state2value;

    QString unitsname;
    QString state1highmessage;
    QString state1lowmessage;
    QString state2highmessage;
    QString state2lowmessage;
    QString channelname;
};

#endif // CHANNEL1_H
