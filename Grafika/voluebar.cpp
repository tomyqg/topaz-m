#include "voluebar.h"
#include "ui_voluebar.h"

wVolueBar::wVolueBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::wVolueBar)
{
    ui->setupUi(this);
}

wVolueBar::~wVolueBar()
{
    delete ui;
}

void wVolueBar::setVolue(double vol)
{
    int maxHeightBar = ui->placeBar->height();
    if(vol >= 0)
    {
        ui->curHBar->setGeometry(0, \
                                 maxHeightBar/2 * (1 - (vol / 50)), \
                                 ui->curHBar->width(), \
                                 vol * maxHeightBar/2 / 50 + 1);
    }
    else if(vol < 0)
    {
        ui->curHBar->setGeometry(0, \
                                 maxHeightBar/2 - 1, \
                                 ui->curHBar->width(), \
                                 vol * maxHeightBar/2 * (-1) / 50 );
    }
    ui->volBar->setText(QString::number(vol));
}
