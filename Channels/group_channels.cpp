#include "group_channels.h"

cGroupChannels::cGroupChannels(QObject *parent) : QObject(parent)
{
    enabled = false;
    for(int i = 0; i < MAX_NUM_CHAN_GROUP; i++)
    {
        typeInput[i] = 0;
        channel[i] = -1;
        mathChannel[i] = -1;
        freqChannel[i] = -1;
    }
}


