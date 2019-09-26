#include <QDebug>
#include <stdlib.h>
#include "channelOptions.h"
#include "../uartdriver.h"
#include "../metrologicalcalc.h"
#include "deviceparams.h"

extern QVector<double> X_Coordinates;
extern QVector<double> X_Date_Coordinates;

double cnt;

// constructor
ChannelOptions::ChannelOptions()
{
    SetConfirmationNeed(true);
    enable = false;
    currentvalue = 0;
    newValue = false;
    measureperiod = 1;
    diapason = Voltage_1V;
    diapasonUser = Voltage1V;
    slot = 0;       //по-умолчанию присваиваем несуществующий слот
    registrationtype = 0;
    channelname = "Ch";
    MathematicalState = false;
    lowermeasurelimit = -100;
    highermeasurelimit = 100;
    higherlimit = 100;
    lowerlimit = -100;
    signaltype = 2;
    demphervalue = 0;
    memset(&outputData, 0, sizeof(outputData));
    memset(&inputData, 0, sizeof(inputData));
    outputData.chanSignalType = VoltageMeasure;
//    precision = Precision_X_X;
    memset(outputData.chanAdditionalParameter1, 0, sizeof(outputData.chanAdditionalParameter1));
    outputData.chanAdditionalParameter1[0] = diapason;
    unitsname = "mV";
    indexMultiplier = 1;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->setInterval(measureperiod);
    timer->stop();
    buffermutex = new QMutex();
    listStr << "SignalType" \
            << "DataFlags" \
            << "Error" \
            << "Status" \
            << "AdditionalParameter1"\
            << "AdditionalParameter2"\
            << "FSRinternal";
    iteratorParam = 0;
    timerUpdateParam = new QTimer();
    connect(timerUpdateParam, SIGNAL(timeout()), this, SLOT(updateParam()));
    updateParam();
    timerUpdateParam->start(30000);
    valueVoltage = Value_Real;
    precision = (typePrecision)optimalPrecision();


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

    return outputData.chanAdditionalParameter1[0];
//    if(outputData.chanSignalType == VoltageMeasure)
//    {
//        return outputData.chanAdditionalParameter1[0];
//    }
//    else if(outputData.chanSignalType == TermoResistanceMeasure)
//    {
//        return outputData.chanAdditionalParameter1[2];
//    }
//    else if(outputData.chanSignalType == TermoCoupleMeasure)
//    {
//        return outputData.chanAdditionalParameter1[2];
//    }
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
    if (!enable) return;
    // Отправка тут же актуальной информации в плату
    QString nameParam = "chan" + QString::number(slotChannel) + "SignalType";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    trans.volInt = outputData.chanSignalType;
    emit sendToWorker(trans);
}

void ChannelOptions::SetCurSignalType(uint16_t newsignaltype)
{
    this->cursignaltype = newsignaltype;
}

//void ChannelOptions::SetDiapason(int newdiapason)
//{
//    this->diapason = newdiapason;
//    if(outputData.chanSignalType == VoltageMeasure)
//    {
//        outputData.chanAdditionalParameter1[0] = newdiapason;
//    }
//    else if(outputData.chanSignalType == TermoResistanceMeasure)
//    {
//        outputData.chanAdditionalParameter1[2] = newdiapason;
//    }
//    else if(outputData.chanSignalType == TermoCoupleMeasure)
//    {
//        outputData.chanAdditionalParameter1[2] = newdiapason;
//    }
//    // Отправка тут же актуальной информации в плату
//    QString nameParam = "chan" + QString::number(slotChannel) + "AdditionalParameter1";
//    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
//    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
//    memcpy(trans.paramA12, outputData.chanAdditionalParameter1, sizeof(outputData.chanAdditionalParameter1));
//    emit sendToWorker(trans);

//}

