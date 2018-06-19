#include <QFile>
#include <log.h>
#include <assert.h>
#include <settings.h>
#include <ui_settings.h>
#include <defines.h>
#include <filemanager.h>
#include <options.h>
#include <keyboard.h>
#include <uartdriver.h>

enum numItems{
    NoSignal = 0,
    Current = 1,
    Voltage = 2,
    TermoCouple = 3,
    TermoResistance =4
};

typedef struct {
    numItems numItem;
    ModBus::SignalType st;
    QString nameItem;
} typeTableSignalTypes;

const typeTableSignalTypes tableSignalTypes[] = {
    {NoSignal, ModBus::NoMeasure, "ОТКЛ."},     //0
    {Current, ModBus::CurrentMeasure, "ТОК"},       //1
    {Voltage, ModBus::VoltageMeasure, "НАРЯЖЕНИЕ"},     //2
    {TermoCouple, ModBus::TermoCoupleMeasure, "Т/П"},       //3
    {TermoResistance, ModBus::TermoResistanceMeasure, "Т/С"},   //4
};



#define TIME_UPDATE DateLabelUpdateTimer
#define TIME_UPDATE_BAR DateLabelUpdateTimer

extern QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;

dSettings::dSettings(QList<ChannelOptions*> channels, QList<Ustavka*> ustavki, int num, int page, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dSettings)
{
    ui->setupUi(this);

    //списки типов датчиков
    StringListNapryagenie.clear();
    StringListNapryagenie.append("Нет");
    StringListNapryagenie.append("0-100 мВ");
    StringListNapryagenie.append("0-1   В");
    StringListNapryagenie.append("0-5   В");
    StringListNapryagenie.append("±10   В");
    StringListNapryagenie.append("±30   В");
    StringListTC.clear();
    StringListTC.append("Тип S (Pt10Rh-Pt)");
    StringListTC.append("Тип K (NiCr-Ni)");
    StringListTC.append("Тип L (Fe-CuNi)");
    StringListTC.append("Тип B (Pt30Rh-Pt60Rh)");
    StringListTC.append("Тип А1(W5Re-W20Re)");
    StringListTC.append("Тип J (Fe-CuNi)");
    StringListTC.append("Тип N (NiCrSi-NiSi)");
    StringListRTD.clear();
    StringListRTD.append("Pt50   (GOST, a=3910)");
    StringListRTD.append("Pt100  (GOST, a=3910)");
    StringListRTD.append("Cu50   (GOST, a=4260)");
    StringListRTD.append("Cu100  (GOST, a=4260)");
    StringListRTD.append("Pt50   (GOST, a=3850)");
    StringListRTD.append("Pt100  (GOST, a=3850)");
    StringListRTD.append("Cu50   (GOST, a=4280)");
    StringListRTD.append("Cu100  (GOST, a=4280)");
    StringListRTD.append("Pt21   (ТСП21)");
    StringListRTD.append("Cu23   (ТСМ23)");

    // <---- временно скрыть некоторые пункты настроек уставок
    ui->ustavkaChannel->hide();
    ui->label_19->hide();
    ui->typeFix->hide();
    ui->label_21->hide();
    ui->ustavkaTimer->hide();
    ui->label_24->hide();

    // ----->

    ui->saveButton->setColorText(ColorBlue);
    ui->saveButton->setColorBg(QColor(0xff,0xff,0xff));
    ui->exitButton->setColorText(ColorBlue);
    ui->exitButton->setColorBg(QColor(0xff,0xff,0xff));

    //инициализация иконки анимации
    ui->load->setHidden(true);
    mo.setFileName(pathtoloadgif);
    ui->load->setMovie(&mo);
    mo.start();

    //логотип
    QPixmap pix(pathtologotip);
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);

    //таймер сохранения настроек
    connect(&timerLoad, SIGNAL(timeout()), this, SLOT(timeoutLoad()));


    connect(&tUpdateTime, SIGNAL(timeout()), this, SLOT(DateUpdate()));
    tUpdateTime.start(TIME_UPDATE);
    DateUpdate();

    ui->buttonUstavk->setColorText(ColorBlue);
    ui->buttonUstavk->setColorBg(QColor(0xf0,0xf0,0xf0));
    ui->buttonBackUstavki->setColorText(ColorBlue);
    ui->buttonBackUstavki->setColorBg(QColor(0xf0,0xf0,0xf0));

    addChannel(channels, ustavki, num);

    //адаптация окна под отображение сообщений
    ui->stackedWidget->setCurrentIndex(page);
    if(page == 0)
    {

    }
    //обновим параметры виджетов, чтобы всё на своих местах стояло
    updateWidgets();

    //обновление параметров: тип сигнала, тип датчика, схема включения
    updateUiSignalTypeParam(getIndexSignalTypeTable(channel->GetSignalType()));


    // устанавливаем евент фильтры чтобы при нажатии на поле появлялась клавиатура
    QList<QSpinBox*> spinList = findChildren<QSpinBox*> ();
    for (int i = 0; i < spinList.count(); ++i) {
        QSpinBox *sb = spinList.at(i);
        sb->installEventFilter(this);
    }
    QList<QLineEdit*> lineeditList = findChildren<QLineEdit*> (  );
    for (int i = 0; i < lineeditList.count(); ++i) {
        QLineEdit *le = lineeditList.at(i);
        le->installEventFilter(this);
    }
    QList<QDoubleSpinBox*> SpinBox= findChildren<QDoubleSpinBox*> ();
    for (int i = 0; i < SpinBox.count(); ++i) {
        QDoubleSpinBox *spbox = SpinBox.at(i);
        spbox->installEventFilter(this);
    }
}

