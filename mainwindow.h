#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <Channels/channelOptions.h>
#include <QGraphicsScene>
#include <QQueue>
#include "messages.h"
#include "worker.h"
#include "mathresolver.h"
#include "uartdriver.h"
#include "src/modbus.h"
#include "device.h"
#include "transaction.h"
#include "Channels/channelslotcontroller.h"
#include "Relais/relayslotcontroller.h"
#include "Slots/slotsconfig.h"
#include "archivator.h"
#include "log.h"
#include "defines.h"
#include "menu.h"
#include "settings.h"
#include "systemoptions.h"
#include "steel.h"
#include "steel_technology.h"
#include "steel_controller.h"
#include "relay.h"
#include "serverhttp.h"
#include <QNetworkProxyFactory>
#include "usb_flash.h"
#include "ip_controller.h"
#ifndef Q_OS_WIN32
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include "Drivers/driverI2C.h"
#include "Communicator/communicator.h"
#endif



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QDialog * dialogMenu;
//    dMenu * menu;

    void ShowMessageBox (QString title,QString message);
    void resizeWindow(QWidget & qw, qreal mratio);
    void resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution);
    void resizeSelf(qreal xresolution, qreal yresolution);
    void desktopSize(int h, int w)
    {
        desktopHeight = h;
        desktopWidth = w;
    }

    int GetWindowWidthPixels();
    int GetWindowHeightPixels();
//    int GetMonitorWidthPixels();
    int GetMonitorHeightPixels();
    void SetWindowWidthPixels(int neww);
    void SetWindowHeightPixels(int newh);
    void GetAllUartPorts();
    void ReadUstavkiFromFile();

    int GetXOffsetForAlign(int smallrectinglewidth, QGraphicsTextItem *ChannelValueText, int alerttextsize);
    int GetXOffset(int smallrectinglewidth, QGraphicsTextItem *ChannelValueText);




//    transaction device1;
//    transaction device2;
//    transaction device3;
//    transaction device4;
//    transaction typeSign1;


public slots:
    void tickLoadWidget();
    void destroyedslot(QObject *);
//    void NewTouchscreenCalibration();
    void LabelsInit();
    void LabelsUpdate();
    void LabelsCorrect();
    void WriteGpio(int num, bool val);
    void RefreshScreen();
    void WriteArchiveToFile();
    void CreateMODBusConfigFile();
    void HalfSecondGone();
    void ModbusConnectionErrorSlot();
    void SetEcoMode(bool EcoMode);
    void updateDevicesComplect();
//    void updateChannelSlot(int ch);
//    void UpdateChannel1Slot();
//    void UpdateChannel2Slot();
//    void UpdateChannel3Slot();
//    void UpdateChannel4Slot();
//    void UpdSignalTypeSlot(uint8_t ch);
    bool GetEcoMode();
    void getTransFromWorkerSlot(Transaction tr);
    void parseWorkerReceive();
    int getIndexSteelBySlotAndCh(int slot, int ch);
    int getIndexRelayBySlotAndCh(int slot, int ch);
    int getIndexAnalogBySlotAndCh(int slot, int ch);
//    void releOutSlot(uint8_t code);
    void WorkerMessSlot(QString mess);
    void sendRelayStateToWorker(int relay, bool state);
    void retransToWorker(Transaction tr);
    void openSettingsChannel(int num);
    void updateSystemOptions();
    void updateSteel();
    void writeArchiveSteel(int steelNum);

private slots:

//    void OpenSerialPort( int );
    void updateDateLabel();
    void UpdateGraphics();
//    void UpdateLog();
//    void GrafsUpdateBars();
    void GrafsUpdateTrends();
    void GrafsUpdateNone();
    void AddValuesToBuffer();
    void DelaySec(int n);
    void on_MenuButton_clicked();
    void on_MessButton_clicked();
    void ChangePalette(bool i);
    void on_WorkButton_clicked();
    void on_ArchiveButton_clicked();
//    void on_EcoCheckBox_toggled(bool checked);
//    void on_timeButton_clicked();
//    void on_bWriteTypeSignal_clicked();
    void UpdUst();
    void logginStates(int channel, QString mess);
    void newUstavkaConnect(int num);
    void slotRelay(uint8_t sl, uint8_t num, bool state);

