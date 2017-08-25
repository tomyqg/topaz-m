#include "worker.h"
#include "defines.h"
#include "uartdriver.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"


// constructor
worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{


}

ModBus MB;
UartDriver UD;
mathresolver mr;

ModbusDeviceStruct modbusdevice;
QList<ModbusDeviceStruct> ModbusDevicesList;


static QString descriptiveDataTypeName( int funcCode )
{
    switch( funcCode )
    {
    case _FC_READ_COILS:
    case _FC_WRITE_SINGLE_COIL:
    case _FC_WRITE_MULTIPLE_COILS:
        return "Coil (binary)";
    case _FC_READ_DISCRETE_INPUTS:
        return "Discrete Input (binary)";
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_WRITE_SINGLE_REGISTER:
    case _FC_WRITE_MULTIPLE_REGISTERS:
        return "Holding Register (16 bit)";
    case _FC_READ_INPUT_REGISTERS:
        return "Input Register (16 bit)";
    default:
        break;
    }
    return "Unknown";
}

void worker::sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, float *data_dest_float)
{
    if( m_modbus == NULL )
    {
        return;
    }

    uint8_t dest[1024];
    uint16_t * dest16 = (uint16_t *) dest;

    memset( dest, 0, 1024 );

    int ret = -1;
    bool is16Bit = false;
    bool writeAccess = false;
    const QString dataType = descriptiveDataTypeName( func );

    modbus_set_slave( m_modbus, slave );

    switch( func )
    {
    case _FC_READ_COILS:
        ret = modbus_read_bits( m_modbus, addr, num, dest );
        break;
    case _FC_READ_DISCRETE_INPUTS:
        ret = modbus_read_input_bits( m_modbus, addr, num, dest );
        break;
    case _FC_READ_HOLDING_REGISTERS:
        ret = modbus_read_registers( m_modbus, addr, num, dest16 );
        is16Bit = true;
        break;
    case _FC_READ_INPUT_REGISTERS:
        ret = modbus_read_input_registers( m_modbus, addr, num, dest16 );
        is16Bit = true;
        break;
    case _FC_WRITE_SINGLE_COIL:
        ret = modbus_write_bit( m_modbus, addr, state);
        writeAccess = true;
        num = 1;
        break;
    case _FC_WRITE_SINGLE_REGISTER:
        ret = modbus_write_register( m_modbus, addr, state);
        writeAccess = true;
        num = 1;
        break;

    case _FC_WRITE_MULTIPLE_COILS:
    {
        uint8_t * data = new uint8_t[num];
        for( int i = 0; i < num; ++i )
        {
            data[i] = ( uint8_t ) data_src[i];
        }
        ret = modbus_write_bits( m_modbus, addr, num, data );
        delete[] data;
        writeAccess = true;
        break;
    }
    case _FC_WRITE_MULTIPLE_REGISTERS:
    {
        uint16_t * data = new uint16_t[num];
        for( int i = 0; i < num; ++i )
        {
            data[i] = ( uint16_t ) data_src[i];
        }
        ret = modbus_write_registers( m_modbus, addr, num, data );
        delete[] data;
        writeAccess = true;
        break;
    }
    default:
        break;
    }

    if( ret == num  )
    {
        if( writeAccess )
        {
            //qDebug() << "Values successfully sent" ;
            QTimer::singleShot( 2000, this, SLOT( resetStatus() ) );
        }
        else
        {
            // перешли сюда значит нужно преобразовать считанные значения из массива HEX во float
            QByteArray arraytofloat;

            for( int i = num-1; i >=0; --i )
            {
                //qDebug() << num<< "num" ;
                int data = is16Bit ? dest16[i] : dest[i];
                arraytofloat.append((data & 0xFF00)>>8);
                arraytofloat.append(data & 0x00FF);
                //data_dest_float[num - 1 - i] = data;
            }

            float val;
            //convert hex to double
            QDataStream stream(arraytofloat);
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
            stream >> val;
            data_dest_float[0] = val;
        }
    }
    else
    {
        if( ret < 0 )
        {
            if(
        #ifdef WIN32
                    errno == WSAETIMEDOUT ||
        #endif
                    errno == EIO
                    )
            {
                qDebug() << "I/O error"  << "I/O error: did not receive any data from slave" ;
            }
            else
            {
                qDebug() << "Protocol error"  << "Slave threw exception \"%1\" or function not implemented. " ;
            }
        }
        else
        {
            qDebug() << "Protocol error"  << "Number of registers returned does not match number of registers requested! " ;
        }
    }
}



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

                //UartDriver::needtoupdatechannel[0] = 0;
                currentdata = MB.ReadDataChannel(ModBus::DataChannel1);

                float destfloat[1024];
                memset( destfloat, 0, 1024 );

                // делаем запросики
                sendModbusRequest(ModBus::Board4AIAddress, ModBus::ReadInputRegisters, ModBus::ElmetroChannelAB1Address, 2, 0, 0, destfloat);

                qDebug() << destfloat[0] << "destfloat[0]";

                currentdata = destfloat[0];

                this->thread()->usleep(5000);

                if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
                {
                    //пишем по адресу 32816 (User calibration 2, gain) отрицательное значение
                    //MB.WriteDataChannel(ModBus::BadGoodCommAddress, currentdata*(-1));
                    //this->thread()->usleep(500*1000);
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
                //                currentdata = MB.ReadDataChannel(ModBus::DataChannel2);
                //                currentdata = MB.ModBusGetHoldingRegister(ModBus::Board4AIAddress,ModBus::BadGoodCommAddress,ModBus::DataChannelLenght); //ModBus::ElmetroChannelAB2Address
                if ( (currentdata!=BADCRCCODE)&&(currentdata!=CONNECTERRORCODE) )
                {
                    if (ThreadChannelOptions2->IsChannelMathematical())
                    {
                        // читаем по адресу 32816 (User calibration 2, gain) отрицательное значение
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
                //                currentdata = MB.ReadDataChannel(ModBus::DataChannel3);
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
                //                currentdata = MB.ReadDataChannel(ModBus::DataChannel4);
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




void worker::changeSerialPort( int )
{
    //    qDebug() << "changeSerialPort ( int )" ;

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {

#ifdef Q_OS_WIN32
        const QString port = comportname;
#else
        // const QString port = ports[iface].physName;
        // точно знаем что этот порт работает на rs485
        const QString port = "/dev/ttyO1";
#endif

        char parity;

        parity = comportparity;

        if( m_modbus )
        {
            modbus_close( m_modbus );
            modbus_free( m_modbus );
        }

        m_modbus = modbus_new_rtu( comportname,comportbaud,comportparity,comportdatabit,comportstopbit);

        if( modbus_connect( m_modbus ) == -1 )
        {
            qDebug() << "Connection failed"  << "Could not connect serial port!" ;
        }
        else
        {
        }
    }
    else
    {
        qDebug() << "No serial port found" << "Could not find any serial port " << "on this computer!"  ;
    }
}




void worker::GetObectsSlot(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4)
{
    //qDebug() << "Worker Constructor" ;
    // находим все com - порты

    int portIndex = 0;
    int i = 0;
    QSettings s;
    foreach( QextPortInfo port, QextSerialEnumerator::getPorts() )
    {
        //        qDebug() << port.portName;
        if( port.friendName == s.value( "serialinterface" ) )
        {
            portIndex = i;
        }
        ++i;
    }

    // активируем сериал порт для модбаса
    changeSerialPort( portIndex );

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
    i = 0;

    foreach(ChannelOptions * cobj, ChannelsObjectsList)
    {
        modbusdevice.ModbusNetworkAddress = ModBus::Board4AIAddress; // пока только 1 адрес
        modbusdevice.Status = ModBus::StatusOn; // канал включен
        modbusdevice.DeviceType = ModBus::DeviceType4AI;
        modbusdevice.SignalType = cobj->GetSignalType(); // какой тип сигнала

        switch (modbusdevice.SignalType ) {
        case ModBus::CurrentMeasure:
            modbusdevice.MeasureType  = 0; // у тока всегда ноль
            break;
        case ModBus::VoltageMeasure:
            modbusdevice.MeasureType  = ModBus::Voltage1VoltNoBreakControl; // для напряжения оставим пока 1 вольт
            break;
        case ModBus::ResistanceMeasure:
            modbusdevice.MeasureType  = ModBus::Wire3NoBreakControl; // для ТС ставим 3-х проводку пока.
            break;
        case ModBus::TermoCoupleMeasure:
            modbusdevice.MeasureType  = ModBus::R; // для термопары ставим тип ТПП 13
            break;
        case ModBus::TermoResistanceMeasure:
            modbusdevice.MeasureType  =  ModBus::Wire3NoBreakControl; // для ТС ставим 3-х проводку
            break;
        default:
            modbusdevice.MeasureType  = 0; // по умолчанию ставим ноль.
            break;
        }

        modbusdevice.ID = i;
        modbusdevice.name = cobj->GetChannelName();
        modbusdevice.SupportedSignals = ModBus::SupportedSignalCurrent | ModBus::SupportedSignalVoltage | ModBus::SupportedSignalTermoCouple| ModBus::SupportedSignalTermoResistance;
        modbusdevice.UserCalibration1DateAddress = 32814;

        //старший байт: год (в формате YY + 2000), месяц (от 0 до 12), день (от 1 до 31). Младший байт пустой (не используется).
        modbusdevice.UserCalibration1Date = 0x17083100;
        ModbusDevicesList.append(modbusdevice);
        //MB.ConfigureChannel(&modbusdevice);
        i++;
    }

    MB.ConfigureDevices(&ModbusDevicesList);
    thread()->usleep(10000);
}

