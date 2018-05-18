#include "voluebar.h"
#include "ui_voluebar.h"

#define VOL_TEXT_MIN_HEIGHT 20
#define VOL_TEXT_PADDING_BOTTOM 5
#define BAR_WIDTH 90
#define BAR_PADDING_BOTTOM 12

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
    if(ui->curHBar->height() <= VOL_TEXT_MIN_HEIGHT)
    {
        //если поле в теле бара слишком узкое для текста
        ui->volBar->setGeometry( \
                    0,  \
                    ui->placeBar->height() / 2 - ui->volBar->height() - VOL_TEXT_PADDING_BOTTOM, \
                    ui->placeBar->width(), \
                    VOL_TEXT_MIN_HEIGHT \
                    );
    }
    else
    {
        // если места достаточно для размещения текста и выравнивания по середине
        ui->volBar->setGeometry(ui->curHBar->geometry());

//        //коррекция положения текста измеренной величины
//        if(ui->volBar->y() < VOL_TEXT_MIN_HEIGHT / 2 - ui->placeBar->height())
//            ui->volBar->setGeometry(ui->volBar->x(), 0, ui->volBar->width(), VOL_TEXT_MIN_HEIGHT );
//        if(ui->volBar->height() + VOL_TEXT_MIN_HEIGHT / 2 > ui->placeBar->height())
//            ui->volBar->setGeometry(ui->volBar->x(), \
//                                    ui->placeBar->height() - VOL_TEXT_MIN_HEIGHT, \
//                                    ui->volBar->width(), \
//                                    VOL_TEXT_MIN_HEIGHT );
    }


}

void wVolueBar::resizeEvent(QResizeEvent * s)
{
    ui->placeBar->setGeometry(      \
                ui->placeBar->x(),  \
                ui->placeBar->y(),  \
                ui->placeBar->width(),      \
                ui->frameBar->height() - BAR_PADDING_BOTTOM \
                );
    ui->shadowBar->setGeometry(      \
                ui->shadowBar->x(),  \
                ui->shadowBar->y(),  \
                ui->shadowBar->width(),      \
                ui->frameBar->height() - BAR_PADDING_BOTTOM \
                );
    ui->lineZero->setGeometry(  \
                0,  \
                ui->placeBar->height()/2 - 1,  \
                ui->placeBar->width(),      \
                2 );


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
