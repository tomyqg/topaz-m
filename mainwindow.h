#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>


namespace Ui {
class MainWindow;
}

class NewThreadClass:public QObject
{
   Q_OBJECT

private slots:
   void updatethread();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void focusChanged(QWidget* , QWidget* );
    void textupdate();

private slots:


    void on_dial_sliderMoved(int position);
    void on_lcdNumber_overflow();
    void on_dial_actionTriggered(int action);
    void updateCaption();
    void updatepicture();
    void updategraph();
    void updatevalue();
    void mousePress();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_2_pressed();
    void on_checkBox_stateChanged(int arg1);
    void on_textEdit_textChanged();
    double returnmathresult(double dval);
    void on_horizontalSlider_actionTriggered(int action);
    void on_dial_valueChanged(int value);
    void on_pushButton_3_clicked();
    void delay(int n);
    void on_horizontalSlider_2_valueChanged(int value);

    void on_comboBox_13_currentTextChanged(const QString &arg1);

    void on_comboBox_13_currentIndexChanged(int index);

    void on_comboBox_13_activated(const QString &arg1);

    void on_comboBox_13_currentIndexChanged(const QString &arg1);

signals:

    void error(const QString &s);

private:
    Ui::MainWindow *ui;
    QTimer *tmr;
    QMutex mutex;

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent* event);
    void touchReleaseEvent(QTouchEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

protected:
     bool eventFilter(QObject *obj, QEvent *event);


};

#endif // MAINWINDOW_H
