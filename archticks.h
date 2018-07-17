#ifndef ARCHTICKS_H
#define ARCHTICKS_H

#define TOTAL_NUM_CHANNELS 24
// время в 32битном формате
typedef  uint32_t timeStamp2000;

#pragma pack(push, 1)
typedef struct
{
    timeStamp2000 time;
    float channel[TOTAL_NUM_CHANNELS];
} sTickCh;
#pragma pack(pop)

#endif // ARCHTICKS_H
