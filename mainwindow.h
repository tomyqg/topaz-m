#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <channelOptions.h>
#include <QGraphicsScene>
#include <QQueue>
#include "messages.h"
#include "worker.h"
#include "mathresolver.h"
#include "uartdriver.h"
#include "src/modbus.h"
#include "device.h"
#include "transaction.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ShowMessageBox (QString title,QString message);
    void resizeWindow(QWidget & qw, qreal mratio);
    void resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution);
    void resizeSelf(qreal xresolution, qreal yresolution);

    int GetWindowWidthPixels();
    int GetWindowHeightPixels();
    int GetMonitorWidthPixels();
    int GetMonitorHeightPixels();
    void SetWindowWidthPixels(int neww);
    void SetWindowHeightPixels(int newh);
    void GetAllUartPorts();

    int GetXOffsetForAlign(int smallrectinglewidth, QGraphicsTextItem *ChannelValueText, int alerttextsize);
    int GetXOffset(int smallrectinglewidth, QGraphicsTextItem *ChannelValueText);

    Device device;
    QQueue<Transaction> queueTransaction;
//    transaction device1;
//    transaction device2;
//    transaction device3;
//    transaction device4;
//    transaction typeSign1;


public slots:

    void destroyedslot(QObject *);
    void NewTouchscreenCalibration();
    void LabelsInit();
    void LabelsUpdate();
    void LabelsCorrect();
    void WriteGpio(int num, bool val);
    void RefreshScreen();
    void WriteArchiveToFile();
    void CreateMODBusConfigFile();
    void resetStatus( void );
    void HalfSecondGone();
    void ModbusConnectionErrorSlot();
    void SetEcoMode(bool EcoMode);
    void UpdateChannel1Slot();
    void UpdateChannel2Slot();
    void UpdateChannel3Slot();
    void UpdateChannel4Slot();
    void UpdSignalTypeSlot(uint8_t ch);
    bool GetEcoMode();
    void getTransFromWorkerSlot(Transaction tr);
    void releOutSlot(uint8_t code);
    void readReleSlot(uint8_t code);


private:

    ChannelOptions channel1;
    ChannelOptions channel2;
    ChannelOptions channel3;
    ChannelOptions channel4;

    MessageWrite messwrite;

    bool eventFilter(QObject* watched, QEvent* event);
    bool needupdatePainter;
    bool needConfirmationchannel1;
    bool needConfirmationchannel2;
    bool needConfirmationchannel3;
    bool needConfirmationchannel4;
    void InitPins();
    void InitProcessorMaxFreq();
    void InitProcessorMinFreq();
    void InitTimers();
    void InitTouchScreen();
    void PaintOnWidget();
    void ReactOnTouch();
    void ReactOnMouseSlide();
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
    void CheckAndLogginStates(ChannelOptions&  channel);
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
    int dateindex;
    bool EcoMode;
    int polar_angle;

    QGraphicsScene  *scene;

    QStringList datestrings, timestrings;

private slots:
//    void OpenSerialPort( int );
    void updateDateLabel();
    void UpdateGraphics();
    void GrafsUpdateBars();
    void GrafsUpdateTrends();
    void GrafsUpdateTrendsAndBars();
    void GrafsUpdateNone();
    void AddValuesToBuffer();
    void DelaySec(int n);
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void sendModbusRequest( );
    void ChangePalette(bool i);
    void on_WorkButton_clicked();
    void on_ArchiveButton_clicked();
    void on_EcoCheckBox_toggled(bool checked);
    void on_timeButton_clicked();
    void on_bWriteTypeSignal_clicked();

signals:
    void error(const QString &s);
    void ThreadSignal(ChannelOptions*  channel);
    void startWorkSignal();
    void stopWorkSignal();
//    void SendObjectsToWorker(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4);
    void sendTransToWorker(Transaction tr);
    void setReleToOptionsForm(int code);

private:
    Ui::MainWindow *ui;

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
    QTimer *halfSecondTimer;
    QTimer *displayrefreshtimer;
    QTimer *tmr;

    QThread *WorkerThread;

    QMutex *mutex;

    int Xrange;
    int Yrange;
    int windowwidth;
    int windowheight;

    void DrawRectangles(QList<ChannelOptions *> ChannelsObjectsList, int alerttextsize, int smalltextsize);
    void DrawAlertsAndStatesRectangles(QList<ChannelOptions *> ChannelsObjectsList, QPainter painter, int alerttextsize, int smalltextsize);
    void DrawAlertsAndStatesRectangles(QList<ChannelOptions *> ChannelsObjectsList, int smalltextsize, int alerttextsize);
    void SetChannelRectPosition(int alertwindowwidth, int alertwindowheight);
    void PaintCyfrasBottomSeparate();
    void CheckState(ChannelOptions &channel);
    void DrawScene();
    void DrawSceneBottom();


protected:
    void paintEvent(QPaintEvent *event) ;
};

#endif // MAINWINDOW_H
