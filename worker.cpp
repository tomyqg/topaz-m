#include "worker.h"
#include "defines.h"
#include "uartdriver.h"
//#include <QDebug>

worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{}

ModBus MB;
UartDriver UD;
mathresolver mr;

ModbusDeviceStruct modbusdevice;
QList<ModbusDeviceStruct> ModbusDevicesList;

void worker::do_Work()
{
    double mathresult;
    double currentdata;

    emit SignalToObj_mainThreadGUI();

    if ( !isrunning || isstopped ) // если воркер остановлен
    {
        this->thread()->usleep(100); // 100 мксек ждем прост. чтобы проц не перегружался и не перегревался
    }

    if ( isrunning || !isstopped ) // если воркер запущен
    {
        this->thread()->usleep(100); // 100 мксек ждем прост.
        this->thread()->setPriority(QThread::LowPriority);
        if (ThreadChannelOptions1->GetSignalType() != ModBus::MeasureOff)
            if (UartDriver::needtoupdatechannel[0] == 1)
            {
                this->thread()->usleep(100); // 100 мксек ждем прост.
                UartDriver::needtoupdatechannel[0] = 0;
                currentdata = MB.ReadDataChannel(ModBus::DataChannel1);

                MB.WriteDataChannel(ModBus::DataChannel2, currentdata );


                if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
                {
                    if (ThreadChannelOptions1->IsChannelMathematical())
                    {
                        mathresult = mr.SolveEquation(ThreadChannelOptions1->GetMathString(),currentdata);
                        currentdata = mathresult;
                    }
                    UD.writechannelvalue(1,currentdata);
                }
                this->thread()->usleep(1000); // 1000 мксек ждем прост.
            }

        if (ThreadChannelOptions2->GetSignalType() != ModBus::MeasureOff) // если не нужно мерить то пропускаем измерения
            if (UartDriver::needtoupdatechannel[1] == 1)
            {
                this->thread()->usleep(100); // 100 мксек ждем прост.
                UartDriver::needtoupdatechannel[1] = 0;

                currentdata = MB.ReadDataChannel(ModBus::DataChannel2);
                if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
                {
                    if (ThreadChannelOptions2->IsChannelMathematical())
                    {
                        mathresult = mr.SolveEquation(ThreadChannelOptions2->GetMathString(),currentdata);
                        currentdata = mathresult;
                    }
                    UD.writechannelvalue(2,currentdata);
                }
                this->thread()->usleep(1000); // 1000 мксек ждем прост.
            }

        if (ThreadChannelOptions3->GetSignalType() != ModBus::MeasureOff) // если не нужно мерить то пропускаем измерения
            if (UartDriver::needtoupdatechannel[2] == 1)
            {
                this->thread()->usleep(100); // 100 мксек ждем прост.
                UartDriver::needtoupdatechannel[2] = 0;
                currentdata = MB.ReadDataChannel(ModBus::DataChannel3);
                if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
                {
                    if (ThreadChannelOptions3->IsChannelMathematical())
                    {
                        mathresult = mr.SolveEquation(ThreadChannelOptions3->GetMathString(),currentdata);
                        currentdata = mathresult;
                    }
                    UD.writechannelvalue(3,currentdata);
                }
                this->thread()->usleep(1000); // 1000 мксек ждем прост.
            }

        if (ThreadChannelOptions4->GetSignalType() != ModBus::MeasureOff) // если не нужно мерить то пропускаем измерения
            if (UartDriver::needtoupdatechannel[3] == 1)
            {
                this->thread()->usleep(100); // 100 мксек ждем прост.
                UartDriver::needtoupdatechannel[3] = 0;
                currentdata = MB.ReadDataChannel(ModBus::DataChannel4);
                if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
                {
                    if (ThreadChannelOptions4->IsChannelMathematical())
                    {
                        mathresult = mr.SolveEquation(ThreadChannelOptions4->GetMathString(),currentdata);
                        currentdata = mathresult;
                    }
                    UD.writechannelvalue(4,currentdata);
                }
                this->thread()->usleep(1000); // 1000 мксек ждем прост.
            }

        if (UartDriver::needtoupdatechannel[0] == 0)
            if (UartDriver::needtoupdatechannel[1] == 0)
                if (UartDriver::needtoupdatechannel[2] == 0)
                    if (UartDriver::needtoupdatechannel[3] == 0)
                    {
                        this->thread()->usleep(1000);
                    }
    }

    emit Finished(); // вызываем сигнал что обработка канала завершилась. ждем следующего запуска канала
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
    this->thread()->usleep(50000);
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
    thread()->usleep(100000);

    ThreadChannelOptions1 = c1;
    ThreadChannelOptions2 = c2;
    ThreadChannelOptions3 = c3;
    ThreadChannelOptions4 = c4;

    uint16_t type;

    //устанавливаем новый тип сигнала для каждого типа сигнала (посылаем соотв-ю ком-ду по модбас)
    type = ThreadChannelOptions1->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel1, type);
    switch (type) {
    case ModBus::CurrentMeasure:
        break;
    case ModBus::VoltageMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Voltage100mVoltNoBreakControl);
        break;
    case ModBus::ResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Wire3NoBreakControl);
        break;
    case ModBus::TermoCoupleMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::R);
        break;
    case ModBus::TermoResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Wire3NoBreakControl);
        break;
    default:
        break;
    }

    type = ThreadChannelOptions2->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel2, type);
    switch (type) {
    case ModBus::CurrentMeasure:
        break;
    case ModBus::VoltageMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel2, ModBus::Voltage100mVoltNoBreakControl);
        break;
    case ModBus::ResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel2, ModBus::Wire3NoBreakControl);
        break;
    case ModBus::TermoCoupleMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel2, ModBus::R);
        break;
    case ModBus::TermoResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel2, ModBus::Wire3NoBreakControl);
        break;
    default:
        break;
    }

    type = ThreadChannelOptions3->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel3, type);
    switch (type) {
    case ModBus::CurrentMeasure:
        break;
    case ModBus::VoltageMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel3, ModBus::Voltage100mVoltNoBreakControl);
        break;
    case ModBus::ResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel3, ModBus::Wire3NoBreakControl);
        break;
    case ModBus::TermoCoupleMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel3, ModBus::R);
        break;
    case ModBus::TermoResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel3, ModBus::Wire3NoBreakControl);
        break;
    default:
        break;
    }

    type = ThreadChannelOptions4->GetSignalType();
    MB.SetChannelSignalType(ModBus::DataChannel4, type);
    switch (type) {
    case ModBus::CurrentMeasure:
        break;
    case ModBus::VoltageMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel4, ModBus::Voltage100mVoltNoBreakControl);
        break;
    case ModBus::ResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel4, ModBus::Wire4NoBreakControl);
        break;
    case ModBus::TermoCoupleMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel4, ModBus::R);
        break;
    case ModBus::TermoResistanceMeasure:
        MB.SetChannelAdditionalParametr(ModBus::DataChannel4, ModBus::Wire4NoBreakControl);
        break;
    default:
        break;
    }

    //делаем список объектов каналов, чтобы в цикле можно было их обрабатывать
    QList<ChannelOptions *> ChannelsObjectsList;
    ChannelsObjectsList.append(c1);
    ChannelsObjectsList.append(c2);
    ChannelsObjectsList.append(c3);
    ChannelsObjectsList.append(c4);
    //qDebug() << ChannelsObjectsList.at(1)->GetChannelName();
    ModbusDevicesList.clear();

    // Переносим список этих объектов в список структур
    int i = 0;
    foreach(ChannelOptions * cobj, ChannelsObjectsList)
    {
        modbusdevice.address = ModBus::Board4AIAddress; // пока только 1 адрес
        modbusdevice.Status = ModBus::StatusOn; // канал включен
        modbusdevice.DeviceType = ModBus::DeviceType4AI;
        modbusdevice.SignalType = cobj->GetSignalType(); // какой тип сигнала

        switch (modbusdevice.SignalType ) {
        case ModBus::CurrentMeasure:
            modbusdevice.Measuretype  = 0; // у тока всегда ноль
            break;
        case ModBus::VoltageMeasure:
            modbusdevice.Measuretype  = ModBus::Voltage1VoltNoBreakControl; // для напряжения оставим пока 1 вольт
            break;
        case ModBus::ResistanceMeasure:
            modbusdevice.Measuretype  = ModBus::Wire3NoBreakControl; // для ТС ставим 3-х проводку пока.
            break;
        case ModBus::TermoCoupleMeasure:
            modbusdevice.Measuretype  = ModBus::R; // для термопары ставим тип ТПП 13
            break;
        case ModBus::TermoResistanceMeasure:
            modbusdevice.Measuretype  =  ModBus::Wire3NoBreakControl; // для ТС ставим 3-х проводку
            break;
        default:
            modbusdevice.Measuretype  = 0; // по умолчанию ставим ноль.
            break;
        }

        modbusdevice.name = cobj->GetChannelName();
        modbusdevice.ID = i;
        modbusdevice.SupportedSignals = ModBus::SupportedSignalCurrent | ModBus::SupportedSignalVoltage | ModBus::SupportedSignalTermoCouple| ModBus::SupportedSignalTermoResistance;
        ModbusDevicesList.append(modbusdevice);
        qDebug() << i << ":" << modbusdevice.Measuretype;
        i++;
    }

    //список структур девайсов готов.

    //проверяем
    //qDebug() << ModbusDevicesList.at(0).ID << ":" << ModbusDevicesList.at(0).name;
    //qDebug() << ModbusDevicesList.at(1).ID << ":" << ModbusDevicesList.at(1).name;
    //qDebug() << ModbusDevicesList.at(2).ID << ":" << ModbusDevicesList.at(2).name;
    //qDebug() << ModbusDevicesList.at(3).ID << ":" << ModbusDevicesList.at(3).name;

    thread()->usleep(10000);
}

