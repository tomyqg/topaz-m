#include "ustavka.h"
#include <QMessageBox>
#include "kvitirovanie.h"
#include "log.h"
#include "defines.h"

Ustavka::Ustavka(QWidget *parent)
{
    numChannel = 0;
    stateHiValue = 0;
//    stateLowValue = 0;
    hiHisteresis = 0;
//    lowHisteresis = 0;
    numRelayUp = 0;
//    numRelayDown = 0;
    setUp = false;
//    setDown = false;
    kvitirEnUp = false;
//    kvitirEnDown = false;
    waitKvitirUp = false;
//    waitKvitirDown = false;
    timeFilter = new QTimer();
    timeFilter->stop();
    connect(timeFilter, SIGNAL(timeout()), this, SLOT(timeoutToWorkRelay()));

}

Ustavka::~Ustavka()
{
    delete timeFilter;
}


void Ustavka::setUstavka(
        QString name,
        int ch,
        bool type,
        double hi,
//        double low,
        double hiHist,
//        double lowHist,
        int relayUp
//        int relayDown
        )
{
    identifikator = name;
    numChannel = ch;
    typeFix = type;
//    stateLowValue = low;
    stateHiValue = hi;
//    lowHisteresis = lowHist;
    hiHisteresis = hiHist;
    numRelayUp = relayUp;
//    numRelayDown = relayDown;
    setUp = false;
//    setDown = false;
    timeFilter->stop();
}

//int Ustavka::getUstavka()
//{
//    if(setUp)
//    {
//        return numRelayUp;
//    }
//    else if(setDown)
//    {
//        return numRelayDown;
//    }
//    else
//    {
//        return 0;
//    }
//}

void Ustavka::update(double cur)
{
    // если контроллируемый канал не установлен, то ничего не делать
    if(!numChannel) return;

    // если уровни заданы неверно, то тоже ничего не делать
//    if((stateHiValue - hiHisteresis) <= (stateLowValue + lowHisteresis)) return;

    // запомнить текущее состояние
    bool up = setUp;
//    bool down = setDown;

    // проверка превышения верхнего уровня
    if(cur > (stateHiValue + hiHisteresis)) setUp = true;
    else if(cur < (stateHiValue - hiHisteresis)) setUp = false;

    // проверка принижения нижнего уровня
//    if(cur > (stateLowValue + lowHisteresis)) setDown = false;
//    else if(cur < (stateLowValue - lowHisteresis)) setDown = true;

    // изменилось ли состояние
    if((setUp != up)/* || (setDown != down)*/)
    {
        timeFilter->start(DELAY_RELAY);
        // запись события в лог
        QString mess = (setUp == typeFix) ? stateInHighMess : stateNormHighMess;
        cLogger log(pathtomessages, cLogger::USTAVKA);
        log.addMess(identifikator + " | " + mess + " | " + QString::number(cur, 'f', 2), \
                    cLogger::LIMIT);

        //        if(setDown != down)
        //            emit messToLogSignal(numChannel-1, (setDown ? stateInLowMess : stateNormLowMess));
    }
}

void Ustavka::timeoutToWorkRelay()
{
    timeFilter->stop();
    // формирование сигнала на реле
    if(numRelayUp != 0) emit workReleSignal(numRelayUp, setUp);
//    if(numRelayDown != 0) emit workReleSignal(numRelayDown, setDown);
//    QMessageBox kw(QMessageBox::Warning,\
//                   "Квитирование",\
//                   "Сработала уставка. Требуется подтверждение",\
//                   QMessageBox::Ok);
//    kw.exec();
    kvitirovanie kv;
    if((setUp == typeFix) && kvitirEnUp && !waitKvitirUp)
    {
        kv.showInfo(stateInHighMess, identifikator);
        waitKvitirUp = true;
        kv.exec();
//        emit messToLogSignal(numChannel, "(ОК)" + stateInHighMess);
        cLogger log(pathtomessages, cLogger::USTAVKA);
        log.addMess(identifikator + " confirmed", cLogger::LIMIT);
        waitKvitirUp = false;
    }
//    else if(setDown && kvitirEnDown && !waitKvitirDown)
//    {
//        kv.showInfo(stateInLowMess, identifikator);
//        waitKvitirDown = true;
//        kv.exec();
//        emit messToLogSignal(numChannel, "(ОК)" + stateInLowMess);
//        waitKvitirDown = false;
//    }

}
