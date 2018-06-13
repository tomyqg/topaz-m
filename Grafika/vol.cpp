#include "vol.h"
#include "ui_vol.h"

#define PADDING_FRAME   10
#define SHIFT_SHADOW    7

wVol::wVol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wVol)
{
    ui->setupUi(this);
}

wVol::~wVol()
{
    delete ui;
}

void wVol::resizeEvent(QResizeEvent * s)
{
    int sizeRect = ui->frameName->width();  //ну или  ui->frameMes->width()

    ui->frameMes->setGeometry(ui->frameMes->x(), ui->frameMes->y(), sizeRect, sizeRect);
    ui->frameName->setGeometry(ui->frameName->x(), ui->frameName->y(), sizeRect, sizeRect);

    ui->labelVol->setGeometry(0, 0, ui->frameVol->width() - PADDING_FRAME, \
                                    sizeRect - PADDING_FRAME);
    ui->shadowVol->setGeometry(SHIFT_SHADOW, SHIFT_SHADOW, ui->frameVol->width() - PADDING_FRAME, \
                                                            sizeRect - PADDING_FRAME);
    ui->labelMes->setGeometry(0, 0, ui->frameMes->width() - PADDING_FRAME, \
                                    ui->frameMes->height() - PADDING_FRAME);
    ui->shadowMes->setGeometry(SHIFT_SHADOW, SHIFT_SHADOW, ui->frameMes->width() - PADDING_FRAME, \
                                                            ui->frameMes->height() - PADDING_FRAME);
    ui->labelName->setGeometry(0, 0, ui->frameName->width() - PADDING_FRAME, \
                                    ui->frameName->height() - PADDING_FRAME);
    ui->shadowName->setGeometry(SHIFT_SHADOW, SHIFT_SHADOW, ui->frameName->width() - PADDING_FRAME, \
                                                            ui->frameName->height() - PADDING_FRAME);
}

void wVol::setText(QString name, QString mes)
{
    //реализовать тут контроль переданных строк
    if(name.size()>2)
        name.resize(2);
    if(mes.size()>2)
        mes.resize(2);

    if(numBar != 0)
    {
        ui->labelName->setText("<html><head/><body><p>" + \
                             name + \
                             "<span style=\" vertical-align:sub;\">" + \
                             QString::number(numBar) + \
                             "</span></p></body></html>");
    }
    else
    {
        ui->labelName->setText(name);
    }
    ui->labelMes->setText(mes);
}

/*
 * Задание номера канала, индекс
 */
void wVol::changeNum(int num)
{
    if(num < 10)
        numBar = num;
}

void wVol::setVol(double vol)
{
    int prec;
    if(vol >= 1000) prec = 0;
    else if(vol >= 100) prec = 1;
    else if(vol >= 10) prec = 2;
    else prec = 3;
    ui->labelVol->setText(QString::number(vol, 'f', prec));
}
