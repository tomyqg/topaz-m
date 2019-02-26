#ifndef CGROUPCHANNELS_H
#define CGROUPCHANNELS_H

#include <QObject>
#include "channelOptions.h"
#include "math_channel.h"

#define MAX_NUM_CHAN_GROUP    4

class cGroupChannels : public QObject
{
    Q_OBJECT
public:
    explicit cGroupChannels(QObject *parent = 0);
    int index;
    bool enabled;      //состояние группы: 0 - выкл, 1 - вкл.
    QString groupName;      //название группы
//    ChannelOptions * channel[MAX_NUM_CHAN_GROUP];
    int channel[MAX_NUM_CHAN_GROUP];
//    cMathChannel * mathChannel[MAX_NUM_CHAN_GROUP];
    int mathChannel[MAX_NUM_CHAN_GROUP];
    int typeInput[MAX_NUM_CHAN_GROUP];            //тип входа: 0 - не назначен
                                                //1 - AI, 2 - MI, 3 - DI
    int numChannel;    //номер канала в системе для идентификации при включении

signals:

public slots:
};

#endif // CGROUPCHANNELS_H
