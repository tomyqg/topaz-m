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

public slots:
   void updatethread();

signals:
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_dial_sliderMoved(int position);

    void on_lcdNumber_overflow();

    void on_dial_actionTriggered(int action);

    void updateCaption();
    void updatepicture();
    void updategraph();
    void updatevalue();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_2_pressed();

    void on_checkBox_stateChanged(int arg1);

    void on_textEdit_textChanged();

    double returnmathresult(double dval);

    void on_horizontalSlider_actionTriggered(int action);

    void on_dial_valueChanged(int value);

    void on_pushButton_3_clicked();

signals:

    void error(const QString &s);

private:
    Ui::MainWindow *ui;
    QMutex mutex;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
