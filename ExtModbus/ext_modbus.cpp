#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "ext_modbus.h"
#include "defines.h"
#include "expert_access.h"
#include "systemoptions.h"

#include <QMessageBox>

#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

//#define DEBUG_EXT_MODBUS


// Below are the wrapper function for executions of methods from pure "C" code
extern "C" {

// register read/write wrapper function (access protection)
uint8_t mbExtRegRwCheckFunc(void* param, void* buffer)
{


    return 0;
}
}

extern cSystemOptions systemOptions;
QString accessPass = "";

cExtModbus::cExtModbus(QObject *parent) : QObject(parent)
{
    socket = 0;
    mb_mapping = NULL;
    use_backend = MB_OFF;
    set_backend = MB_OFF;
    query = NULL;
    header_length = 0;
    maxNbInputRegisters = 0;
    maxNbHoldingRegisters = 0;
    maxNbCoil = 0;
    maxNbDiscrete = 0;
    master_socket = 0;
    ctx = NULL;
    fdmax = 0;
    tcp_port = 502;
    needReinit = false;
}

cExtModbus::~cExtModbus()
{
    modbus_close(ctx);
    if (use_backend == TCP) {
        close(socket);
    }
    modbus_mapping_free(mb_mapping);
    free(query);
    modbus_free(ctx);
}

int cExtModbus::init(int type)
{
    if((type != MB_OFF) && (type != TCP) && (type != RTU)) type = TCP;


    use_backend = type;
    set_backend = use_backend;

    if(use_backend == MB_OFF) return 1;

    maxNbInputRegisters = 0;
    maxNbHoldingRegisters = 0;
    maxNbCoil = 0;
    maxNbDiscrete = 0;
    for(int i = 0; i < numLookupRegisters; i++)
    {
        if(g_extModbusLookupRegisters[i].typeMapping == LKUP_MAP_COILS)
        {
            maxNbCoil++;
        }
        else if(g_extModbusLookupRegisters[i].typeMapping == LKUP_MAP_DISCRETE)
        {
            maxNbDiscrete++;
        }
        if(g_extModbusLookupRegisters[i].typeMapping == LKUP_MAP_INPUTS)
        {
            maxNbInputRegisters++;
        }
        else if(g_extModbusLookupRegisters[i].typeMapping == LKUP_MAP_HILDING)
        {
            maxNbHoldingRegisters++;
        }
    }


    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", tcp_port);
        query = (uint8_t *) malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    } else {
        QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
        if( !ports.isEmpty() )
        {
            ctx = modbus_new_rtu(comportextmodbusrtu, extmodbus_baud, extmodbus_parity, extmodbus_databit, extmodbus_stopbit);
            modbus_set_slave(ctx, extmodbus_server_id);
            query = (uint8_t *) malloc(MODBUS_RTU_MAX_ADU_LENGTH);
        }
        else
        {
            qDebug() << "No serial port found" << "Could not find any serial port " << "on this computer!"  ;
        }
    }
    header_length = modbus_get_header_length(ctx);

#ifdef DEBUG_EXT_MODBUS
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx, TRUE);
#endif

    if(ctx != NULL)
    {
        if (use_backend == TCP) {

            socket = modbus_tcp_listen(ctx, 1);

            /* Clear the reference set of socket */
            FD_ZERO(&refset);
            /* Add the server socket */
            FD_SET(socket, &refset);

            /* Keep track of the max file descriptor */
            fdmax = socket;

        } else {
            int rc = modbus_connect(ctx);
            if( rc == -1 )
            {
            //            fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
                qDebug() << "Connection failed"  << "Could not connect serial port!" ;
                modbus_free(ctx);
                return -1;
            }
            else
            {
                qDebug() << "Port OK";
            }
        }
    }

    return 0;
}

void cExtModbus::slotReinit()
{
    set_backend = systemOptions.extModbus.type;
    tcp_port = systemOptions.extModbus.port;
    needReinit = true;
}

void cExtModbus::reinit(int type)
{
    if(use_backend != MB_OFF)
    {
        //    modbus_mapping_free(mb_mapping);
        if(query != NULL) free(query);
        if(ctx != NULL)
        {
            modbus_close(ctx);
            if (use_backend == TCP) {
                close(socket);
            }
            modbus_free(ctx);
        }
    }
    init(type);
}

