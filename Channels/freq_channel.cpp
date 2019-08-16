#include "freq_channel.h"
#include "registersmap.h"
#include "mathresolver.h"

extern QVector<double> X_Coordinates;

cFreqChannel::cFreqChannel(QObject *parent) : QObject(parent)
{
    buffermutex = new QMutex();
    enable = false;
    currentvalue = 0;
    measureperiod = 1;
    slot = 0;       //по-умолчанию присваиваем несуществующий слот
    name = "Fr";
    lowermeasurelimit = 0;
    highermeasurelimit = 100;
    unit = "Гц";
    weight = 1;
    lowerlimit = 0;
    higherlimit = 100;
    demphervalue = 0;
    outputData.impulseDuration = 40;
    inputData.digitSignalType = FreqNoMeasure;
    outputData.digitSignalType = FreqNoMeasure;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->setInterval(measureperiod);
    timer->start();

    timerUpdateParam = new QTimer();
    connect(timerUpdateParam, SIGNAL(timeout()), this, SLOT(updateParam()));
    updateParam();
    timerUpdateParam->start(30000);

}

void cFreqChannel::setImpulseDuration(float duration)
{
    outputData.impulseDuration = duration;
    QString nameParam = "chan" + QString::number(slotChannel) + "ImpulseDuration";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    trans.volFlo = outputData.impulseDuration;
    emit sendToWorker(trans);
}


void cFreqChannel::slotResetImpulsBuffer()
{
    outputData.impulseBuff = inputData.impulse;
    QString nameParam = "chan" + QString::number(slotChannel) + "ImpulseBuff";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    trans.volFlo = outputData.impulseBuff;
    emit sendToWorker(trans);
}

void cFreqChannel::SetCurrentChannelValue(double value)
{
    currentvalue = value;

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
        channeltimebuffer.append(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
    }
    buffermutex->unlock();
}

double cFreqChannel::GetDempheredChannelValue()
{
    double res;
    int count = demphervalue;
    if (count>1)
        res = mathresolver::dGetDempheredValue(channelvaluesbuffer, count);
    else
        res = currentvalue;
    return res;
}

void cFreqChannel::SetSignalType(uint16_t newsignaltype)
{
    outputData.digitSignalType = newsignaltype;
    if (!enable) return;
    // Отправка тут же актуальной информации в плату
    QString nameParam = "chan" + QString::number(slotChannel) + "DigitSignalType";
    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
    Transaction trans = Transaction(Transaction::W, (uint8_t)slot, offset);
    trans.volInt = outputData.digitSignalType;
    emit sendToWorker(trans);
}

double cFreqChannel::GetCurrentChannelValue()
{
    if (demphervalue > 1)
    {
        currentvalue = GetDempheredChannelValue();
    }
    return currentvalue;
}

void cFreqChannel::timerSlot()
{

    if(enable && (outputData.digitSignalType != FreqNoMeasure))
    {
        QString nameParam = "chan" + QString::number(slotChannel);
        if(outputData.digitSignalType == FrequencyMeasure)
        {
            nameParam += "MeasureFreq";
        }
        else if(outputData.digitSignalType == ImpulseCounterMeasure)
        {
            nameParam += "Impulse";
        }
        uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
        Transaction tr(Transaction::R, (uint8_t)slot, offset, 0);
        emit sendToWorker(tr);
    }

    timer->setInterval((int)(measureperiod*1000));
}

void cFreqChannel::updateParam()
{
    if (!enable) return;
    listStr.clear();
    listStr << "chan" + QString::number(slotChannel) + "DigitSignalType";
    if(outputData.digitSignalType == ImpulseCounterMeasure)
    {
        listStr << "chan" + QString::number(slotChannel) + "ImpulseDuration";
    }
    Transaction tr;
    tr.dir = Transaction::R;
    tr.slave = slot;

    foreach (QString str, listStr) {
        tr.offset = cRegistersMap::getOffsetByName(str);
        emit sendToWorker(tr);
    }

}

void cFreqChannel::parserChannel(Transaction tr)
{
    Transaction trans = tr;
    trans.dir = Transaction::W;
    QString paramName = cRegistersMap::getNameByOffset(tr.offset);
    QString chanName = "chan" + QString::number(slotChannel);
    if(paramName == chanName + "MeasureFreq")
    {
        SetCurrentChannelValue((double)tr.volFlo);
        inputData.measureFreq = tr.volFlo;
    }
    else if(paramName == chanName + "Impulse")
    {
        // вычисляется расход единиц за секунду
        double rate = weight * (tr.volFlo - inputData.impulse) / measureperiod;
        SetCurrentChannelValue(rate);
        inputData.impulse = tr.volFlo;
    }
    else if(paramName == chanName + "DigitSignalType")
    {
        inputData.digitSignalType = (uint16_t)tr.volInt;
        if(inputData.digitSignalType != outputData.digitSignalType)
        {
            trans.volInt = (uint32_t)outputData.digitSignalType;
            emit sendToWorker(trans);
        }
    }
    else if(paramName == chanName + "ImpulseDuration")
    {
        inputData.impulseDuration = (uint16_t)tr.volFlo;
        if(inputData.impulseDuration != outputData.impulseDuration)
        {
            trans.volFlo = (uint32_t)outputData.impulseDuration;
            emit sendToWorker(trans);
        }
    }
}

void cFreqChannel::SetMeasurePeriod(double mp)
{
    this->measureperiod = mp;
    if(measureperiod != 0)
    {
        timer->start(measureperiod);
    }
    else
    {
        timer->stop();
    }
}

QVector<double> cFreqChannel::GetChannelValuesBuffer()
{
    QVector<double> buf;

    buffermutex->lock();

    if (demphervalue<=1)
        buf = channelvaluesbuffer;
    else
        buf = dempheredvaluesbuffer;

    buffermutex->unlock();

    return buf;
}

QVector<double> cFreqChannel::GetChannelTimeBuffer()
{
    QVector<double> buf;
    buffermutex->lock();
    buf = channeltimebuffer;
    buffermutex->unlock();
    return buf;
}

double cFreqChannel::GetValuePercent()
{
    float razmah = GetHigherMeasureLimit() - GetLowerMeasureLimit();
    if(razmah == 0) return 0;
    float x = (100 * (GetCurrentChannelValue() - GetLowerMeasureLimit()))/razmah;
    return x;
}