dSettings::~dSettings()
{
    delete ui;
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

void dSettings::updateWidgets()
{
    int h = 72;
    if(ui->stackedWidget->currentIndex() == 0)
    {
        h = 72;
        ui->saveButton->show();
        ui->nameSubMenu->setText("<html><head/><body><p>НАСТРОЙКИ<br>КАНАЛА</p></body></html>");
    }
    else if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->saveButton->hide();
        ui->nameSubMenu->setText("СООБЩЕНИЯ");
        h = 44;
        QJsonArray messagesarray;
        cLogger log(pathtomessages);
        messagesarray = log.MessRead();

        for (int var = 0; var < messagesarray.count() ; ++var) {
            QJsonObject mes = messagesarray.at(var).toObject();
            QString num = QString("%1").arg(var+1, 4, 10, QChar('0'));
            ui->listWidget->addItem(num + " | " + mes.value("D").toString() +" "+  mes.value("T").toString()+" | "+ mes.value("M").toString());
        }
        ui->listWidget->scrollToBottom();
    }
    else if(ui->stackedWidget->currentIndex() == 2)
    {
        h = 44;
        //страница архива
        ui->saveButton->hide();
        ui->nameSubMenu->setText("АРХИВ");
        /*
        InitiateArchive();
        UpdateArchiveData();
        */
        ui->customPlot->yAxis->setRange(-300, 500);
        ui->customPlot->xAxis->setRange(-300, 300);
        ui->customPlot->replot();
        // по умолчанию отобразим архив за период 1 минута
        updateGraf(60 * ValuesUpdateTimer);
    }
    if(ui->stackedWidget->currentIndex() == 3)
    {
        ui->saveButton->show();
        ui->nameSubMenu->setText("<html><head/><body><p>НАСТРОЙКА<br>УСТАВОК</p></body></html>");
        h = 72;
    }
    else
    {
        h = 72;
    }
    ui->nameSubMenu->setGeometry(ui->nameSubMenu->x(), ui->nameSubMenu->y(), \
                                 ui->nameSubMenu->width(), h);
    ui->shadowSubMenu->setGeometry(ui->shadowSubMenu->x(), ui->shadowSubMenu->y(), \
                                   ui->shadowSubMenu->width(), h);

}

void dSettings::on_verticalScrollBar_sliderMoved(int position)
{
    ui->listWidget->setCurrentRow(position);
}

