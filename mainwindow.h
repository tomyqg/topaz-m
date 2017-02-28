#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <channel1.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QString startdate;
    static QString endtime;
    static QString starttime;

    QTimer *channeltimer1 ;
    QTimer *channeltimer2;
    QTimer *channeltimer3;
    QTimer *channeltimer4;

//    UartDriver UD;
    ChannelOptions ch1;
    ChannelOptions ch2;
    ChannelOptions ch3;
    ChannelOptions ch4;

public slots:

    void focusChanged(QWidget* , QWidget* );
    void touchupdate();
    void WriteGpio();
    void WriteArchiveToFile();

    void UpdateDataChannel1();
    void UpdateDataChannel2();
    void UpdateDataChannel3();
    void UpdateDataChannel4();

private slots:

    void on_dial_sliderMoved(int position);
    void on_lcdNumber_overflow();
    void on_dial_actionTriggered(int action);
    void updateCaption();
    void updatepicture();
    void updategraph();
    void updatevalue();
    void delay(int n);
    double returnmathresult(double dval);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_2_pressed();
    void on_textEdit_textChanged();
    void on_dial_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_customPlot_destroyed();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();

signals:
    void error(const QString &s);

private:

    Ui::MainWindow *ui;
    QTimer *tmr;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
