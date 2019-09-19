#include "math_channel.h"
#include "channelOptions.h"

extern QList<ChannelOptions *> listChannels;
extern QVector<double> X_Coordinates;
extern QMutex mListChannel;



cMathChannel::cMathChannel(QObject *parent) : QObject(parent)
{
    // по молчанию все аргументы отключены (0)
    memset(numChannel, 0, sizeof(numChannel));
    mathstring = "x1 + x2 + x3 + x4";
    higherLimit = 100;
    lowerLimit = 0;
    currentvalue = 0;
    connect(&timerUpdate, SIGNAL(timeout()), this, SLOT(slotUpdate()));
    timerUpdate.start(MATH_UPDATE_PERIOD_MS);
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

double cMathChannel::testFormula(QString formula, int ch1, int ch2, int ch3, int ch4)
{
    double arg[NUM_MATH_ARG];
    int ch[NUM_MATH_ARG] = { ch1, ch2, ch3, ch4 };

    for(int i=0; i<NUM_MATH_ARG; i++)
    {
        if((ch[i] != -1) && (ch[i] < listChannels.size()))
        {
            mListChannel.lock();
            arg[i] = listChannels.at(ch[i])->GetCurrentChannelValue();
            mListChannel.unlock();
        }
        else
        {
            arg[i] = NAN;
        }
    }

    return (mathresolver::SolveEquation(formula, arg[0], arg[1], arg[2], arg[3]));
}

void cMathChannel::slotUpdate()
{
    double arg[NUM_MATH_ARG];

    for(int i = 0; i < NUM_MATH_ARG; i++)
    {
        if((numChannel[i] != -1) && (numChannel[i] < listChannels.size()))
        {
            mListChannel.lock();
            arg[i] = listChannels.at(numChannel[i])->GetCurrentChannelValue();
            mListChannel.unlock();
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
    while (mathtimebuffer.length() > 300)
        mathtimebuffer.removeFirst();
    if(!X_Coordinates.isEmpty())
    {
        mathvaluesbuffer.append(currentvalue);
        //        dempheredvaluesbuffer.append(GetDempheredMathValue());
        mathxbuffer.append(X_Coordinates.last()); // добавляем последнюю координату
        mathtimebuffer.append(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
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

QVector<double> cMathChannel::GetMathTimeBuffer()
{
    QVector<double> buf;
    buffermutex->lock();
    buf = mathtimebuffer;
    buffermutex->unlock();
    return buf;
}

double cMathChannel::GetMaximumMathValue()
{
    if(mathvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetMaximumValue(mathvaluesbuffer);
}

double cMathChannel::GetMinimumMathValue()
{
    if(mathvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetMinimumValue(mathvaluesbuffer);
}

double cMathChannel::GetAverageMathValue()
{
    if(mathvaluesbuffer.isEmpty()) return 0;
    return mathresolver::dGetAverageValue(mathvaluesbuffer);
}
