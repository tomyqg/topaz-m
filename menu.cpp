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
    ui->exitButton->setColorText(ColorBlue);
    ui->exitButton->setColorBg(QColor(0xff,0xff,0xff));
    ui->bWork->setColorText(QColor(0xff,0xff,0xff));
    ui->bWork->setColorBg(ColorButtonNormal);
    ui->bWork->setAlignLeft();
    ui->bSettings->setColorText(QColor(0xff,0xff,0xff));
    ui->bSettings->setColorBg(ColorButtonNormal);
    ui->bSettings->setAlignLeft();
    ui->bDiagnost->setColorText(QColor(0xff,0xff,0xff));
    ui->bDiagnost->setColorBg(ColorButtonNormal);
    ui->bDiagnost->setAlignLeft();
    ui->bExpert->setColorText(QColor(0xff,0xff,0xff));
    ui->bExpert->setColorBg(ColorButtonNormal);
    ui->bExpert->setAlignLeft();
}

dMenu::~dMenu()
{
    delete ui;
}