//    void askAnybusIRQ();

//    void on_sendI2C_clicked();
//    void on_readI2C_clicked();

//    void on_doubleSpinBox_valueChanged(double arg1);

//    void on_doubleSpinBox_valueChanged(double arg1);
    void plotPress(QMouseEvent * pe);
    void plotReleas(QMouseEvent * pe);
    void plotMove(QMouseEvent * pe);
    void updateAutoScale();

//    void on_pushButton_clicked();

    void on_buttonInputsGraphs_clicked();

//    void on_buttonInputsGraphs_pressed();

//    void on_buttonInputsGraphs_released();

    void on_PlavkaButtonUp_clicked();
    void on_PlavkaButtonDown_clicked();
    void on_PlavkaButtonUp_2_clicked();
    void on_PlavkaButtonDown_2_clicked();
    void on_PlavkaButtonUp_3_clicked();
    void on_PlavkaButtonDown_3_clicked();
    void on_PlavkaButtonUp_4_clicked();
    void on_PlavkaButtonDown_4_clicked();

signals:
    void error(const QString &s);
    void ThreadSignal(ChannelOptions*  channel);
    void startWorkSignal();
    void stopWorkSignal();
//    void SendObjectsToWorker(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4);
    void sendTransToWorker(Transaction tr);
    void setReleToOptionsForm(int code);
    void retransToSlotConfig(Transaction tr);

private:
    Ui::MainWindow *ui;

//    dMenu * menu;

    QTimer timerLoad;
    int countLoader;

    int desktopHeight;
    int desktopWidth;

//    ChannelOptions channel1;
//    ChannelOptions channel2;
//    ChannelOptions channel3;
//    ChannelOptions channel4;
//    QList<ChannelOptions *> listCh;

    bool eventFilter(QObject* watched, QEvent* event);
//    void keyPressEvent(QKeyEvent *event);
    bool needUpdatePainter;
    bool needConfirmationchannel1;
    bool needConfirmationchannel2;
    bool needConfirmationchannel3;
    bool needConfirmationchannel4;
    void InitPins();
    void InitProcessorMaxFreq();
    void InitProcessorMinFreq();
    void InitDevices();
    void InitChannels();
    void InitUstavka();
    void InitTimers();
    void InitTouchScreen();
    void PaintOnWidget();
//    void ReactOnTouch();
    void PaintCyfrasBottom();
    void PaintCyfrasRight();
    void PaintCyfrasNew();
    void PaintCyfrasFullScreen();
    void PaintPolarDiagramm();
    void PaintStatesAndAlertsAtTop();
    void OpenMessagesWindow();
    void OpenOptionsWindow( int index);
    void OpenWorkWindow( );
    void OpenArchiveWindow( );
    void DateUpdate();
    void PowerOff();
    void CloseApplication();
    void WriteAllChannelsOptionsToFile();
    void updateBars(void);
    void setStyleBars(void);
    void setTextBars(void);
    void updateWidgetsVols(void);
    void updateVols();
    void selectWidgetDiagram(void);


    void SetXRange(int newxrange) {Xrange = newxrange;}
    void SetYRange(int newyrange) {Yrange = newyrange;}
    uint8_t GetHalfSecFlag();
    int  GetXRange() {return Xrange;}
    int  GetYRange() {return Yrange;}
    int  GetPolarAngle();
    void  ClearPolarCoords();
    void SetPolarAngle(int newangle);
    int GetTimePeriodSecs() {return 20;}
    int GetTickCountInOneSecond() {return 5;}
    int GetGraphWidthInPixels() {return 600;}
    int GetTotalLabelsCount() {return GetGraphWidthInPixels() / (GetTimePeriodSecs() * GetTickCountInOneSecond() ) ;}
    int GetTickStep() {return GetGraphWidthInPixels() / GetTotalLabelsCount() ;}
    bool EcoMode;
    int polar_angle;

    QGraphicsScene  *scene;

    QTranslator* translator;
    void changeTranslator(int langindex) ;

    modbus_t * m_modbus;

    void MainWindowInitialization();
    uint8_t halfSecondflag;
    QPen graphPen;
    QPainter painter;

    QVector<QPoint> PolarChartPointsChannel1;
    QVector<QPoint> PolarChartPointsChannel2;
    QVector<QPoint> PolarChartPointsChannel3;
    QVector<QPoint> PolarChartPointsChannel4;

    static QString startdate;
    static QString endtime;
    static QString starttime;
    static QVector<QDateTime> Dates;
    QVector<QString> Labels;
    QVector<QString> LabelsBar;

    QTimer *channeltimer1 ;
    QTimer *channeltimer2;
    QTimer *channeltimer3;
    QTimer *channeltimer4;
    QTimer *archivetimer;
    QTimer *UpdateGraficsTimer;
