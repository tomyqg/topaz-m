#include "customperiod.h"
#include "ui_custom_period.h"
#include <QDateTime>
#include "defines.h"

QDateTime cCustomPeriod::startDT = QDateTime::currentDateTime().addDays(-7);
QDateTime cCustomPeriod::finishDT = QDateTime::currentDateTime();

cCustomPeriod::cCustomPeriod(QDateTime s, QDateTime f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cCustomPeriod)
{
    ui->setupUi(this);
    start = s;
    finish = f;
    startDT = s;
    finishDT = f;
    ui->bOK->setColorText(ColorBlue);
    ui->bOK->setColorBg(QColor(0xf0,0xf0,0xf0));
    ui->bCancel->setColorText(ColorBlue);
    ui->bCancel->setColorBg(QColor(0xf0,0xf0,0xf0));
    updUiTimeDate();

}

cCustomPeriod::~cCustomPeriod()
{
    delete ui;
}

void cCustomPeriod::updUiTimeDate()
{
    if(start > finish)
    {
        start = startDT;
        finish = finishDT;
    }
    ui->startDay->setDateTime(start);
    ui->startMonth->setDateTime(start);
    ui->startYear->setDateTime(start);
    ui->startHour->setDateTime(start);
    ui->startMin->setDateTime(start);
    ui->startSec->setDateTime(start);
    ui->finishDay->setDateTime(finish);
    ui->finishMonth->setDateTime(finish);
    ui->finishYear->setDateTime(finish);
    ui->finishHour->setDateTime(finish);
    ui->finishMin->setDateTime(finish);
    ui->finishSec->setDateTime(finish);
}

void cCustomPeriod::on_startDayUp_clicked()
{
    start = start.addDays(1);
    updUiTimeDate();
}

void cCustomPeriod::on_startMonthUp_clicked()
{
    start = start.addMonths(1);
    updUiTimeDate();
}

void cCustomPeriod::on_startYearUp_clicked()
{
    start = start.addYears(1);
    updUiTimeDate();
}

void cCustomPeriod::on_startDayDown_clicked()
{
    start = start.addDays(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_startMonthDown_clicked()
{
    start = start.addMonths(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_startYearDown_clicked()
{
    start = start.addYears(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_startHourUp_clicked()
{
    start = start.addSecs(3600);
    updUiTimeDate();
}

void cCustomPeriod::on_startMinUp_clicked()
{
    start = start.addSecs(60);
    updUiTimeDate();
}

void cCustomPeriod::on_startSecUp_clicked()
{
    start = start.addSecs(1);
    updUiTimeDate();
}

void cCustomPeriod::on_startHourDown_clicked()
{
    start = start.addSecs(-3600);
    updUiTimeDate();
}

void cCustomPeriod::on_startMinDown_clicked()
{
    start = start.addSecs(-60);
    updUiTimeDate();
}

void cCustomPeriod::on_startSecDown_clicked()
{
    start = start.addSecs(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishDayUp_clicked()
{
    finish = finish.addDays(1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishMonthUp_clicked()
{
    finish = finish.addMonths(1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishYearUp_clicked()
{
    finish = finish.addYears(1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishDayDown_clicked()
{
    finish = finish.addDays(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishMonthDown_clicked()
{
    finish = finish.addMonths(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishYearDown_clicked()
{
    finish = finish.addYears(-1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishHourUp_clicked()
{
    finish = finish.addSecs(3600);
    updUiTimeDate();
}

void cCustomPeriod::on_finishMinUp_clicked()
{
    finish = finish.addSecs(60);
    updUiTimeDate();
}

void cCustomPeriod::on_finishSecUp_clicked()
{
    finish = finish.addSecs(1);
    updUiTimeDate();
}

void cCustomPeriod::on_finishHourDown_clicked()
{
    finish = finish.addSecs(-3600);
    updUiTimeDate();
}

void cCustomPeriod::on_finishMinDown_clicked()
{
    finish = finish.addSecs(-60);
    updUiTimeDate();
}

void cCustomPeriod::on_finishSecDown_clicked()
{
    finish = finish.addSecs(-1);
    updUiTimeDate();
}


void cCustomPeriod::on_bOK_clicked()
{
    startDT = start;
    finishDT = finish;
    this->close();
}

void cCustomPeriod::on_bCancel_clicked()
{
    this->close();
}
