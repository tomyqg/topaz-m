#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channel1.h"
#include "uartdriver.h"
#include "worker.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPen>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>
#include <QMetaType>
#include "defines.h"

extern MainWindow * globalMainWin;


void MainWindow::MainWindowInitialization()
{
    CreateMODBusConfigFile();

    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("VISION"));

    QPixmap pix(pathtologotip);


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

    //    ui->label->setScaledContents(true);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    // нужно установить евент филтер чтобы отрисовывалась графика
    ui->MessagesWidget->installEventFilter(this); // если закоментить то не будет уставок и цифр внизу

    SetXRange(XRange);
    SetYRange(YRange);

    ui->customPlot->yAxis->setRange(-GetXRange(), GetXRange());
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);

    //    MessageWrite mr ;
    messwrite.LogAddMessage("Programm Started");
    //    mr.deleteLater();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));

    UpdateGraficsTimer = new QTimer(this);

    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));

    QTimer *timetouch = new QTimer(this);
    connect(timetouch, SIGNAL(timeout()), this, SLOT(NewTouchscreenCalibration()));

    tmr = new QTimer();
    tmr->setInterval(ValuesUpdateTimer);

    QTimer *tmrarchive = new QTimer(this);
    connect(tmrarchive, SIGNAL(timeout()), this, SLOT(WriteArchiveToFile()));
    tmrarchive->start(ArchiveUpdateTimer);

    connect(tmr, SIGNAL(timeout()), this, SLOT(AddValuesToBuffer()));

    tmr->start(ValuesUpdateTimer);// этот таймер тоже за обновление значений (частота запихивания значений в буфер, оставить пока так должно быть сто
    UpdateGraficsTimer->start(GraphicsUpdateTimer); // этот таймер отвечает за обновление графика (частота отрисовки графика) должно быть 100-200 милисекунд

    timer->start(DateLabelUpdateTimer);
    timetouch->start(ArchiveUpdateTimer);

    InitTimers();
    LabelsInit();

    channel1object.ReadSingleChannelOptionFromFile(1);
    channel2object.ReadSingleChannelOptionFromFile(2);
    channel3object.ReadSingleChannelOptionFromFile(3);
    channel4object.ReadSingleChannelOptionFromFile(4);

    SetWindowWidthPixels(1280);
    SetWindowHeightPixels(720);

    WorkerThread = new QThread;
    worker* myWorker = new worker;
    myWorker->moveToThread(WorkerThread);

    connect(this, SIGNAL(startWorkSignal()), myWorker, SLOT(StartWorkSlot()) );
    //    connect(this, SIGNAL(stopWorkSignal()), myWorker, SLOT(StopWorkSlot()));
    connect(myWorker, SIGNAL(Finished()), myWorker, SLOT(StopWorkSlot()));

    connect(this, SIGNAL(SetObjectsSignal(ChannelOptions*,ChannelOptions*,ChannelOptions* ,ChannelOptions*)), myWorker, SLOT(GetObectsSlot(ChannelOptions* ,ChannelOptions* ,ChannelOptions*  ,ChannelOptions* )) );

    SetObjectsSignal(&channel1object,&channel2object,&channel3object,&channel4object);


    // активируем сериал порт для модбаса
    changeSerialPort( portIndex );

    //    WorkerThread->start(); // запускаем сам поток

    Options op;
    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
    op.deleteLater();

    // сразу активируем отладку по USB
    QProcess process;
    process.startDetached("ifconfig usb0 192.168.1.115");

    needConfirmation = 1;
}



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



void MainWindow::LabelsInit()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();
    // очищаем dates

    Dates.clear();
    Labels.clear();

    int half = GetTotalLabelsCount()/2;
    int timeperiodseconds = GetTimePeriodSecs();

    for (int var = 0; var <= GetTotalLabelsCount(); ++var) {
        Dates.append(fisttime.addSecs((var-half)*timeperiodseconds));// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels.append(fisttime.addSecs((var-half)*timeperiodseconds).toString("hh:mm:ss") );
    }
}




void MainWindow::InitPins()
{
#ifdef MYD // если плата MYD то ничего нам с пинами инициализировать не нужно.
    return;
#endif
    // an object for make terminal requests
    QProcess process;

    //    importante!
    //    Use NOPASSWD line for all commands, I mean:
    //    debian ALL=(ALL) NOPASSWD: ALL
    //    Put the line after all other lines in the sudoers file.

    // allow to pin use
    process.startDetached("sudo chmod 777 /sys/class/gpio/gpio66/value");
    process.startDetached("sudo chmod 777 /sys/class/gpio/gpio66/direction");

    // config pins as uart
    process.startDetached("sudo config-pin P9.24 uart");
    process.startDetached("sudo config-pin P9.26 uart");

    //config RTS pin as gpio pull-down
    process.startDetached("sudo config-pin P8.7 gpio_pd");

    QFile filedir("/sys/class/gpio/gpio66/direction");
    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);
    outdir << "out";
    filedir.close();
}




