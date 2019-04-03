#include "steel.h"
#include <assert.h>
#include "defines.h"

#define MAX_COUNT_BAD_PARAM 3
#define TIME_UPDATE_PARAM_STEEL_MC  500

int cSteel::countSteel = 0;

cSteel::cSteel(QObject *parent) : QObject(parent)
{
    allVectorsReceived = false;
    vectorTempReceived = false;
    vectorEdsReceived = false;
    lastItemEds = false;
    lastItemTemp = false;
    status = 0;
    vectorEds = QVector<double>(SIZE_ARRAY, NAN);
    vectorTemp = QVector<double>(SIZE_ARRAY, NAN);
    memset(relais, -1, SUM_RELAYS);
    num = countSteel;
    countSteel++;
    numSmelt = 1;
    state = STEEL_WAIT;
    mode = Device_Mode_Regular;
    //запуск периодического запроса данных
    connect(&timerUpdate, SIGNAL(timeout()), this, SLOT(update()));
    timerUpdate.start(UpdateSteelTime);
    connect(&timerUpdateParam, SIGNAL(timeout()), this, SLOT(updateParam()));
    timerUpdateParam.start(TIME_UPDATE_PARAM_STEEL_MC);
    //список параметров конфигурации в карте регистров
    listUpdateParam << "TimeSquareTemp" << "RangeTemp" << "TimeMeasureTemp" << "LowTemp" << "HiTemp" << "SensorType"\
        << "TimeSquareEDS" << "RangeEDS" << "TimeMeasureEDS" << "Crystallization" << "MassCoeff" \
        << "FinalOx" << "Assimilation" << "MassMelting" << "AdditionalParameter1";
    listDiagnosticParam << "Data" << "PrimaryActivity" << "ResultCjValue";
    countParam = 0;
    countDiagnostic = 0;
}

