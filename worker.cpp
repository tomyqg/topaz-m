#include "worker.h"
#include "defines.h"
#include <QDebug>

worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{}

ModBus MB;
UartDriver UD;
mathresolver mr;

void worker::do_Work()
{
    emit SignalToObj_mainThreadGUI();


    if ( !isrunning || isstopped ) // если воркер остановлен
    {
        this->thread()->usleep(100); // 100 мксек ждем прост.
        return;
    }
    if ( isrunning || !isstopped ) // если воркер запущен
    {
        this->thread()->usleep(100); // 100 мксек ждем прост.

        double currentdata;
        this->thread()->setPriority(QThread::LowPriority);

        if (UartDriver::needtoupdatechannel[0] == 1)
        {
            this->thread()->usleep(100); // 100 мксек ждем прост.
            UartDriver::needtoupdatechannel[0] = 0;
            //  currentdata = ClickRelay(ModBus::Board4AIAddress);

            currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB1Address);

            if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
            {
                if (ThreadChannelOptions1->IsChannelMathematical())
                {

                    double mathresult = mr.SolveEquation(ThreadChannelOptions1->GetMathString(),currentdata);
                    currentdata = mathresult;
                }
                UD.writechannelvalue(1,currentdata);
            }
            // this->thread()->usleep(25000);
        }


        if (UartDriver::needtoupdatechannel[1] == 1)
        {
            this->thread()->usleep(100); // 100 мксек ждем прост.
            UartDriver::needtoupdatechannel[1] = 0;
            currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB2Address);
            if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
            {
                if (ThreadChannelOptions2->IsChannelMathematical())
                {

                    double mathresult = mr.SolveEquation(ThreadChannelOptions2->GetMathString(),currentdata);
                    currentdata = mathresult;
                }
                UD.writechannelvalue(2,currentdata);
            }
            // this->thread()->usleep(25000);
        }

        if (UartDriver::needtoupdatechannel[2] == 1)
        {
            this->thread()->usleep(100); // 100 мксек ждем прост.
            UartDriver::needtoupdatechannel[2] = 0;
            currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB3Address);
            if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
            {
                if (ThreadChannelOptions3->IsChannelMathematical())
                {

                    double mathresult = mr.SolveEquation(ThreadChannelOptions3->GetMathString(),currentdata);
                    currentdata = mathresult;
                }
                UD.writechannelvalue(3,currentdata);
            }
        }

        if (UartDriver::needtoupdatechannel[3] == 1)
        {
            this->thread()->usleep(100); // 100 мксек ждем прост.
            UartDriver::needtoupdatechannel[3] = 0;
            currentdata = MB.ReadDataChannel(ModBus::ElmetroChannelAB4Address);
            if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
            {
                if (ThreadChannelOptions4->IsChannelMathematical())
                {

                    double mathresult = mr.SolveEquation(ThreadChannelOptions4->GetMathString(),currentdata);
                    currentdata = mathresult;
                }
                UD.writechannelvalue(4,currentdata);
            }
            // this->thread()->usleep(25000);
        }

        if (UartDriver::needtoupdatechannel[0] == 0)
            if (UartDriver::needtoupdatechannel[1] == 0)
                if (UartDriver::needtoupdatechannel[2] == 0)
                    if (UartDriver::needtoupdatechannel[3] == 0)
                    {
                        this->thread()->usleep(1000);
                    }
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
    thread()->sleep(100000);

    ThreadChannelOptions1 = c1;
    ThreadChannelOptions2 = c2;
    ThreadChannelOptions3 = c3;
    ThreadChannelOptions4 = c4;

    uint16_t type;

    //устанавливаем новый тип сигнала для каждого типа сигнала (посылаем соотв-ю ком-ду по модбас)
    type = ThreadChannelOptions1->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel1, type);
    type = ThreadChannelOptions2->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel2, type);
    type = ThreadChannelOptions3->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel3, type);
    type = ThreadChannelOptions4->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel4, type);


    MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Voltage1VoltNoBreakControl);

    thread()->sleep(100000);
}
