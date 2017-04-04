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

    QPen graphPen;

    static QString startdate;
    static QString endtime;
    static QString starttime;
    QVector<QString> Labels;
    static QVector<QDateTime> Dates;


    QTimer *channeltimer1 ;
    QTimer *channeltimer2;
    QTimer *channeltimer3;
    QTimer *channeltimer4;
    QTimer *labelstimer;
    QThread *thread;

//    UartDriver UD;
    ChannelOptions ch1;
    ChannelOptions ch2;
    ChannelOptions ch3;
    ChannelOptions ch4;

    MessageWrite mr;

public slots:

    void touchupdate();
    void LabelsInit();
    void LabelsUpdate();
    void LabelsCorrect();

    void WriteGpio(int num, bool val);
    void WriteArchiveToFile();

    void UpdateDataChannel1();
    void UpdateDataChannel2();
    void UpdateDataChannel3();
    void UpdateDataChannel4();

    void UpdateDataChannel111();
    void UpdateDataChannel222();
    void UpdateDataChannel333();
    void UpdateDataChannel444();

private slots:

    void on_dial_sliderMoved(int position);
    void on_lcdNumber_overflow();
    void on_dial_actionTriggered(int action);
    void updateCaption();
    void updatepicture();
    void updategraph();
    void delay(int n);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_2_pressed();
    void on_dial_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

signals:
    void error(const QString &s);

private:
    Ui::MainWindow *ui;
    QTimer *tmr;
    void Initialization();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