void cSteel::parserSteel(Transaction tr)
{
    Transaction trans = tr;
    trans.dir = Transaction::W;
    QString paramName = cRegistersMap::getNameByOffset(tr.offset);
    QString chanName = "chan" + QString::number(slotIndex);
    if(paramName == chanName + "Status")
    {
        status = tr.volInt;
    }
    else if(paramName == chanName + "Data")
    {
        temp = tr.volFlo;
    }
    else if(paramName == chanName + "PrimaryActivity")
    {
        eds = tr.volFlo;
    }
    else if(paramName == chanName + "ResultCjValue")
    {
        cj = tr.volFlo;
    }
    else if(paramName == chanName + "OxActivity")
    {
        ao = tr.volInt;
    }
    else if(paramName == chanName + "MassAl")
    {
        alg = tr.volInt;
    }
    else if(paramName == chanName + "Carbon")
    {
        allVectorsReceived = true;
        if(tr.paramA12[0] != 0x7FFF)
            cl = (float)tr.paramA12[0] / 1000;
        else
            cl = NAN;
    }
    else if(paramName.contains("TEMParray") || paramName.contains("EMFarray"))
    {
        int curArray = 0;
        for(int i = 0; i < 5; i++)
        { //перебор массивов по номерам в карте регистров
            if(paramName == QString("chan" + QString::number(slotIndex) \
                                    + "EMFarray" + QString::number(i)))
            {   //подобран номер массива
                vectorEdsReceived = true;
                lastItemEds = true;
                for(int j = 0; j < 32; j++)
                {   //поэлементное копирование полученного массива в соответствующий объект стали
                    curArray = i;
                    int indexVec = j + i * 32;
                    if(indexVec < SIZE_ARRAY) //проверка на всякий случай, чтобы не выйти за пределы массива
                    {
                        if(tr.paramInt16[j] != 0x7FFF)
                        {
                            if(technology->COH != 0)
                            {
                                vectorEds.replace(indexVec, (double)tr.paramInt16[j]);
                            }
                        }
                    }
                }
            }
            else if(paramName == QString("chan" + QString::number(slotIndex) \
                                         + "TEMParray" + QString::number(i)))
            {
                vectorTempReceived = true;
                lastItemTemp = true;
                for(int j = 0; j < 32; j++)
                {   //поэлементное копирование полученного массива в соответствующий объект стали
                    curArray = i;
                    int indexVec = j + i * 32;
                    if(indexVec < SIZE_ARRAY) //проверка на всякий случай, чтобы не выйти за пределы массива
                    {
                        if(tr.paramInt16[j] != 0x7FFF)
                        {
                            vectorTemp.replace(indexVec, (double)tr.paramInt16[j]);
                        }
                    }
                }
            }
            //оба масива после запроса
            if((vectorEdsReceived || (technology->COH == 0)) && \
                    vectorTempReceived)
            {
//                askNewArray = true;
                vectorTempReceived = false;
                vectorEdsReceived = false;
                if(state == STEEL_MEASURE)
                {
                    //очередной запрос массива
                    readArrays();
                }
            }

        }
    }
    if(state == STEEL_WAIT)
    {
        if(paramName == chanName + "TimeSquareTemp")
        {
            if(technology->dSt != tr.volFlo)
            {
                trans.volFlo = technology->dSt;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "RangeTemp")
        {
            if(technology->dt != tr.volFlo)
            {
                trans.volFlo = technology->dt;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "TimeMeasureTemp")
        {
            if(technology->tPt != tr.volFlo)
            {
                trans.volFlo = technology->tPt;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "LowTemp")
        {
            if(technology->LPtl != tr.volInt)
            {
                trans.volInt = technology->LPtl;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "HiTemp")
        {
            if(technology->LPth != tr.volInt)
            {
                trans.volInt = technology->LPth;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "SensorType")
        {
            if(technology->COH != tr.volInt)
            {
                trans.volInt = technology->COH;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "TimeSquareEDS")
        {
            if(technology->dSE != tr.volFlo)
            {
                trans.volFlo = technology->dSE;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "RangeEDS")
        {
            if(technology->dE != tr.volFlo)
            {
                trans.volFlo = technology->dE;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "TimeMeasureEDS")
        {
            if(technology->tPE != tr.volFlo)
            {
                trans.volFlo = technology->tPE;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "Crystallization")
        {
            if(technology->b1 != tr.volInt)
            {
                trans.volInt = technology->b1;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "MassCoeff")
        {
            if(technology->b2 != tr.volInt)
            {
                trans.volInt = technology->b2;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "FinalOx")
        {
            if(!isBlackListParam(paramName))
            {
                if(technology->O != tr.volInt)
                {
                    addBadParam(paramName);
                    trans.volInt = technology->O;
                    emit sendTransToWorker(trans);
                }
            }
        }
        else if(paramName == chanName + "Assimilation")
        {
            if(technology->Y != tr.volInt)
            {
                trans.volInt = technology->Y;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "MassMelting")
        {
            if(technology->G != tr.volInt)
            {
                trans.volInt = technology->G;
                emit sendTransToWorker(trans);
            }
        }
        else if(paramName == chanName + "AdditionalParameter1")
        {
            uint16_t param[6] = {0, technology->nSt, 0, 0, 0, 0};
            if(memcmp(tr.paramA12, param, sizeof(tr.paramA12)) != 0)
            {
                memcpy(trans.paramA12, param, sizeof(trans.paramA12));
                emit sendTransToWorker(trans);
            }
        }
    }
}

void cSteel::update()
{
    verifStatus = status;

    Transaction tr(Transaction::R, slot);
    if(enable)
    {

        //нужны постоянно в режими диагностики
        if(mode == Device_Mode_Metrological)
        {
            tr.offset = cRegistersMap::getOffsetByName("chan" \
                                                       + QString::number(slotIndex) \
                                                       + listDiagnosticParam.at(countDiagnostic++));
            emit sendTransToWorker(tr);
            if(countDiagnostic >= listDiagnosticParam.size()) countDiagnostic = 0;
        }
        else //mode == Device_Mode_Regular
        {
            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + "Status");
            emit sendTransToWorker(tr);
        }

        if(state == STEEL_WAIT)
        {
            if(status == StatusCh_SteelUpdateData)
            {
                state = STEEL_MEASURE;
                emit signalMeasure(num);
                emit signalDevicesPause(true);
                emit signalSteelFrame(false);
                vectorEds = QVector<double>(SIZE_ARRAY, NAN);
                vectorTemp = QVector<double>(SIZE_ARRAY, NAN);
                readArrays();
            }
        }
        else if(state == STEEL_MEASURE)
        {
            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + "Data");
            emit sendTransToWorker(tr);

            if((status == StatusCh_SteelSquaresOK) ||\
                    (status == StatusCh_SteelNotFoundSquareTemp) || \
                    (status == StatusCh_SteelNotFoundSquareEds) || \
                    (status == StatusCh_SteelNotFoundSquares))
            {
                state = STEEL_READY;
                emit signalReady(num);
                verifStatus = StatusCh_SteelUpdateData;
                QList<QString> str;
                str /*<< "Data" */<< "OxActivity" << "MassAl" << "MassAl" << "PrimaryActivity" << "Carbon";
                if(technology->COH != 0)
                {
                    foreach (QString s, str) {
                        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + s);
                        emit sendTransToWorker(tr);
                    }
                }
                else
                {
//                    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + "Data");
//                    emit sendTransToWorker(tr);
                    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + "Carbon");
                    emit sendTransToWorker(tr);
                }
                vectorTempReceived = false;
                vectorEdsReceived = false;
                allVectorsReceived = false;
                fConfirm = false;
                alg = 0x7FFF;
                ao = 0x7FFF;
                temp = NAN;
                eds = NAN;
                cl = NAN;
                emit signalDevicesPause(false);
                timeUpdateData = QDateTime::currentDateTime();
                readArrays();

            }
            else if((status == StatusCh_SteelWaitData)\
                    || (status == StatusCh_WaitConf) || (status == StatusCh_EndConfig))
            {
                state = STEEL_WAIT;
                emit signalDevicesPause(false);
            }
            else if((status == StatusCh_SteelErrorTC) || (status == StatusCh_SteelErrorEds))
            {
                state = STEEL_WAIT;
                emit signalDevicesPause(false);
            }
        }
        else if(state == STEEL_READY)
        {

            if(allVectorsReceived)
            {
                //запись в журнал
                emit signalArchive(num);
                allVectorsReceived = false;
                fConfirm = true;
            }
            if(!fConfirm)
            {
                //если ещё весь массив не считали, то нельзя подтверждать статус
                verifStatus = StatusCh_SteelUpdateData;
            }
            if((status == StatusCh_SteelWaitData)\
                    || (status == StatusCh_SteelErrorTC)\
                    || (status == StatusCh_SteelErrorEds)\
                    || (status == StatusCh_EndConfig))
            {
                //            steelReady = false;
                //            steelMeasure = false;
                state = STEEL_WAIT;
                emit signalDevicesPause(false);
            }
        }
        if(status != StatusCh_Off)
        {
            tr = Transaction(Transaction::W, slot);
            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + "VerificationRead");
            tr.volInt = verifStatus;
            emit sendTransToWorker(tr);
        }
    }
}


void cSteel::updateParam()
{
    Transaction tr(Transaction::R, slot);

    if(enable)
    {
        if((state == STEEL_WAIT) \
                && ((status == StatusCh_SteelWaitData) || (status == StatusCh_EndConfig)))
        {
            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) + listUpdateParam.at(countParam++));
            emit sendTransToWorker(tr);
            if(countParam >= listUpdateParam.size()) countParam = 0;
        }
    }
}

int cSteel::getIndexArray()
{
    for(int i = 1; i <= (SIZE_ARRAY / SIZE_ONE_ARRAY); i++)
    {
        //проверка на NaN
        if((vectorTemp.at(i * SIZE_ONE_ARRAY - 1)) != (vectorTemp.at(i * SIZE_ONE_ARRAY - 1)))
        {
            return (i - 1);
        }
    }
}

void cSteel::readArrays()
{
    Transaction tr(Transaction::R, slot);
    //запрос текущего участка графика температуры
    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) \
                                               + "TEMParray" + QString::number(getIndexArray()));
    emit sendTransToWorker(tr);

    //запрос текущего участка графика ЭДС, если задан тип первичного преобразователя окисленности
    if(technology->COH != 0) {
        //            tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(numArraySteel));
        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(slotIndex) \
                                                   + "EMFarray" + QString::number(getIndexArray()));
        emit sendTransToWorker(tr);
    }
}

bool cSteel::isBlackListParam(QString param)
{
    foreach(QString str, blackListParam)
    {
        if(str == param) return true;
    }

    return false;
}

void cSteel::addBadParam(QString param)
{
    int count = 0;
    listBadParam << param;
    foreach (QString str, listBadParam) {
        if(str == param){
            count++;
        }
    }
    if(count >= MAX_COUNT_BAD_PARAM)
    {
        blackListParam << param;
    }
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
