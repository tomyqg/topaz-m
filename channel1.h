#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <QObject>
#include <QColor>
#include <QVector>
#include "mathresolver.h"

class ChannelOptions: public QObject
{
    Q_OBJECT

public:

    // constructor

    explicit ChannelOptions();

public:

    uint16_t GetSignalType();
    int GetDiapason();
    int GetRegistrationType();
    double GetLowerLimit();
    double GetHigherLimit();
    double GetLowerMeasureLimit();
    double GetHigherMeasureLimit();
    double GetMeasurePeriod();
    double GetState1Value();
    double GetState2Value();
    double GetDempherValue();

    double GetCurrentChannelValue();
    double GetMaximumChannelValue();
    double GetMinimumChannelValue();
    double GetAverageChannelValue();
    double GetMaxplusMinChannelValue();
    double GetDempheredChannelValue();
    double GetValuePercent();

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

    void SetConfirmationNeed(bool confirmationstate);
    void SetSignalType(uint16_t newsignaltype);
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
    void SetDempher(double newdempher);
    void SetDiapason(int newdiapason);
    void SetRegistrationType(int newdregistrationtype);
    void SetNormalColor(QColor newcolor);
    void SetMaximumColor(QColor newmaxcolor);
    void SetMinimumColor(QColor newmincolor);


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

    int xposition;
    int yposition;
    int w;
    int h;

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
    double demphervalue;

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
    int diapason;
    int registrationtype;

    mathresolver MR;

    QVector<double> channelbuffer;
    QVector<double> channelpercentbuffer;

signals:
    void updateUI(const QString text);
};
#endif // CHANNEL1_H
