#include "worker.h"
#include <QDebug>

worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{}

ModBus MB;

void worker::do_Work()
{
    emit SignalToObj_mainThreadGUI();


    if ( !isrunning || isstopped )
    {
//        qDebug()<< "Stop_Work";
//        emit finished();
        return;
    }

    if ( isrunning || !isstopped )

    {
//        qDebug()<< "Start_Work";

        UartDriver UD;

        double currentdata;
        this->thread()->setPriority(QThread::LowPriority);

//        this->thread()->sleep(100);


//        UD.writechannelvalue(1,321);

       // while (1)
        {
            /*while (
                   (UartDriver::needtoupdatechannel[0] == 0)&&
                   (UartDriver::needtoupdatechannel[1] == 0)&&
                   (UartDriver::needtoupdatechannel[2] == 0)&&
                   (UartDriver::needtoupdatechannel[3] == 0))
            {
                this->thread()->msleep(20);
            }*/

            if (UartDriver::needtoupdatechannel[0] == 1)
            {
                UartDriver::needtoupdatechannel[0] = 0;
                //  currentdata = ClickRelay(ModBus::Board4AIAddress);

                currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB1Address);

                if ( (currentdata!=-9999)&&(currentdata!=-9998) )
                {
                    UD.writechannelvalue(1,currentdata);
                }

            }

            currentdata=0;

            if (UartDriver::needtoupdatechannel[1] == 1)
            {
                UartDriver::needtoupdatechannel[1] = 0;
                currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB2Address);
                if ( (currentdata!=-9999)&&(currentdata!=-9998) )
                {
                    UD.writechannelvalue(2,currentdata);
                }

            }

            currentdata=0;

            if (UartDriver::needtoupdatechannel[2] == 1)
            {
                UartDriver::needtoupdatechannel[2] = 0;
                currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB3Address);
                if ( (currentdata!=-9999)&&(currentdata!=-9998) )
                {
                    UD.writechannelvalue(3,currentdata);
                }

            }
            currentdata=0;

            if (UartDriver::needtoupdatechannel[3] == 1)
            {
                UartDriver::needtoupdatechannel[3] = 0;
                currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB4Address);
                if ( (currentdata!=-9999)&&(currentdata!=-9998) )
                {
                    UD.writechannelvalue(4,currentdata);
                }

            }
            currentdata=0;
        }
//        MB.deleteLater();
    }

    // do important work here

    // allow the thread's event loop to process other events before doing more "work"
    // for instance, your start/stop signals from the MainWindow
    QMetaObject::invokeMethod( this, "do_Work", Qt::QueuedConnection );
}

void worker::StopWorkSlot()
{
    isstopped = true;
    isrunning = false;
    emit stopped();
}

void worker::StartWorkSlot()
{
    isstopped = false;
    isrunning = true;
    emit running();
    do_Work();
}

void worker::GetObectsSlot(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4)
{
//    qDebug() << c1->GetChannelName();
//    qDebug() << c2->GetChannelName();
//    qDebug() << c3->GetChannelName();
//    qDebug() << c4->GetChannelName();
}
