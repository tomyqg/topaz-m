//#include <QtCore>
#include "voluebar.h"
#include "ui_voluebar.h"
#include "defines.h"
#include "math.h"

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
//    ui->metkaLow->setPixmap(QPixmap(pathtolowlimico));
//    ui->metkaHi->setPixmap(QPixmap(pathtohilimico));
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
void wVolueBar::setValue(double vol)
{
    int prec = 2;
    if(abs(vol) >= 1000) prec = 0;
    else if(abs(vol) >= 100) prec = 1;
    else if(abs(vol) >= 10) prec = 2;
    else prec = 3;

    double lineZeroY = ui->lineZero->y() + 1;

    //геометрия виждета текущей величины
    if(vol >= 0)
    {
//        ui->curHBar->setGeometry(0, \
                                 (ui->placeBar->height() / 2) * (1 - (vol / razmah)), \
                                 ui->placeBar->width(), \
                                 (vol / razmah) * (ui->placeBar->height() / 2) \
                                 );
        ui->curHBar->setGeometry(0, \
                                 lineZeroY - ui->placeBar->height() * (vol / razmah), \
                                 ui->placeBar->width(), \
                                 (vol / razmah) * ui->placeBar->height() \
                                 );
    }
    else if(vol < 0)
    {
//        ui->curHBar->setGeometry(0, \
                                 ui->placeBar->height() / 2, \
                                 ui->placeBar->width(), \
                                 vol * ui->placeBar->height() / 2 * (-1) / razmah \
                                 );
        ui->curHBar->setGeometry(0, \
                                 lineZeroY, \
                                 ui->placeBar->width(), \
                                 (abs(vol) / razmah) * ui->placeBar->height() \
                                 );
    }

    QString valBar;
    if(!std::isnan(vol))
    {
        valBar = QString::number(vol, 'f', prec);
    }
    else
    {
        valBar = NaNMessage;
    }

    ui->volBar->setText(valBar);

    //геометрия поля значения
    if(ui->curHBar->height() <= VOL_TEXT_MIN_HEIGHT)
    {
        //если поле в теле бара слишком узкое для текста
        if(vol >= 0)
            ui->volBar->setGeometry( \
                        0, lineZeroY - ui->volBar->height() - VOL_TEXT_PADDING_BOTTOM, \
                        ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT \
                        );
        else
            ui->volBar->setGeometry( \
                        0, lineZeroY, \
                        ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT \
                        );

    }
    else
    {
        // если места достаточно для размещения текста и выравнивания по середине
        ui->volBar->setGeometry(ui->curHBar->geometry());
    }

    //коррекция положения текста измеренной величины
    if(ui->curHBar->y() + (ui->curHBar->height() - VOL_TEXT_MIN_HEIGHT) / 2 < 0)
    {
        ui->volBar->setGeometry(ui->placeBar->x(), 0, ui->placeBar->width(), VOL_TEXT_MIN_HEIGHT );
    }
    else if(ui->curHBar->y() + (ui->curHBar->height() + VOL_TEXT_MIN_HEIGHT) / 2 > ui->placeBar->height())
    {
        ui->volBar->setGeometry(ui->placeBar->x(), \
                                ui->placeBar->height() - VOL_TEXT_MIN_HEIGHT, \
                                ui->placeBar->width(), \
                                VOL_TEXT_MIN_HEIGHT );
    }

    QList<QLabel *> metki = ui->placeBar->findChildren<QLabel *>();
    foreach(QLabel * metka, metki)
    {
        if(metka->objectName().contains("metka"))
        {
            metka->deleteLater();
        }
    }

    int i = 0;
    foreach (cMarker * mark, listMarker) {
        QLabel * metka = new QLabel(ui->placeBar);
        metka->setObjectName(QStringLiteral("metka") + QString::number(i++));
        metka->setGeometry(0, (lineZeroY - ui->placeBar->height() * (mark->vol / razmah)) - METKA_HEIGHT / 2, \
                                  ui->placeBar->width(), METKA_HEIGHT);
        metka->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0);"));
        if(mark->dir)
        {
            metka->setPixmap(QPixmap(QString/*::fromUtf8*/(pathtohilimico)));
        } else {
            metka->setPixmap(QPixmap(QString/*::fromUtf8*/(pathtolowlimico)));
        }
        metka->show();
        metka->raise();
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
//    ui->lineZero->setGeometry(  \
                0,  \
                ui->placeBar->height()/2 - 1,  \
                ui->placeBar->width(),      \
                2 );
    ui->lineZero->setGeometry(  \
                0,  \
                (higherMeasure / razmah) * ui->placeBar->height() - 1,  \
                ui->placeBar->width(),      \
                2 );
}

