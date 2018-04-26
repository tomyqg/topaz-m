#include "communicator.h"
#include "abcc.h"
#include "QDebug"
#include "appl_abcc_handler.h"

#

cCommunicator::cCommunicator(QObject * parent) : QObject(parent)
{
    init();
}

int cCommunicator::init()
{
    ABCC_ErrorCodeType res = ABCC_EC_NO_ERROR;
//    qDebug() << "cCommunicator::init() - 1";
//    ABCC_ErrorCodeType res = ABCC_StartDriver(0);
//    if (res != ABCC_EC_NO_ERROR)
//    {
////        qDebug() << "Error ABCC_StartDriver = " << res;
//        return -1;
//    }
//    qDebug() << "cCommunicator::init() - 2";
    res = ABCC_HwInit();
    if( res != ABCC_EC_NO_ERROR )
    {
//       qDebug() << "Error ABCC_HwInit = " << res;
       return -2;
    }
//    qDebug() << "cCommunicator::init() - OK";
    return 0;
}

void cCommunicator::run()
{
    APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;
    eAbccHandlerStatus = APPL_HandleAbcc();
//    qDebug() << "eAbccHandlerStatus = " << eAbccHandlerStatus;

//    ABCC_ErrorCodeType res = ABCC_RunDriver();
//    if (res != ABCC_EC_NO_ERROR)
//    {
//        qDebug() << "Error ABCC_RunDriver = " << res;
//        return;
//    }
    ABCC_Tick( TIMEOUT_COMMUNICATOR_MS );
}
