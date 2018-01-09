#include "worker.h"
#include "defines.h"
#include "device.h"
#include "uartdriver.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"
#include "registermap.h"
#include <QDebug>
#include <QList>
#include <QQueue>
#include <QTime>
#include <QMutex>

// constructor
worker::worker(QObject *parent) : QObject(parent)

{
    // активируем сериал порт для модбаса
    OpenSerialPort( 1 );

    //сброс счётчиков
    for(int i = 0; i < (sizeof(slaves)/sizeof(typeStateSlave)); i++)
    {
        slaves[i].cntBad = 0;
        slaves[i].cntBadCurr = 0;
        slaves[i].cntGood = 0;
        slaves[i].state = 0;
    }
}


int globalindex = 0;
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

void worker::WriteModbusData(uint8_t sl, const deviceparametrs* dp, float value, uint32_t data32)
{
    if ( ( dp->WorkLevelAccess!= Device::W ) && ( dp->WorkLevelAccess!= Device::RW ))
        return;

    int num;
    //int slave = ModBus::Board4AIAddress;
    int slave = sl;
    uint16_t * data = (uint16_t*) &data32;

    switch (dp->ParamType) {
    case Device::A12:
        num = 12;
        break;
    case Device::U16:
        num = 1;
        sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->Offset, num ,0, data,0);
        break;
    case Device::U32:
        num = 2;
    {
        sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->Offset, num ,0, data,0);
    }
        break;
    case Device::F32:
        num = 2;
//    {
//        QByteArray floatarray(reinterpret_cast<const char*>(&value), sizeof(value));
//        data[0] = ( floatarray.at(1)<<8 ) | (floatarray.at(0)&0xFF) ;
//        data[1] = ( floatarray.at(3)<<8 ) | (floatarray.at(2)&0xFF);
        sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->Offset, num , 0, data,0);
//    }
        break;
    default:
        break;
    }
}

void worker::ReadModbusData(uint8_t sl, const deviceparametrs* dp, uint32_t *data_dest)
{
    // если запрещено чтение, а только запись разрешена, то возвращаем функцию
    if ( ( dp->WorkLevelAccess != RegisterMap::R ) && ( dp->WorkLevelAccess != RegisterMap::RW ))
        return;

    int slave = sl;
    int num;
    int comm;
    int add;

    // количество блоков в зависимости от типа параметра
    switch (dp->ParamType) {
    case RegisterMap::A12:
        num = 12;
        break;
    case RegisterMap::U16:
        num = 1;
        break;
    case RegisterMap::U32:
        num = 2;
        break;
    case RegisterMap::F32:
        num = 2;
        break;
    default:
        num = 0; // просто так решил что ноль, мб другое число.
        break;
    }

    switch (dp->RegisterType) {
    case RegisterMap::HoldingReg:
        comm = _FC_READ_HOLDING_REGISTERS;
        break;
    case RegisterMap::InputReg:
        comm = _FC_READ_INPUT_REGISTERS;
        break;
    default:
        comm = _FC_READ_INPUT_REGISTERS;
        break;
    }

    add = dp->Offset;

    sendModbusRequest(slave, comm, add, num, 0, 0, data_dest);

    switch (dp->ParamType) {
    case RegisterMap::A12:
        num = 12;
        break;
    case RegisterMap::U16:
        num = 1;
        break;
    case RegisterMap::U32:
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

        data_dest[0] = val;
    }
        break;
    case RegisterMap::F32:
        num = 2;

    {

    }
        break;
    default:
        num = 0; // просто так решил что ноль, мб другое число.
        break;
    }
}

