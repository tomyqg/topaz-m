#include "channel1.h"
#include "QDebug"


int tata ;

extern QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;

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

void ChannelOptions::SetConfirmationNeed(bool confirmationstate)
{
    needConfirmationchannel = confirmationstate;
}

double ChannelOptions::GetCurrentChannelValue()
{

    //    int regtype = GetRegistrationType();
    //    regtype = 1;

    //    switch (regtype) {
    //    case 0: // мгновенное значение
    //        currentvalue;
    //        break;
    //    case 1: // среднее значение
    //    {
    //        currentvalue = MR.dGetAverageValue(channelbuffer);
    //        qDebug() << channelname << currentvalue ;
    //        qDebug() << channelbuffer;
    //    }
    //        break;
    //    case 2: // минимум значение
    //        break;
    //    case 3: // максимум значение
    //        break;
    //    case 4: // минимум плюс максимум значение
    //        break;
    //    default:
    //        currentvalue;
    //        break;
    //    }

    return currentvalue;
}

double ChannelOptions::GetValuePercent()
{
    float razmah = highermeasurelimit - lowermeasurelimit;
    float x = 100 * (currentvalue - lowermeasurelimit)/razmah;

    return x;
}

void ChannelOptions::SetCurrentChannelValue(double value)
{
    currentvalue = value;
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