void MainWindow::OpenMessagesWindow()
{
    messwrite.WriteAllLogToFile();
    Messages *messages = new Messages;
    connect( messages, SIGNAL(destroyed(QObject*)), this, SLOT( destroyedslot(QObject*)) ); //
    this->resizeWindow(*messages,this->GetWindowWidthPixels(),this->GetWindowHeightPixels());
    messages->setModal(true);
    messages->exec();
    messages->deleteLater();
}




void MainWindow::DelaySec(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}




void MainWindow::OpenOptionsWindow()
{
    //    startWorkSignal();
    Options *optionsobj = new Options;
    this->resizeWindow(*optionsobj,this->GetWindowWidthPixels(),this->GetWindowHeightPixels());

    GetMonitorWidthPixels();
    optionsobj->exec();
    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
    channel1object.ReadSingleChannelOptionFromFile(1);
    channel2object.ReadSingleChannelOptionFromFile(2);
    channel3object.ReadSingleChannelOptionFromFile(3);
    channel4object.ReadSingleChannelOptionFromFile(4);

    //    если вдруг поменялось время то нужно обновить лейблы
    LabelsInit();
    LabelsCorrect();

    // если что меняем разрешение
    if (Options::displayResolution == "1024x768")
    {
        resizeSelf(1024,768);
    }

    if (Options::displayResolution == "1280x800")
    {
        resizeSelf(1280,720);
    }

    optionsobj->deleteLater(); // удаляем объект опций

    //останавливаем поток, загружаем объекты в поток , и запускаем его уже с новыми параметрами

    SetObjectsSignal(&channel1object,&channel2object,&channel3object,&channel4object);
}

void MainWindow::PowerOff()
{
    QProcess process;
    process.startDetached("poweroff");
    QApplication::exit();
}

void MainWindow::CloseApplication()
{
    QApplication::exit();
}

void MainWindow::InitProcessorMaxFreq()
{
    // an object for make terminal requests
    QProcess process;

    // the maximum processor speed
    process.startDetached("sudo cpufreq-set -f 1000MHz");
}

void MainWindow::InitProcessorMinFreq()
{
    // an object for make terminal requests
    QProcess process;
    // the maximum processor speed
    process.startDetached("sudo cpufreq-set -f 300MHz");
    process.startDetached("sudo cpufreq-set --governor powersave"); // min perfomance on
}

void MainWindow::InitTimers()
{
    channeltimer1 = new QTimer();
    channeltimer2 = new QTimer();
    channeltimer3 = new QTimer();
    channeltimer4 = new QTimer();
    archivetimer  = new QTimer();
    halfSecondTimer  = new QTimer();

#ifdef MultiThread
    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateChannel1Slot()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateChannel2Slot()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateChannel3Slot()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateChannel4Slot()));
#endif

    connect(halfSecondTimer, SIGNAL(timeout()), this, SLOT(HalfSecondGone()));
    channeltimer1->start(100);
    channeltimer2->start(100);
    channeltimer3->start(100);
    channeltimer4->start(100);
    halfSecondTimer->start(500);
    archivetimer->start(6000); // каждые 6 минут записываем архив на флешку
}

void MainWindow::InitTouchScreen()
{
    // an object for make terminal requests
    QProcess process;
    // the initial touchscreen
    process.startDetached("sudo xinput set-prop 7 \"Evdev Axis Calibration\" 3383 3962 234 599"); // вручную ввели координаты тача
}

void MainWindow::DateUpdate()
{
    QDateTime local(QDateTime::currentDateTime());
    //    messwrite.LogAddMessage(local.time().toString());
    ui->time_label->setText(local.time().toString() + local.date().toString(" dd.MM.yyyy"));

    float destfloat[1024];
    memset( destfloat, 0, 1024 );

    sendModbusRequest(0x01, 0x04, 0x00, 0x04, 0, 0, destfloat);

    qDebug() << destfloat[0] << "destfloat[0]";

    //    qDebug() << destfloat[0]<< "destfloat[0]";
    //    qDebug() << destfloat[1]<< "destfloat[1]";
    //    sendModbusRequest(0x01,0x05,0x01,0x01,0x01,0);
    //    Sleep(500);
    //    sendModbusRequest(0x01,0x05,0x01,0x01,0x00,0);
    //    Sleep(500);
}