void dSettings::on_period_currentIndexChanged(int index)
{
    // Реализовать тут смену периода отображаемых данных из архива
    // Тут же обновить график
    //                 1 мин 10 мин 1 час 10 часов сутки  неделя  месяц    3 месяца год
    int periods[10] = {60,   600,   3600, 36000,   86400, 604800, 2592000, 7776000, 31104000};
    assert((sizeof(periods) / sizeof(int)) >= ui->period->count());
    int archivePeriod = ValuesUpdateTimer * periods[index];
}

void dSettings::updateGraf(int period)
{
    if(X_Coordinates_archive.isEmpty())
        return;

    ui->customPlot->xAxis->setRange(-1000, 2000);
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");

    if(!Y_coordinates_Chanel_1_archive.isEmpty())
    ui->customPlot->graph()->setData(X_Coordinates_archive, Y_coordinates_Chanel_1_archive);


    // add the arrow:

    QPen graphPen;

    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(ColorCh1);

    ui->customPlot->graph()->setPen(graphPen);

    if(!Y_coordinates_Chanel_2_archive.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates_archive, Y_coordinates_Chanel_2_archive);
        graphPen.setColor(ColorCh2);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if(!Y_coordinates_Chanel_3_archive.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates_archive, Y_coordinates_Chanel_3_archive);
        graphPen.setColor(ColorCh3);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if(!Y_coordinates_Chanel_4_archive.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates_archive, Y_coordinates_Chanel_4_archive);
        graphPen.setColor(ColorCh4);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if((!Y_coordinates_Chanel_1_archive.isEmpty()) &&\
            !Y_coordinates_Chanel_2_archive.isEmpty() &&\
            !Y_coordinates_Chanel_3_archive.isEmpty() &&\
            !Y_coordinates_Chanel_4_archive.isEmpty())
    {
        // авто масшабирование
        ui->customPlot->rescaleAxes();
        ui->customPlot->replot();
        ui->customPlot->clearItems();// удаляем стрелочку а то она будет потом мешаться
    }
}

