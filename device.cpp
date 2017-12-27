#include "device.h"

deviceparametrs Device::getDevParam(uint16_t addr)
{
    deviceparametrs ret({0, 0, 0, 0, 0, 0});
    foreach (deviceparametrs dp, parametrs)
    {
        if(dp.Offset == addr)
        {
            ret = dp;
            break;
        }
    }
    bool fExit = false;
    foreach (channelparametrs ch, channels)
    {
        foreach (deviceparametrs dp, ch.parametrs)
        {
            if(dp.Offset == addr)
            {
                ret = dp;
                fExit = true;
                break;
            }
        }
        if(fExit) break;
    }
    return ret;
}
