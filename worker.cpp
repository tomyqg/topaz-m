#include "worker.h"
#include "defines.h"
//#include "device.h"
#include "uartdriver.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"
//#include "registermap.h"
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

int worker::WriteModbusData(uint8_t sl, const tLookupRegisters * dp, uint32_t * data32)
{
    int ret = -1;
    if (((dp->access & LKUP_ACCESS_WORK_MASK) != LKUP_ACCESS_WORK_W ) \
            && ((dp->access & LKUP_ACCESS_WORK_MASK) != LKUP_ACCESS_WORK_RW ))
        return -1;

    int num;
    //int slave = ModBus::Board4AIAddress;
    int slave = sl;
    uint16_t * data = (uint16_t *) &data32;

    switch (dp->type) {
    case LKUP_TYPE_ASCII:
        num = 6;
        ret = sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->offset, num ,0, data,0);
        break;
    case LKUP_TYPE_U16:
        num = 1;
        ret = sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->offset, num ,0, data,0);
        break;
    case LKUP_TYPE_U32:
        num = 2;
    {
        ret = sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->offset, num ,0, data,0);
    }
        break;
    case LKUP_TYPE_FLOAT:
        num = 2;
        ret = sendModbusRequest(slave, _FC_WRITE_MULTIPLE_REGISTERS, dp->offset, num , 0, data,0);
        break;
    default:
        break;
    }
    return ret;
}

int worker::ReadModbusData(uint8_t sl, const tLookupRegisters* dp, uint32_t *data_dest)
{
    int ret = -1;
    // если запрещено чтение, а только запись разрешена, то возвращаем функцию
    if (((dp->access & LKUP_ACCESS_WORK_MASK) != LKUP_ACCESS_WORK_R ) \
            && ((dp->access & LKUP_ACCESS_WORK_MASK) != LKUP_ACCESS_WORK_RW ))
        return -1;

    int slave = sl;
    int num = (dp->size >> 1);
    int comm = _FC_READ_HOLDING_REGISTERS;
    int add = dp->offset;

//    // количество блоков в зависимости от типа параметра
//    switch (dp->type) {
//    case LKUP_TYPE_ASCII:
//        num = 6;
//        break;
//    case LKUP_TYPE_U16:
//        num = 1;
//        break;
//    case LKUP_TYPE_U32:
//        num = 2;
//        break;
//    case LKUP_TYPE_FLOAT:
//        num = 2;
//        break;
//    default:
//        num = 0; // просто так решил что ноль, мб другое число.
//        break;
//    }

    /* Пока тип регистра в карте не содержится
     * работаем только с HOLDING_REGISTERS
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
    */

    ret = sendModbusRequest(slave, comm, add, num, 0, 0, data_dest);

    switch (dp->type) {
    case LKUP_TYPE_ASCII:
        num = 6;
        break;
    case LKUP_TYPE_U16:
        num = 1;
        break;
    case LKUP_TYPE_U32:
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
    case LKUP_TYPE_FLOAT:
        num = 2;

    {

    }
        break;
    default:
        num = 0; // просто так решил что ноль, мб другое число.
        break;
    }
    return ret;
}

int worker::sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, uint32_t *data_dest)
{

    if( m_modbus == NULL )
    {
        return -1;
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
            //статистика
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
                for(int i = 0; i < ((num>>1)+1); i++)
                {
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
        #else
                    errno == ETOUT ||
        #endif
                    errno == EIO
                    )
            {
                qDebug() << "I/O error"  << "I/O error: did not receive any data from slave" << slave ;
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
                            //emit sendMessToLog("Slave " + QString::number(slave) + " is out: "\
                            //                   + " Good " + QString::number(slaves[slave-1].cntGood) \
                            //                   + " Bad " + QString::number(slaves[slave-1].cntBad));

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

//                qDebug() << "Protocol error"  << "Slave threw exception \"%1\" or function not implemented. " ;
//                qDebug() << modbus_strerror( errno ) << "(errno:" << errno << ")";
//                qDebug() << stderr;
            }
        }
        else
        {
            qDebug() << "Protocol error"  << "Number of registers returned does not match number of registers requested! " ;
        }
    }
    return ret;
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
        const timeval tv = {0, MODBUS_TIMEVAL_MKS};
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

        int res = 0;

        if(!emptyTrans) {
            mQueue.lock();
            Transaction tr = trans.dequeue();
            mQueue.unlock();

            time.restart();
            tLookupRegisters dp = cRegistersMap::getDpByOffset(tr.offset);
            if(tr.dir == Transaction::R)
            {
                res = ReadModbusData(tr.slave, &dp, &tr.volInt);
                if(res > 0)
                {
#ifdef DEBUG_WORKER
                    qDebug() << "Modbus Read  slave:" << tr.slave \
                             << "name:" << cRegistersMap::getNameByOffset(tr.offset).toStdString().c_str() \
                             << " offset:" << tr.offset \
                             << " vol:" << tr.volInt << "(" << tr.volFlo << ")" \
                             << "Time: " << time.elapsed();
#endif
                    emit sendTrans(tr);
                }
            } else {    //(tr.dir == Transaction::W)
                res = WriteModbusData(tr.slave, &dp, (uint32_t *)(tr.volInt));


#ifdef DEBUG_WORKER
                    qDebug() << "Modbus Write  slave:" << tr.slave \
                             << "name:" << cRegistersMap::getNameByOffset(tr.offset).toStdString().c_str() \
                             << " offset:" << tr.offset \
                             << " vol:" << tr.volInt << "(" << tr.volFlo << ")" \
                             << "Time: " << time.elapsed();
#endif
                tr.dir = Transaction::R;
                mQueue.lock();
                trans.enqueue(tr);
                mQueue.unlock();
            }
        }

        this->thread()->usleep(10000);   //Vag: привязать время задержки к скорости порта
    }
}

void worker::getTransSlot(Transaction tr)
{
    mQueue.lock();
//    qDebug() << "worker::getTransSlot" << tr.slave << tr.offset;
    if(trans.size() >= 1000)
    {
        if(!fQueueOver1000) qDebug() << "Warning: Queue is over 1000 elements!!!";
        fQueueOver1000 = true;
    } else {
        fQueueOver1000 = false;
    }
//    qDebug() << "before DEBUG_WORKER";
#ifdef DEBUG_WORKER
//    std::string utf8_text = cRegistersMap::getNameByOffset(tr.offset).toUtf8().constData();
    if(tr.dir == Transaction::R)
    {
        qDebug() << "Modbus Ask  slave:" << tr.slave \
                 << "name:"  << cRegistersMap::getNameByOffset(tr.offset).toStdString().c_str() \
                 << " offset:" << tr.offset;
    }
#endif
//    qDebug() << "after DEBUG_WORKER";
    trans.enqueue(tr);
    mQueue.unlock();

}