void cExtModbus::run()
{
    while(1)
    {
        //пауза для разгрузки процессора
        this->thread()->usleep(1);

        if(needReinit)
        {
            reinit(set_backend);
            needReinit = false;
        }

        if( ctx == NULL ) continue;

        if (use_backend == TCP)
        {
            rdset = refset;

            timeval timeout =  {0, 1000};
            if (select(fdmax+1, &rdset, NULL, NULL, &timeout) == -1) {
                perror("Server select() failure.");
                close(socket);
            }
            else
            {
                for (master_socket = 0; master_socket <= fdmax; master_socket++)
                {
                    if (FD_ISSET(master_socket, &rdset)) {
                        if (master_socket == socket) {
                            /* A client is asking a new connection */
                            socklen_t addrlen;
                            struct sockaddr_in clientaddr;
                            int newfd;

                            /* Handle new connections */
                            addrlen = sizeof(clientaddr);
                            memset(&clientaddr, 0, sizeof(clientaddr));
                            newfd = accept(socket, (struct sockaddr *)&clientaddr, &addrlen);
                            if (newfd == -1) {
                                perror("Server accept() error");
                            } else {
                                FD_SET(newfd, &refset);

                                if (newfd > fdmax) {
                                    /* Keep track of the maximum */
                                    fdmax = newfd;
                                }
                                printf("New connection from %s:%d on socket %d\n",
                                       inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
                            }
                            //сброс уровней доступа при новом подключении
                            cExpertAccess::resetExtAccess();
                        } else {

                            int rc = modbus_receive(ctx, master_socket, query);
                            if (rc == -1)
                            {
                                /* Connection closed by the client, end of server */
                                printf("Connection closed on socket %d\n", master_socket);
                                close(master_socket);

                                /* Remove from reference set */
                                FD_CLR(master_socket, &refset);

                                if (master_socket == fdmax) {
                                    fdmax--;
                                }
                                continue;
                            }
                            reply(rc);
                        }
                    }
                }
            }

        }
        else if (use_backend == RTU)
        {
            int rc = modbus_receive(ctx, -1, query);
            if (rc == -1)
            {
                /* Connection closed by the client or error */
                continue;
            }
            reply(rc);
        }
    }
}

void cExtModbus::reply(int req_length){
    //
    uint8_t func = query[header_length];
    // чтение адреса параметра в карте регистров
    uint16_t offset = (query[header_length + 1] << 8) + query[header_length + 2];
    // число считываемых/записываемых значений
    int nb = 1;
    if((func != _FC_WRITE_SINGLE_REGISTER)
            && (func != _FC_WRITE_SINGLE_COIL))
    {
        nb = (query[header_length + 3] << 8) + query[header_length + 4];
    }

    //проверка адреса и количества
    const tExtLookupRegisters * param;
    int size = 1;
    for(int i = offset; i < offset + nb; i += size)
    {
        param = getLookupElementByOffsetAndFunc(i, func);
        if(param == NULL)   //отсутствие параметра
        {
            modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
            return;
        }
        if((offset + nb) < (i + (param->size >> 1)))
        {   // последний параметр на умещается в пакете данных
            modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
            return;
        }
        if(!isAccess(func, param->access))
        {
            modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
            return;
        }
        if((func == _FC_READ_HOLDING_REGISTERS)
                || (func == _FC_WRITE_SINGLE_REGISTER)
                || (func == _FC_WRITE_MULTIPLE_REGISTERS)
                || (func == _FC_READ_INPUT_REGISTERS))
        {
            //Vag: Запустить сигнал в основной поток для обновления запрошенных регистров
            emit signalActualizeParam(QString(param->nameParam));

            size = (param->size >> 1);
        }
        else
        {
            size = param->size;
        }

    }

    // указатель на элемент в карте регистров
    const tExtLookupRegisters * lookupElement = getLookupElementByOffsetAndFunc(offset, func);
    if(lookupElement == NULL)
    {
        return;
    }

    uint16_t * addrLookupElement = (uint16_t *)GET_PARAM_ADDRESS(lookupElement);
    /* Read/write holding registers */
    if((func == _FC_READ_HOLDING_REGISTERS)
            || (func == _FC_WRITE_SINGLE_REGISTER)
            || (func == _FC_WRITE_MULTIPLE_REGISTERS))
    {
        // обработка запроса регистров настроек
        mb_mapping = modbus_mapping_new(0, 0, offset+nb, 0);
        memcpy(mb_mapping->tab_registers+offset, addrLookupElement, nb*2);
    }
    else if(func == _FC_READ_INPUT_REGISTERS) /* Read input data */
    {
        // подготовка миникарты для ответа
        mb_mapping = modbus_mapping_new(0, 0, 0, offset+nb);

        // указатель на первый элемент в миникарту
        memcpy(mb_mapping->tab_input_registers+offset, addrLookupElement, nb*2);
    }
    else if(func == _FC_READ_DISCRETE_INPUTS) /* Read discrete inputs */
    {
        // подготовка миникарты для ответа
        mb_mapping = modbus_mapping_new(0, offset+nb, 0, 0);

        // указатель на первый элемент в миникарту
        memcpy(mb_mapping->tab_input_bits+offset, addrLookupElement, nb*2);
    }
    else if((func == _FC_READ_COILS)
            || (func == _FC_WRITE_SINGLE_COIL)
            || (func == _FC_WRITE_MULTIPLE_COILS)) /* Read/write coil */
    {
        // подготовка миникарты для ответа
        mb_mapping = modbus_mapping_new(offset+nb, 0, 0, 0);

        // указатель на первый элемент в миникарту
        memcpy(mb_mapping->tab_bits+offset, addrLookupElement, nb*2);
    }
    else    // не корректная функция
    {
        modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_FUNCTION);
        return;
    }

    // проверять не пусто ли в mapping

    // обнуление адреса для корректного смещения по нашей миникарте
    // * больше адрес нигде не используется
//    query[header_length + 1] = 0;
//    query[header_length + 2] = 0;

    mutex.lock();
    int rc = modbus_reply(ctx, query, req_length, mb_mapping);
    mutex.unlock();
    if (rc == -1)
    {
        return;
    }

    //Забираем данные из миникарты
    if((func == _FC_WRITE_SINGLE_REGISTER)
            || (func == _FC_WRITE_MULTIPLE_REGISTERS))
    {
        memcpy(addrLookupElement, mb_mapping->tab_registers+offset, nb*2);
    }
    else if((func == _FC_WRITE_SINGLE_COIL)
            || (func == _FC_WRITE_MULTIPLE_COILS))
    {
        memcpy(addrLookupElement, mb_mapping->tab_bits+offset, nb*2);
    }
    modbus_mapping_free(mb_mapping);

    // применение новых полученных данных
    size = 1;
    if((func == _FC_WRITE_SINGLE_COIL)
            || (func == _FC_WRITE_MULTIPLE_COILS)
            || (func == _FC_WRITE_SINGLE_REGISTER)
            || (func == _FC_WRITE_MULTIPLE_REGISTERS))
    {
        for(int i = 0; i < nb; i += size)
        {
            lookupElement = getLookupElementByOffsetAndFunc(i+offset, func);
            size = updateParam(lookupElement);
        }
    }
}

