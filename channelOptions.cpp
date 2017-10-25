#include "channelOptions.h"
#include "uartdriver.h"
#include "metrologicalcalc.h"
#include "QDebug"

extern QVector<double> X_Coordinates;

int cnt;

double ChannelOptions::GetHigherLimit()
{
    return this->higherlimit;
}

double ChannelOptions::GetHigherMeasureLimit()
{
    return this->highermeasurelimit;
}

double ChannelOptions::GetLowerLimit()
{
    return this->lowerlimit;
}

double ChannelOptions::GetLowerMeasureLimit()
{
    return this->lowermeasurelimit;
}

double ChannelOptions::GetMeasurePeriod()
{
    return this->measureperiod;
}

uint16_t ChannelOptions::GetSignalType()
{
    return signaltype;
}

int ChannelOptions::GetDiapason()
{
    return diapason;
}

int ChannelOptions::GetRegistrationType()
{
    return registrationtype;
}

void ChannelOptions::SetSignalType(uint16_t newsignaltype)
{
    this->signaltype = newsignaltype;
}

void ChannelOptions::SetDiapason(int newdiapason)
{
    this->diapason = newdiapason;
}

void ChannelOptions::SetRegistrationType(int newdregistrationtype)
{
    this->registrationtype = newdregistrationtype;
}

void ChannelOptions::SetUnitsName(QString unitsname)
{
    this->unitsname = unitsname;
}

void ChannelOptions::SetMeasurePeriod(double np)
{
    this->measureperiod = np;
}

QString ChannelOptions::GetUnitsName()
{
    return this->unitsname;
}

void ChannelOptions::SetHigherLimit(double newlimit)
{
    this->higherlimit=newlimit;
}

void ChannelOptions::SetLowerLimit(double newlimit)
{
    this->lowerlimit=newlimit;
}

void ChannelOptions::SetHigherMeasureLimit(double newlimit)
{
    this->highermeasurelimit=newlimit;
}

void ChannelOptions::SetLowerMeasureLimit(double newlimit)
{
    this->lowermeasurelimit=newlimit;
}

QString ChannelOptions::GetState1HighMessage()
{
    return this->state1highmessage;
}

QString ChannelOptions::GetState1LowMessage()
{
    return this->state1lowmessage;
}

QString ChannelOptions::GetState2HighMessage()
{
    return this->state2highmessage;
}

QString ChannelOptions::GetState2LowMessage()
{
    return this->state2lowmessage;
}

QString ChannelOptions::GetChannelName()
{
    return this->channelname;
}

QString ChannelOptions::GetMathString()
{
    return this->mathequationstring;
}

QColor ChannelOptions::GetStateDependentColor()
{
    QColor color;

    if (this->GetCurrentChannelValue() > this->GetState1Value())
        color = this->GetMaximumColor();
    else if (this->GetCurrentChannelValue() < this->GetState2Value())
        color = this->GetMinimumColor();
    else
        color = this->GetNormalColor();

    return color;
}

QColor ChannelOptions::GetNormalColor()
{
    return normalcolor;
}

QColor ChannelOptions::GetMaximumColor()
{
    return maximumcolor;
}

QColor ChannelOptions::GetMinimumColor()
{
    return minimumcolor;
}

void ChannelOptions::SetMathEquation(QString newmathstring)
{
    this->mathequationstring = newmathstring;
}

void ChannelOptions::SetChannelName(QString newname)
{
    this->channelname = newname;
}

void ChannelOptions::SetState1HighMessage(QString newmessage)
{
    this->state1highmessage = newmessage;
}

void ChannelOptions::SetState1LowMessage(QString newmessage)
{
    this->state1lowmessage = newmessage;
}

void ChannelOptions::SetState2HighMessage(QString newmessage)
{
    this->state2highmessage = newmessage;
}

void ChannelOptions::SetState2LowMessage(QString newmessage)
{
    this->state2lowmessage = newmessage;
}

void ChannelOptions::SetState1Value(double value)
{
    this->state1value = value;
}

void ChannelOptions::SetState2Value(double value)
{
    this->state2value = value;
}

