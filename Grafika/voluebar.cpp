#include "voluebar.h"
#include "ui_voluebar.h"

wVolueBar::wVolueBar(/*int num, */QWidget *parent) :
//    numBar(num),
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

    //геометрия виждета текущей величины
    if(vol >= 0)
    {
        ui->curHBar->setGeometry(0, \
                                 ui->placeBar->height() / 2 * (1 - (vol / 50)), \
                                 ui->curHBar->width(), \
                                 vol * ui->placeBar->height() /2 / 50 + 1 \
                                 );
    }
    else if(vol < 0)
    {
        ui->curHBar->setGeometry(0, \
                                 ui->placeBar->height() / 2 - 1, \
                                 ui->curHBar->width(), \
                                 vol * ui->placeBar->height() /2 * (-1) / 50 \
                                 );
    }

    ui->volBar->setText(QString::number(vol, 'f', 2));


    //геометрия поля значения
    if(ui->curHBar->height() <= 20)
    {
        ui->volBar->setGeometry( \
                    0,  \
                    ui->placeBar->height() / 2 - ui->volBar->height() - 5, \
                    ui->placeBar->width(), \
                    20 \
                    );
    }
    else
    {
//        ui->volBar->setGeometry( \
//                    0,
//                    ui->curHBar->x() + (ui->curHBar->height() - ui->volBar->height()) / 2, \
//                    ui->placeBar->width(), \
//                    20 \
//                    );
        ui->volBar->setGeometry(ui->curHBar->geometry());
    }


}

void wVolueBar::resizeEvent(QResizeEvent * s)
{
    ui->placeBar->setGeometry(      \
                ui->placeBar->x(),  \
                ui->placeBar->y(),  \
                ui->placeBar->width(),      \
                ui->frameBar->height() - 12 \
                );
    ui->shadowBar->setGeometry(      \
                ui->shadowBar->x(),  \
                ui->shadowBar->y(),  \
                ui->shadowBar->width(),      \
                ui->frameBar->height() - 12 \
                );
}


void wVolueBar::setExtr(double min, double max)
{
    ui->amplBar->setGeometry(0, \
                             ui->placeBar->height() / 2 * (1 - (max / 50)), \
                             ui->amplBar->width(), \
                             ui->placeBar->height() / 2 * (max - min) / 50 + 1 \
                             );
}

void wVolueBar::setColor(QString cssColor, QString cssColorLight)
{
    ui->curHBar->setStyleSheet(cssColor);
    ui->amplBar->setStyleSheet(cssColorLight);
    ui->typeBar->setStyleSheet(cssColor);
    ui->mesBar->setStyleSheet(cssColor);
}

void wVolueBar::setText(QString type, QString mes)
{
    //реализовать тут контроль переданных строк
    if(type.size()>2)
        type.resize(2);
    if(mes.size()>2)
        mes.resize(2);

    if(numBar != 0)
    {
        ui->typeBar->setText("<html><head/><body><p>" + \
                             type + \
                             "<span style=\" vertical-align:sub;\">" + \
                             QString::number(numBar) + \
                             "</span></p></body></html>");
    }
    else
    {
        ui->typeBar->setText(type);
    }
    ui->mesBar->setText(mes);
}

void wVolueBar::changeNum(int num)
{
    if(num < 10)
        numBar = num;
}