void worker::sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, uint32_t *data_dest)
{

    if( m_modbus == NULL )
    {
        return;
    }

    uint8_t dest[1024];
    uint16_t * dest16 = (uint16_t *) dest;
    uint32_t * dest32 = (uint32_t *) dest;

    memset(dest, 0, sizeof(dest));

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
        if((slave > 0) && (slave <= (sizeof(slaves)/sizeof(typeStateSlave))))
        {
            slaves[slave-1].cntBadCurr = 0;
            slaves[slave-1].state = 0;
            slaves[slave-1].cntGood++;
        }
        if( writeAccess )
        {
            //qDebug() << "Values successfully sent" ;
            //QTimer::singleShot( 2000, this, SLOT( resetStatus() ) );
        }
        else
        {
            if(is16Bit)
            {
                for( int i = num/2-1; i >=0; --i ) {
                    data_dest[i] = dest32[i];
                }
            }
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
                //фиксировать потерю связи при многократном повторении
                if((slave > 0) && (slave <= (sizeof(slaves)/sizeof(typeStateSlave))))
                {
                    slaves[slave-1].cntBad++;
                    int tmp = slaves[slave-1].cntBad;
                    if(slaves[slave-1].cntBadCurr >= TOTAL_BAD_TR_MODBAS)
                    {
                        slaves[slave-1].cntBadCurr = 0;
                        tmp = slaves[slave-1].state;
                        if(slaves[slave-1].state == 0)
                        {
                            emit sendMessToLog("Did not receive any data from slave " + QString::number(slave));
                        }
                        slaves[slave-1].state = 1;
                    }
                    else
                    {
                        slaves[slave-1].cntBadCurr++;
                    }
                }
            }
            else
            {

                qDebug() << "Protocol error"  << "Slave threw exception \"%1\" or function not implemented. " ;
                qDebug() << modbus_strerror( errno ) ;
                qDebug() << stderr;
            }
        }
        else
        {
            qDebug() << "Protocol error"  << "Number of registers returned does not match number of registers requested! " ;
        }
    }
}

void worker::OpenSerialPort( int )
{
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {

//        foreach (QString str, ports) {
//            QString cPort;
//            if(QString.compare(comportname, ports.physName)) {
//                // инициализируем  объект модбаса...
//                m_modbus = modbus_new_rtu( comportname,comportbaud,comportparity,comportdatabit,comportstopbit);
//            } else {

//            }
//        }

        // инициализируем  объект модбаса...
        m_modbus = modbus_new_rtu( comportname,comportbaud,comportparity,comportdatabit,comportstopbit);
        //задание таймаута ожидания ответа от платы
        const timeval tv = {0, 200000};
        modbus_set_timeout_begin(m_modbus, &tv);
        //Включение отладки MODBAS
        //modbus_set_debug(m_modbus, true);
        if( modbus_connect( m_modbus ) == -1 )
        {
            qDebug() << "Connection failed"  << "Could not connect serial port!" ;
            emit ModbusConnectionError();
        }
        else
        {
            qDebug() << "Port OK";
        }
    }
    else
    {
        qDebug() << "No serial port found" << "Could not find any serial port " << "on this computer!"  ;
    }
}


void worker::run()
{
    QTime time;
    bool emptyTrans;
    while(1)
    {
        mQueue.lock();
        emptyTrans = trans.isEmpty();
        mQueue.unlock();

        if(!emptyTrans) {

            mQueue.lock();
            Transaction tr = trans.dequeue();
            mQueue.unlock();

            time.restart();
            deviceparametrs dp = device.getDevParam(tr.offset);
            if(tr.dir == Transaction::R)
            {
                ReadModbusData(tr.slave, &dp, &tr.volInt);
                if(tr.slave == 2)
                {
                    qDebug() << "worker SIGNAL" << tr.offset << "=" << tr.volFlo << "Time: " << time.elapsed();
                }
                emit sendTrans(tr);
            } else {    //(tr.dir == Transaction::W)
                WriteModbusData(tr.slave, &dp, tr.volFlo, tr.volInt);
                tr.dir = Transaction::R;

                mQueue.lock();
                trans.enqueue(tr);
                mQueue.unlock();
            }
        }

        this->thread()->usleep(1000);
    }
}

void worker::getTransSlot(Transaction tr)
{
    mQueue.lock();
    if(trans.size() >= 1000)
    {
        if(!fQueueOver1000) qDebug() << "Warning: Queue is over 1000 elements!!!";
        fQueueOver1000 = true;
    } else {
        fQueueOver1000 = false;
    }
    if(tr.slave == 2)
    {
        qDebug() << "worker SLOT" << tr.offset;
    }
    trans.enqueue(tr);
    mQueue.unlock();
}
