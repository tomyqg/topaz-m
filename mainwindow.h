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
//#include "device.h"
#include "transaction.h"
//#include "Channels/channelslotcontroller.h"
//#include "Relais/relayslotcontroller.h"
//#include "Slots/slotsconfig.h"
#include "archivator.h"
#include "log.h"
#include "defines.h"
#include "menu.h"
#include "settings.h"
#include "systemoptions.h"
#include "steel.h"
#include "steel_technology.h"
//#include "steel_controller.h"
#include "relay.h"
#include "serverhttp.h"
#include <QNetworkProxyFactory>
#include "usb_flash.h"
#include "ip_controller.h"
#include "ExtModbus/ext_modbus.h"
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
//    void ReadUstavkiFromFile();

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
    void retransDeviceToWorker(Transaction tr);
    void openSettingsChannel(int num);
    void updateSystemOptions();
    void updateSteel();
    void writeArchiveSteel(int steelNum);
    void slotReadySteel(int n);
    void slotMeasureSteel(int n);
    void slotWaitSteel(int n);
    void slotSteelArchivate(int n);
    void slotSteelFrame(bool steelFrame);
    void devicesPause(bool f);
    void updateExtIntefaceData();
    void slotUpdateExtIntefaceData(QString name);
    void slotFromExtModbus(QString name,tModbusBuffer data);
    void updateObjectsOfMainThread();

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
    void slotGetRelay(uint8_t sl, uint8_t num);
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
    void signalToExtModbus(QString name,tModbusBuffer buffer);
    void signalReinitExtModbus();

private:
    Ui::MainWindow *ui;


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
    void InitFreq();
    void InitUstavka();
    void InitTimers();
    void InitTouchScreen();
    void PaintOnWidget();
//    void ReactOnTouch();
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
    int GetTotalLabelsCount() {return GetGraphWidthInPixels() / (GetTimePeriodSecs() * GetTickCountInOneSecond() ) ;}   // 600/20*5=150
    int GetTickStep() {return GetGraphWidthInPixels() / GetTotalLabelsCount() ;}    // 600/150=4
    bool EcoMode;
    int polar_angle;

    QGraphicsScene  *scene;

    QTranslator* translator;
    void changeTranslator(int langindex) ;

    modbus_t * m_modbus;

    //    Extern Interface
    QTimer timerExtInterface;
    typedef void (MainWindow::*funcExtInterface)(QString name);
    typedef struct {
        QString name;
        funcExtInterface funcGet;
    } typeTableExtInterface;
//    QList<typeTableExtInterface> tablExtInterfaceConstants;
//    QList<typeTableExtInterface> tablExtInterfaceSlowInfo;
//    QList<typeTableExtInterface> tablExtInterfaceInfo;
//    QList<typeTableExtInterface> tablExtInterfaceState;
//    QList<typeTableExtInterface> tablExtInterfaceChannels;
    QList<typeTableExtInterface> tablExtInterfaceRegisters;
    typedef void (MainWindow::*funcApplyNewParamFromExtInterface)(QString name, uint8_t * data);
    typedef struct {
        QString name;
        funcExtInterface funcGet;
        funcApplyNewParamFromExtInterface funcSet;
    } typeTableSetParamExtInterface;
    QList<typeTableSetParamExtInterface> tablSetParamExtInterface;

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
    QThread * extModbusThread;

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
    bool pauseUpdateGraph;  // пауза посторения графиков
    bool updateGraph;       // нужно обновить график

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

    void InitChannelSlotTable();

