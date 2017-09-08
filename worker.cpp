#include "worker.h"
#include "defines.h"
#include "device.h"
#include "uartdriver.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"
#include <QDebug>

// constructor
worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{
    //qDebug() << "Worker Constructor" ;
    // открываем порт

    // активируем сериал порт для модбаса
    OpenSerialPort( 1 );
}

ModBus MB;
UartDriver UD;
mathresolver mr;

int ic ;

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

void worker::sendModbusRequest(const deviceparametrs* dp)
{
    //    qDebug() << dp->name;
    //    sendModbusRequest(ModBus::Board4AIAddress, ModBus::ReadInputRegisters, ModBus::ElmetroChannelAB1Address+4, 2, 0, 0, destfloat);
}

void worker::WriteModbusData(const deviceparametrs* dp, float value)
{
    if ( ( dp->WorkLevelAccess!= Device::W ) && ( dp->WorkLevelAccess!= Device::RW ))
        return;
    int num;
    uint16_t * data;

    switch (dp->ParamType) {
    case Device::A12:
        num = 12;
        break;
    case Device::U16:
        num = 1;
        data = new uint16_t[num];
        data[0] = value;
        sendModbusRequest(ModBus::Board4AIAddress, _FC_WRITE_MULTIPLE_REGISTERS, dp->Offset, num ,0,data,0);
        break;
    case Device::U32:
        num = 2;

    {
        uint32_t a = (uint32_t)(value);
        data = new uint16_t[num];
        data[1] = ( a >> 16 );
        data[0] = ( a & 0xFFFF);
        sendModbusRequest(ModBus::Board4AIAddress, _FC_WRITE_MULTIPLE_REGISTERS, dp->Offset, num ,0,data,0);
    }

        break;
    case Device::F32:
    {
        num = 2;
        QByteArray floatarray(reinterpret_cast<const char*>(&value), sizeof(value));
        data = new uint16_t[num];
        data[0] = ( floatarray.at(1)<<8 ) | (floatarray.at(0)&0xFF) ;
        data[1] = ( floatarray.at(3)<<8 ) | (floatarray.at(2)&0xFF);
        sendModbusRequest(ModBus::Board4AIAddress, _FC_WRITE_MULTIPLE_REGISTERS, dp->Offset, num ,0,data,0);
    }
        break;
    default:
        break;
    }



}

void worker::ReadModbusData(const deviceparametrs* dp, float *data_dest)
{
    // если запрещено чтение, а только запись разрешена, то возвращаем функцию
    if ( ( dp->WorkLevelAccess!= Device::R ) && ( dp->WorkLevelAccess!= Device::RW ))
        return;

    int num;
    int comm;
    int add;

    // количество блоков в зависимости от типа параметра

    switch (dp->ParamType) {
    case Device::A12:
        num = 12;
        break;
    case Device::U16:
        num = 1;
        break;
    case Device::U32:
        num = 2;
        break;
    case Device::F32:
        num = 2;
        break;
    default:
        num = 0; // просто так решил что ноль, мб другое число.
        break;
    }

    switch (dp->RegisterType) {
    case Device::HoldingReg:
        comm = _FC_READ_HOLDING_REGISTERS;
        break;
    case Device::InputReg:
        comm = _FC_READ_INPUT_REGISTERS;
        break;
    default:
        comm = _FC_READ_INPUT_REGISTERS;
        break;
    }

    add = dp->Offset;

    sendModbusRequest(ModBus::Board4AIAddress, comm, add, num, 0, 0,data_dest);


    switch (dp->ParamType) {
    case Device::A12:
        num = 12;
        break;
    case Device::U16:
        num = 1;
        break;
    case Device::U32:
        num = 2;
    {
        qDebug() << data_dest[0] << data_dest[1] << "U32";

        uint32_t a = (uint32_t)(data_dest[0]);
        uint32_t b = (uint32_t)(data_dest[1]);

        uint32_t val;
        val = ( a<<16 ) | ( b ) ;

        QByteArray arraytofloat;

        // в массив раскладываем принятые данные чтобы преобразовать в флоат
        for( int i = 0; i <num; i++ )
        {
            arraytofloat.append( ( ( (int)data_dest[i] ) & 0xFF00) >>8);
            arraytofloat.append( ( (int)data_dest[i] ) & 0x00FF);
        }

        data_dest[ 0 ] = arraytofloat.at( 0 );
        data_dest[ 1 ] = arraytofloat.at( 1 );
        data_dest[ 2 ] = arraytofloat.at( 2 );
        data_dest[ 3 ] = arraytofloat.at( 3 );

        qDebug() << data_dest[ 0 ] <<data_dest[ 1 ] << data_dest[ 2 ] << data_dest[ 3 ] << "U32"; //

        //convert hex to double
        //QDataStream stream(arraytofloat);
        //stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
        //stream >> val;

        data_dest[0] = val;
    }
        break;
    case Device::F32:
        num = 2;

    {
        qDebug() << data_dest[0] << data_dest[1] << "F32"; // пришли два слова, парсим два слова


        QByteArray arraytofloat;

        // в массив раскладываем принятые данные чтобы преобразовать в флоат
        for( int i = 0; i <num; i++ )
        {
            arraytofloat.append( ( ( (int)data_dest[i] ) & 0xFF00) >>8);
            arraytofloat.append( ( (int)data_dest[i] ) & 0x00FF);
        }

        float val;
        //convert hex to double
        QDataStream stream(arraytofloat);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
        stream >> val;
        data_dest[0] = val;
    }
        break;
    default:
        num = 0; // просто так решил что ноль, мб другое число.
        break;
    }
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
            //QTimer::singleShot( 2000, this, SLOT( resetStatus() ) );
        }
        else
        {
            // перешли сюда значит нужно преобразовать считанные значения из массива HEX во float
            //            QByteArray arraytofloat;

            for( int i = num-1; i >=0; --i )
            {
                //qDebug() << num<< "num" ;
                int data = is16Bit ? dest16[i] : dest[i];
                //                arraytofloat.append((data & 0xFF00)>>8);
                //                arraytofloat.append(data & 0x00FF);
                data_dest_float[num - 1 - i] = data;
            }

            //            float val;
            //            //convert hex to double
            //            QDataStream stream(arraytofloat);
            //            stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // convert bytearray to float
            //            stream >> val;
            //            data_dest_float[0] = val;
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
                //                qDebug() << "I/O error"  << "I/O error: did not receive any data from slave" ;
            }
            else
            {

                //qDebug() << "Protocol error"  << "Slave threw exception \"%1\" or function not implemented. " ;
                //qDebug() << modbus_strerror( errno ) ;
                //qDebug() << stderr;
            }
        }
        else
        {
            //            qDebug() << "Protocol error"  << "Number of registers returned does not match number of registers requested! " ;
        }
    }
}


