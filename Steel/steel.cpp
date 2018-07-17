#include "steel.h"
#include <assert.h>

cSteel::cSteel(QObject *parent) : QObject(parent)
{
    allVectorsReceived = false;
    vectorTempReceived = false;
    vectorEdsReceived = false;
}

//void cSteel::resetDefault(int group)
//{
//    assert(group > NUM_TECHNOLOGIES);
//    if(group >= NUM_TECHNOLOGIES) return;

//    curGroup = group;

//    technology.nSt = defTech[group].nSt;
//    technology.COH = defTech[group].COH;
//    technology.b1 = defTech[group].b1;
//    technology.dSt = defTech[group].dSt;
//    technology.dt = defTech[group].dt;
//    technology.tPt = defTech[group].tPt;
//    technology.LPtl = defTech[group].LPtl;
//    technology.LPth = defTech[group].LPth;
//    technology.dSE = defTech[group].dSE;
//    technology.tPE = defTech[group].tPE;
//    technology.b2 = defTech[group].b2;
//    technology.O = defTech[group].O;
//    technology.Y = defTech[group].Y;
//    technology.G = defTech[group].G;
//}