//    cRelaySlotController rsc;
    void InitRelaySlotTable();

    uint32_t getDevOffsetByChannel(int ch, uint32_t offset);

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

    cServerHttp * server;

    int curGroupChannel;        //текущая группа для отображения на экране

    bool plotReady;            // готовность CustomPlot для рисования графиков

    void reactOnMousePress();
    void reactOnMouseRelease();
    void ReactOnMouseSlide();


    int getStateSteel();
    cSteel *getReadySteel();
    cSteel *getMeasureSteel();
    void updateVer();
    void updateGroupWodgets();
    void initExtInterface();
    void extGetChannel(QString name);
    void extGetProtVer(QString name);
    void applyParam(QString name, uint8_t *data);
    void extSetDisplayMode(QString name, uint8_t * data);
    void extGetDisplayMode(QString name);

    int getIndexFreqBySlotAndCh(int slot, int ch);
    void extGetHardVer(QString name);
    void extGetSoftVer(QString name);
    void extGetDevType(QString name);
    void extGetDevName(QString name);
    void extGetManufact(QString name);
    void extGetWebSite(QString name);
    void extGetSerialNum(QString name);
    void extGetManufDate(QString name);
    void extGetDevID(QString name);
    void extGetDevState(QString name);
    void extGetWorkingTime(QString name);
    void extGetModbusErrs(QString name);
    void extGetModbusGoods(QString name);
    void extGetAccessType(QString name);
    void extGetCountModules(QString name);
    void extGetModelMatherboard(QString name);
    void extGetModuleHardVer(QString name);
    void extGetModuleProtocolVer(QString name);
    void extGetModulAccessType(QString name);
    void extGetModulIsOnline(QString name);
    void extGetModulIsErrors(QString name);
    void extGetModuleSerialNumber(QString name);
    void extGetModuleSoftVer(QString name);
    void extGetModuleFactoryDate(QString name);
    void extGetModuleType(QString name);
    void extGetChannelStatus(QString name);
    void extGetBroadcast(QString name);
    void extSetBroadcast(QString name, uint8_t *data);

    void extGetIpMask(QString name);
    void extSetIpMask(QString name, uint8_t *data);
    void extGetIpAddress(QString name);
    void extSetIpAddress(QString name, uint8_t *data);
    void extGetCurrentDate(QString name);
    void extSetCurrentDate(QString name, uint8_t *data);
    void extGetDevMode(QString name);
    void extSetDevMode(QString name, uint8_t *data);
    void extGetDisplayBrightness(QString name);
    void extSetDisplayBrightness(QString name, uint8_t *data);
    void extGetDisplayArrows(QString name);
    void extSetDisplayArrows(QString name, uint8_t *data);
    void extGetDisplayAutoscale(QString name);
    void extSetDisplayAutoscale(QString name, uint8_t *data);
    void extGetModbusRtuBaudrate(QString name);
    void extSetModbusRtuBaudrate(QString name, uint8_t *data);
    void extGetModbusSlave(QString name);
    void extSetModbusSlave(QString name, uint8_t *data);
    void extGetPortModbusType(QString name);
    void extSetPortModbusType(QString name, uint8_t *data);
    void extGetPortModbus(QString name);
    void extSetPortModbus(QString name, uint8_t *data);
    void extGetPortWebServer(QString name);
    void extSetPortWebServer(QString name, uint8_t *data);
    void extGetCurrentTime(QString name);
    void extSetCurrentTime(QString name, uint8_t *data);
    void extSetAccessPass(QString name, uint8_t *data);
    void extGetAccessPass(QString name);
    void extSetSetPoint(QString name, uint8_t *data);
    void extGetSetPoint(QString name);
    void extSetTotalSetpoint(QString name, uint8_t *data);
    void extGetTotalSetpoint(QString name);
    void extSetSetpointIdentifikator(QString name, uint8_t *data);
    void extGetSetpointIdentifikator(QString name);
    void extGetSetpointChannel(QString name);
    void extSetSetpointChannel(QString name, uint8_t *data);
    void extGetSetpointType(QString name);
    void extSetSetpointType(QString name, uint8_t *data);
    void extSetSetpointValue(QString name, uint8_t *data);
    void extGetSetpointValue(QString name);
    void extSetSetpointHister(QString name, uint8_t *data);
    void extGetSetpointHister(QString name);
    void extSetSetpointRelay(QString name, uint8_t *data);
    void extGetSetpointRelay(QString name);
    void extSetSetpointOverMess(QString name, uint8_t *data);
    void extGetSetpointOverMess(QString name);
    void extSetSetpointNormMess(QString name, uint8_t *data);
    void extGetSetpointNormMess(QString name);
    void extSetSetpointConfirm(QString name, uint8_t *data);
    void extGetSetpointConfirm(QString name);
    void extSetSetpointTimeFilter(QString name, uint8_t *data);
    void extGetSetpointTimeFilter(QString name);
    void extSetCurrentMath(QString name, uint8_t *data);
    void extGetCurrentMath(QString name);
    void extSetTotalMath(QString name, uint8_t *data);
    void extGetTotalMath(QString name);
    void extSetMathName(QString name, uint8_t *data);
    void extGetMathName(QString name);
    void extGetMathFormula(QString name);
    void extSetMathFormula(QString name, uint8_t *data);
    void extGetMathArgument(QString name);
    void extSetMathArgument(QString name, uint8_t *data);
    void extSetMathUnit(QString name, uint8_t *data);
    void extGetMathUnit(QString name);
    void extSetMathHiLimit(QString name, uint8_t *data);
    void extGetMathHiLimit(QString name);
    void extSetMathLowLimit(QString name, uint8_t *data);
    void extGetMathLowLimit(QString name);
    void extSetMathPeriod(QString name, uint8_t *data);
    void extGetMathPeriod(QString name);
    void extSetChannelSignalType(QString name, uint8_t *data);
    void extGetChannelSignalType(QString name);
    void extSetChannelAdditionalPar1(QString name, uint8_t *data);
    void extGetChannelAdditionalPar1(QString name);
    void extSetChannelAdditionalPar2(QString name, uint8_t *data);
    void extGetChannelAdditionalPar2(QString name);
    void extGetChannelCorrectionCj(QString name);
    void extSetChannelCorrectionCj(QString name, uint8_t *data);
    void extSetChannelLowLim(QString name, uint8_t *data);
    void extGetChannelLowLim(QString name);
    void extSetChannelHiLim(QString name, uint8_t *data);
    void extGetChannelHiLim(QString name);
    void extSetChannelName(QString name, uint8_t *data);
    void extGetChannelName(QString name);
    void extSetChannelPeriod(QString name, uint8_t *data);
    void extGetChannelPeriod(QString name);
    void extSetChannelTypeRegistration(QString name, uint8_t *data);
    void extGetChannelTypeRegistration(QString name);
    void extGetChannelDempher(QString name);
    void extSetChannelDempher(QString name, uint8_t *data);
    void extSetChannelTypeValue(QString name, uint8_t *data);
    void extGetChannelTypeValue(QString name);
    void extSetModuleMode(QString name, uint8_t *data);
    void extGetModuleMode(QString name);
    void extGetModuleModbusBaud(QString name);
    void extSetModuleModbusBaud(QString name, uint8_t *data);
    void extGetModuleModbusParity(QString name);
    void extSetModuleModbusParity(QString name, uint8_t *data);
    void extGetModuleModbusStopBits(QString name);
    void extSetModuleModbusStopBits(QString name, uint8_t *data);
    void extGetModuleModbusDataOrder(QString name);
    void extSetModuleModbusDataOrder(QString name, uint8_t *data);
    void extSetFreqSignalType(QString name, uint8_t *data);
    void extGetFreqSignalType(QString name);
    void extGetFreqImpulseDuration(QString name);
    void extSetFreqImpulseDuration(QString name, uint8_t *data);
    void extGetFreqAdditionalParameter(QString name);
    void extSetFreqAdditionalParameter(QString name, uint8_t *data);
    void extSetFreqImpWeight(QString name, uint8_t *data);
    void extGetFreqImpWeight(QString name);
    void extGetFreqLowMeasureLim(QString name);
    void extSetFreqLowMeasureLim(QString name, uint8_t *data);
    void extGetFreqHighMeasureLim(QString name);
    void extSetFreqHighMeasureLim(QString name, uint8_t *data);
    void extSetFreqPeriod(QString name, uint8_t *data);
    void extGetFreqPeriod(QString name);
    void extSetFreqDempher(QString name, uint8_t *data);
    void extGetFreqDempher(QString name);
    void extSetFreqTypeValue(QString name, uint8_t *data);
    void extGetFreqTypeValue(QString name);
    void extSetFreqName(QString name, uint8_t *data);
    void extGetFreqName(QString name);
    void extGetFreqUnit(QString name);
    void extSetFreqUnit(QString name, uint8_t *data);
    void extGetChanCalibr(QString name);
    void extSetChanCalibr(QString name, uint8_t *data);
    void extSetNeadCalibr(QString name, uint8_t *data);
    void extGetNeadCalibr(QString name);
    void extGetProcessReadCalibr(QString name);
    QString getNameDevice();
protected:
    void paintEvent(QPaintEvent *event) ;
};


#endif // MAINWINDOW_H