//    QTimer *updLogTimer;
    QTimer *halfSecondTimer;
    QTimer *displayrefreshtimer;
    QTimer *tmr;
    QTimer *timeUpdUst;         //  таймер обновления состояния уставок
    QTimer *timerUpdateDevices;
    //QTimer *timerUpdConfig;

    QThread *WorkerThread;

    QMutex *mutex;

    int Xrange;
    int Yrange;
    int windowwidth;
    int windowheight;
    int xPos;
    int yPos;
    double sizePlot;
    double posPlot;
    bool mouseOnScalede;
    bool mouseOnMove;
    bool waitAutoScale;
    QTimer timerScale;

//    void DrawRectangles(QList<ChannelOptions *> ChannelsObjectsList, int alerttextsize, int smalltextsize);
    void DrawAlertsAndStatesRectangles(QList<ChannelOptions *> ChannelsObjectsList, QPainter painter, int alerttextsize, int smalltextsize);
    void DrawAlertsAndStatesRectangles(QList<ChannelOptions *> ChannelsObjectsList, int smalltextsize, int alerttextsize);
    void SetChannelRectPosition(int alertwindowwidth, int alertwindowheight);
//    void PaintCyfrasBottomSeparate();
    void CheckState(ChannelOptions &channel);
    void DrawScene();
//    void DrawSceneBottom();


    bool isChannelInMaxNow(int ch);
    bool isChannelInMinNow(int ch);

//    cChannelSlotController csc;
    void InitChannelSlotTable();

//    cRelaySlotController rsc;
    void InitRelaySlotTable();

    uint32_t getDevOffsetByChannel(int ch, uint32_t offset);

    cSlotsConfig * sc;

    QQueue<Transaction> queueTransaction;
    QMutex * mQTr;
    QTimer * timerQueueTrans;

    void sendConfigChannelsToSlave();

    cArchivator * arch;
    cLogger * logger;

    QJsonObject ObjectFromString(const QString& in);

    //таймер для слежения IRQ от модуля Anybus
    QTimer * timerAnybusEv;

#ifndef Q_OS_WIN
    cCommunicator * comm;
    QTimer * commRun;
#endif
    dSettings * dialogSetingsChannel;

    void initSteel();
    void InitSteelSlotTable();
    bool slotSteelOnline;       //признак наличия платы STEEL в слоте
    bool slotAnalogOnline;       //признак наличия платы 4AI
    bool allDeviceStable;       // признак установившего статуса плат онлайн/оффлайн
    bool steelMeasure;          //признак начала измерения температуры
    bool steelReady;            //признак готовности данных по площадке
    int stateWidgetSteel;       //состояние главного экрана виджетов по стали
    enum {
        STEEL_WAIT,             //ожидание измерения на одной из входных групп
        STEEL_MEASURE,          //измерение температуры
        STEEL_READY,            //измерения выполнены
    };
    int steelReadyNum;          //номер входной группы площадки, где найдена площадка
    uint8_t numArraySteel;      //номер массива с измерениями для текущего обновления графика
    bool askNewArray;           //необходимость запроса массива
    QTimer * timerUpdateSteel;  //таймер одновления данных с платы STEEL
    int indexSteel;             //индекс массива температур и эдс
    void updateSteelWidget();   //обновление виджета с данными и графиком стали
    bool steelSelectFrame;
    void logginSteel(int numSteel);
    void devicesPause(bool f);

    cServerHttp * server;

    int curGroupChannel;        //текущая группа для отображения на экране

    bool plotReady;            // готовность CustomPlot для рисования графиков

    void reactOnMousePress();
    void reactOnMouseRelease();
    void ReactOnMouseSlide();



protected:
    void paintEvent(QPaintEvent *event) ;
};


#endif // MAINWINDOW_H