bool cExtModbus::isAccess(uint8_t func, uint8_t access)
{
    bool res = false;

    if(access == ACCESS_USER)
    {
        res = true;
    }
    else if(access == ACCESS_EXPERT)
    {
        res = true;
        switch(func)
        {
        case _FC_WRITE_SINGLE_COIL:
        case _FC_WRITE_MULTIPLE_COILS:
        case _FC_WRITE_SINGLE_REGISTER:
        case _FC_WRITE_MULTIPLE_REGISTERS:
        {
            if(cExpertAccess::accessExtRequest(accessPass) == Access_User)
            {
                res = false;
            }
        }
        case _FC_READ_COILS:
        case _FC_READ_DISCRETE_INPUTS:
        case _FC_READ_HOLDING_REGISTERS:
        case _FC_READ_INPUT_REGISTERS:
            break;
        default:
            res = false;
            break;
        }
    }
    else if(access == ACCESS_SUPERADMIN)
    {
        if(cExpertAccess::accessExtRequest(accessPass) == Access_Admin)
        {
            res = true;
        }
    }
    return res;
}

#define LENGHT_STR_PASS 32
uint8_t cExtModbus::updateParam(const void * param)
{
    const tExtLookupRegisters * parametr = (tExtLookupRegisters*) param;

    if((QString(parametr->nameParam) == "accessPass") && (parametr->typeMapping == LKUP_MAP_HILDING))
    {
        uint8_t size = parametr->size;
        void * buffer = (void *)GET_PARAM_ADDRESS(parametr);
        char str[LENGHT_STR_PASS];
        memcpy(&str, buffer, LENGHT_STR_PASS);
        accessPass = QString(QByteArray((char*)str, LENGHT_STR_PASS)); //.toStdString()
        cExpertAccess::accessExtRequest(accessPass);
        tModbusBuffer data;
        memset(&data, '*', 32);
        emit signalUpdateParam(parametr->nameParam, data);
        return size;
    }
    else if((parametr->typeMapping == LKUP_MAP_HILDING) || (parametr->typeMapping == LKUP_MAP_COILS))
    {
        uint8_t size = parametr->size;
        void * buffer = (void *)GET_PARAM_ADDRESS(parametr);
        tModbusBuffer data;
        memset(&data, 0, sizeof(data));
        memcpy(&data, buffer, size);
        emit signalUpdateParam(parametr->nameParam, data);
        if(parametr->typeMapping == LKUP_MAP_HILDING)
        {
            size /= 2;
        }
        return size;
    }
    return 0;
}

