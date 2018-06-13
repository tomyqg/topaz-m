#include "alarm.h"


cAlarm::cAlarm(int ch, QObject *parent) : QObject(parent),
    channel(ch) //обязательно сообщить номер подключаемого канала
{
    reg = ControlUp;
    name = "Channel " + QString::number(ch+1);
    delay = 100;    //100мс по-умолчанию
    relay = -1;     //реле отключены вначале
    mesOn = name + " is over";
    mesOn = name + " is normal";
    connect(&timer, SIGNAL(timeout()), this, SLOT(setAlarm()));
}

void cAlarm::setAlarm()
{
    //послать сигнал о включении реле
    emit alarm(relay, true, mesOn);
    if(fKvitir)
    {
        kvitDialog.showInfo(mesOn, name);
        kvitDialog.show();
    }

}
