#include "ustavka.h"
#include <QMessageBox>
#include "kvitirovanie.h"

Ustavka::Ustavka(QWidget *parent)
{
    numChannel = 0;
    stateHiValue = 0;
    stateLowValue = 0;
    hiHisteresis = 0;
    lowHisteresis = 0;
    numRelayUp = 0;
    numRelayDown = 0;
    setUp = false;
    setDown = false;
    kvitirEnUp = false;
    kvitirEnDown = false;
    waitKvitirUp = false;
    waitKvitirDown = false;
    timeFilter = new QTimer();
    timeFilter->stop();
    connect(timeFilter, SIGNAL(timeout()), this, SLOT(timeoutToWorkRelay()));
}

Ustavka::~Ustavka()
{

}


void Ustavka::setUstavka(int ch, double hi, double low, double hiHist, double lowHist, int relayUp, int relayDown)
{
    numChannel = ch;
    stateLowValue = low;
    stateHiValue = hi;
    lowHisteresis = lowHist;
    hiHisteresis = hiHist;
    numRelayUp = relayUp;
    numRelayDown = relayDown;
    setUp = false;
    setDown = false;
    timeFilter->stop();
}

int Ustavka::getUstavka()
{
    if(setUp)
    {
        return numRelayUp;
    }
    else if(setDown)
    {
        return numRelayDown;
    }
    else
    {
        return 0;
    }
}

void Ustavka::update(double cur)
{
    // если контроллируемый канал не установлен, то ничего не делать
    if(!numChannel) return;

    // если уровни заданы неверно, то тоже ничего не делать
    if((stateHiValue - hiHisteresis) <= (stateLowValue + lowHisteresis)) return;

    // запомнить текущее состояние
    bool up = setUp;
    bool down = setDown;

    // проверка превышения верхнего уровня
    if(cur > (stateHiValue + hiHisteresis)) setUp = true;
    else if(cur < (stateHiValue - hiHisteresis)) setUp = false;

    // проверка принижения нижнего уровня
    if(cur > (stateLowValue + lowHisteresis)) setDown = false;
    else if(cur < (stateLowValue - lowHisteresis)) setDown = true;

    // изменилось ли состояние
    if((setUp != up) || (setDown != down))
    {
        timeFilter->start(DELAY_RELAY);
        // запись события в лог
        if(setUp != up)
            emit messToLogSignal(numChannel, (setUp ? stateInHighMess : stateNormHighMess));
        if(setDown != down)
            emit messToLogSignal(numChannel, (setDown ? stateInLowMess : stateNormLowMess));
    }
}

void Ustavka::timeoutToWorkRelay()
{
    timeFilter->stop();
    // формирование сигнала на реле
    if(numRelayUp != 0) emit workReleSignal(numRelayUp, setUp);
    if(numRelayDown != 0) emit workReleSignal(numRelayDown, setDown);
//    QMessageBox kw(QMessageBox::Warning,\
//                   "Квитирование",\
//                   "Сработала уставка. Требуется подтверждение",\
//                   QMessageBox::Ok);
//    kw.exec();
    kvitirovanie kv;
    if(setUp && kvitirEnUp && !waitKvitirUp)
    {
        kv.showInfo(stateInHighMess, nameChannel);
        waitKvitirUp = true;
        kv.exec();
        emit messToLogSignal(numChannel, "(ОК)" + stateInHighMess);
        waitKvitirUp = false;
    }
    else if(setDown && kvitirEnDown && !waitKvitirDown)
    {
        kv.showInfo(stateInLowMess, nameChannel);
        waitKvitirDown = true;
        kv.exec();
        emit messToLogSignal(numChannel, "(ОК)" + stateInLowMess);
        waitKvitirDown = false;
    }

}
