#include "settings.h"
#include "ui_settings.h"
#include "defines.h"

dSettings::dSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dSettings)
{
    ui->setupUi(this);

    ui->saveButton->setColorText(ColorBlue);
    ui->saveButton->setColorBg(QColor(0xff,0xff,0xff));
    ui->exitButton->setColorText(ColorBlue);
    ui->exitButton->setColorBg(QColor(0xff,0xff,0xff));

    ui->load->setHidden(true);
    mo.setFileName(pathtoloadgif);
    ui->load->setMovie(&mo);
    mo.start();

    QPixmap pix(pathtologotip);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    connect(&timerLoad, SIGNAL(timeout()), this, SLOT(timeoutLoad()));

    ui->buttonUstavk->setColorText(ColorBlue);
    ui->buttonUstavk->setColorBg(QColor(0xff,0xff,0xff));

    connect(&tUpdateTime, SIGNAL(timeout()), this, SLOT(DateUpdate()));
    tUpdateTime.start(DateLabelUpdateTimer);
    DateUpdate();

}

dSettings::~dSettings()
{
    delete ui;
}

void dSettings::on_saveButton_clicked()
{
    ui->load->setHidden(false);
//     засекаем время записи настроек в файл или ждать сигнал о завершении
    timerLoad.start(1000);
    // ... тут вставить запись файла в потоке ... //
    //Окно закроется по сигналу таймаута
}

void dSettings::on_exitButton_clicked()
{
    this->close();
}

void dSettings::timeoutLoad()
{
    this->close();
}

void dSettings::on_buttonUstavk_clicked()
{
    //тут запускать окно настройки уставок или переключить вид главного фрэйма
}

void dSettings::DateUpdate() // каждую секунду обновляем значок времени
{
    QDateTime local(QDateTime::currentDateTime());
    QString str = "<html><head/><body><p align=\"center\"><span style=\" font-size:22pt; color:#ffffff;\">" \
                  + local.time().toString("hh:mm:ss") + \
                  "</span><span style=\" color:#ffffff;\"><br/></span>" \
                  "<span style=\" font-size:17pt; color:#ffffff;\">" \
                  + local.date().toString("dd.MM.yyyy") + \
                  "</span></p></body></html>";
    ui->date_time->setText(str);
}