void worker::do_Work()
{
    double mathresult;
    double currentdata;

    float destfloat[1024];
    memset( destfloat, 0, 1024 );

    emit SignalToObj_mainThreadGUI();

    if ( !isrunning || isstopped ) // если воркер остановлен
    {
        this->thread()->usleep(100); // 100 мксек ждем прост. чтобы проц не перегружался и не перегревался
    }

    if ( isrunning || !isstopped ) // если воркер запущен
    {
        this->thread()->setPriority(QThread::LowPriority);

        if (ThreadChannelOptions1->GetSignalType() != ModBus::MeasureOff)
            if (UartDriver::needtoupdatechannel[0] == 1)
            {

                UartDriver::needtoupdatechannel[0] = 0;
                this->thread()->usleep(100); // 100 мксек ждем прост.

                ReadModbusData(&device.channel0.Data,destfloat );
                currentdata = destfloat[0];

                //WriteModbusData(&device.badgoodcomm, currentdata*-1);
                WriteModbusData(&device.channel0.FilterType, 2) ;

                if (ThreadChannelOptions1->IsChannelMathematical())
                {
                    mathresult = mr.SolveEquation(ThreadChannelOptions1->GetMathString(),currentdata);
                    currentdata = mathresult;
                }
                UD.writechannelvalue(1,currentdata);
            }

        currentdata = destfloat[0] = 0;

        if (ThreadChannelOptions2->GetSignalType() != ModBus::MeasureOff)
            if (UartDriver::needtoupdatechannel[1] == 1)
            {
                UartDriver::needtoupdatechannel[1] = 0;
                this->thread()->usleep(100); // 100 мксек ждем прост.

                //ReadModbusData(&device.badgoodcomm,destfloat );
                ReadModbusData(&device.channel1.Data,&destfloat[0] );
                currentdata = destfloat[0];
                if (ThreadChannelOptions2->IsChannelMathematical())
                {
                    mathresult = mr.SolveEquation(ThreadChannelOptions2->GetMathString(),currentdata);
                    currentdata = mathresult;
                }
                UD.writechannelvalue(2,currentdata);
            }

        currentdata = destfloat[0] = 0;
        if (ThreadChannelOptions3->GetSignalType() != ModBus::MeasureOff)
            if (UartDriver::needtoupdatechannel[2] == 1)
            {
                UartDriver::needtoupdatechannel[2] = 0;
                this->thread()->usleep(100); // 100 мксек ждем прост.
                WriteModbusData(&device.channel0.UserCalibDate2, 45678910 );
                ReadModbusData(&device.channel0.UserCalibDate2,destfloat );
                currentdata = destfloat[0];
                if (ThreadChannelOptions3->IsChannelMathematical())
                {
                    mathresult = mr.SolveEquation(ThreadChannelOptions3->GetMathString(),currentdata);
                    currentdata = mathresult;
                }

                UD.writechannelvalue(3,currentdata);
            }

        currentdata = destfloat[0] = 0;

        if (ThreadChannelOptions4->GetSignalType() != ModBus::MeasureOff)
            if (UartDriver::needtoupdatechannel[3] == 1)
            {
                UartDriver::needtoupdatechannel[3] = 0;
                this->thread()->usleep(100); // 100 мксек ждем прост.
                ReadModbusData(&device.channel3.Data,destfloat );
                currentdata = destfloat[0];
                if (ThreadChannelOptions4->IsChannelMathematical())
                {
                    mathresult = mr.SolveEquation(ThreadChannelOptions4->GetMathString(),currentdata);
                    currentdata = mathresult;
                }

                UD.writechannelvalue(4,currentdata);
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

void worker::OpenSerialPort( int )
{
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {

        //if( m_modbus )
        //{
        //modbus_close( m_modbus );
        //modbus_free( m_modbus );
        //}

        m_modbus = modbus_new_rtu( comportname,comportbaud,comportparity,comportdatabit,comportstopbit);

        if( modbus_connect( m_modbus ) == -1 )
        {
            //qDebug() << "Connection failed"  << "Could not connect serial port!" ;
        }
        else
        {
        }
    }
    else
    {
        //qDebug() << "No serial port found" << "Could not find any serial port " << "on this computer!"  ;
    }
}


void worker::GetObectsSlot(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4)
{

    thread()->usleep(100000);

    ThreadChannelOptions1 = c1;
    ThreadChannelOptions2 = c2;
    ThreadChannelOptions3 = c3;
    ThreadChannelOptions4 = c4;

    //устанавливаем новый тип сигнала для каждого типа сигнала (посылаем соотв-ю ком-ду по модбас)
    uint16_t type = ThreadChannelOptions1->GetSignalType();

    //    WriteModbusData(&device.channel0.SignalType, type) ;

    switch (type) {
    case ModBus::CurrentMeasure:
        break;
    case ModBus::VoltageMeasure:

        //        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Voltage100mVoltNoBreakControl);
        break;
    case ModBus::ResistanceMeasure:
        //        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Wire3NoBreakControl);
        break;
    case ModBus::TermoCoupleMeasure:
        //        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::R);
        break;
    case ModBus::TermoResistanceMeasure:
        //        MB.SetChannelAdditionalParametr(ModBus::DataChannel1, ModBus::Wire3NoBreakControl);
        break;
    default:
        break;
    }




    //    //делаем список объектов каналов, чтобы в цикле можно было их обрабатывать
    //    QList<ChannelOptions *> ChannelsObjectsList;
    //    ChannelsObjectsList.append(c1);
    //    ChannelsObjectsList.append(c2);
    //    ChannelsObjectsList.append(c3);
    //    ChannelsObjectsList.append(c4);
    //    //qDebug() << ChannelsObjectsList.at(1)->GetChannelName();
    //    ModbusDevicesList.clear();





    // Переносим список этих объектов в список структур

    //    int i = 0;
    //    foreach(ChannelOptions * cobj, ChannelsObjectsList)
    //    {
    //        modbusdevice.ModbusNetworkAddress = ModBus::Board4AIAddress; // пока только 1 адрес
    //        modbusdevice.Status = ModBus::StatusOn; // канал включен
    //        modbusdevice.DeviceType = ModBus::DeviceType4AI;
    //        modbusdevice.SignalType = cobj->GetSignalType(); // какой тип сигнала

    //        switch (modbusdevice.SignalType ) {
    //        case ModBus::CurrentMeasure:
    //            modbusdevice.MeasureType  = 0; // у тока всегда ноль
    //            break;
    //        case ModBus::VoltageMeasure:
    //            modbusdevice.MeasureType  = ModBus::Voltage1VoltNoBreakControl; // для напряжения оставим пока 1 вольт
    //            break;
    //        case ModBus::ResistanceMeasure:
    //            modbusdevice.MeasureType  = ModBus::Wire3NoBreakControl; // для ТС ставим 3-х проводку пока.
    //            break;
    //        case ModBus::TermoCoupleMeasure:
    //            modbusdevice.MeasureType  = ModBus::R; // для термопары ставим тип ТПП 13
    //            break;
    //        case ModBus::TermoResistanceMeasure:
    //            modbusdevice.MeasureType  =  ModBus::Wire3NoBreakControl; // для ТС ставим 3-х проводку
    //            break;
    //        default:
    //            modbusdevice.MeasureType  = 0; // по умолчанию ставим ноль.
    //            break;
    //        }

    //        modbusdevice.ID = i;
    //        modbusdevice.name = cobj->GetChannelName();
    //        modbusdevice.SupportedSignals = ModBus::SupportedSignalCurrent | ModBus::SupportedSignalVoltage | ModBus::SupportedSignalTermoCouple| ModBus::SupportedSignalTermoResistance;
    //        modbusdevice.UserCalibration1DateAddress = 32814;

    //        //старший байт: год (в формате YY + 2000), месяц (от 0 до 12), день (от 1 до 31). Младший байт пустой (не используется).
    //        modbusdevice.UserCalibration1Date = 0x17083100;
    //        ModbusDevicesList.append(modbusdevice);
    //        //MB.ConfigureChannel(&modbusdevice);
    //        i++;
    //    }

    //    MB.ConfigureDevices(&ModbusDevicesList);
    thread()->usleep(10000);
}
