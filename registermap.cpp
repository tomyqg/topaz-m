#include "registermap.h"

uint16_t RegisterMap::getOffsetByName(QString name)
{
    for(int i = 0; i < SIZE_RM; i++)
    {
        if(rm[i].name == name)
        {
            return rm[i].Offset;
        }
    }
    return 0xFFFF;
}

QString RegisterMap::getNameByOffset(uint16_t offset)
{
    for(int i = 0; i < SIZE_RM; i++)
    {
        if(rm[i].Offset == offset)
        {
            return rm[i].name;
        }
    }
    return "NaN";
}
