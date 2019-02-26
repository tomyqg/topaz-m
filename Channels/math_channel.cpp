#include "math_channel.h"
#include "channelOptions.h"

extern QList<ChannelOptions *> listChannels;

cMathChannel::cMathChannel(QObject *parent) : QObject(parent)
{
    // по молчанию все аргументы отключены (0)
    memset(numChannel, 0, sizeof(numChannel));
    mathstring = "x1 + x2 + x3 + x4";
    higherLimit = 100;
    lowerLimit = 0;
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

    return mathresolver::SolveEquation(mathstring, arg[0], arg[1], arg[2], arg[3]);
}

double cMathChannel::GetValuePercent()
{
    float razmah = GetHigherMeasureLimit() - GetLowerMeasureLimit();
    if(razmah == 0) return 0;
    float x = (100 * (GetCurrentMathValue() - GetLowerMeasureLimit()))/razmah;
    return x;
}

double cMathChannel::GetHigherMeasureLimit()
{
    return higherLimit;
}

double cMathChannel::GetLowerMeasureLimit()
{
    return lowerLimit;
}
