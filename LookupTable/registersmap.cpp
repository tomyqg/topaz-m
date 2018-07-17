#include "registersmap.h"
//#include "_lookupTables.h"
#include "lookup_tables.h"

extern tDeviceBasicParams g_deviceDataStorage;

// Below are the wrapper function for executions of methods from pure "C" code
extern "C" {

// register read/write wrapper function (access protection)
uint8_t mbRegRwCheckFunc(void* param, void* buffer)
{
    return 0;
}
}

cRegistersMap::cRegistersMap(QObject *parent) : QObject(parent)
{

}

uint16_t cRegistersMap::getOffsetByName(QString name)
{
    for(int i = 0; i < g_numLookupRegisters; i++)
    {
        QString str(g_stLookupRegisters[i].nameParam);
        if(str == name)
            return g_stLookupRegisters[i].offset;
    }
    return 0xFFFF;
}

QString cRegistersMap::getNameByOffset(uint16_t offset)
{
    for(int i = 0; i < g_numLookupRegisters; i++)
    {
        if(g_stLookupRegisters[i].offset == offset)
        {
            QString str(g_stLookupRegisters[i].nameParam);
            return str;
        }
    }
    return "NaN";
}

tLookupRegisters cRegistersMap::getDpByOffset(uint16_t offset)
{
    tLookupRegisters tlr;

    for(int i = 0; i < g_numLookupRegisters; i++)
    {
        if(g_stLookupRegisters[i].offset == offset)
        {
            tlr = g_stLookupRegisters[i];
        }
    }

    return tlr;
}