void dSettings::addChannel(QList<ChannelOptions *> channels, QList<Ustavka*> ustavki, int num)
{
    //запоминаем канал, чтобы потом в него сохранить изменения
    listChannels = channels;
    listUstavok = ustavki;
    if(num == 0) num = 1;
    if(num > channels.size()) num = channels.size();
    numChannel = num;
    channel = listChannels.at(num-1);
    ustavka = listUstavok.at(num-1);

    ui->typeSignal->setCurrentIndex(getIndexSignalTypeTable(channel->GetSignalType()));
    ui->nameChannel->setText(channel->GetChannelName().toUtf8());
    ui->scaleUp->setValue(channel->GetHigherMeasureLimit());
    ui->scaleDown->setValue(channel->GetLowerMeasureLimit());
    ui->unit->setText(channel->GetUnitsName().toUtf8());
    ui->periodCh->setValue(channel->GetMeasurePeriod());
    ui->labelNumChannel->setText("КАНАЛ #" + QString::number(num));
    ui->bar->changeNum(num);
    ui->dempfer->setValue(channel->GetDempherValue());
    ui->typeReg->setCurrentIndex(channel->GetRegistrationType());
    ui->sensorDiapazon->setCurrentIndex(channel->GetDiapason());
    ui->sensorShema->setCurrentIndex(indexUiShemaFromSensorShema(channel->getShema()));

    //параметры уставок
    ui->ustavkaVol->setValue(ustavka->getHiStateValue());
    ui->ustavkaVolDown->setValue(ustavka->getLowStateValue());
    ui->gisterezis->setValue(ustavka->getHiHisteresis());
    ui->ustavkaChannel->setCurrentIndex(num);
    ui->identifikator->setText("Channel " + QString::number(num));
    ui->labelNumUsatvka->setText("УСТАВКИ #" + QString::number(num));
    ui->reley->setCurrentIndex(ustavka->getnumRelayUp());
    ui->releyDown->setCurrentIndex(ustavka->getnumRelayDown());
    ui->messageOn->setText(ustavka->getMessInHigh());
    ui->messageOff->setText(ustavka->getMessNormHigh());
    ui->messageOnDown->setText(ustavka->getMessInLow());
    ui->messageOffDown->setText(ustavka->getMessNormHigh());


    //стилизация элементов
    /*
    ui->nameChannel->setStyleSheet("background-color: rgb(" \
                                   + QString::number(channel->GetNormalColor().red()) +"," \
                                   + QString::number(channel->GetNormalColor().green()) +"," \
                                   + QString::number(channel->GetNormalColor().blue()) +");" \
                                                                                        "color: rgb(255, 255, 255);" \
                                                                                        "border-radius: 0px;}");
    ui->unit->setStyleSheet(ui->nameChannel->styleSheet());
    ui->sensorDiapazon->setStyleSheet("QComboBox {" \
                + ui->nameChannel->styleSheet() \
                + "}QComboBox::drop-down {width:30px;}");
    ui->identifikator->setStyleSheet(ui->nameChannel->styleSheet());
    ui->kvitir->setStyleSheet(ui->sensorDiapazon->styleSheet());
    ui->ustavkaVol->setStyleSheet("background-color: rgb(" \
                               + QString::number(COLOR_3.red()) + "," \
                               + QString::number(COLOR_3.green()) + "," \
                               + QString::number(COLOR_3.blue()) + ");" \
                                                                   "color: rgb(255, 255, 255);" \
                                                                   "border-radius: 0px;");
    ui->messageOn->setStyleSheet(ui->ustavkaVol->styleSheet());
    ui->reley->setStyleSheet(ui->ustavkaVol->styleSheet());
    ui->messageOnDown->setStyleSheet("background-color: rgb(" \
                                 + QString::number(COLOR_DARK.red()) + "," \
                                 + QString::number(COLOR_DARK.green()) + "," \
                                 + QString::number(COLOR_DARK.blue()) + ");" \
                                                                        "color: rgb(255, 255, 255);" \
                                                                        "border-radius: 0px;");
    ui->typeReg->setStyleSheet("QComboBox {" \
                               + ui->messageOnDown->styleSheet() \
                               + "}QComboBox::drop-down {width:30px;}");
    ui->ustavkaVolDown->setStyleSheet(ui->typeReg->styleSheet());
    ui->releyDown->setStyleSheet(ui->ustavkaVolDown->styleSheet());
    */
    connect(&tUpdateBar, SIGNAL(timeout()), this, SLOT(updateBar()));
    tUpdateBar.start(TIME_UPDATE_BAR);

}

void dSettings::updateBar()
{
    if(channel != NULL) {
        ui->bar->setExtr(channel->GetMinimumChannelValue(), channel->GetMaximumChannelValue());
        ui->bar->setLim(ui->ustavkaVolDown->value(), ui->ustavkaVol->value());
        ui->bar->setColor(channel->GetNormalColor(), channel->GetMinimumColor()); //Vag: переделать на QColor
        ui->bar->setText(ui->nameChannel->text(), ui->unit->text());
        ui->bar->setBarDiapazon(max(abs(ui->scaleUp->value()), abs(ui->scaleDown->value())));
        ui->bar->setVolue(channel->GetCurrentChannelValue());
    }
}

void dSettings::saveParam()
{
    channel->SetChannelName(ui->nameChannel->text().toUtf8());
    channel->SetSignalType(tableSignalTypes[ui->typeSignal->currentIndex()].st);
    channel->SetHigherMeasureLimit(ui->scaleUp->value());
    channel->SetLowerMeasureLimit(ui->scaleDown->value());
    channel->SetUnitsName(ui->unit->text().toUtf8());
    channel->SetMeasurePeriod(ui->periodCh->value());
    channel->SetDempher(ui->dempfer->value());
    channel->SetRegistrationType(ui->typeReg->currentIndex());
    channel->SetDiapason(ui->sensorDiapazon->currentIndex());
    channel->setShema(sensorShemaFromUiShemaIndex(ui->sensorShema->currentIndex()));

    ustavka->setUstavka(numChannel, \
                        ui->ustavkaVol->value(), \
                        ui->ustavkaVolDown->value(), \
                        ui->gisterezis->value(), \
                        ui->gisterezis->value(), \
                        ui->reley->currentIndex(), \
                        ui->releyDown->currentIndex() \
                        );
    ustavka->setMessInHigh(ui->messageOn->text().toUtf8());
    ustavka->setMessNormHigh(ui->messageOff->text().toUtf8());
    ustavka->setMessInLow(ui->messageOnDown->text().toUtf8());
    ustavka->setMessNormHigh(ui->messageOffDown->text().toUtf8());
}

