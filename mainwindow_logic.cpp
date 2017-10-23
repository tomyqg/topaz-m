#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channelOptions.h"
#include "uartdriver.h"
#include "stackedoptions.h"
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

extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;
extern QColor Channel1ColorNormal;
extern QColor Channel2ColorNormal ;
extern QColor Channel3ColorNormal;
extern QColor Channel4ColorNormal ;

extern QColor Channel1ColorMaximum,Channel2ColorMaximum,Channel3ColorMaximum,Channel4ColorMaximum;
extern QColor Channel1ColorMinimum,Channel2ColorMinimum,Channel3ColorMinimum,Channel4ColorMinimum;

extern QColor ChannelColorHighState;
extern QColor ChannelColorLowState;

void MainWindow::MainWindowInitialization()
{
    datestrings.append("dd.MM.yyyy ");
    datestrings.append("MM-dd-yyyy ");
    datestrings.append("dd-MM-yyyy ");
    datestrings.append("dd/MM/yyyy ");
    datestrings.append("MM/dd/yyyy ");
    datestrings.append("dd.MM.yyyy ");
    datestrings.append("MM.dd.yyyy ");
    datestrings.append("yyyy-MM-dd ");

    timestrings.append("hh:mm:ss ");
    timestrings.append("hh.mm.ss ");
    timestrings.append("hh,mm,ss ");

    dateindex = 0 ;

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

    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    // нужно установить евент филтер чтобы отрисовывалась графика
    ui->MessagesWidget->installEventFilter(this); // если закоментить то не будет уставок и цифр внизу


    QList<QPushButton*> ButtonList = MainWindow::findChildren<QPushButton*> ();
    // добавляем все кнопошки в евентфильтр
    for (int i = 0; i < ButtonList.count(); ++i) {
        QPushButton *but = ButtonList.at(i);
        but->installEventFilter(this);
    }

    SetXRange(XRange);
    SetYRange(YRange);

    ui->customPlot->yAxis->setRange(-GetXRange(), GetXRange());
    ui->customPlot->setAntialiasedElements(QCP::aeNone);
    messwrite.LogAddMessage("Programm Started");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDateLabel()));

    UpdateGraficsTimer = new QTimer(this);

    connect(UpdateGraficsTimer, SIGNAL(timeout()), this, SLOT(UpdateGraphics()));

    QTimer *timetouch = new QTimer(this);
    connect(timetouch, SIGNAL(timeout()), this, SLOT(NewTouchscreenCalibration()));

    tmr = new QTimer();
    tmr->setInterval(ValuesUpdateTimer);

    mutex = new QMutex();

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

    channel1.ReadSingleChannelOptionFromFile(1);
    channel2.ReadSingleChannelOptionFromFile(2);
    channel3.ReadSingleChannelOptionFromFile(3);
    channel4.ReadSingleChannelOptionFromFile(4);

    channel1.SetNormalColor(Channel1ColorNormal);
    channel2.SetNormalColor(Channel2ColorNormal);
    channel3.SetNormalColor(Channel3ColorNormal);
    channel4.SetNormalColor(Channel4ColorNormal);

    channel1.SetMaximumColor(Channel1ColorMaximum);
    channel2.SetMaximumColor(Channel2ColorMaximum);
    channel3.SetMaximumColor(Channel3ColorMaximum);
    channel4.SetMaximumColor(Channel4ColorMaximum);

    channel1.SetMinimumColor(Channel1ColorMinimum);
    channel2.SetMinimumColor(Channel2ColorMinimum);
    channel3.SetMinimumColor(Channel3ColorMinimum);
    channel4.SetMinimumColor(Channel4ColorMinimum);

    SetWindowWidthPixels(1280);
    SetWindowHeightPixels(720);

    WorkerThread = new QThread;
    worker* myWorker = new worker;
    connect(myWorker, SIGNAL(ModbusConnectionError()), this, SLOT(ModbusConnectionErrorSlot()) );
    myWorker->moveToThread(WorkerThread);

    connect(this, SIGNAL(startWorkSignal()), myWorker, SLOT(StartWorkSlot()) );
    connect(this, SIGNAL(stopWorkSignal()), myWorker, SLOT(StopWorkSlot()));
    connect(myWorker, SIGNAL(Finished()), myWorker, SLOT(StopWorkSlot()));
    connect(ui->EcoCheckBox, SIGNAL(clicked(bool)), this, SLOT(ChangePalette(int)) );
    connect(this, SIGNAL(SendObjectsToWorker(ChannelOptions*,ChannelOptions*,ChannelOptions* ,ChannelOptions*)), myWorker, SLOT(GetObectsSlot(ChannelOptions* ,ChannelOptions* ,ChannelOptions*  ,ChannelOptions* )) );
    SendObjectsToWorker(&channel1,&channel2,&channel3,&channel4);
    WorkerThread->start(); // запускаем сам поток

    Options op;
    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
    op.deleteLater();

    // сразу активируем отладку по USB
    QProcess process;
    process.startDetached("ifconfig usb0 192.168.1.115");

    // включаем эко режим
    SetEcoMode(true);
    startWorkSignal(); // сигнал который запускает воркер . без него воркер не запустится
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