void ChannelOptions::SetDiapasonShema(int newdiapason, int sh = 0)
{
    diapason = newdiapason;
    shema = sh;

    outputData.chanAdditionalParameter1[0] = newdiapason;
    if(outputData.chanSignalType == TermoResistanceMeasure)
    {
        outputData.chanAdditionalParameter1[2] = sh;
    }

//    if(outputData.chanSignalType == VoltageMeasure)
//    {
//        outputData.chanAdditionalParameter1[0] = newdiapason;
//    }
//    else if(outputData.chanSignalType == TermoResistanceMeasure)
//    {
//        outputData.chanAdditionalParameter1[0] = sh;
//        outputData.chanAdditionalParameter1[2] = newdiapason;
//    }
//    else if(outputData.chanSignalType == TermoCoupleMeasure)
//    {
//        outputData.chanAdditionalParameter1[2] = newdiapason;
//    }

    if (!enable) return;
    // Отправка тут же актуальной информации в плату
    QString nameParam = "chan" + QString::number(slotChannel) + "AdditionalParameter1";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    memcpy(trans.paramA12, outputData.chanAdditionalParameter1, sizeof(outputData.chanAdditionalParameter1));
    emit sendToWorker(trans);

}

void ChannelOptions::enableColdJunction(int en)
{
    enColdJunction = en;
    if(outputData.chanSignalType == TermoCoupleMeasure)
    {
        outputData.chanAdditionalParameter1[2] = enColdJunction;
    }
}

int ChannelOptions::getStateColdJunction()
{
    return outputData.chanAdditionalParameter1[2];
}

void ChannelOptions::setShiftColdJunction(double shift)
{
    shiftColdJunction = shift;
    outputData.chanFSRinternal = (float)shift;
    if (!enable) return;
    // Отправка тут же актуальной информации в плату
    QString nameParam = "chan" + QString::number(slotChannel) + "FSRinternal";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    trans.volFlo = outputData.chanFSRinternal;
    emit sendToWorker(trans);
}

double ChannelOptions::getShiftColdJunction()
{
    return (double)outputData.chanFSRinternal;
}


/*
 *      Проверка наличия новых данных
 *      Тут же сбрасывается признак новых данных
 */
