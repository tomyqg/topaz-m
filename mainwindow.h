#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <channel1.h>
#include "messages.h"

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
    ChannelOptions channel1object;
    ChannelOptions channel2object;
    ChannelOptions channel3object;
    ChannelOptions channel4object;

    MessageWrite mr;

    void ShowMessageBox (QString title,QString message);

public slots:

    void updateText( const QString text );
    void NewTouchscreenCalibration();
    void LabelsInit();
    void LabelsUpdate();
    void LabelsCorrect();
    void WriteGpio(int num, bool val);
    void WriteArchiveToFile();

    void HalfSecondGone();

    void UpdateDataChannel1();
    void UpdateDataChannel2();
    void UpdateDataChannel3();
    void UpdateDataChannel4();

    void UpdateDataChannel111();
    void UpdateDataChannel222();
    void UpdateDataChannel333();
    void UpdateDataChannel444();

    bool eventFilter(QObject* watched, QEvent* event);

private:

    void InitPins();
    void InitProcessorMaxFreq();
    void InitProcessorMinFreq();
    void InitTimers();
    void InitTouchScreen();
    void PaintOnWidget();
    void ReactOnTouch();
    void PaintCyfrasBottom();
    void PaintCyfrasFullScreen();
    void PaintPolarDiagramm();
    void PaintStatesAndAlertsAtTop();
    void InvertHalfSecFlag();
    void OpenMessagesWindow();
    void OpenOptionsWindow();
    void PowerOff();
    void CloseApplication();
    char GetHalfSecFlag();

    int GetTimePeriodSecs() {return 10;}
    int GetTickCountInOneSecond() {return 10;}
    int GetGraphWidthInPixels() {return 600;}

    int GetTotalLabelsCount() {return GetGraphWidthInPixels() / (GetTimePeriodSecs() * GetTickCountInOneSecond() ) ;}
    int GetTickStep() {return GetGraphWidthInPixels() / GetTotalLabelsCount() ;}

    QColor GetChannel1Color();
    QColor GetChannel2Color();
    QColor GetChannel3Color();
    QColor GetChannel4Color();

    void SetChannel1Color( QColor newcolor);
    void SetChannel2Color( QColor newcolor);
    void SetChannel3Color( QColor newcolor);
    void SetChannel4Color( QColor newcolor);

private slots:

    void updateDateLabel();
    void UpdateGraphics();
    void GrafsUpdateBars();
    void GrafsUpdateTrends();
    void GrafsUpdateTrendsAndBars();
    void GrafsUpdateNone();
    void AddValuesToBuffer();
    void DelaySec(int n);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

signals:
    void error(const QString &s);

private:
    Ui::MainWindow *ui;
    void MainWindowInitialization();

    char halfSecondflag;
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
    QThread *thread;

protected:

};

#endif // MAINWINDOW_H