void MainWindow::LabelsUpdate()
{
    QDateTime fisttime;
    fisttime = QDateTime::currentDateTime();

    int i = 0;
    while(i<=GetTotalLabelsCount()) {
        Dates[i] = Dates[i].addSecs(GetTimePeriodSecs());// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
}

void MainWindow::LabelsCorrect()
{
    QDateTime currnttime;
    qint16 timecorrectsec;
    currnttime = QDateTime::currentDateTime();
    int middlelabelnumber = Dates.length()/2;
    timecorrectsec = Dates[middlelabelnumber].secsTo(currnttime);

    int i = 0;
    while(i<=GetTotalLabelsCount())
    {
        Dates[i] = Dates[i].addSecs(timecorrectsec);// << (fisttime.addSecs(i*6)).toString("hh:mm:ss");
        Labels[i] = Dates.at(i).toString("hh:mm:ss"); //Dates.at(i).toString("hh:mm:ss");
        i++;
    }
}

void MainWindow::HalfSecondGone()
{
    InvertHalfSecFlag();
}

char MainWindow::GetHalfSecFlag()
{
    return halfSecondflag;
}

void MainWindow::InvertHalfSecFlag()
{
    halfSecondflag = ~halfSecondflag;
}

void MainWindow::NewTouchscreenCalibration()
{
    QProcess process;
    process.startDetached("xinput set-prop 7 \"Evdev Axis Calibration\" " + Options::calibrationprm); // каждую секунду вводим координаты тача вдруг чтобы не отвалился
}

void MainWindow::paintEvent(QPaintEvent *)
{
    int i = 0;
    i++;
}

void MainWindow::GetAllUartPorts()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    QString infosss;
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        infosss += serialPortInfo.portName();
    }
}

void MainWindow::CheckState(ChannelOptions&  channel)
{
    //    channel.GetCurrentChannelValue();
    double channelcurrentvalue = channel.GetCurrentChannelValue();
    double channelstate1value = channel.GetState1Value();
    double channelstate2value = channel.GetState2Value();
    QString channelstringvalue = (QString::number( channelcurrentvalue, 'f', 3)) + " " + channel.GetUnitsName();

    //    превысили верхнюю уставку
    if ( (channelcurrentvalue>channelstate1value) && ( channel.HighState1Setted == false ) )
    {
        channel.LowState1Setted = false;
        channel.HighState1Setted = true;
        messwrite.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState1HighMessage() + ":" + channelstringvalue);
    }

    // было превышение а стало норма
    else if ( (channelcurrentvalue>=channelstate2value) && (channelcurrentvalue<=channelstate1value) && ( channel.HighState1Setted == true ) )
    {
        channel.HighState1Setted = false;
        messwrite.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState1LowMessage() + ":" + channelstringvalue);
    }

    // было уменьшение а стало норма
    else if ( (channelcurrentvalue>=channelstate2value) && (channelcurrentvalue<=channelstate1value) && ( channel.LowState1Setted == true ) )
    {
        channel.LowState1Setted = false;
        messwrite.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState2HighMessage() + ":" + channelstringvalue);
    }

    //стало ниже нижней уставки
    else if ( (channelcurrentvalue<channelstate2value) && ( channel.LowState1Setted == false ) )
    {
        channel.LowState1Setted = true;
        channel.HighState1Setted = false;
        messwrite.LogAddMessage (channel.GetChannelName() + ":" + channel.GetState2LowMessage() + ":" + channelstringvalue);
    }
}

extern "C" {

void busMonitorAddItem( uint8_t isRequest, uint8_t slave, uint8_t func, uint16_t addr, uint16_t nb, uint16_t expectedCRC, uint16_t actualCRC )
{

}

void busMonitorRawData( uint8_t * data, uint8_t dataLen, uint8_t addNewline )
{

}

}


void MainWindow::sendModbusRequest( int slave, int func, int addr, int num, int state, const uint16_t *data_src, float *data_dest_float)
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
            qDebug() << "Values successfully sent" ;
            QTimer::singleShot( 2000, this, SLOT( resetStatus() ) );
        }
        else
        {
            // перешли сюда значит нужно преобразовать считанные значения из массива HEX во float
            QByteArray arraytofloat;

            for( int i = num-1; i >=0; --i )
            {
                qDebug() << num<< "num" ;

                int data = is16Bit ? dest16[i] : dest[i];
                arraytofloat.append((data & 0xFF00)>>8);
                arraytofloat.append(data & 0x00FF);
//                data_dest_float[num - 1 - i] = data;
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
                QMessageBox::critical( this, tr( "I/O error" ),
                                       tr( "I/O error: did not receive any data from slave." ) );
            }
            else
            {
                QMessageBox::critical( this, tr( "Protocol error" ),
                                       tr( "Slave threw exception \"%1\" or "
                                           "function not implemented." ).
                                       arg( modbus_strerror( errno ) ) );
            }
        }
        else
        {
            QMessageBox::critical( this, tr( "Protocol error" ),
                                   tr( "Number of registers returned does not "
                                       "match number of registers "
                                       "requested!" ) );
        }
    }
}



void MainWindow::resetStatus( void )
{
    ;
    //    qDebug() << "Ready" ;
}



void MainWindow::changeSerialPort( int )
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
            QMessageBox::critical( this, tr( "Connection failed" ),
                                   tr( "Could not connect serial port!" ) );
        }
        else
        {
        }
    }
    else
    {
        QMessageBox::critical( this, tr( "No serial port found" ),
                               tr( "Could not find any serial port "
                                   "on this computer!" ) );
    }
}
