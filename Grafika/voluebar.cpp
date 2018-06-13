#include "voluebar.h"
#include "ui_voluebar.h"
#include "defines.h"

#define VOL_TEXT_MIN_HEIGHT 20
#define VOL_TEXT_PADDING_BOTTOM 5
#define BAR_WIDTH 90
#define BAR_PADDING_BOTTOM 12
#define METKA_HEIGHT    16

wVolueBar::wVolueBar(/*int num, */QWidget *parent) :
//    numBar(num),
    QFrame(parent),
    ui(new Ui::wVolueBar)
{
    ui->setupUi(this);
    ui->metkaLow->setPixmap(QPixmap(pathtolowlimico));
    ui->metkaHi->setPixmap(QPixmap(pathtohilimico));
    ui->curHBar->setGeometry(ui->curHBar->x(), ui->curHBar->y(),\
                             ui->curHBar->width(), 0);
    ui->volBar->setText("0.00");
    ui->volBar->setGeometry( \
                0, ui->placeBar->height() / 2 - ui->volBar->height() - VOL_TEXT_PADDING_BOTTOM, \
                ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT \
                );
    razmah = 50;
    numBar = 0;

    //установка фильтра событий на лэйблы виджет-бара
    QList<QLabel*> labelList = findChildren<QLabel*>();
    foreach (QLabel * label, labelList) {
        label->installEventFilter(this);
    }
}

wVolueBar::~wVolueBar()
{
    delete ui;
}

/*
 * Задание текущей величины
 */
void wVolueBar::setVolue(double vol)
{

    //геометрия виждета текущей величины
    if(vol >= 0)
    {
        ui->curHBar->setGeometry(0, \
                                 (ui->placeBar->height() / 2) * (1 - (vol / razmah)), \
                                 ui->placeBar->width(), \
                                 (vol / razmah) * (ui->placeBar->height() / 2) \
                                 );
    }
    else if(vol < 0)
    {
        ui->curHBar->setGeometry(0, \
                                 ui->placeBar->height() / 2, \
                                 ui->placeBar->width(), \
                                 vol * ui->placeBar->height() / 2 * (-1) / razmah \
                                 );
    }

    ui->volBar->setText(QString::number(vol, 'f', 2));


    //геометрия поля значения
    if(ui->curHBar->height() <= VOL_TEXT_MIN_HEIGHT)
    {
        //если поле в теле бара слишком узкое для текста
        if(vol >= 0)
            ui->volBar->setGeometry( \
                        0, ui->placeBar->height() / 2 - ui->volBar->height() - VOL_TEXT_PADDING_BOTTOM, \
                        ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT \
                        );
        else
            ui->volBar->setGeometry( \
                        0, ui->placeBar->height() / 2, \
                                    ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT \
                        );

    }
    else
    {
        // если места достаточно для размещения текста и выравнивания по середине
        ui->volBar->setGeometry(ui->curHBar->geometry());

        //коррекция положения текста измеренной величины
        if(ui->curHBar->y() - VOL_TEXT_MIN_HEIGHT + ui->placeBar->height() / 2 < 0)
            ui->volBar->setGeometry(ui->placeBar->x(), 0, ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT );
        else if(ui->curHBar->height() + VOL_TEXT_MIN_HEIGHT > ui->placeBar->height())
            ui->volBar->setGeometry(ui->placeBar->x(), \
                                    ui->placeBar->height() - VOL_TEXT_MIN_HEIGHT, \
                                    ui->placeBar->width(), \
                                    VOL_TEXT_MIN_HEIGHT );
    }


}

/*
 * Перерисовка бара
 */
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

/*
 * Задание экстремумов - минимальное и максимальное значения в буфере измерения
 */

void wVolueBar::setExtr(double min, double max)
{
    ui->amplBar->setGeometry(0, \
                             (ui->placeBar->height() / 2) * (1 - (max / razmah)), \
                             ui->placeBar->width(), \
                             (ui->placeBar->height() / 2) * (max - min) / razmah + 1 \
                             );
}

/*
 * Установка меток уставок, предельные значения
 */
void wVolueBar::setLim(double low, double hi)
{
    ui->metkaLow->setGeometry(0, (ui->placeBar->height() / 2) * (1 - (low / razmah)) - METKA_HEIGHT / 2, \
                              ui->placeBar->width(), METKA_HEIGHT);
    ui->metkaHi->setGeometry(0, (ui->placeBar->height() / 2) * (1 - (hi / razmah)) - METKA_HEIGHT / 2, \
                              ui->placeBar->width(), METKA_HEIGHT);
}

/*
 * Установка стилей цветового оформления бара
 */
void wVolueBar::setColor(QColor color, QColor colorL)
{
    colorBar = color;
    colorLight = colorL;

    QString strCSS = QString("color: #FFFFFF; background-color: rgb(" \
                             + QString::number(color.red()) + "," \
                             + QString::number(color.green()) + ","\
                             + QString::number(color.blue()) + ");");
    ui->curHBar->setStyleSheet(strCSS);
    ui->typeBar->setStyleSheet(strCSS);
    ui->mesBar->setStyleSheet(strCSS);

    strCSS = QString("color: #FFFFFF; background-color: rgb(" \
                             + QString::number(colorL.red()) + "," \
                             + QString::number(colorL.green()) + ","\
                             + QString::number(colorL.blue()) + ");");
    ui->amplBar->setStyleSheet(strCSS);
}

/*
 * Задание названия измеряемой велчины и единиц измерения в сокращении 2 символа
 */

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

/*
 * Задание номера канала, индекс
 */
void wVolueBar::changeNum(int num)
{
    if(num < 10)
        numBar = num;
}

/*
 * Задание размаха значений, видимых в баре
 */
void wVolueBar::setBarDiapazon(double diap)
{
    razmah = diap;
}

bool wVolueBar::eventFilter(QObject* watched, QEvent* event)
{
    if(watched == ui->typeBar && event->type() == QEvent::HoverEnter) {
        emit clickedLabel(numBar);
    }

    if(watched == ui->typeBar && event->type() == QEvent::MouseButtonRelease) {
        emit clickedLabel(numBar);
    } else if(watched == ui->mesBar && event->type() == QEvent::MouseButtonRelease) {
        emit clickedLabel(numBar);
    }
    return QObject::eventFilter(watched, event);
}
