#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>

class ChannelOptions: public QObject
{
    Q_OBJECT

public:
    // constructor
    explicit ChannelOptions();

public:

    uint16_t GetSignalType();
    void SetSignalType(uint16_t newsignaltype);
    double GetLowerLimit();
    double GetHigherLimit();
    double GetLowerMeasureLimit();
    double GetHigherMeasureLimit();
    double GetMeasurePeriod();
    double GetState1Value();
    double GetState2Value();
    double GetCurrentChannelValue();

    QString GetUnitsName();
    QString GetState1HighMessage();
    QString GetState1LowMessage();
    QString GetState2HighMessage();
    QString GetState2LowMessage();
    QString GetChannelName();
    QString GetMathString();
    bool GetConfirmationNeed();
    void SetConfirmationNeed(bool confirmationstate);

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

    bool IsHighState1Setted();
    bool IsLowState1Setted();
    bool IsHighState2Setted();
    bool IsLowState2Setted();
    bool IsChannelMathematical();
    bool HighState1Setted ;
    bool LowState1Setted ;
    bool HighState2Setted;
    bool LowState2Setted ;
    bool MathematicalState;

    // приватные переменные настроек канала

private:

    uint16_t signaltype;
    double lowerlimit;
    double higherlimit;
    double lowermeasurelimit;
    double highermeasurelimit;
    double measureperiod;
    double state1value;
    double state2value;
    double currentvalue;

    QString unitsname;
    QString state1highmessage;
    QString state1lowmessage;
    QString state2highmessage;
    QString state2lowmessage;
    QString channelname;
    QString mathequationstring;

    bool needConfirmationchannel;

signals:
    void updateUI(const QString text);


};
#endif // CHANNEL1_H