/*
 * Задание экстремумов - минимальное и максимальное значения в буфере измерения
 */

void wVolueBar::setExtr(double minimum, double maximum)
{
    double lineZeroY = ui->lineZero->y() + 1;
    ui->amplBar->setGeometry(0, \
                             lineZeroY - ui->placeBar->height() * (maximum / razmah), \
                             ui->placeBar->width(), \
                             ui->placeBar->height() * (maximum - minimum) / razmah \
                             );
}

/*
 * Установка меток уставок, предельные значения
 */
void wVolueBar::setLim(double low, double hi)
{
//    ui->metkaLow->setGeometry(0, (ui->placeBar->height() / 2) * (1 - (low / razmah)) - METKA_HEIGHT / 2, \
//                              ui->placeBar->width(), METKA_HEIGHT);
//    ui->metkaHi->setGeometry(0, (ui->placeBar->height() / 2) * (1 - (hi / razmah)) - METKA_HEIGHT / 2, \
//                             ui->placeBar->width(), METKA_HEIGHT);
//    ui->metkaLow->show();
//    ui->metkaHi->show();
}

void wVolueBar::resetLim()
{
//    ui->metkaLow->hide();
//    ui->metkaHi->hide();
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
    if(type.size()>8)
        type.resize(8);
    if(mes.size()>2)
        mes.resize(2);

    if(numChan != 0)
    {
//        ui->typeBar->setText("<html><head/><body><p>" + \
//                             type + \
//                             "<span style=\" vertical-align:sub;\">" + \
//                             QString::number(numChan) + \
//                             "</span></p></body></html>");
        ui->typeBar->setText("<html><head/><body><p>" + \
                             type + \
                             "</p></body></html>");
    }
    else
    {
        ui->typeBar->setText(type);
    }
    ui->mesBar->setText(mes);

    // контроль шрифта
    int size = type.size();
    int fontSize = (int)(48.453*exp(-0.171*size));
    QFont font = ui->typeBar->font();
    font.setPointSize(fontSize);
    ui->typeBar->setFont(font);
    size = mes.size();
    fontSize = (int)(48.453*exp(-0.171*size));
    font = ui->mesBar->font();
    font.setPointSize(fontSize);
    ui->mesBar->setFont(font);

}

/*
 * Задание номера канала, индекс
 */
void wVolueBar::changeNum(int num)
{
    if(num < 10)
        numBar = num;
}

void wVolueBar::cleanMarker()
{
    int size = listMarker.size();
    for(int i = size; i > 0; i--)
    {
        listMarker.at(i-1)->deleteLater();
    }
    listMarker.clear();
}

void wVolueBar::addMarker(double vol, bool dir)
{
    cMarker * marker = new cMarker;
    marker->vol = vol;
    marker->dir = dir;
    listMarker.append(marker);
}


/*
 * Задание размаха значений, видимых в баре
 */
void wVolueBar::setBarDiapazon(double hi, double low)
{
    if(hi == low) hi += 0.000001;
    higherMeasure = hi;
    lowerMeasure = low;
//    razmah = max(abs(hi), abs(low));
    razmah = higherMeasure - lowerMeasure;
    ui->lineZero->setGeometry(  \
                0,  \
                (higherMeasure / razmah) * ui->placeBar->height() - 1,  \
                ui->placeBar->width(),      \
                2 );
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
