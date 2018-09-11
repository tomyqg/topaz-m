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
#include <QScroller>

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
extern QList<cSteel*> listSteel;
extern typeSteelTech steelTech[];
extern cSystemOptions systemOptions;  //класс хранения состемных опций

dSettings::dSettings(QList<ChannelOptions*> channels,
                     QList<Ustavka*> ustavki,
                     int num,
                     int page,
                     cArchivator *ar,
                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dSettings),
    arch(ar)
{
    ui->setupUi(this);
    QString ver = CURRENT_VER;
    ui->name->setText(QString("<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">MULTIGRAPH<br/>Ver. " + ver + "</span></p></body></html>"));

    //списки типов датчиков
    StringListNapryagenie.clear();
    StringListNapryagenie.append("Нет");
    StringListNapryagenie.append("0-100 мВ");
    StringListNapryagenie.append("0-1   В");
    StringListNapryagenie.append("±10   В");
    StringListNapryagenie.append("±30   В");
    StringListTC.clear();
    StringListTC.append("Тип S");           // (Pt10Rh-Pt)
    StringListTC.append("Тип K");           // (NiCr-Ni)
    StringListTC.append("Тип L");           // (Fe-CuNi)
    StringListTC.append("Тип B");           // (Pt30Rh-Pt60Rh)
    StringListTC.append("Тип А1");          // (W5Re-W20Re)
    StringListTC.append("Тип J");           // (Fe-CuNi)
    StringListTC.append("Тип N");           // (NiCrSi-NiSi)
    StringListRTD.clear();
    StringListRTD.append("Pt50 (3910)");
    StringListRTD.append("Pt100 (3910)");
    StringListRTD.append("Cu50 (4260)");
    StringListRTD.append("Cu100 (4260)");
    StringListRTD.append("Pt50 (3850)");
    StringListRTD.append("Pt100 (3850)");
    StringListRTD.append("Cu50 (4280)");
    StringListRTD.append("Cu100 (4280)");
    StringListRTD.append("Pt21 (ТСП21)");
    StringListRTD.append("Cu23 (ТСМ23)");

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
    ui->buttonResetSteel->setColorText(ColorBlue);
    ui->buttonResetSteel->setColorBg(QColor(0xf0,0xf0,0xf0));

    addChannel(channels, ustavki, num);

    //адаптация окна под отображение сообщений
    ui->stackedWidget->setCurrentIndex(page);

    //Настройки для масштабирования архива
    mouseOnScalede = false;
    mouseOnScaledeX = false;
    mouseOnMove = false;
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)),\
            this, SLOT(plotPress(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)),\
            this, SLOT(plotReleas(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)),\
            this, SLOT(plotMove(QMouseEvent*)));
    connect(&timerUpdateGraf, SIGNAL(timeout()), this, SLOT(replotGraf()));
    timerUpdateGraf.start(200);

    //настройки для скролинга списка сообщений
//    ui->listWidget->viewport()->installEventFilter(this);
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);
    mouseScroll = false;
    //скролинг виджетов настроек каналов
//    ui->scrollArea->installEventFilter(this);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
//    ui->scrollArea->verticalScrollBar()->setSingleStep(1);
    QScroller::grabGesture(ui->scrollArea_2, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_3, QScroller::LeftMouseButtonGesture);