void ChannelOptions::SetMathematical(bool newstate)
{
    this->MathematicalState = newstate;
}

double ChannelOptions::GetState1Value()
{
    return this->state1value;
}

double ChannelOptions::GetState2Value()
{
    return this->state2value;
}

double ChannelOptions::GetDempherValue()
{
    return demphervalue;
}

bool ChannelOptions::IsHighState1Setted()
{
    return this->HighState1Setted;
}

bool ChannelOptions::IsLowState1Setted()
{
    return this->LowState1Setted;
}

bool ChannelOptions::IsHighState2Setted()
{
    return this->HighState2Setted;
}

bool ChannelOptions::IsLowState2Setted()
{
    return this->LowState2Setted;
}

bool ChannelOptions::IsChannelMathematical()
{
    return this->MathematicalState;
}

// constructor
ChannelOptions::ChannelOptions()
{
    this->HighState1Setted = false;
    this->LowState1Setted= false;
    this->HighState2Setted= false;
    this->LowState2Setted= false;
    SetConfirmationNeed(true);
}

bool ChannelOptions::GetConfirmationNeed()
{
    return this->needConfirmationchannel;
}

bool ChannelOptions::MaximumNow()
{
    bool result = (this->GetCurrentChannelValue() > this->GetState1Value() ? true : false);
    return result;
}

bool ChannelOptions::MinimumNow()
{
    bool result = (this->GetCurrentChannelValue() < this->GetState2Value() ? true : false);
    return result;
}

QVector<double> ChannelOptions::GetChannelValuesBuffer()
{
    QVector<double> buf;

    if (GetDempherValue()<=1)
        buf = channelvaluesbuffer;
    else
        buf = dempheredvaluesbuffer;

    return buf;
}

QVector<double> ChannelOptions::GetChannelXBuffer()
{
    return channelxbuffer;
}

void ChannelOptions::SetConfirmationNeed(bool confirmationstate)
{
    needConfirmationchannel = confirmationstate;
}

double ChannelOptions::GetCurrentChannelValue()
{
    int regtype = GetRegistrationType();

    //    qDebug() << GetChannelName()<< " " << GetRegistrationType() ;
    switch (regtype) {
    case 0: // мгновенное значение
        currentvalue;
        if (GetDempherValue()>1)
            currentvalue = GetDempheredChannelValue();
        break;
    case 1: // среднее значение
    {
        currentvalue = GetAverageChannelValue();
        break;
    }
    case 2: // минимум значение
    {
        currentvalue = GetMinimumChannelValue();
        break;
    }
    case 3: // максимум значение
    {
        currentvalue = GetMaximumChannelValue();
        break;
    }
    case 4: // минимум плюс максимум значение
    {
        currentvalue = GetMaxplusMinChannelValue();
        break;
    }
    default:
    {
        currentvalue;
    }
        break;
    }

    return currentvalue;
}

double ChannelOptions::GetMaximumChannelValue()
{
    return mathresolver::dGetMaximumValue(channelvaluesbuffer);
}

double ChannelOptions::GetMinimumChannelValue()
{
    return mathresolver::dGetMinimumValue(channelvaluesbuffer);
}

double ChannelOptions::GetAverageChannelValue()
{
    return mathresolver::dGetAverageValue(channelvaluesbuffer);
}

double ChannelOptions::GetMaxplusMinChannelValue()
{
    return mathresolver::dGetMaximumValue(channelvaluesbuffer) + mathresolver::dGetMinimumValue(channelvaluesbuffer);
}

double ChannelOptions::GetDempheredChannelValue()
{
    double res;
    if (GetDempherValue()>1)
        res = mathresolver::dGetDempheredValue(channelvaluesbuffer,GetDempherValue());
    else
        res = currentvalue;
    return res;
}

double ChannelOptions::GetValuePercent()
{
    float razmah = GetHigherMeasureLimit() - GetLowerMeasureLimit();
    float x = 100 * (GetCurrentChannelValue() - GetLowerMeasureLimit())/razmah;
    return x;
}