void MainWindow::OpenOptionsWindow( int index )
{
    //здесь запускаем меню обновленное как в эндресе

    StackedOptions *sw= new StackedOptions(index,0);
    sw->exec();
    //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
    channel1.ReadSingleChannelOptionFromFile(1);
    channel2.ReadSingleChannelOptionFromFile(2);
    channel3.ReadSingleChannelOptionFromFile(3);
    channel4.ReadSingleChannelOptionFromFile(4);
    // после чтения параметров сразу запихиваем их в сигнал для воркера (передаем воркеру значения каждого канала )
    SendObjectsToWorker(&channel1,&channel2,&channel3,&channel4);
    //если вдруг поменялось время то нужно обновить лейблы
    LabelsInit();
    LabelsCorrect();
    sw->deleteLater();
    resizeSelf(1024,768);
    return;
}

void MainWindow::OpenArchiveWindow()
{
    OpenOptionsWindow(23);
}


void MainWindow::OpenWorkWindow()
{
    OpenOptionsWindow(2);
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

    connect(channeltimer1, SIGNAL(timeout()), this, SLOT(UpdateChannel1Slot()));
    connect(channeltimer2, SIGNAL(timeout()), this, SLOT(UpdateChannel2Slot()));
    connect(channeltimer3, SIGNAL(timeout()), this, SLOT(UpdateChannel3Slot()));
    connect(channeltimer4, SIGNAL(timeout()), this, SLOT(UpdateChannel4Slot()));

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

void MainWindow::DateUpdate() // каждую секунду обновляем значок времени
{
    QDateTime local(QDateTime::currentDateTime());
    ui->time_label->setText(local.date().toString(datestrings.at(dateindex) ) + local.time().toString(timestrings.at(0)));
    resizeSelf(1024,768);
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

void MainWindow::ModbusConnectionErrorSlot()
{
    qDebug() << "Sss" ;
    QMessageBox::critical( this, tr( "Connection Error" ),
                           tr( "Could not connect serial port!" ) );
}

void MainWindow::SetEcoMode(bool seteco)
{
    EcoMode = seteco;
    switch (seteco) {
    case 0:
    {
        ui->customPlot->setBackground(QBrush(NotEcoColor));
        ui->customPlot->xAxis->setTickLabelFont (QFont(Font, 12, QFont::ExtraBold));
        ui->customPlot->xAxis->setTickLabelColor(QColor( Qt::black));
        ui->customPlot->yAxis->setTickLabelFont (QFont(Font, 12, QFont::ExtraBold));
        ui->customPlot->yAxis->setTickLabelColor(QColor( Qt::black));
    }
        break;
    case 1:
    {
        ui->customPlot->setBackground(QBrush(EcoColor));
        ui->customPlot->xAxis->setTickLabelFont(QFont(Font, 12, QFont::ExtraBold));
        ui->customPlot->xAxis->setTickLabelColor(QColor( Qt::white));
        ui->customPlot->yAxis->setTickLabelFont(QFont(Font, 12, QFont::ExtraBold));
        ui->customPlot->yAxis->setTickLabelColor(QColor( Qt::white));
    }
        break;
    default:
        break;
    }
}

bool MainWindow::GetEcoMode()
{
    return EcoMode;
}

void MainWindow::HalfSecondGone()
{
    if (halfSecondflag == 1)
        halfSecondflag = 0;
    else
        halfSecondflag = 1;
}

uint8_t MainWindow::GetHalfSecFlag()
{
    return halfSecondflag;
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

void MainWindow::ChangePalette(int i)
{

    if (ui->EcoCheckBox->checkState())
    {
        Channel1Color = Channel1ColorNormal = QColor(8,124,205);
        Channel2Color = Channel2ColorNormal = QColor(2,115,72);
        Channel3Color = Channel3ColorNormal = QColor(99,98,102);
        Channel4Color = Channel4ColorNormal = QColor(125,70,46);

        Channel1ColorMaximum = QColor(43,40,59);
        Channel2ColorMaximum = QColor(0,56,40);
        Channel3ColorMaximum = QColor(44,48,51);
        Channel4ColorMaximum = QColor(87,58,42);

        Channel1ColorMinimum = QColor(73,111,130);
        Channel2ColorMinimum = QColor(79,125,49);
        Channel3ColorMinimum = QColor(106,107,107);
        Channel4ColorMinimum = QColor(130,79,31);
    }
    else
    {
        Channel1Color = Channel1ColorNormal = QColor(0, 137, 182);// RAL 5012 colour
        Channel2Color = Channel2ColorNormal = QColor(0, 131, 81); // RAL 6024 colour
        Channel3Color = Channel3ColorNormal = QColor(91, 104, 109);// RAL 7031 colour
        Channel4Color = Channel4ColorNormal = QColor(126, 75, 38);// RAL 8003 colour

        Channel1ColorMaximum = QColor(61, 56, 85);
        Channel2ColorMaximum = QColor(0, 105, 76);
        Channel3ColorMaximum = QColor(56,62,66);
        Channel4ColorMaximum = QColor(121,80,56);

        Channel1ColorMinimum = QColor(96,147,172);
        Channel2ColorMinimum = QColor(97,153,59);
        Channel3ColorMinimum = QColor(142,146,145);
        Channel4ColorMinimum = QColor(157,98,43);
    }

    channel1.SetNormalColor(Channel1ColorNormal);
    channel2.SetNormalColor(Channel2ColorNormal);
    channel3.SetNormalColor(Channel3ColorNormal);
    channel4.SetNormalColor(Channel4ColorNormal);

    channel1.SetMaximumColor(Channel1ColorMaximum);
    channel2.SetMaximumColor(Channel2ColorMaximum);
    channel3.SetMaximumColor(Channel3ColorMaximum);
    channel4.SetMaximumColor(Channel4ColorMaximum);

    channel1.SetMinimumColor(Channel1ColorMinimum);
    channel2.SetMinimumColor(Channel2ColorMinimum);
    channel3.SetMinimumColor(Channel3ColorMinimum);
    channel4.SetMinimumColor(Channel4ColorMinimum);

    ui->label_3->setText("#" + QString::number( ui->horizontalScrollBar->value() ) );
}

void MainWindow::sendModbusRequest( void )
{
    if( m_modbus == NULL )
    {
        return;
    }

    const int slave = 0x01;
    const int func = 0x04;
    const int addr = 0x00;
    int num = 0x02;
    int state = 0x02;
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
        //        ret = modbus_write_bit( m_modbus, addr,
        //                                ui->regTable->item( 0, DataColumn )->
        //                                text().toInt(0, 0) ? 1 : 0 );
        writeAccess = true;
        num = 1;
        break;
    case _FC_WRITE_SINGLE_REGISTER:
        ret = modbus_write_register( m_modbus, addr,state);
        writeAccess = true;
        num = 1;
        break;

    case _FC_WRITE_MULTIPLE_COILS:
    {
        uint8_t * data = new uint8_t[num];
        //        for( int i = 0; i < num; ++i )
        //        {
        //            data[i] = ui->regTable->item( i, DataColumn )->
        //                    text().toInt(0, 0);
        //        }
        ret = modbus_write_bits( m_modbus, addr, num, data );
        delete[] data;
        writeAccess = true;
        break;
    }
    case _FC_WRITE_MULTIPLE_REGISTERS:
    {
        uint16_t * data = new uint16_t[num];
        //        for( int i = 0; i < num; ++i )
        //        {
        //            data[i] = ui->regTable->item( i, DataColumn )->
        //                    text().toInt(0, 0);
        //        }
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
            //            m_statusText->setText(
            //                        tr( "Values successfully sent" ) );
            //            m_statusInd->setStyleSheet( "background: #0b0;" );
            QTimer::singleShot( 200, this, SLOT( resetStatus() ) );
        }
        else
        {

            //            qDebug() << dest16[0]<< dest16[1]<< dest16[2] <<  "dest16";

            //            bool b_hex = is16Bit && ui->checkBoxHexData->checkState() == Qt::Checked;
            QString qs_num;

            //            ui->regTable->setRowCount( num );
            for( int i = 0; i < num; ++i )
            {
                int data = is16Bit ? dest16[i] : dest[i];

                QTableWidgetItem * dtItem =
                        new QTableWidgetItem( dataType );
                QTableWidgetItem * addrItem =
                        new QTableWidgetItem(
                            QString::number( addr+i ) );
                //                qs_num.sprintf( b_hex ? "0x%04x" : "%d", data);
                QTableWidgetItem * dataItem =
                        new QTableWidgetItem( qs_num );
                dtItem->setFlags( dtItem->flags() &
                                  ~Qt::ItemIsEditable );
                addrItem->setFlags( addrItem->flags() &
                                    ~Qt::ItemIsEditable );
                dataItem->setFlags( dataItem->flags() &
                                    ~Qt::ItemIsEditable );

                //                ui->regTable->setItem( i, DataTypeColumn,
                //                                       dtItem );
                //                ui->regTable->setItem( i, AddrColumn,
                //                                       addrItem );
                //                ui->regTable->setItem( i, DataColumn,
                //                                       dataItem );
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
}

void MainWindow::changeTranslator(int langindex)
{
    QApplication::removeTranslator(translator);
    translator = new QTranslator();

    switch (langindex) {
    case 0:
        translator->load("untitled2_en_EN");
        break;
    case 1:
        translator->load("untitled2_ru_RU");
        break;
    case 2:
        translator->load("untitled2_de_DE");
        break;
    default:
        return;
    }
    QApplication::installTranslator(translator);
}

void MainWindow::OpenSerialPort( int )
{
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    if( !ports.isEmpty() )
    {
        //подключаемси.
        m_modbus = modbus_new_rtu(comportname,comportbaud,comportparity,comportdatabit,comportstopbit);

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