void dSettings::on_buttonBackUstavki_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    updateWidgets();
}

void dSettings::on_saveButton_clicked()
{
    ui->load->setHidden(false);
//     засекаем время записи настроек в файл или ждать сигнал о завершении
    timerLoad.start(1000);

    //сохраняемся
    saveParam();
    cFileManager::writeChannelsSettings(pathtooptions, listChannels, listUstavok);

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
    ui->stackedWidget->setCurrentIndex(3);
    updateWidgets();
}

void dSettings::resizeEvent(QResizeEvent * s)
{
    updateBar();
}

bool dSettings::eventFilter(QObject *object, QEvent *event)
{
    if ( (event->type() == QEvent::MouseButtonRelease) && \
         (object->property("enabled").toString() == "true") && \
         ( QString::fromLatin1(object->metaObject()->className()) != "QPushButton" ) )
    {
        Options::olderprop = object->property("text").toString();
        //Vag: нужно переделать и передавать строку напрямую в конструктор клавиатуры
        keyboard kb(this);
        kb.setModal(true);
        kb.exec();
        object->setProperty("value", kb.getcustomstring() );
        object->setProperty("text",kb.getcustomstring() );
//        ui->pushButton->setFocus();
        kb.close();
        kb.deleteLater();
    }

    return QObject::eventFilter(object, event);
}

void dSettings::on_typeSignal_currentIndexChanged(int index)
{
    updateUiSignalTypeParam(index);
}

/*
 * Обновление элементов UI по
 */
void dSettings::updateUiSignalTypeParam(int index)
{
    if(index == NoSignal)
    {
        ui->sensorDiapazon->hide();
        ui->sensorShema->hide();
        ui->labelDiapazon->hide();
        ui->labelShema->hide();
    }
    else if(index == Current)
    {
        ui->sensorDiapazon->hide();
        ui->sensorShema->hide();
        ui->labelDiapazon->hide();
        ui->labelShema->hide();
    }
    else  if(index == Voltage)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListNapryagenie);
        ui->sensorDiapazon->show();
        ui->labelDiapazon->show();
        ui->sensorShema->hide();
        ui->labelShema->hide();
    }
    else  if(index == TermoCouple)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListTC);
        ui->sensorDiapazon->show();
        ui->labelDiapazon->show();
        ui->sensorShema->hide();
        ui->labelShema->hide();
    }
    else  if(index == TermoResistance)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListRTD);
        ui->sensorDiapazon->show();
        ui->labelDiapazon->show();
        ui->sensorShema->show();
        ui->labelShema->show();
    }
}

/*
 * Получение индекса в таблице по типу сигнала
 */
int dSettings::getIndexSignalTypeTable(int st)
{
    for(int i = 0; i < sizeof(tableSignalTypes)/sizeof(typeTableSignalTypes); i++)
    {
        if(tableSignalTypes[i].st == st)
            return i;
    }
    return 0;
}

int dSettings::sensorShemaFromUiShemaIndex(int index)
{
    if(index == 0)          //3-х проводная схема подключения
    {
        return 1;
    }
    else if(index == 1)     //4-х проводная схема подключения
    {
        return 2;
    }
    return 3;
}

int dSettings::indexUiShemaFromSensorShema(int sh)
{
    if(sh == 1)          //3-х проводная схема подключения
    {
        return 0;
    }
    else if(sh == 2)     //4-х проводная схема подключения
    {
        return 1;
    }
    return 0;
}