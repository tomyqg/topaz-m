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

private slots:
    void on_dial_sliderMoved(int position);

    void on_lcdNumber_overflow();

    void on_dial_actionTriggered(int action);

    void updateCaption();
    void updatepicture();
    void updategraph();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_2_pressed();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
