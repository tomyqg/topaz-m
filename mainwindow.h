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

//    UartDriver UD;
    ChannelOptions ch1;
    ChannelOptions ch2;
    ChannelOptions ch3;
    ChannelOptions ch4;

    MessageWrite mr;

public slots:

    void updateText( const QString text );
    void touchupdate();
    void InitLabels();
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
    void InitProcessor();
    void InitTimers();
    void InitTouchScreen();
    void PaintOnWidget();
    void PaintCyfrasBottom();
    void PaintCyfrasFullScreen();
    void PaintPolarDiagramm();
    char GetHalfSecFlag();
    void InvertHalfSecFlag();

private slots:

    void on_lcdNumber_overflow();
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
    QTimer *tmr;
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
    QThread *thread;

    QTimer *UpdateGraficsTimer;

    QTimer *halfSecondTimer;

protected:

};

#endif // MAINWINDOW_H
