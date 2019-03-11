#include "channelOptions.h"
#include "../uartdriver.h"
#include "../metrologicalcalc.h"
#include <QDebug>
#include <stdlib.h>

extern QVector<double> X_Coordinates;

double cnt;

// constructor
ChannelOptions::ChannelOptions()
{
    SetConfirmationNeed(true);
    enable = false;
    currentvalue = 0;
    measureperiod = 1;
    diapason = 1;
    slot = 0;       //по-умолчанию присваиваем несуществующий слот
    registrationtype = 0;
    channelname = "Ch";
    MathematicalState = false;
    lowermeasurelimit = -100;
    highermeasurelimit = 100;
    signaltype = 2;
    outputData.chanSignalType = VoltageMeasure;
    memset(outputData.chanAdditionalParameter1, 0, sizeof(outputData.chanAdditionalParameter1));
    outputData.chanAdditionalParameter1[0] = diapason;
    unitsname = "mV";
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->setInterval(measureperiod);
    timer->stop();
    buffermutex = new QMutex();
    timerUpdateParam = new QTimer();
    connect(timerUpdateParam, SIGNAL(timeout()), this, SLOT(updateParam()));
    updateParam();
    timerUpdateParam->start(5000);

}

ChannelOptions::~ChannelOptions()
{
    delete buffermutex;
}

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
    return outputData.chanSignalType;
//    return signaltype;
}

uint16_t ChannelOptions::GetCurSignalType()
{
    return inputData.chanSignalType;
//    return cursignaltype;
}

int ChannelOptions::GetDiapason()
{
    if(outputData.chanSignalType == VoltageMeasure)
    {
        return outputData.chanAdditionalParameter1[0];
    }
    else if(outputData.chanSignalType == TermoResistanceMeasure)
    {
        return outputData.chanAdditionalParameter1[2];
    }
    else if(outputData.chanSignalType == TermoCoupleMeasure)
    {
        return outputData.chanAdditionalParameter1[0];
    }
//    return diapason;
}

int ChannelOptions::GetRegistrationType()
{
    return registrationtype;
}

void ChannelOptions::SetSignalType(uint16_t newsignaltype)
{
    this->signaltype = newsignaltype;
    outputData.chanSignalType = newsignaltype;
}

void ChannelOptions::SetCurSignalType(uint16_t newsignaltype)
{
    this->cursignaltype = newsignaltype;
}

