#ifndef CCUSTOMPERIOD_H
#define CCUSTOMPERIOD_H

#include <QWidget>
#include <QDialog>
#include <QDateTime>



namespace Ui {
class cCustomPeriod;
}

class cCustomPeriod : public QDialog
{
    Q_OBJECT

public:
    explicit cCustomPeriod(QDateTime s, QDateTime f, QWidget *parent = 0);
    ~cCustomPeriod();
    static QDateTime startDT;
    static QDateTime finishDT;

private:
    Ui::cCustomPeriod *ui;
    void updUiTimeDate();
    QDateTime start;
    QDateTime finish;

private slots:
    void on_startDayUp_clicked();
    void on_startDayDown_clicked();
    void on_startMonthUp_clicked();
    void on_startMonthDown_clicked();
    void on_startYearUp_clicked();
    void on_startYearDown_clicked();
    void on_startHourUp_clicked();
    void on_startHourDown_clicked();
    void on_startMinUp_clicked();
    void on_startMinDown_clicked();
    void on_startSecUp_clicked();
    void on_startSecDown_clicked();
    void on_finishDayUp_clicked();
    void on_finishDayDown_clicked();
    void on_finishMonthUp_clicked();
    void on_finishMonthDown_clicked();
    void on_finishYearUp_clicked();
    void on_finishYearDown_clicked();
    void on_finishHourUp_clicked();
    void on_finishHourDown_clicked();
    void on_finishMinUp_clicked();
    void on_finishMinDown_clicked();
    void on_finishSecUp_clicked();
    void on_finishSecDown_clicked();

    void on_bOK_clicked();
    void on_bCancel_clicked();
};

#endif // CCUSTOMPERIOD_H
