#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QString starttime;
    static QString endtime;

public slots:

    void focusChanged(QWidget* , QWidget* );
    void touchupdate();
    void WritetoFile();
    void WriteArchiveToFile();

private slots:

    void on_dial_sliderMoved(int position);
    void on_lcdNumber_overflow();
    void on_dial_actionTriggered(int action);
    void updateCaption();
    void updatepicture();
    void updategraph();
    void updatevalue();
    void mousePress();
    void delay(int n);
    double returnmathresult(double dval);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_2_pressed();
    void on_checkBox_stateChanged(int arg1);
    void on_textEdit_textChanged();
    void on_horizontalSlider_actionTriggered(int action);
    void on_dial_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_horizontalSlider_2_valueChanged(int value);
    void on_comboBox_13_currentTextChanged(const QString &arg1);
    void on_comboBox_13_currentIndexChanged(int index);
    void on_comboBox_13_activated(const QString &arg1);
    void on_comboBox_13_currentIndexChanged(const QString &arg1);
    void on_pushButton_4_clicked();
    void on_horizontalSlider_2_actionTriggered(int action);

    void on_customPlot_destroyed();

    void on_pushButton_5_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

signals:
    void error(const QString &s);

private:

    Ui::MainWindow *ui;
    QTimer *tmr;
    QTimer *channeltimer1;
    QTimer *channeltimer2;
    QTimer *channeltimer3;
    QTimer *channeltimer4;
    QMutex mutex;

protected:
    void paintEvent(QPaintEvent *e);
};




#endif // MAINWINDOW_H