void ChannelOptions::SetDiapason(int newdiapason)
{
    this->diapason = newdiapason;
    if(outputData.chanSignalType == VoltageMeasure)
    {
        outputData.chanAdditionalParameter1[0] = newdiapason;
    }
    else if(outputData.chanSignalType == TermoResistanceMeasure)
    {
        outputData.chanAdditionalParameter1[2] = newdiapason;
    }
    else if(outputData.chanSignalType == TermoCoupleMeasure)
    {
        outputData.chanAdditionalParameter1[0] = newdiapason;
    }
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
    if(measureperiod != 0)
    {
        timer->start(measureperiod);
    }
    else
    {
        timer->stop();
    }
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

//    if (this->GetCurrentChannelValue() > this->ustavka1.getStateValue())
//        color = this->GetMaximumColor();
//    else if (this->GetCurrentChannelValue() < this->ustavka2.getStateValue())
//        color = this->GetMinimumColor();
//    else
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

//void ChannelOptions::SetChannelCoords(int width, int xpos, int ypos, int height)
//{
//    this->xposition = xpos;
//    this->yposition = ypos;
//    this->w = width;
//    this->h = height;
//}

double ChannelOptions::GetState1Value()
{
    return this->state1value;
}

double ChannelOptions::GetState2Value()
{
    return this->state2value;
}

int ChannelOptions::GetDempherValue()
{
    return demphervalue;
}


bool ChannelOptions::IsChannelMathematical()
{
    return this->MathematicalState;
}

void ChannelOptions::parserChannel(Transaction tr)
{
    Transaction trans = tr;
    trans.dir = Transaction::W;
    QString paramName = cRegistersMap::getNameByOffset(tr.offset);
    QString chanName = "chan" + QString::number(slotChannel);
    if(paramName == chanName + "Data")
    {
        SetCurrentChannelValue((double)tr.volFlo);
        inputData.chanData = tr.volFlo;
    }
    if(paramName == chanName + "DataFlags")
    {
        inputData.chanDataFlags = (uint16_t)tr.volInt;
    }
    if(paramName == chanName + "Status")
    {
        inputData.chanStatus = (uint16_t)tr.volInt;
    }
    if(paramName == chanName + "Error")
    {
        inputData.chanError = (uint16_t)tr.volInt;
    }
    if(paramName == chanName + "Quantity")
    {
        inputData.chanQuantity = (uint32_t)tr.volInt;
    }
    if(paramName == chanName + "Uptime")
    {
        inputData.chanUptime = (uint32_t)tr.volInt;
    }
    if(paramName == chanName + "RawData")
    {
        inputData.chanRawData = (float)tr.volFlo;
    }
    else if(paramName == chanName + "SignalType")
    {
        inputData.chanSignalType = (uint16_t)tr.volInt;
        if(inputData.chanSignalType != outputData.chanSignalType)
        {
            trans.volInt = (uint32_t)outputData.chanSignalType;
            emit sendToWorker(trans);
        }
    }
    else if(paramName == chanName + "AdditionalParameter1")
    {
        memcpy(inputData.chanAdditionalParameter1, tr.paramA12, sizeof(tr.paramA12));
        if(memcmp(inputData.chanAdditionalParameter1, outputData.chanAdditionalParameter1, sizeof(inputData.chanAdditionalParameter1)) != 0)
        {
            memcpy(trans.paramA12, outputData.chanAdditionalParameter1, sizeof(outputData.chanAdditionalParameter1));
            emit sendToWorker(trans);
        }
    }
    else if(paramName == chanName + "AdditionalParameter2")
    {
        memcpy(inputData.chanAdditionalParameter2, tr.paramA12, sizeof(tr.paramA12));
        if(memcmp(inputData.chanAdditionalParameter2, outputData.chanAdditionalParameter2, sizeof(inputData.chanAdditionalParameter2)) != 0)
        {
            memcpy(trans.paramA12, outputData.chanAdditionalParameter2, sizeof(outputData.chanAdditionalParameter2));
            emit sendToWorker(trans);
        }
    }
}

void ChannelOptions::timerSlot()
{

    if(enable && (outputData.chanSignalType != NoMeasure))
    {
        QString nameParam = "chan" + QString::number(slotChannel) + "Data";
        uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
        Transaction tr(Transaction::R, (uint8_t)slot, offset, 0);
        emit sendToWorker(tr);
    }
    else if(!enable)
    {
#ifdef RANDOM_CHAN
        if(numChannel <= NUM_CHAN_IN_4AI)
        {
            SetCurrentChannelValue(currentvalue + ((double)((rand()%101) - 50) / 100));
        }
#endif
    }

    timer->setInterval((int)(measureperiod*1000));
}

void ChannelOptions::updateParam()
{
    if (!enable) return;
    Transaction tr;
    tr.dir = Transaction::R;
    tr.slave = slot;
    int devCh = slotChannel;
    QStringList listStr;
    listStr << "chan" + QString::number(devCh) + "SignalType" \
            << "chan" + QString::number(devCh) + "AdditionalParameter1";
    foreach (QString str, listStr) {
        tr.offset = cRegistersMap::getOffsetByName(str);
        emit sendToWorker(tr);
    }

}

bool ChannelOptions::GetConfirmationNeed()
{
    return this->needConfirmationchannel;
}

bool ChannelOptions::MaximumNow()
{
//    bool result = (this->GetCurrentChannelValue() > this->ustavka1.getStateValue() ? true : false);
//    return result;
    return 0;
}

bool ChannelOptions::MinimumNow()
{
//    bool result = (this->GetCurrentChannelValue() < this->ustavka2.getStateValue() ? true : false);
//    return result;
    return 0;
}

QVector<double> ChannelOptions::GetChannelValuesBuffer()
{
    QVector<double> buf;

    buffermutex->lock();

    if (GetDempherValue()<=1)
        buf = channelvaluesbuffer;
    else
        buf = dempheredvaluesbuffer;

    buffermutex->unlock();

    return buf;
}

QVector<double> ChannelOptions::GetChannelXBuffer()
{
    QVector<double> buf;
    buffermutex->lock();
    buf = channelxbuffer;
    buffermutex->unlock();
    return buf;
}

void ChannelOptions::SetConfirmationNeed(bool confirmationstate)
{
    needConfirmationchannel = confirmationstate;
}



double ChannelOptions::GetCurrentChannelValue()
{
#ifndef RANDOM_CHAN
    if(!enable) return NAN;
#endif

    int regtype = GetRegistrationType();

//        qDebug() << GetChannelName()<< " " << GetRegistrationType() ;
    switch (regtype) {
    case 0: // мгновенное значение
//        currentvalue;
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
//        currentvalue;
        break;
    }

    }

    return currentvalue;
}