double ChannelOptions::ConvertSignalToValue(double signal)
{
    double value = signal;

    int lowlimit = GetLowerLimit();
    int hilimit = GetHigherLimit();
    double hisignal = 0, lowsignal = 0 ;


    if (GetSignalType() == MeasureCurrent)
    {

        switch (GetDiapason()) {
        case Current4_20mA:
        {
            hisignal = 20;
            lowsignal = 4;
            break;
        }
        case Current0_20mA:
        {
            hisignal = 20;
            lowsignal = 0;
            break;
        }
        case Current0_5mA:
        {
            hisignal = 5;
            lowsignal = 0;
            break;
        }

        case Current0_20mA_sqrt:
        {
            hisignal = 20;
            lowsignal = 0;
            break;
        }

        case Current4_20mA_sqrt:
        {
            hisignal = 20;
            lowsignal = 4;
            break;
        }

        case Current_20_20mA:
        {
            hisignal = 20;
            lowsignal = -20;
            break;
        }

        default:
            break;
        }

        value = MetrologicalCalc::ConvertSignalToValue(signal,lowsignal,hisignal,lowlimit,hilimit); // берем начало и конец под-диапазона
    }

    if (GetSignalType() == MeasureVoltage)
    {
        switch (GetDiapason()) {
        case Voltage0_1V:
        {
            hisignal = 1;
            lowsignal = 0;
            break;
        }

        case Voltage0_10V:
        {
            hisignal = 10;
            lowsignal = 0;
            break;
        }

        case Voltage0_5V:
        {
            hisignal = 5;
            lowsignal = 0;
            break;
        }

        case Voltage1_5V:
        {
            hisignal = 5;
            lowsignal = 1;
            break;
        }

        case Voltage150_150mV:
        {
            hisignal = 0.15;
            lowsignal = -0.15;
            break;
        }


        case Voltage1_1V:
        {
            hisignal = 1;
            lowsignal = -1;
            break;
        }

        case Voltage10_10V:
        {
            hisignal = 10;
            lowsignal = -10;
            break;
        }


        case Voltage30_30V:
        {
            hisignal = 30;
            lowsignal = -30;
            break;
        }

        case Voltage0_1V_sqrt:
        {
            hisignal = 1;
            lowsignal = 0;
            break;
        }

        case Voltage0_10V_sqrt:
        {
            hisignal = 10;
            lowsignal = 0;
            break;
        }

        case Voltage1_5V_sqrt:
        {
            hisignal = 5;
            lowsignal = 1;
            break;
        }
        default:
            break;
        }

        value = MetrologicalCalc::ConvertSignalToValue(signal,lowsignal,hisignal,lowlimit,hilimit); // берем начало и конец под-диапазона
    }

    return value;
}

void ChannelOptions::SetCurrentChannelValue(double value)
{
    currentvalue = ConvertSignalToValue(value);
    while (channelxbuffer.length()>300)
        channelxbuffer.removeFirst();
    while (channelvaluesbuffer.length()>300)
        channelvaluesbuffer.removeFirst();

    while (dempheredvaluesbuffer.length()>300)
        dempheredvaluesbuffer.removeFirst();

    qDebug() << channelxbuffer.length() <<  'x'; // << "channelxbuffer.length" ;
    qDebug() << channelvaluesbuffer.length() <<  'v'; //<< "channelvaluesbuffer.length" ;
    qDebug() << dempheredvaluesbuffer.length() <<  'd'; //<< "channelvaluesbuffer.length" ;

    channelvaluesbuffer.append(currentvalue);
    dempheredvaluesbuffer.append(GetDempheredChannelValue());
    channelxbuffer.append(X_Coordinates.last()); // добавляем последнюю координату

    qDebug() << ++cnt <<  "cnt"; //<< "channelvaluesbuffer.length" ;
}

void ChannelOptions::SetDempher(double newdempher)
{
    demphervalue = newdempher;
}

void ChannelOptions::SetNormalColor(QColor newcolor)
{
    normalcolor = newcolor;
}

void ChannelOptions::SetMaximumColor(QColor newmaxcolor)
{
    maximumcolor = newmaxcolor;
}

void ChannelOptions::SetMinimumColor(QColor newmincolor)
{
    minimumcolor = newmincolor;
}