//    ui->scrollArea_2->installEventFilter(this);
//    ui->scrollArea_3->installEventFilter(this);

    //обновим параметры виджетов, чтобы всё на своих местах стояло и написано, что надо
    updateWidgets();

    //обновление параметров: тип сигнала, тип датчика, схема включения
    updateUiSignalTypeParam(getIndexSignalTypeTable(channel->GetSignalType()));

    //настройки для архива
    if(arch != NULL)
        connect(arch, SIGNAL(loadFinished()), this, SLOT(drowGraf()));

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

    if(page == 4)
    {
        ui->labelNumSteel->setText("СТАЛЬ #" + QString::number(num));
        assert(num <= listSteel.size());
        if(num > listSteel.size()) return;
        curSteel = listSteel.at(num-1);
        updateUIfromSteel();
    }
    if(page == 5)
    {
        ui->timeSteel->clear();
        ui->timeSteel->addItem("Выбрать");
        QDir dirArchSteel(pathtolog);
        QFileInfoList dirContent = dirArchSteel.entryInfoList(QStringList() \
                  << "steel_*.txt", QDir::Files, QDir::Time);
        foreach (QFileInfo file, dirContent) {
            QString str = file.baseName().split("_").at(1);
            QDateTime time = QDateTime::fromString(str, "ddMMyyhhmmss");
            ui->timeSteel->addItem(time.toString("dd.MM.yy hh:mm:ss"));
        }

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
//        ui->customPlot->yAxis->setRange(-300, 500);
//        ui->customPlot->xAxis->setRange(-300, 300);
//        ui->customPlot->replot();
        //        updateGraf(60);

        //иконка анимации загрузки архива
        ui->loadArchive->setVisible(true);
        moArch.setFileName(pathtoloadgif);
        ui->loadArchive->setMovie(&moArch);
        moArch.start();
        archivePeriod = 60;
        ui->period->setEnabled(false);
        arch->load(archivePeriod);

    }
    if(ui->stackedWidget->currentIndex() == 3)
    {
        ui->saveButton->show();
        ui->nameSubMenu->setText("<html><head/><body><p>НАСТРОЙКА<br>УСТАВОК</p></body></html>");
        h = 72;
    }
    else if(ui->stackedWidget->currentIndex() == 4)
    {
        ui->saveButton->show();
        ui->nameSubMenu->setText("<html><head/><body><p>АНАЛИЗ<br>СТАЛИ</p></body></html>");
        h = 72;
    }
    else if(ui->stackedWidget->currentIndex() == 5)
    {
        ui->saveButton->hide();
        ui->nameSubMenu->setText("<html><head/><body><p>АРХИВ<br>СТАЛИ</p></body></html>");
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
    archivePeriod = periods[index]; //число секундных точек отсчёта

//    QVector<QDateTime> Dates;
//    QVector<QString> Labels;
//    QDateTime curTime = QDateTime::currentDateTime();
//    QDateTime firstTime = curTime.addSecs(-archivePeriod);
    ui->loadArchive->setVisible(true);
    moArch.start();
    ui->period->setEnabled(false);
    //Сброс буфера перед новым запросом
    Y_coordinates_Chanel_1.reserve(0);
    Y_coordinates_Chanel_2.reserve(0);
    Y_coordinates_Chanel_3.reserve(0);
    Y_coordinates_Chanel_4.reserve(0);
    // Запрос данных из файлов архива
    arch->load(archivePeriod);

//    updateGraf(archivePeriod);
}