double ChannelOptions::GetMaximumChannelValue()
{
    if(channelvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetMaximumValue(channelvaluesbuffer);
}

double ChannelOptions::GetMinimumChannelValue()
{
    if(channelvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetMinimumValue(channelvaluesbuffer);
}

double ChannelOptions::GetAverageChannelValue()
{
    if(channelvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetAverageValue(channelvaluesbuffer);
}

double ChannelOptions::GetMaxplusMinChannelValue()
{
    if(channelvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetMaximumValue(channelvaluesbuffer) + mathresolver::dGetMinimumValue(channelvaluesbuffer);
}

double ChannelOptions::GetDempheredChannelValue()
{
    double res;
    int count = GetDempherValue();
    if (count>1)
        res = mathresolver::dGetDempheredValue(channelvaluesbuffer,count);
    else
        res = currentvalue;
    return res;
}

double ChannelOptions::GetValuePercent()
{
    float razmah = GetHigherMeasureLimit() - GetLowerMeasureLimit();
    if(razmah == 0) return 0;
    float x = (100 * (GetCurrentChannelValue() - GetLowerMeasureLimit()))/razmah;
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
        case Voltage0_100mV:
        {
            hisignal = 0.1;
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
//    currentvalue = ConvertSignalToValue(value);
    currentvalue = value;

    if (this->IsChannelMathematical())
    {
        double mathresult = mathresolver::SolveEquation(this->GetMathString(),currentvalue);
        currentvalue = mathresult;
    }


    buffermutex->lock();
    while (channelxbuffer.length()>300)
        channelxbuffer.removeFirst();
    while (channelvaluesbuffer.length()>300)
        channelvaluesbuffer.removeFirst();
    while (dempheredvaluesbuffer.length()>300)
        dempheredvaluesbuffer.removeFirst();

    if(!X_Coordinates.isEmpty())
    {
        channelvaluesbuffer.append(currentvalue);
        dempheredvaluesbuffer.append(GetDempheredChannelValue());
        channelxbuffer.append(X_Coordinates.last()); // добавляем последнюю координату
    }

    buffermutex->unlock();
}

void ChannelOptions::SetDempher(int newdempher)
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

int ChannelOptions::getShema()
{

    if(outputData.chanSignalType == TermoResistanceMeasure)
    {
        return outputData.chanAdditionalParameter1[0];
    }
    return 0;
}

void ChannelOptions::setShema(int sh)
{
    shema = sh;
    if(outputData.chanSignalType == TermoResistanceMeasure)
    {
        outputData.chanAdditionalParameter1[0];
    }
}
