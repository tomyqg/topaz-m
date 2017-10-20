#include "worker.h"
#include "defines.h"
#include "device.h"
#include "uartdriver.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"
#include <QDebug>
#include <QList>

uint32_t total =0;

// constructor
worker::worker(QObject *parent) :
    QObject(parent), isstopped(false), isrunning(false)
{
    // активируем сериал порт для модбаса
    OpenSerialPort( 1 );
}

ModBus MB;
DataBuffer UD;
mathresolver mr;

int ic ;

int globalindex;
int globalindex2;

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
        data[0] = ( a & 0xFFFF);
        data[1] = ( a >> 16 );
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
        //        qDebug() << data_dest[0] << data_dest[1] << "U32";

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

        //qDebug() << data_dest[ 0 ] <<data_dest[ 1 ] << data_dest[ 2 ] << data_dest[ 3 ] << "U32";
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
        //        qDebug() << data_dest[0] << data_dest[1] << "F32"; // пришли два слова, парсим два слова

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

    this->thread()->msleep(100); // 100 мксек ждем между запросами чтобы плата не зависала

    if( m_modbus == NULL )
    {
        return;
    }

    total ++;

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

    memset( destfloat, 0, 1024 );//заполняем нулями массив

    emit SignalToObj_mainThreadGUI();

    if ( !isrunning || isstopped ) // если воркер остановлен
    {
        this->thread()->usleep(1000); // 100 мксек ждем прост. чтобы проц не перегружался и не перегревался
    }

    if ( isrunning || !isstopped ) // если воркер запущен
    {
//        qDebug() <<  "isrunning" ;
        this->thread()->setPriority(QThread::LowPriority);

        // пихаем все каналы в один массив
        // тут опрашиваем каждый канал

        int chanelindex = 0;

        if ( (++globalindex2) %4==0)
            globalindex++;

        if (globalindex > 199)
            globalindex = 40;


        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            if ( (Chanel->GetSignalType() != ModBus::MeasureOff) && (DataBuffer::readupdatestatus(chanelindex)) )
            {

                 qDebug() << globalindex << "globalindex";
                QCoreApplication::applicationDirPath();
                DataBuffer::writeupdatestatus(chanelindex,false);

                //ReadModbusDataMutex.lock();
                //ReadModbusData(&device.Channels.at(index).Data,&destfloat[0] ); //если не симуляция то читаем канал по модбас
                //ReadModbusDataMutex.unlock();

                currentdata = destfloat[0];

                if (Chanel->IsChannelMathematical())
                {
                    mathresult = mr.SolveEquation(Chanel->GetMathString(),currentdata);
                    currentdata = mathresult;
                }

                double r = rand()%10;

                switch (chanelindex) {
                case 0:
                    currentdata = mr.SolveEquation("sin(x/5)*10",globalindex ) + 0 + r;
                    break;
                case 1:
                    currentdata = mr.SolveEquation("sin(x/5)*10",globalindex ) + 5 + r;
                    break;
                case 2:
                    currentdata =  0.1*globalindex;
                    break;
                case 3:
                    currentdata =  0.1*globalindex;
                    break;
                default:
                    break;
                }

                Chanel->SetCurrentChannelValue(currentdata );
                //DataBuffer::writechannelvalue(chanelindex,currentdata);
                //Chanel->SetCurrentChannelValue(chanelindex*10+10);
                //DataBuffer::channeltempbuffer[1] = 1;
            }
            ++chanelindex;
        }
    }

    emit Finished(); // вызываем сигнал что обработка канала завершилась. ждем следующего запуска канала
    // do important work here
    // allow the thread's event loop to process other events before doing more "work"
    // for instance, your start/stop signals from the MainWindow
    QMetaObject::invokeMethod(this, "do_Work", Qt::QueuedConnection );
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
        // инициализируем  объект модбаса...
        m_modbus = modbus_new_rtu( comportname,comportbaud,comportparity,comportdatabit,comportstopbit);
        if( modbus_connect( m_modbus ) == -1 )
        {
            //            qDebug() << "Connection failed"  << "Could not connect serial port!" ;
            emit ModbusConnectionError();
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
    ChannelsObjectsList.append(ThreadChannelOptions1);
    ChannelsObjectsList.append(ThreadChannelOptions2);
    ChannelsObjectsList.append(ThreadChannelOptions3);
    ChannelsObjectsList.append(ThreadChannelOptions4);
    thread()->usleep(10000);
}