void dSettings::updateGraf(int period)
{

    assert(arch != NULL);
    if(arch == NULL) return;

    QDateTime firstTime = QDateTime::currentDateTime().addSecs(-period);
    QString strLabel = "hh:mm:ss";

    int multiplier = 1;
    if(period >= 604800)
    {
        multiplier = 600;
    }
    int periodToGraf = period / multiplier;

    // копирование уже считанных с файлов данных
    Y_coordinates_Chanel_1 = arch->getVector(0);
    Y_coordinates_Chanel_2 = arch->getVector(1);
    Y_coordinates_Chanel_3 = arch->getVector(2);
    Y_coordinates_Chanel_4 = arch->getVector(3);

    //вставить сюда усреднитель данных,
    //чтобы большие периоды свести числу точек от 1000 до 10000

    X_Coordinates.resize(0);
    Labels.resize(0);
    int firstLabel = 0;

    //Поиск начала отсчётов
    for(int i = 0; i < periodToGraf; i++)
    {
        // проверка на NaN
        if((Y_coordinates_Chanel_1.at(i) == Y_coordinates_Chanel_1.at(i)) \
                || (Y_coordinates_Chanel_2.at(i) == Y_coordinates_Chanel_2.at(i)) \
                || (Y_coordinates_Chanel_3.at(i) == Y_coordinates_Chanel_3.at(i)) \
                || (Y_coordinates_Chanel_4.at(i) == Y_coordinates_Chanel_4.at(i)))
        {
            firstLabel = i;
            break;
        }
    }

    //Генерация шкалы Х
    for(int i = 0; i < periodToGraf; i ++)
    {
        X_Coordinates.append(i);

        //создание массива подписей
        if((i%(periodToGraf/5) == 0)/* && (i >= firstLabel)*/) //
            Labels.append(firstTime.addSecs(i*multiplier).toString(strLabel));
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(periodToGraf/5); //

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");

    if(!Y_coordinates_Chanel_1_archive.isEmpty())
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);


    // add the arrow:

    QPen graphPen;

    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(ColorCh1);

    ui->customPlot->graph()->setPen(graphPen);

    if(!Y_coordinates_Chanel_2_archive.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(ColorCh2);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if(!Y_coordinates_Chanel_3_archive.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(ColorCh3);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if(!Y_coordinates_Chanel_4_archive.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(ColorCh4);
        ui->customPlot->graph()->setPen(graphPen);
    }


//    ui->customPlot->rescaleAxes();
//    ui->customPlot->replot();
//    ui->customPlot->clearItems();

    if((!Y_coordinates_Chanel_1_archive.isEmpty()) &&\
            !Y_coordinates_Chanel_2_archive.isEmpty() &&\
            !Y_coordinates_Chanel_3_archive.isEmpty() &&\
            !Y_coordinates_Chanel_4_archive.isEmpty())
    {
        // авто масшабирование
        ui->customPlot->rescaleAxes();
        ui->customPlot->xAxis->setRange(0, periodToGraf);
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

    if(0)
    {

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
    }
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
    if(ui->stackedWidget->currentIndex() == 4)
    {
        typeSteelTech * tech = &steelTech[ui->groupTech->currentIndex()];
        curSteel->technology = tech;
//        int size = min(ui->nameSteelTech->text().size(), SIZE_TECH_NAME_STR);
        strcpy(tech->name, ui->groupName->text().toUtf8().data());
        int typeTC[] ={TC_Type_S, TC_Type_B, TC_Type_A1};
        tech->nSt = typeTC[ui->typeTermoCouple->currentIndex()];
        tech->dSt = ui->steel_dSt->value();
        tech->dt = ui->steel_dt->value();
        tech->tPt = ui->steel_tPt->value();
        tech->LPtl = ui->steel_LPtl->value();
        tech->LPth = ui->steel_LPth->value();
        tech->COH = ui->steel_COH->currentIndex();
        tech->dSE = ui->steel_dSE->value();
        tech->dE = ui->steel_dE->value();
        tech->tPE = ui->steel_tPE->value();
        tech->b1 = ui->steel_b1->value();
        tech->b2 = ui->steel_b2->value();
        tech->O = ui->steel_O->value();
        tech->Y = ui->steel_Y->value();
        tech->G = ui->steel_G->value();

        curSteel->relais[0] = ui->steelRelayBreak->currentIndex() - 1;
        curSteel->relais[1] = ui->steelRelayReady->currentIndex() - 1;
        curSteel->relais[2] = ui->steelRelayMeasure->currentIndex() - 1;
        curSteel->relais[3] = ui->steelRelayTimeOut->currentIndex() - 1;
    }
    else
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
        ustavka->setMessNormLow(ui->messageOffDown->text().toUtf8());
    }





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
    if(systemOptions.display == cSystemOptions::Steel)
    {
       cFileManager::writeSteelsSettings(pathtosteeloptions);
    }
    else
    {
        cFileManager::writeChannelsSettings(pathtooptions, listChannels, listUstavok);
    }


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

bool dSettings::eventFilter(QObject *watched, QEvent *event)
{
#ifndef Q_OS_WIN
    if ( (event->type() == QEvent::MouseButtonRelease) && \
         (watched->property("enabled").toString() == "true") && \
         (( QString::fromLatin1(watched->metaObject()->className()) == "QSpinBox") || \
          (QString::fromLatin1(watched->metaObject()->className()) == "QLineEdit") || \
          (QString::fromLatin1(watched->metaObject()->className()) == "QDoubleSpinBox")))
    {
        Options::olderprop = watched->property("text").toString();
        //Vag: нужно переделать и передавать строку напрямую в конструктор клавиатуры
        keyboard kb(this);
        kb.setModal(true);
        kb.exec();
        watched->setProperty("value", kb.getcustomstring() );
        watched->setProperty("text",kb.getcustomstring() );
//        ui->pushButton->setFocus();
        kb.close();
        kb.deleteLater();
    }
#endif


//    QListWidget *lw = (QListWidget*)(watched);
//    if(lw/* && (watched == ui->listWidget)*/)
//    {
//        if(event->type() == QEvent::MouseButtonPress)
//        {
//            yPosList = QCursor::pos().y();
//            curRow = ui->listWidget->currentRow();
//            mouseScroll = true;
//        }
//        if(event->type() == QEvent::MouseMove)
//        {
//            if(mouseScroll)
//            {
//                int y = QCursor::pos().y();
//                int h = ui->listWidget->height();
//                double move = ((double)y - (double)yPosList) / (double)h;
//                int count = ui->listWidget->count() * 15 / h;
//                int row;
//                if (count > 0)
//                    row = curRow - (count*move) + 9;
//                else if(count < 0)
//                    row = curRow - (count*move) - 9;
//                else
//                    curRow - (count*move);

//                ui->listWidget->setCurrentRow(row);
//            }
//        }
//        if(event->type() == QEvent::MouseButtonRelease)
//        {
//            mouseScroll = false;
//        }
//        return QObject::eventFilter(watched, event);
//    }

//    QScrollArea *sa = (QScrollArea*)(watched);
//    if(sa)
//    {
//        if(event->type() == QEvent::MouseButtonPress)
//        {
//            yPos = QCursor::pos().y();
//            yPosArea = sa->verticalScrollBar()->value();
//            mouseScroll = true;
//        }
//        if(event->type() == QEvent::MouseMove)
//        {
//            if(mouseScroll)
//            {
//                sa->verticalScrollBar()->setValue(yPosArea + yPos - QCursor::pos().y());
//            }
//        }
//        if(event->type() == QEvent::MouseButtonRelease)
//        {
//            mouseScroll = false;
//        }
//    }


    return QObject::eventFilter(watched, event);
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
    ui->sensorDiapazon->setCurrentIndex(channel->GetDiapason());
    ui->sensorShema->setCurrentIndex(indexUiShemaFromSensorShema(channel->getShema()));
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


void dSettings::plotPress(QMouseEvent * pe)
{
    int x = pe->pos().x();
    int y = pe->pos().y();
    int width = ui->customPlot->width();
    int height = ui->customPlot->height();
    if((x > (width / 10)) && (y < (height * 0.9)))
    {
        mouseOnMove = true;
    }
    else if((x < (width / 10)) && (y < (height * 0.9)))
    {
        mouseOnScalede = true;
    }
    else if((x > (width / 10)) && (y > (height * 0.9)))
    {
        mouseOnScaledeX = true;
    }

    xPos = QCursor::pos().x();
    yPos = QCursor::pos().y();
    sizePlot = ui->customPlot->yAxis->range().size();
    posPlot = ui->customPlot->yAxis->range().center();
    sizePlotX = ui->customPlot->xAxis->range().size();
    posPlotX = ui->customPlot->xAxis->range().center();
//    waitAutoScale = true;
}

void dSettings::plotReleas(QMouseEvent * pe)
{
    mouseOnScalede = false;
    mouseOnMove = false;
    mouseOnScaledeX= false;
//    timerScale.start(3000);
}

void dSettings::plotMove(QMouseEvent * pe)
{
    if(mouseOnScalede && !mouseOnMove)
    {
        int y = QCursor::pos().y();
        double scale = 1 + (((double)y - (double)yPos) / 2000);
        double pos = ui->customPlot->yAxis->range().center();
        double size = sizePlot * scale * scale ;
        ui->customPlot->yAxis->setRange(pos, size, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    else if(mouseOnScaledeX && !mouseOnMove)
    {
        int x = QCursor::pos().x();
        double scale = 1 + (((double)xPos - (double)x) / 2000);
        double pos = ui->customPlot->xAxis->range().center();
        double size = sizePlotX * scale * scale ;
        ui->customPlot->xAxis->setRange(pos, size, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    else if(mouseOnMove)
    {
        int y = QCursor::pos().y();
        int x = QCursor::pos().x();
        double move = ((double)y - (double)yPos) / (double)ui->customPlot->height();
        double moveX = ((double)xPos - (double)x) / (double)ui->customPlot->width();
        double pos = posPlot + move * sizePlot;
        double posX = posPlotX + moveX * sizePlotX;
        ui->customPlot->yAxis->setRange(pos, sizePlot, Qt::AlignCenter);
        ui->customPlot->xAxis->setRange(posX, sizePlotX, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
}

void dSettings::replotGraf()
{
    ui->customPlot->replot();
}

void dSettings::drowGraf()
{
    ui->loadArchive->setVisible(false);
    ui->period->setEnabled(true);
    updateGraf(archivePeriod);
}

//void dSettings::addSteel(cSteel * st, typeSteelTech * tech)
//{

//}

void dSettings::updateUIfromSteel()
{
    typeSteelTech * tech = curSteel->technology;
    ui->steelRelayBreak->setCurrentIndex(curSteel->relais[0] + 1);
    ui->steelRelayReady->setCurrentIndex(curSteel->relais[1] + 1);
    ui->steelRelayMeasure->setCurrentIndex(curSteel->relais[2] + 1);
    ui->steelRelayTimeOut->setCurrentIndex(curSteel->relais[3] + 1);
    UpdateSteelUI(tech);
}

void dSettings::on_groupTech_currentIndexChanged(int index)
{
    typeSteelTech * tech = &steelTech[index];
    UpdateSteelUI(tech);
}

void dSettings::on_buttonResetSteel_clicked()
{
    int i = ui->groupTech->currentIndex();
    typeSteelTech tech = defTech[i];
    UpdateSteelUI(&tech);
}

void dSettings::UpdateSteelUI(typeSteelTech * tech)
{
    ui->groupTech->setCurrentIndex(tech->num);
    ui->groupName->setText(tech->name);
    int indexTC[] = {0, 0, 0, 1, 2, 0, 0};
    ui->typeTermoCouple->setCurrentIndex(indexTC[tech->nSt]);
    ui->steel_dSt->setValue(tech->dSt);
    ui->steel_dt->setValue(tech->dt);
    ui->steel_tPt->setValue(tech->tPt);
    ui->steel_LPtl->setValue(tech->LPtl);
    ui->steel_LPth->setValue(tech->LPth);
    ui->steel_COH->setCurrentIndex(tech->COH);
    ui->steel_dSE->setValue(tech->dSE);
    ui->steel_dE->setValue(tech->dE);
    ui->steel_tPE->setValue(tech->tPE);
    ui->steel_b1->setValue(tech->b1);
    ui->steel_b2->setValue(tech->b2);
    ui->steel_O->setValue(tech->O);
    ui->steel_Y->setValue(tech->Y);
    ui->steel_G->setValue(tech->G);
}

void dSettings::on_timeSteel_currentIndexChanged(const QString &arg1)
{
    //открыть файл архива анализа стали
    ui->customPlotSteel->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlotSteel->yAxis2->setVisible(true);
    ui->customPlotSteel->yAxis2->setTickLabels(true);
//    ui->customPlotSteel->
    //QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeDragAxes,
    //\ref QCPAxisRect::setRangeZoomAxes
    QDateTime timeSteel;
    timeSteel = QDateTime::fromString(arg1, "dd.MM.yy hh:mm:ss");
    QString pathtologs = pathtolog;
    pathtologs.append("steel_");
    pathtologs.append(timeSteel.toString("ddMMyy"));
    pathtologs.append(timeSteel.toString("hhmmss"));
    pathtologs.append(".txt");
    QFile fileArchSteel(pathtologs);
    if(fileArchSteel.open(QIODevice::ReadOnly))
    {
        QTextStream in(&fileArchSteel);
        QString sss = in.readLine();
        QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
        QJsonObject json = doc.object();
        QJsonArray archives = json["Archives"].toArray();

        QJsonObject eds = archives.at(0).toObject();
        QJsonArray valEds = eds["values"].toArray();
        Y_SteelEds = QVector<double>(SIZE_ARRAY, NAN);
        for(int i = 0; i < valEds.size(); i++)
        {
            Y_SteelEds.replace(i, valEds.at(i).toString().toDouble());
        }

        Y_SteelTemp = QVector<double>(SIZE_ARRAY, NAN);
        QJsonObject temp = archives.at(1).toObject();
        QJsonArray valTemp = temp["values"].toArray();
        for(int i = 0; i < valTemp.size(); i++)
        {
            Y_SteelTemp.replace(i, valTemp.at(i).toString().toDouble());
        }

        for(int i = 0; i < max(valTemp.size(), valEds.size()); i++)
        {
            X_Steel.append(i);
        }

        ui->customPlotSteel->clearGraphs();
        ui->customPlotSteel->addGraph();
        ui->customPlotSteel->graph()->setData(X_Steel, Y_SteelTemp);
        ui->customPlotSteel->xAxis->setLabel("t,sec");
        ui->customPlotSteel->yAxis2->setLabel("Emf, mV");
        ui->customPlotSteel->yAxis->setLabel("Temp, °C");
        ui->customPlotSteel->graph()->setPen(QPen(QBrush(ColorCh3), 2));

        ui->customPlotSteel->addGraph(ui->customPlotSteel->xAxis, ui->customPlotSteel->yAxis2);
        ui->customPlotSteel->graph()->setData(X_Steel, Y_SteelEds);
        ui->customPlotSteel->graph()->setPen(QPen(QBrush(ColorCh4), 2));
        ui->customPlotSteel->rescaleAxes();
        ui->customPlotSteel->replot();
        ui->customPlotSteel->clearItems();

        QString string = json["Technology"].toString();
        ui->nameSteelTech->setText(string);
        ui->nameSteelSmelt->setText(json["Smelt"].toString());
        ui->steelTemp->setText(json["Temp"].toString());
        ui->steelEmf->setText(json["Eds"].toString());
        ui->steelAO->setText(json["OxActivity"].toString());
        ui->steelAl->setText(json["MassAl"].toString());
        ui->steelC->setText(json["Carbon"].toString());

        fileArchSteel.close();
    }

}

void dSettings::on_steelRelayBreak_activated(int index)
{
//    curSteel->relais[0] = index - 1;
}

void dSettings::on_steelRelayReady_activated(int index)
{
//    curSteel->relais[1] = index - 1;
}

void dSettings::on_steelRelayMeasure_activated(int index)
{
//    curSteel->relais[2] = index - 1;
}

void dSettings::on_steelRelayTimeOut_activated(int index)
{
//    curSteel->relais[3] = index - 1;
}
