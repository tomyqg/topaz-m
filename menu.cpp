#include "menu.h"
#include "ui_menu.h"
#include "defines.h"


#define HEIGHT 768
#define WIDTH 1024



dMenu::dMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dMenu)
{
    ui->setupUi(this);

    ui->saveButton->setColorText(ColorBlue);
    ui->saveButton->setColorBg(QColor(0xff,0xff,0xff));
//    ui->saveButton->setText("СОХРАНИТЬ");
    ui->exitButton->setColorText(ColorBlue);
    ui->exitButton->setColorBg(QColor(0xff,0xff,0xff));
//    ui->exitButton->setText("ВЫХОД");
    ui->bWork->setColorText(QColor(0xff,0xff,0xff));
    ui->bWork->setColorBg(ColorButtonNormal);
    ui->bWork->setAlignLeft();
//    ui->bWork->setText("РАБОТА");
    ui->bSettings->setColorText(QColor(0xff,0xff,0xff));
    ui->bSettings->setColorBg(ColorButtonNormal);
    ui->bSettings->setAlignLeft();
//    ui->bSettings->setText("НАСТРОЙКИ");
    ui->bDiagnost->setColorText(QColor(0xff,0xff,0xff));
    ui->bDiagnost->setColorBg(ColorButtonNormal);
    ui->bDiagnost->setAlignLeft();
//    ui->bDiagnost->setText("ДИАГНОСТИКА");
    ui->bExpert->setColorText(QColor(0xff,0xff,0xff));
    ui->bExpert->setColorBg(ColorButtonNormal);
    ui->bExpert->setAlignLeft();
//    ui->bExpert->setText("ЭКСПЕРТ");

//    ui->load->setHidden(true);
//    mo.setFileName(pathtoloadgif);
//    ui->load->setMovie(&mo);
//    mo.start();

//    timerLoad = new QTimer(this);
//    connect(timerLoad, SIGNAL(timeout()), this, SLOT(timeoutLoad()));

    QPixmap pix(pathtologotip);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);
}

dMenu::~dMenu()
{
    delete ui;
}

void dMenu::on_exitButton_clicked()
{
    this->close();
}

void dMenu::on_saveButton_clicked()
{
//    ui->load->setHidden(false);
//    fTimeoutLoad = false;

    // засекаем время записи настроек в файл или ждать сигнал о завершении

//    timerLoad->start(1000);
    //ждать пока файл настроек не сохранится
//    while(!fTimeoutLoad);
//    mo.start();
    this->close();
}

//void dMenu::timeoutLoad()
//{
//    fTimeoutLoad = true;
//}
