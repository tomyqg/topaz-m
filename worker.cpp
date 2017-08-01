#include "worker.h"
#include <QDebug>

worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{}

void worker::do_Work()
{
    emit SignalToObj_mainThreadGUI();


    if ( !isrunning || isstopped )
    {
        qDebug()<< "Stop_Work";
        return;
    }

    if ( isrunning || !isstopped )
        qDebug()<< "Start_Work";

    // do important work here

    // allow the thread's event loop to process other events before doing more "work"
    // for instance, your start/stop signals from the MainWindow
    QMetaObject::invokeMethod( this, "do_Work", Qt::QueuedConnection );
}

void worker::StopWork()
{
    isstopped = true;
    isrunning = false;
    emit stopped();
}

void worker::StartWork()
{
    isstopped = false;
    isrunning = true;
    emit running();
    do_Work();
}