const tExtLookupRegisters * cExtModbus::getLookupElementByOffsetAndFunc(uint16_t offset, uint8_t func)
{
    const tExtLookupRegisters * tlr = NULL;
    uint8_t map = LKUP_MAP_INPUTS;
    switch(func)
    {
    case _FC_READ_COILS:
    case _FC_WRITE_SINGLE_COIL:
    case _FC_WRITE_MULTIPLE_COILS:
        map = LKUP_MAP_COILS;
        break;
    case _FC_READ_DISCRETE_INPUTS:
        map = LKUP_MAP_DISCRETE;
        break;
    case _FC_READ_HOLDING_REGISTERS:
    case _FC_WRITE_SINGLE_REGISTER:
    case _FC_WRITE_MULTIPLE_REGISTERS:
        map = LKUP_MAP_HILDING;
        break;
    case _FC_READ_INPUT_REGISTERS:
        map = LKUP_MAP_INPUTS;
        break;
    default:
        map = NULL;
        break;
    }

    for(int i = 0; i < numLookupRegisters; i++)
    {
        if((g_extModbusLookupRegisters[i].offset == offset)
                && (g_extModbusLookupRegisters[i].typeMapping == map))
        {
            tlr = &g_extModbusLookupRegisters[i];
            break;
        }
    }

    return tlr;
}

void cExtModbus::updateData(QString name, tModbusBuffer buffer)
{
    const tExtLookupRegisters * lookupReg = NULL;
    for(int i = 0; i < numLookupRegisters; i++)
    {
        if(QString(g_extModbusLookupRegisters[i].nameParam) == name)
        {
            lookupReg = &g_extModbusLookupRegisters[i];
            break;
        }
    }

    if(lookupReg == NULL)
    {
        // нет парамемтра с таким именем
        return;
    }

    uint8_t type = lookupReg->type;
    uint8_t size = lookupReg->size;

    mutex.lock();
    if(type == LKUP_TYPE_FLOAT)
    {
        float * param = (float *)GET_PARAM_ADDRESS(lookupReg);
        memcpy(param, &buffer, size);
    }
    else if(type == LKUP_TYPE_U8)
    {
        uint8_t * param = (uint8_t *)GET_PARAM_ADDRESS(lookupReg);
        memcpy(param, &buffer, size);
    }
    else if(type == LKUP_TYPE_U16)
    {
        uint16_t * param = (uint16_t *)GET_PARAM_ADDRESS(lookupReg);
        memcpy(param, &buffer, size);
    }
    else if(type == LKUP_TYPE_U32)
    {
        uint32_t * param = (uint32_t *)GET_PARAM_ADDRESS(lookupReg);
        memcpy(param, &buffer, size);
    }
    else if(type == LKUP_TYPE_ASCII)
    {
        uint8_t * param = (uint8_t *)GET_PARAM_ADDRESS(lookupReg);
        memcpy(param, &buffer, size);
    }
    mutex.unlock();

}
