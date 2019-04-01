#include "math_channel.h"
#include "channelOptions.h"

extern QList<ChannelOptions *> listChannels;
extern QVector<double> X_Coordinates;

cMathChannel::cMathChannel(QObject *parent) : QObject(parent)
{
    // по молчанию все аргументы отключены (0)
    memset(numChannel, 0, sizeof(numChannel));
    mathstring = "x1 + x2 + x3 + x4";
    higherLimit = 100;
    lowerLimit = 0;
    currentvalue = 0;
    connect(&timerUpdate, SIGNAL(timeout()), this, SLOT(slotUpdate()));
    timerUpdate.start(500);
    buffermutex = new QMutex();
}

cMathChannel::~cMathChannel()
{
    delete buffermutex;
}

QString cMathChannel::GetMathString()
{
    return mathstring;
}

void cMathChannel::SetMathEquation(QString newmathstring)
{
    mathstring = newmathstring;
}

double cMathChannel::GetCurrentMathValue()
{
    return currentvalue;
}

void cMathChannel::slotUpdate()
{
    const int countArgs = 4;
    double arg[countArgs];

    for(int i = 0; i < countArgs; i++)
    {
        if((numChannel[i] != -1) && (numChannel[i] < listChannels.size()))
        {
            arg[i] = listChannels.at(numChannel[i])->GetCurrentChannelValue();
        }
        else
        {
            arg[i] = NAN;
        }
    }

    currentvalue = mathresolver::SolveEquation(mathstring, arg[0], arg[1], arg[2], arg[3]);

    buffermutex->lock();
    while (mathxbuffer.length()>300)
        mathxbuffer.removeFirst();
    while (mathvaluesbuffer.length()>300)
        mathvaluesbuffer.removeFirst();
    while (dempheredvaluesbuffer.length()>300)
        dempheredvaluesbuffer.removeFirst();
    if(!X_Coordinates.isEmpty())
    {
        mathvaluesbuffer.append(currentvalue);
        //        dempheredvaluesbuffer.append(GetDempheredMathValue());
        mathxbuffer.append(X_Coordinates.last()); // добавляем последнюю координату
    }
    buffermutex->unlock();
}


double cMathChannel::GetValuePercent()
{
    float razmah = GetHigherMeasureLimit() - GetLowerMeasureLimit();
    if(razmah == 0) return 0;
    float x = (100 * (GetCurrentMathValue() - GetLowerMeasureLimit()))/razmah;
    return x;
}





QVector<double> cMathChannel::GetMathValuesBuffer()
{
    QVector<double> buf;

    buffermutex->lock();

//    if (GetDempherValue()<=1)
        buf = mathvaluesbuffer;
//    else
//        buf = dempheredvaluesbuffer;

    buffermutex->unlock();

    return buf;
}


QVector<double> cMathChannel::GetMathXBuffer()
{
    QVector<double> buf;
    buffermutex->lock();
    buf = mathxbuffer;
    buffermutex->unlock();
    return buf;
}
