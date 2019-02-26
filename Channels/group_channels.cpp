#include "group_channels.h"

cGroupChannels::cGroupChannels(QObject *parent) : QObject(parent)
{
    memset(typeInput, 0, sizeof(typeInput[0]) * MAX_NUM_CHAN_GROUP);
    for(int i = 0; i < MAX_NUM_CHAN_GROUP; i++)
    {
        channel[i] = -1;
        mathChannel[i] = -1;
    }
//    memset(channel, 0, sizeof(channel));
//    memset(mathChannel, 0, sizeof(mathChannel));
//    for(int i = 0; i < MAX_NUM_CHAN_GROUP; i++)
//    {
//        channel[i] = nullptr;
//    }
}


