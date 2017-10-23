#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <channelOptions.h>
#include "messages.h"
#include "worker.h"
#include "mathresolver.h"
#include "uartdriver.h"
#include "src/modbus.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //UartDriver UD;
    ChannelOptions channel1;
    ChannelOptions channel2;
    ChannelOptions channel3;
    ChannelOptions channel4;

    mathresolver eee;

    MessageWrite messwrite;

    void ShowMessageBox (QString title,QString message);

    void resizeWindow(QWidget & qw, qreal mratio);
    //    void resizeWidgets(QObject & qobj, qreal xratio, qreal yratio);
    void resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution);
    void resizeSelf(qreal xresolution, qreal yresolution);

    bool needupdatePainter;
    bool needConfirmationchannel1;
    bool needConfirmationchannel2;
    bool needConfirmationchannel3;
    bool needConfirmationchannel4;

    void GetAllUartPorts();
    int GetWindowWidthPixels();
    int GetWindowHeightPixels();

    int GetMonitorWidthPixels();
    int GetMonitorHeightPixels();

    void SetWindowWidthPixels(int neww);
    void SetWindowHeightPixels(int newh);

public slots:

    void destroyedslot(QObject *);
    void NewTouchscreenCalibration();
    void LabelsInit();
    void LabelsUpdate();
    void LabelsCorrect();
    void WriteGpio(int num, bool val);
    void WriteArchiveToFile();
    void CreateMODBusConfigFile();
    void resetStatus( void );
    void HalfSecondGone();
    void ModbusConnectionErrorSlot();
    void SetEcoMode(bool seteco);

    bool GetEcoMode();

    void UpdateChannel1Slot();
    void UpdateChannel2Slot();
    void UpdateChannel3Slot();
    void UpdateChannel4Slot();
    bool eventFilter(QObject* watched, QEvent* event);

private:

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
    void CheckState(ChannelOptions&  channel);
    void SetXRange(int newxrange) {Xrange = newxrange;}
    void SetYRange(int newyrange) {Yrange = newyrange;}
    uint8_t GetHalfSecFlag();
    int  GetXRange() {return Xrange;}
    int  GetYRange() {return Yrange;}
    int GetTimePeriodSecs() {return 20;}
    int GetTickCountInOneSecond() {return 5;}
    int GetGraphWidthInPixels() {return 600;}
    int GetTotalLabelsCount() {return GetGraphWidthInPixels() / (GetTimePeriodSecs() * GetTickCountInOneSecond() ) ;}
    int GetTickStep() {return GetGraphWidthInPixels() / GetTotalLabelsCount() ;}
    int dateindex;
    bool EcoMode;
//    QVector<QString> datestrings;
//    QVector<QString> timestrings;

    QStringList datestrings, timestrings;

private slots:
    void OpenSerialPort( int );
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
    void ChangePalette(int i);
    void on_WorkButton_clicked();
    void on_ArchiveButton_clicked();
    void on_EcoCheckBox_toggled(bool checked);
    void on_smoothCheckBox_toggled(bool checked);
    void on_timeButton_clicked();

signals:
    void error(const QString &s);
    void ThreadSignal(ChannelOptions*  channel);

signals:
  void startWorkSignal();
  void stopWorkSignal();
  void SendObjectsToWorker(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4);

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
    QTimer *tmr;

    QThread *WorkerThread;
    QThread *thread;
    QThread *optionsthread;

    QMutex *mutex;

    int Xrange;
    int Yrange;
    int windowwidth;
    int windowheight;

protected:
    void paintEvent(QPaintEvent *event) ;
};

#endif // MAINWINDOW_H
