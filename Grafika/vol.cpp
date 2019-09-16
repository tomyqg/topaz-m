#include "vol.h"
#include "ui_vol.h"
#include "defines.h"

#define PADDING_FRAME   10
#define SHIFT_SHADOW    7

wVol::wVol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wVol)
{
    ui->setupUi(this);

    //установка фильтра событий на лэйблы виджет-бара
    QList<QLabel*> labelList = findChildren<QLabel*>();
    foreach (QLabel * label, labelList) {
        label->installEventFilter(this);
    }
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
    if(name.size()>8)
        name.resize(8);
    if(mes.size()>2)
        mes.resize(2);

    if(numChan != 0)
    {
//        ui->labelName->setText("<html><head/><body><p>" + \
//                             name + \
//                             "<span style=\" vertical-align:sub;\">" + \
//                             QString::number(numChan) + \
//                             "</span></p></body></html>");
        ui->labelName->setText("<html><head/><body><p><strong>" + \
                               name + \
                               "</strong></p></body></html>");
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

void wVol::setVol(double vol, int prec)
{
//    int prec;
    if(prec == 10)
    {
        if(abs(vol) >= 1000) prec = 0;
        else if(abs(vol) >= 100) prec = 1;
        else if(abs(vol) >= 10) prec = 2;
        else prec = 3;
    }
    if(!std::isnan(vol))
    {
        ui->labelVol->setText(QString::number(vol, 'f', prec));
    }
    else
    {
        ui->labelVol->setText(NaNMessage);
    }
}

void wVol::setColor(QColor color)
{
    QColor colorBar = color;

    QString strCSS = QString("color: #FFFFFF; background-color: rgb(" \
                             + QString::number(color.red()) + "," \
                             + QString::number(color.green()) + ","\
                             + QString::number(color.blue()) + ");");
    ui->labelName->setStyleSheet(strCSS);
    ui->labelVol->setStyleSheet(strCSS);
    ui->labelMes->setStyleSheet(strCSS);
}

bool wVol::eventFilter(QObject* watched, QEvent* event)
{

//    if(watched == ui->labelName && event->type() == QEvent::HoverEnter) {
//        emit clickedLabel(numBar);
//    }

    if(watched == ui->labelName && event->type() == QEvent::MouseButtonRelease) {
        emit clickedLabel(numBar);
    } else if(watched == ui->labelMes && event->type() == QEvent::MouseButtonRelease) {
        emit clickedLabel(numBar);
    }
    return QObject::eventFilter(watched, event);
}