bool ChannelOptions::isNewValue()
{
    bool ret = newValue;
    newValue = false;
    return ret;
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


int ChannelOptions::GetDempherValue()
{
    return demphervalue;
}


bool ChannelOptions::IsChannelMathematical()
{
    return this->MathematicalState;
}

void ChannelOptions::newTransaction(QString param, Transaction::dir_t dir, uint32_t value)
{
    QString name = "chan" + QString::number(slotChannel) + param;
    uint16_t offset = cRegistersMap::getOffsetByName(name);
    Transaction tr(dir, (uint8_t)slot, offset, 0);
    tr.volInt = value;
    emit sendToWorker(tr);
}

void ChannelOptions::parserChannel(Transaction tr)
{
    Transaction trans = tr;
    trans.dir = Transaction::W;
    QString paramName = cRegistersMap::getNameByOffset(tr.offset);
    QString chanName = "chan" + QString::number(slotChannel);
    if(paramName == chanName + "Data")
    {
        double data = tr.volFlo;
        SetCurrentChannelValue(data);
        if(std::isnan(data) != std::isnan(inputData.chanData))
        {
            // запросить статусы, флаги и ошибки
            newTransaction("DataFlags", Transaction::R);
            newTransaction("Status", Transaction::R);
            newTransaction("Error", Transaction::R);
        }
        inputData.chanData = data;
    }
    else if(paramName == chanName + "DataFlags")
    {
        inputData.chanDataFlags = (uint16_t)tr.volInt;
    }
    else if(paramName == chanName + "Status")
    {
        inputData.chanStatus = (uint16_t)tr.volInt;
    }
    else if(paramName == chanName + "Error")
    {
        inputData.chanError = (uint16_t)tr.volInt;
    }
    else if(paramName == chanName + "Quantity")
    {
        inputData.chanQuantity = (uint32_t)tr.volInt;
    }
    else if(paramName == chanName + "Uptime")
    {
        inputData.chanUptime = (uint32_t)tr.volInt;
    }
    else if(paramName == chanName + "RawData")
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
    else if(paramName == chanName + "FSRinternal")
    {
        inputData.chanFSRinternal = tr.volFlo;
        if(inputData.chanFSRinternal != outputData.chanFSRinternal)
        {
            trans.volFlo = outputData.chanFSRinternal;
            emit sendToWorker(trans);
        }
    }
    else if(paramName == chanName + "SysOCR")
    {
        calibrations.chanSysOCR = tr.volInt;

    }
    else if(paramName == chanName + "SysFSR")
    {
        calibrations.chanSysFSR = tr.volInt;
    }
    else if(paramName == chanName + "OCR20mV")
    {
        calibrations.chanOCR20mV = tr.volInt;
    }
    else if(paramName == chanName + "FSR20mV")
    {
        calibrations.chanFSR20mV = tr.volInt;
    }
    else if(paramName == chanName + "Date20mV")
    {
        calibrations.chanDate20mV = tr.volInt;
    }
    else if(paramName == chanName + "OCR100mV")
    {
        calibrations.chanOCR100mV = tr.volInt;
    }
    else if(paramName == chanName + "FSR100mV")
    {
        calibrations.chanFSR100mV = tr.volInt;
    }
    else if(paramName == chanName + "Date100mV")
    {
        calibrations.chanDate100mV = tr.volInt;
    }
    else if(paramName == chanName + "OCR1V")
    {
        calibrations.chanOCR1V = tr.volInt;
    }
    else if(paramName == chanName + "FSR1V")
    {
        calibrations.chanFSR1V = tr.volInt;
    }
    else if(paramName == chanName + "Date1V")
    {
        calibrations.chanDate1V = tr.volInt;
    }
    else if(paramName == chanName + "OCR10V")
    {
        calibrations.chanOCR10V = tr.volInt;
    }
    else if(paramName == chanName + "FSR10V")
    {
        calibrations.chanFSR10V = tr.volInt;
    }
    else if(paramName == chanName + "Date10V")
    {
        calibrations.chanDate10V = tr.volInt;
    }
    else if(paramName == chanName + "OCR30V")
    {
        calibrations.chanOCR30V = tr.volInt;
    }
    else if(paramName == chanName + "FSR30V")
    {
        calibrations.chanFSR30V = tr.volInt;
    }
    else if(paramName == chanName + "Date30V")
    {
        calibrations.chanDate30V = tr.volInt;
    }
    else if(paramName == chanName + "OCR20mA")
    {
        calibrations.chanOCR20mA = tr.volInt;
    }
    else if(paramName == chanName + "FSR20mA")
    {
        calibrations.chanFSR20mA = tr.volInt;
    }
    else if(paramName == chanName + "Date20mA")
    {
        calibrations.chanDate20mA = tr.volInt;
    }
    else if(paramName == chanName + "OCR4x")
    {
        calibrations.chanOCR4x = tr.volInt;
    }
    else if(paramName == chanName + "FSR4x")
    {
        calibrations.chanFSR4x = tr.volInt;
    }
    else if(paramName == chanName + "Date4x")
    {
        calibrations.chanDate4x = tr.volInt;
    }
    else if(paramName == chanName + "OCR3x")
    {
        calibrations.chanOCR3x = tr.volInt;
    }
    else if(paramName == chanName + "FSR3x")
    {
        calibrations.chanFSR3x = tr.volInt;
    }
    else if(paramName == chanName + "Date3x")
    {
        calibrations.chanDate3x = tr.volInt;
    }
    else if(paramName == chanName + "OCRinternal")
    {
        calibrations.chanOCRinternal = tr.volInt;
    }
    else if(paramName == chanName + "FSRinternal")
    {
        calibrations.chanFSRinternal = tr.volInt;
    }
    else if(paramName == chanName + "DateInternal")
    {
        calibrations.chanDateInternal = tr.volInt;
    }

    // Если пришли калибровки, то подсчитать долю прочитанных калибровок
    if(tr.offset >= CALIB_START_OFFSET)
    {
        foreach (QString calib, listCalibrationRegisters) {
            if(paramName == (chanName + calib))
            {
                processReadCalibrations += (1.0 / listCalibrationRegisters.size()) ;
            }
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
#ifdef RANDOM_CHAN
    else if(!enable)
    {
        if(numChannel <= NUM_CHAN_IN_4AI)
        {
            SetCurrentChannelValue(currentvalue + ((double)((rand()%101) - 50) / 100));
        }
    }
#endif

    timer->setInterval((int)(measureperiod*1000));
}

void ChannelOptions::updateParam()
{
    if (!enable) return;
//    Transaction tr;
//    tr.dir = Transaction::R;
//    tr.slave = slot;
//    int devCh = slotChannel;

//    listStr << "chan" + QString::number(devCh) + "SignalType" \
//            << "chan" + QString::number(devCh) + "Error" \
//            << "chan" + QString::number(devCh) + "AdditionalParameter1"\
//            << "chan" + QString::number(devCh) + "AdditionalParameter2";
//    if(outputData.chanSignalType == TermoCoupleMeasure)
//    {
//        listStr << "chan" + QString::number(devCh) + "FSRinternal";
//    }

    QString str = listStr.at(iteratorParam % listStr.size());
    if((str != "FSRinternal") || (outputData.chanSignalType == TermoCoupleMeasure))
    {
//        QString s = "chan" + QString::number(devCh) + str;
//        tr.offset = cRegistersMap::getOffsetByName(s);
//        emit sendToWorker(tr);
        newTransaction(str, Transaction::R);
    }
    iteratorParam++;
    if(iteratorParam >= listStr.size())
    {
        iteratorParam = 0;
    }
}


void ChannelOptions::initCalibration()
{
    listCalibrationRegisters.clear();

    listCalibrationRegisters /*<< "SysOCR" << "SysFSR" */\
                             << "OCR20mV" << "FSR20mV" << "Date20mV" \
                             << "OCR100mV" << "FSR100mV" << "Date100mV" \
                             << "OCR1V" << "FSR1V" << "Date1V" \
                             << "OCR10V" << "FSR10V" << "Date10V"
                             << "OCR30V" << "FSR30V" << "Date30V" \
                             << "OCR20mA" << "FSR20mA" << "Date20mA" \
                             << "OCR4x" << "FSR4x" << "Date4x" \
                             << "OCR3x" << "FSR3x" << "Date3x" \
                             << "OCRinternal" << "FSRinternal" << "DateInternal";
    iteratorCalibration = 0;
    neadRead = false;
    processReadCalibrations = 0;

    timerCalibrations = new QTimer(this);
    connect(timerCalibrations, SIGNAL(timeout()), this, SLOT(updateCalibrations()));
    periodUpdateCalibrations = 1000;    // 1сек
    timerCalibrations->start(periodUpdateCalibrations);
}

void ChannelOptions::updateCalibrations()
{
    if (!enable) return;
    Transaction tr;
    tr.dir = Transaction::R;
    tr.slave = slot;
    QString name;
    if(neadRead)
    {
        for(int i=0; i<listCalibrationRegisters.size(); i++)
        {
            name = "chan" + QString::number(slotChannel) + listCalibrationRegisters.at(i);
            tr.offset = cRegistersMap::getOffsetByName(name);
            emit sendToWorker(tr);
        }
        neadRead = false;
    }
    if((calibrations.chanSysFSR != 0) || (calibrations.chanSysOCR != 0))
    {
        name = "chan" + QString::number(slotChannel) + "SysFSR";
        tr.offset = cRegistersMap::getOffsetByName(name);
        emit sendToWorker(tr);
        name = "chan" + QString::number(slotChannel) + "SysOCR";
        tr.offset = cRegistersMap::getOffsetByName(name);
        emit sendToWorker(tr);
    }
}

void ChannelOptions::writeCalibration(QString paramName, uint32_t value)
{
    if (!enable) return;
    Transaction tr;
    tr.dir = Transaction::W;
    tr.slave = slot;
    QString name = "chan" + QString::number(slotChannel) + paramName;
    tr.offset = cRegistersMap::getOffsetByName(name);
    tr.volInt = value;

    if(paramName == "SysOCR")
    {
        calibrations.chanSysOCR = value & 0xFFFF;
    }
    else if(paramName == "SysFSR")
    {
        calibrations.chanSysFSR = value & 0xFFFF;
    }
    else if(paramName == "OCR20mV")
    {
        calibrations.chanOCR20mV = value;
    }
    else if(paramName == "FSR20mV")
    {
        calibrations.chanFSR20mV = value;
    }
    else if(paramName == "Date20mV")
    {
        calibrations.chanDate20mV = value;
    }
    else if(paramName == "OCR100mV")
    {
        calibrations.chanOCR100mV = value;
    }
    else if(paramName == "FSR100mV")
    {
        calibrations.chanFSR100mV = value;
    }
    else if(paramName == "Date100mV")
    {
        calibrations.chanDate100mV = value;
    }
    else if(paramName == "OCR1V")
    {
        calibrations.chanOCR1V = value;
    }
    else if(paramName == "FSR1V")
    {
        calibrations.chanFSR1V = value;
    }
    else if(paramName == "Date1V")
    {
        calibrations.chanDate1V = value;
    }
    else if(paramName == "OCR10V")
    {
        calibrations.chanOCR10V = value;
    }
    else if(paramName == "FSR10V")
    {
        calibrations.chanFSR10V = value;
    }
    else if(paramName == "Date10V")
    {
        calibrations.chanDate10V = value;
    }
    else if(paramName == "OCR30V")
    {
        calibrations.chanOCR30V = value;
    }
    else if(paramName == "FSR30V")
    {
        calibrations.chanFSR30V = value;
    }
    else if(paramName == "Date30V")
    {
        calibrations.chanDate30V = value;
    }
    else if(paramName == "OCR20mA")
    {
        calibrations.chanOCR20mA = value;
    }
    else if(paramName == "FSR20mA")
    {
        calibrations.chanFSR20mA = value;
    }
    else if(paramName == "Date20mA")
    {
        calibrations.chanDate20mA = value;
    }
    else if(paramName == "OCR4x")
    {
        calibrations.chanOCR4x = value;
    }
    else if(paramName == "FSR4x")
    {
        calibrations.chanFSR4x = value;
    }
    else if(paramName == "Date4x")
    {
        calibrations.chanDate4x = value;
    }
    else if(paramName == "OCR3x")
    {
        calibrations.chanOCR3x = value;
    }
    else if(paramName == "FSR3x")
    {
        calibrations.chanFSR3x = value;
    }
    else if(paramName == "Date3x")
    {
        calibrations.chanDate3x = value;
    }
    else if(paramName == "OCRinternal")
    {
        calibrations.chanOCRinternal = value;
    }
    else if(paramName == "FSRinternal")
    {
        calibrations.chanFSRinternal = value;
    }
    else if(paramName == "DateInternal")
    {
        calibrations.chanDateInternal = value;
    }
    emit sendToWorker(tr);
//    tr.dir = Transaction::R;
//    emit sendToWorker(tr);

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

QVector<double> ChannelOptions::GetChannelTimeBuffer()
{
    QVector<double> buf;
    buffermutex->lock();
    buf = channeltimebuffer;
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


    if (GetSignalType() == CurrentMeasure)
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

    if (GetSignalType() == VoltageMeasure)
    {
        switch (GetDiapason()) {

        case Voltage_1V:
        {
            hisignal = 1000;
            lowsignal = -1000;
            break;
        }

        case Voltage_10V:
        {
            hisignal = 10;
            lowsignal = -10;
            break;
        }

        case Voltage_30V:
        {
            hisignal = 30;
            lowsignal = -30;
            break;
        }
        default:
            break;
        }

        value = MetrologicalCalc::ConvertSignalToValue(signal,lowsignal,hisignal,lowlimit,hilimit); // берем начало и конец под-диапазона
    }

    return value;
}

double ChannelOptions::getMaxInDiapason(int diapason)
{
    if(diapason == -1) diapason = GetUserDiapason();
    double ret = GetHigherMeasureLimit();
    if (GetSignalType() == VoltageMeasure)
    {
        switch (diapason) {

        case Voltage150mV:   // значение приходит в милливольтах
            ret = 150;
            break;
        case Voltage300mV:   // значение приходит в милливольтах
            ret = 300;
            break;
        case Voltage0_1V:   // значение приходит в милливольтах
            ret = 1000;
            break;
        case Voltage0_5V:   // значение приходит в вольтах
            ret = 5;
            break;
        case Voltage0_10V:   // значение приходит в вольтах
            ret = 10;
            break;
        case Voltage1V:   // значение приходит в милливольтах
            ret = 1000;
            break;
        case Voltage10V: // значение приходит в вольтах
            ret = 10;
            break;
        case Voltage30V: // значение приходит в вольтах
            ret = 30;
            break;
        default:
            break;
        }
    }
    else if (GetSignalType() == CurrentMeasure)
    {
        switch (diapason) {

        case Current4_20mA:   // значение приходит в милливольтах
            ret = 20;
            break;
        case Current0_20mA:   // значение приходит в милливольтах
            ret = 20;
            break;
        case Current0_5mA:   // значение приходит в милливольтах
            ret = 5;
            break;
        default:
            break;
        }
    }
    return ret;
}

double ChannelOptions::getMinInDiapason(int diapason)
{
    if(diapason == -1) diapason = GetUserDiapason();
    double ret = GetLowerMeasureLimit();
    if (GetSignalType() == VoltageMeasure)
    {
        switch (diapason) {

        case Voltage150mV:   // значение приходит в милливольтах
            ret = -150;
            break;
        case Voltage300mV:   // значение приходит в милливольтах
            ret = -300;
            break;
        case Voltage0_1V:   // значение приходит в милливольтах
            ret = 0;
            break;
        case Voltage0_5V:   // значение приходит в вольтах
            ret = 0;
            break;
        case Voltage0_10V:   // значение приходит в вольтах
            ret = 0;
            break;
        case Voltage1V:   // значение приходит в милливольтах
            ret = -1000;
            break;
        case Voltage10V: // значение приходит в вольтах
            ret = -10;
            break;
        case Voltage30V: // значение приходит в вольтах
            ret = -30;
            break;
        default:
            break;
        }
    }
    else if (GetSignalType() == CurrentMeasure)
    {
        switch (diapason) {

        case Current4_20mA:   // значение приходит в милливольтах
            ret = 4;
            break;
        case Current0_20mA:   // значение приходит в милливольтах
            ret = 0;
            break;
        case Current0_5mA:   // значение приходит в милливольтах
            ret = 0;
            break;
        default:
            break;
        }
    }
    return ret;
}

double ChannelOptions::ConvertVisualValue(double signal, int diapason, int valueType, int multIndex)
{
    double value = signal;
    if(diapason == -1) diapason = GetUserDiapason();
    if(valueType == -1) valueType = getVoltageType();
    double hisignal = getMaxInDiapason(diapason);
    double lowsignal = getMinInDiapason(diapason);
    double lowlimit = 0;
    double hilimit = 0;

    if(multIndex == -1) multIndex = getIndexMultiplier();
    double multiplier = getSelectMultiplier(multIndex, diapason, GetSignalType(), valueType);


    switch (valueType) {
    case Value_Real:
        lowlimit = lowsignal;
        hilimit = hisignal;
        break;
    case Value_Procent:
//        multiplier = 1;
        lowlimit = 0;
        hilimit = 100;
    default:
        break;
    }

    value = MetrologicalCalc::ConvertSignalToValue(signal,lowsignal,hisignal,lowlimit,hilimit); // берем начало и конец под-диапазона

    return (value * multiplier);
}

int ChannelOptions::optimalPrecision()
{
    typePrecision prec = Precision_X_XX;

    if (GetSignalType() == CurrentMeasure)
    {
        prec = Precision_X_XXX;
    }
    else if (GetSignalType() == VoltageMeasure)
    {
        switch (GetDiapason()) {

        case Voltage_1V:   // значение приходит в милливольтах
        {
            prec = Precision_X_X;
            break;
        }
        case Voltage_10V: // значение приходит в вольтах
        {
            prec = Precision_X_XX;
            break;
        }
        case Voltage_30V: // значение приходит в вольтах
        {
            prec = Precision_X_XX;
            break;
        }
        default:
            break;
        }
    }
    else if (GetSignalType() == TermoCoupleMeasure)
    {
        prec = Precision_X_X;
    }
    else if (GetSignalType() == TermoResistanceMeasure)
    {
        prec = Precision_X_XX;
    }

    switch (getVoltageType()) {
    case Value_Real:
        //prec = prec;
        break;
    case Value_Procent:
        prec = Precision_X_XX;
    default:
        break;
    }

    return (int)prec;
}


void ChannelOptions::SetCurrentChannelValue(double value)
{
//    currentvalue = ConvertSignalToValue(value);
    currentvalue = value;
    newValue = true;

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
    while (channeltimebuffer.length() > 300)
        channeltimebuffer.removeFirst();

    if(!X_Coordinates.isEmpty())
    {
        channelvaluesbuffer.append(currentvalue);
        dempheredvaluesbuffer.append(GetDempheredChannelValue());
        channelxbuffer.append(X_Coordinates.last()); // добавляем последнюю координату
//        channelxbuffer.append(X_Date_Coordinates.last());
        channeltimebuffer.append(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
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

//    if(outputData.chanSignalType == TermoResistanceMeasure)
//    {
        return outputData.chanAdditionalParameter1[2];
//    }
//    return 0;
}

//void ChannelOptions::setShema(int sh)
//{
//    shema = sh;
//    if(outputData.chanSignalType == TermoResistanceMeasure)
//    {
//        outputData.chanAdditionalParameter1[0] = sh;
//    }
//    // Отправка тут же актуальной информации в плату
//    QString nameParam = "chan" + QString::number(slotChannel) + "AdditionalParameter1";
//    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
//    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
//    memcpy(trans.paramA12, outputData.chanAdditionalParameter1, sizeof(outputData.chanAdditionalParameter1));
//    emit sendToWorker(trans);
//}

void ChannelOptions::copyOptions(ChannelOptions * ch)
{
    outputData = ch->outputData;
    signaltype = ch->signaltype;
    lowerlimit = ch->lowerlimit;
    higherlimit = ch->higherlimit;
    lowermeasurelimit = ch->lowermeasurelimit;
    highermeasurelimit = ch->highermeasurelimit;
    measureperiod = ch->measureperiod;
    unitsname = ch->unitsname;
    channelname = ch->channelname;
    diapason = ch->diapason;
    diapasonUser = ch->diapasonUser;
    shema = ch->shema;
    registrationtype = ch->registrationtype;

}

//QString ChannelOptions::getNameUnitByParam(void)
//{
//    QString unit = "..";
//    if(GetSignalType() == VoltageMeasure)
//    {
//        if(diapason == Voltage_1V)
//        {
//            unit = "мВ";
//        }
//        else
//        {
//            unit = "В";
//        }
//    }
//    else if(GetSignalType() == CurrentMeasure)
//    {
//        unit = "мА";
//    }
//    else if(GetSignalType() == TermoCoupleMeasure)
//    {
//        unit = "°C";
//    }
//    else if(GetSignalType() == TermoResistanceMeasure)
//    {
//        unit = "°C";
//    }
//    else
//    {
//        unit = "..";
//    }

//    return unit;
//}

QString ChannelOptions::getNameUnitByParam(uint16_t type, int indexMult)
{
    QString unit = "..";
    if(indexMult == -1) indexMult = getIndexMultiplier();
//    double mult = getSelectMultiplier(indexMult);
    if(type == VoltageMeasure)
    {
        if(indexMult == voltUnit_mV)
        {
            unit = "мВ";
        }
        else
        {
            unit = "В";
        }
    }
    else if(type == CurrentMeasure)
    {
        unit = "мА";
    }
    else if(type == TermoCoupleMeasure)
    {
        unit = "°C";
    }
    else if(type == TermoResistanceMeasure)
    {
        unit = "°C";
    }
    else
    {
        unit = "..";
    }

    return unit;
}

void ChannelOptions::setAdditionalParametr1(uint8_t * param)
{
    memcpy(outputData.chanAdditionalParameter1, param, sizeof(outputData.chanAdditionalParameter1));
    if (!enable) return;
    // Отправка тут же актуальной информации в плату
    QString nameParam = "chan" + QString::number(slotChannel) + "AdditionalParameter1";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    memcpy(trans.paramA12, outputData.chanAdditionalParameter1, sizeof(outputData.chanAdditionalParameter1));
    emit sendToWorker(trans);
}

void ChannelOptions::setAdditionalParametr2(uint8_t * param)
{
    memcpy(outputData.chanAdditionalParameter2, param, sizeof(outputData.chanAdditionalParameter2));
    if (!enable) return;
    // Отправка тут же актуальной информации в плату
    QString nameParam = "chan" + QString::number(slotChannel) + "AdditionalParameter2";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    memcpy(trans.paramA12, outputData.chanAdditionalParameter2, sizeof(outputData.chanAdditionalParameter2));
    emit sendToWorker(trans);
}

void ChannelOptions::getAdditionalParametr1(uint8_t * param)
{
    memcpy(param, outputData.chanAdditionalParameter1, sizeof(outputData.chanAdditionalParameter1));
}

void ChannelOptions::getAdditionalParametr2(uint8_t * param)
{
    memcpy(param, outputData.chanAdditionalParameter2, sizeof(outputData.chanAdditionalParameter2));
}

/*
 * index = 0 - мВ
 * index = 1 - В
 */
double ChannelOptions::getSelectMultiplier(int index, int diap, int signal, int type)
{
    double mult = 1;

    if(diap != -1) diap = GetUserDiapason();
    if(signal != -1) signal = GetSignalType();
    if(type != -1) getVoltageType();

    if((type == Value_Real) && (signal == VoltageMeasure))
    {
        if((diap == Voltage150mV) || (diap == Voltage300mV) || \
                (diap == Voltage0_1V) || (diap == Voltage1V))
        {
            if(index == 1) mult = 0.001;
        }
        else
        {
            if(index == 0) mult = 1000;
        }
    }

    return mult;
}

bool ChannelOptions::isError()
{
    if((inputData.chanDataFlags == ECDF_BREAK) || (inputData.chanDataFlags == ECDF_ERROR))
        return true;
    if(inputData.chanError != EDE_NO_ERROR)
        return true;
    if((inputData.chanStatus == ECS_CONFIG) || (inputData.chanStatus == ECS_ERROR))
        return true;
    return false;
}

