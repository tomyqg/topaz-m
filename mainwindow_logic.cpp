#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channel1.h"
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
extern QColor ChannelColorNormal;
extern QColor Channel2ColorNormal ;
extern QColor Channel3ColorNormal;
extern QColor Channel4ColorNormal ;
extern QColor ChannelColorHighState;
extern QColor ChannelColorLowState;


void MainWindow::MainWindowInitialization()
{
    //    CreateMODBusConfigFile();

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


    channel1object.SetColor(Channel1Color);
    channel2object.SetColor(Channel2Color);
    channel3object.SetColor(Channel3Color);
    channel4object.SetColor(Channel4Color);

    SetWindowWidthPixels(1280);
    SetWindowHeightPixels(720);


    //    SetWindowWidthPixels(1024);
    //    SetWindowHeightPixels(768);

    WorkerThread = new QThread;
    worker* myWorker = new worker;
    connect(myWorker, SIGNAL(ModbusConnectionError()), this, SLOT(ModbusConnectionErrorSlot()) );

    myWorker->moveToThread(WorkerThread);

    connect(this, SIGNAL(startWorkSignal()), myWorker, SLOT(StartWorkSlot()) );
    connect(this, SIGNAL(stopWorkSignal()), myWorker, SLOT(StopWorkSlot()));
    connect(myWorker, SIGNAL(Finished()), myWorker, SLOT(StopWorkSlot()));

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(ChangePalette(int)) );

    connect(this, SIGNAL(SendObjectsToWorker(ChannelOptions*,ChannelOptions*,ChannelOptions* ,ChannelOptions*)), myWorker, SLOT(GetObectsSlot(ChannelOptions* ,ChannelOptions* ,ChannelOptions*  ,ChannelOptions* )) );
    SendObjectsToWorker(&channel1object,&channel2object,&channel3object,&channel4object);

    WorkerThread->start(); // запускаем сам поток

    Options op;
    op.ReadSystemOptionsFromFile(); // читаем опции из файла (это режим отображения и т.п.)
    op.deleteLater();

    // сразу активируем отладку по USB
    QProcess process;
    process.startDetached("ifconfig usb0 192.168.1.115");

    // включаем эко режим
    SetEcoMode(true);

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
    if (ui->endressmenucheckbox->checkState())
    {
        StackedOptions *sw= new StackedOptions(index,0);
        sw->exec();
        //читаем параметры каналов прямо после закрытия окна настроек и перехода в меню режима работы
        channel1object.ReadSingleChannelOptionFromFile(1);
        channel2object.ReadSingleChannelOptionFromFile(2);
        channel3object.ReadSingleChannelOptionFromFile(3);
        channel4object.ReadSingleChannelOptionFromFile(4);

        SendObjectsToWorker(&channel1object,&channel2object,&channel3object,&channel4object);
        //    если вдруг поменялось время то нужно обновить лейблы
        LabelsInit();
        LabelsCorrect();
        sw->deleteLater();

        resizeSelf(1024,768);
        return;
    }

    //startWorkSignal();

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


    // пересылаем ссылки на объекты в воркер
    SendObjectsToWorker(&channel1object,&channel2object,&channel3object,&channel4object);

    // если что меняем разрешение
    if (Options::displayResolution == "1024x768")
    {
        resizeSelf(1024,768);
    }

    if (Options::displayResolution == "1280x800")
    {
        resizeSelf(1280,720);
    }

    resizeSelf(1024,768);

    optionsobj->deleteLater(); // удаляем объект опций
    //останавливаем поток, загружаем объекты в поток , и запускаем его уже с новыми параметрами
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
    //    ui->time_label->setText(local.time().toString() + local.date().toString(" dd.MM.yyyy"));
    ui->time_label->setText(local.date().toString("dd.MM.yyyy " ) + local.time().toString());

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
    switch (seteco) {
    case 0:
    {
        ui->customPlot->setBackground(QBrush(QColor(0xff,0xff,0xff)));
        ui->customPlot->xAxis->setTickLabelFont (QFont(Font, 12, QFont::ExtraBold));
        ui->customPlot->xAxis->setTickLabelColor(QColor( Qt::black));
        ui->customPlot->yAxis->setTickLabelFont (QFont(Font, 12, QFont::ExtraBold));
        ui->customPlot->yAxis->setTickLabelColor(QColor( Qt::black));
    }
        break;
    case 1:
    {
        ui->customPlot->setBackground(QBrush(QColor (0x00,0x00,0x4d) )); // ("#b3daff")
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
    switch (ui->horizontalScrollBar->value()) {
    case 1:

        Channel1Color = ChannelColorNormal = QColor(0x00, 0x71, 0x43);
        Channel2Color = Channel2ColorNormal = QColor(0x6C, 0x8D, 0xD5);
        Channel3Color = Channel3ColorNormal = QColor(0xFF, 0xCF, 0x73);
        Channel4Color = Channel4ColorNormal = QColor(0xFF, 0x9D, 0x73);

        channel1object.SetColor(QColor(0x00, 0x71, 0x43));
        channel2object.SetColor( QColor(0x6C, 0x8D, 0xD5));
        channel3object.SetColor( QColor(0xFF, 0xCF, 0x73));
        channel4object.SetColor (QColor(0xFF, 0x9D, 0x73));


        break;
    case 2:
        Channel1Color = ChannelColorNormal = QColor(0xAB, 0x2B, 0x52);
        Channel2Color = Channel2ColorNormal = QColor(0xFF, 0x49, 0x00);
        Channel3Color = Channel3ColorNormal = QColor(0x00, 0xAF, 0x64);
        Channel4Color = Channel4ColorNormal = QColor(0x67, 0xE3, 0x00);


        channel1object.SetColor(QColor(0xAB, 0x2B, 0x52));
        channel2object.SetColor( QColor(0xFF, 0x49, 0x00));
        channel3object.SetColor(QColor(0x00, 0xAF, 0x64));
        channel4object.SetColor (QColor(0x67, 0xE3, 0x00));

        break;
    case 3:
        Channel1Color = ChannelColorNormal = QColor(0x00, 0xC1, 0x2B);
        Channel2Color = Channel2ColorNormal = QColor(0x04, 0x85, 0x9D);
        Channel3Color = Channel3ColorNormal = QColor(0xFF, 0x7C, 0x00);
        Channel4Color = Channel4ColorNormal = QColor(0xFF, 0x52, 0x40);

        channel1object.SetColor(QColor(0x00, 0xC1, 0x2B));
        channel2object.SetColor( QColor(0x04, 0x85, 0x9D));
        channel3object.SetColor(QColor(0xFF, 0x7C, 0x00));
        channel4object.SetColor (QColor(0xFF, 0x52, 0x40));

        break;
    case 4:
        Channel1Color = ChannelColorNormal = QColor(0xCF, 0xF7, 0x00);
        Channel2Color = Channel2ColorNormal = QColor(0x00, 0xAE, 0x68);
        Channel3Color = Channel3ColorNormal = QColor(0xFF, 0x4C, 0x00);
        Channel4Color = Channel4ColorNormal = QColor(0xA1, 0x01, 0xA6);

        channel1object.SetColor(QColor(0xCF, 0xF7, 0x00));
        channel2object.SetColor( QColor(0x00, 0xAE, 0x68));
        channel3object.SetColor(QColor(0xFF, 0x4C, 0x00));
        channel4object.SetColor (QColor(0xA1, 0x01, 0xA6));

        break;

    case 5:
        Channel1Color = ChannelColorNormal = QColor(0x00, 0xb0, 0x60);
        Channel2Color = Channel2ColorNormal = QColor(0x1d, 0x1a, 0xb2);
        Channel3Color = Channel3ColorNormal = QColor(0xd5, 0xf8, 0x00);
        Channel4Color = Channel4ColorNormal = QColor(0xff, 0x45, 0x00);


        channel1object.SetColor(QColor(0x00, 0xb0, 0x60));
        channel2object.SetColor( QColor(0x1d, 0x1a, 0xb2));
        channel3object.SetColor(QColor(0xd5, 0xf8, 0x00));
        channel4object.SetColor (QColor(0xff, 0x45, 0x00));

        break;

    case 6:
        Channel1Color = ChannelColorNormal = QColor(0x10, 0x49, 0xa9);
        Channel2Color = Channel2ColorNormal = QColor(0x6c, 0x0a, 0xab);
        Channel3Color = Channel3ColorNormal = QColor(0x34, 0xd8, 0x00);
        Channel4Color = Channel4ColorNormal = QColor(0xff, 0xa4, 0x00);

        channel1object.SetColor(QColor(0x10, 0x49, 0xa9));
        channel2object.SetColor( QColor(0x6c, 0x0a, 0xab));
        channel3object.SetColor(QColor(0x34, 0xd8, 0x00));
        channel4object.SetColor (QColor(0xff, 0xa4, 0x00));

        break;


    case 7:
        Channel1Color = ChannelColorNormal = QColor(0xD2, 0x35, 0xD2);
        Channel2Color = Channel2ColorNormal = QColor(0xFF, 0x8C, 0x00);
        Channel3Color = Channel3ColorNormal = QColor(0x3D, 0x9A, 0x01);
        Channel4Color = Channel4ColorNormal = QColor(0xa1, 0xb9, 0x2e);

        channel1object.SetColor(QColor(0xD2, 0x35, 0xD2));
        channel2object.SetColor( QColor(0xFF, 0x8C, 0x00));
        channel3object.SetColor(QColor(0x3D, 0x9A, 0x01));
        channel4object.SetColor (QColor(0xa1, 0xb9, 0x2e));

        break;


    default:
        Channel1Color = ChannelColorNormal = QColor(0x00, 0x71, 0x43);
        Channel2Color = Channel2ColorNormal = QColor(0x6C, 0x8D, 0xD5);
        Channel3Color = Channel3ColorNormal = QColor(0xFF, 0xCF, 0x73);
        Channel4Color = Channel4ColorNormal = QColor(0xFF, 0x9D, 0x73);

        channel1object.SetColor(QColor(0x00, 0x71, 0x43));
        channel2object.SetColor( QColor(0x6C, 0x8D, 0xD5));
        channel3object.SetColor(QColor(0xFF, 0xCF, 0x73));
        channel4object.SetColor (QColor(0xFF, 0x9D, 0x73));

        break;

    }

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

#ifdef Q_OS_WIN32
        const QString port = comportname;
#else
        // const QString port = ports[iface].physName;
        // точно знаем что этот порт работает на rs485
        const QString port = "/dev/ttyO1";
#endif

        char parity;

        parity = comportparity;

        // TODO: если раскомментить то  будет вываливаться ошибка при дебаге.
        //        if( m_modbus )
        //        {
        //            modbus_close( m_modbus );
        //            modbus_free( m_modbus );
        //        }

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
