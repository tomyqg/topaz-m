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
#include <customperiod.h>

enum numItems{
    NoSignal = 0,
    Current = 1,
    Voltage = 2,
    TermoCouple = 3,
    TermoResistance =4
};

typedef struct {
    numItems numItem;
    SignalType st;
    QString nameItem;
} typeTableSignalTypes;

const typeTableSignalTypes tableSignalTypes[] = {
    {NoSignal, NoMeasure, "ОТКЛ."},     //0
    {Current, CurrentMeasure, "ТОК"},       //1
    {Voltage, VoltageMeasure, "НАРЯЖЕНИЕ"},     //2
    {TermoCouple, TermoCoupleMeasure, "Т/П"},       //3
    {TermoResistance, TermoResistanceMeasure, "Т/С"},   //4
};

typedef struct {
    int numItem;
    voltageType diapason;
    QString nameItem;
} typeTableDiapasone;
const typeTableDiapasone tableVoltageDiapasone[] = {
//    {0, Voltage_None, "НЕТ"},     //0
    {ChannelOptions::Voltage150mV,  Voltage_1V,   "±150 мВ"},       //0
    {ChannelOptions::Voltage300mV,  Voltage_1V,   "±300 мВ"},       //1
    {ChannelOptions::Voltage0_1V,   Voltage_1V,   "0-1   В"},       //2
    {ChannelOptions::Voltage0_5V,   Voltage_10V,  "0-5   В"},       //3
    {ChannelOptions::Voltage0_10V,  Voltage_10V,  "0-10  В"},       //4
    {ChannelOptions::Voltage1V,     Voltage_1V,   "±1    В"},       //5
    {ChannelOptions::Voltage10V,    Voltage_10V,  "±10   В"},       //6
    {ChannelOptions::Voltage30V,    Voltage_30V,  "±30   В"},       //7
};
typedef struct {
    int numItem;
    termoCoupleType diapason;
    QString nameItem;
} typeTableDiapasoneTC;
const typeTableDiapasoneTC tableDiapasoneTC[] = {
    {0, TC_Type_S,  "Тип S"},     //0
    {1, TC_Type_K,  "Тип K"},       //2
    {2, TC_Type_L,  "Тип L"},     //3
    {3, TC_Type_B,  "Тип B"},       //4
    {4, TC_Type_A1, "Тип А1"},     //0
    {5, TC_Type_J,  "Тип J"},       //2
    {6, TC_Type_N,  "Тип N"},     //3
};
typedef struct {
    int numItem;
    rtdType diapason;
    QString nameItem;
} typeTableDiapasoneRTD;
const typeTableDiapasoneRTD tableDiapasoneRTD[] = {
    {0, RTD_Pt50_3910,  "50П"},     //0
    {1, RTD_Pt100_3910, "100П"},       //1
    {2, RTD_Cu50_4260,  "50М(1.426)"},     //2
    {3, RTD_Cu100_4260, "100М(1.426)"},       //3
    {4, RTD_Pt50_3850,  "Pt50"},     //4
    {5, RTD_Pt100_3850, "Pt100"},       //5
    {6, RTD_Cu50_4280,  "50М(1.428)"},     //6
    {7, RTD_Cu100_4280, "100М(1.428)"},       //7
//    {8, RTD_Pt21_TSP21, "ТСП21"},       //8
//    {9, RTD_Cu23_TSM23, "ТСМ23"},       //9
    {8, RTD_Pt500_3850, "Pt500"},       //8
    {9, RTD_Pt1000_3850, "Pt1000"},       //9
};

typedef struct {
    int numItem;
    ChannelOptions::typePrecision precision;
    QString nameItem;
} typeTablePrecisions;
const typeTablePrecisions tablePrecisions[] = {
    {0, ChannelOptions::Precision_X,        "X"},     //0
    {1, ChannelOptions::Precision_X_X,      "X.X"},       //2
    {2, ChannelOptions::Precision_X_XX,     "X.XX"},     //3
    {3, ChannelOptions::Precision_X_XXX,    "X.XXX"},       //4
    {4, ChannelOptions::Precision_X_XXXX,   "X.XXXX"},     //0
    {5, ChannelOptions::Precision_X_XXXXX,  "X.XXXXX"},       //2
    {6, ChannelOptions::Precision_X_XXXXXX, "X.XXXXXX"},     //3
};


#define TIME_UPDATE DateLabelUpdateTimer
#define TIME_UPDATE_BAR DateLabelUpdateTimer

//extern QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;

extern QList<ChannelOptions *> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cSteel*> listSteel;
extern QList<cRelay*> listRelais;
extern typeSteelTech steelTech[];
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern QMutex mListUstvok;
extern QMutex mListChannel;
//extern int dateindex;
//extern int timeindex;
extern QStringList datestrings, timestrings;

dSettings::dSettings(QList<ChannelOptions*> channels,
                     int num,
                     int page,
                     cArchivator *ar,
                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dSettings),
    arch(ar)
{
    fInitComboChannles = false;
    fInitCompoCopyChannels = false;
    ui->setupUi(this);
    updateVer();
    //списки типов датчиков
    StringListCurrent.clear();
    StringListCurrent.append("0-20 мА");
    StringListCurrent.append("4-20 мА");
    StringListCurrent.append("0-5 мА");
    StringListNapryagenie.clear();
    for(int i = 0; i < (sizeof(tableVoltageDiapasone)/sizeof(typeTableDiapasone)); i++)
    {
        StringListNapryagenie.append(tableVoltageDiapasone[i].nameItem);
    }
//    StringListNapryagenie.append("Нет");
////    StringListNapryagenie.append("0-100 мВ");
//    StringListNapryagenie.append("±1    В");
//    StringListNapryagenie.append("±10   В");
//    StringListNapryagenie.append("±30   В");
    StringListTC.clear();
    for(int i = 0; i < (sizeof(tableDiapasoneTC)/sizeof(typeTableDiapasoneTC)); i++)
    {
        StringListTC.append(tableDiapasoneTC[i].nameItem);
    }
//    StringListTC.append("Тип S");           // (Pt10Rh-Pt)
//    StringListTC.append("Тип K");           // (NiCr-Ni)
//    StringListTC.append("Тип L");           // (Fe-CuNi)
//    StringListTC.append("Тип B");           // (Pt30Rh-Pt60Rh)
//    StringListTC.append("Тип А1");          // (W5Re-W20Re)
//    StringListTC.append("Тип J");           // (Fe-CuNi)
//    StringListTC.append("Тип N");           // (NiCrSi-NiSi)
    StringListRTD.clear();
    for(int i = 0; i < (sizeof(tableDiapasoneRTD)/sizeof(typeTableDiapasoneRTD)); i++)
    {
        StringListRTD.append(tableDiapasoneRTD[i].nameItem);
    }

    StringListPrecisions.clear();
    for(int i = 0; i < (sizeof(tablePrecisions)/sizeof(typeTablePrecisions)); i++)
    {
        StringListPrecisions.append(tablePrecisions[i].nameItem);
    }
    ui->comboPrecision->clear();
    ui->comboPrecision->addItems(StringListPrecisions);
    ui->comboPrecision->setCurrentIndex(ChannelOptions::Precision_X_X);


    ui->ustavkaVolDown->hide();
    ui->label_29->hide();
    ui->releyDown->hide();
    ui->label_32->hide();
    ui->messageOnDown->hide();
    ui->label_30->hide();
    ui->messageOffDown->hide();
    ui->label_31->hide();
    ui->unit->hide();
    ui->label_11->hide();
//    ui->comboPrecision->hide();
//    ui->labelPrecision->hide();
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

    ui->buttonResetSteel->setColorText(ColorBlue);
    ui->buttonResetSteel->setColorBg(QColor(0xf0,0xf0,0xf0));
    ui->bDeleteUstavka->setColorText(ColorBlue);
    ui->bDeleteUstavka->setColorBg(QColor(0xf0,0xf0,0xf0));

    ui->bUserPeriod->setColorText(ColorBlue);
    ui->bUserPeriod->setColorBg(QColor(0xf0,0xf0,0xf0));
    ui->bUserPeriod->setFontSize(16);

    addChannel(listChannels, num);

    //адаптация окна под отображение сообщений (по-умолчанию)
    ui->stackedWidget->setCurrentIndex(page);

    multiplier = 1;

    //Настройки для масштабирования архива
    mouseOnScalede = false;
    mouseOnScaledeX = false;
    mouseOnMove = false;
    ui->customPlot->installEventFilter(this);

    connect(&timerUpdateGraf, SIGNAL(timeout()), this, SLOT(replotGraf()));
    timerUpdateGraf.start(200);

    //обновим параметры виджетов, чтобы всё на своих местах стояло и написано, что надо
    updateWidgets();

    //обновление параметров: тип сигнала, тип датчика, схема включения
    updateUiSignalTypeParam(getIndexSignalTypeTable(channel->GetSignalType()));

    //настройки для скролинга списка сообщений
    QScroller::grabGesture(ui->listWidget->viewport(), QScroller::LeftMouseButtonGesture);
    mouseScroll = false;
    //скролинг виджетов настроек каналов
    //QScroller *scroller = QScroller::scroller(ui->scrollAreaChannelOptions);
//    scroller->grabGesture(ui->scrollAreaChannelOptions, QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollAreaChannelOptions->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_2->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller::grabGesture(ui->scrollArea_3->viewport(), QScroller::LeftMouseButtonGesture);

    //настройки для архива
    if(arch != NULL)
    {
        connect(arch, SIGNAL(loadFinished()), this, SLOT(drowGraf()));
        connect(arch, SIGNAL(loadFinished()), this, SLOT(initComboChannels()));
    }

    // устанавливаем евент фильтры чтобы при нажатии на поле появлялась клавиатура
//    QList<QSpinBox*> spinList = findChildren<QSpinBox*> ();
//    for (int i = 0; i < spinList.count(); ++i) {
//        QSpinBox *sb = spinList.at(i);
//        sb->installEventFilter(this);
//    }
    QList<QLineEdit*> lineeditList = findChildren<QLineEdit*> (  );
    for (int i = 0; i < lineeditList.count(); ++i) {
        QLineEdit *le = lineeditList.at(i);
        le->installEventFilter(this);
    }
//    QList<QDoubleSpinBox*> SpinBox = findChildren<QDoubleSpinBox*> ();
//    for (int i = 0; i < SpinBox.count(); ++i) {
//        QDoubleSpinBox *spbox = SpinBox.at(i);
//        spbox->installEventFilter(this);
//    }
    QList<QComboBox*> ComboBox = findChildren<QComboBox*> ();
    for (int i = 0; i < ComboBox.count(); ++i) {
        QComboBox *combox = ComboBox.at(i);
        combox->installEventFilter(this);
    }
    QList<QComboBox *> combos = ui->stackedWidget->findChildren<QComboBox *>();
    foreach(QComboBox * combo, combos)
    {
        QListView *view = new QListView(combo);
        view->setStyleSheet("QListView::item{height: 50px;}\n QListView::item:selected{background-color:rgb(44, 61, 77);}");
        QFont font;
        font.setFamily(QStringLiteral("Open Sans"));
        font.setPointSize(14);
        view->setFont(font);
        QScroller::grabGesture(view->viewport(), QScroller::LeftMouseButtonGesture);
        combo->setView(view);
        QString comboStyle = combo->styleSheet();
        QString styleAppend = "\nQComboBox::drop-down {\n	width:0px;\n }";
        combo->setStyleSheet(comboStyle + styleAppend);
//        view->deleteLater();
    }

    // стили для всех QScrollBar
    QString styleScrollBars = "QScrollBar:vertical {              \n        background:rgb(179, 179, 179);\n        margin: 0px 0px 0px 0px;\n		border-radius: 0px;\n		width:30px; \n    }\nQScrollBar::handle:vertical {\n       background:rgb(77, 77, 77);\n        min-height: 50px;\n		border-radius: 0px;\n    }\nQScrollBar::add-line:vertical {\n       background:rgb(179, 179, 179);\n        height: 0px;\n        subcontrol-position: bottom;\n        subcontrol-origin: margin;\n		border-radius: 0px;\n    }\n\nQScrollBar::sub-line:vertical {\n        background:rgb(77, 77, 77);\n        height: 0 px;\n        subcontrol-position: top;\n        subcontrol-origin: margin;\n		border-radius: 0px;\n    }\nQScrollBar::up-arrow:vertica, QScrollBar::down-arrow:verticall {\n        background:rgb(77, 77, 77);\n        height: 0px;\n		border-radius: 0px;\n    }\n\nQScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n    background: none;\n}\n";
    QList<QScrollBar *> scrolls = ui->stackedWidget->findChildren<QScrollBar *>();
    foreach(QScrollBar * scroll, scrolls)
    {
        scroll->setStyleSheet(styleScrollBars);
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


void dSettings::updateVer()
{
    QString ver = CURRENT_VER;
    QString name = "MULTIGRAPH";
    if(systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel)
    {
        ver = ver + "S";
        name = name + "<br>STEEL";
    }
    ui->name->setText(QString("<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">" + name + "<br/>Ver. " + ver + "</span></p></body></html>"));

}

dSettings::~dSettings()
{
    disconnect(&timerUpdateGraf, SIGNAL(timeout()), this, SLOT(replotGraf()));
    disconnect(&timerLoad, SIGNAL(timeout()), this, SLOT(timeoutLoad()));
    disconnect(&tUpdateTime, SIGNAL(timeout()), this, SLOT(DateUpdate()));
    if(arch != NULL)
    {
        disconnect(arch, SIGNAL(loadFinished()), this, SLOT(drowGraf()));
        disconnect(arch, SIGNAL(loadFinished()), this, SLOT(initComboChannels()));
        delete arch;
    }
    disconnect(&tUpdateBar, SIGNAL(timeout()), this, SLOT(updateBar()));
    delete ui;
}

void dSettings::DateUpdate() // каждую секунду обновляем значок времени
{
    QDateTime local(QDateTime::currentDateTime());
    QString str = "<html><head/><body><p align=\"center\"><span style=\" font-size:22pt; color:#ffffff;\">" \
                  + local.time().toString(timestrings.at(systemOptions.timeindex)) + \
                  "</span><span style=\" color:#ffffff;\"><br/></span>" \
                  "<span style=\" font-size:17pt; color:#ffffff;\">" \
                  + local.date().toString(datestrings.at(systemOptions.dateindex)) + \
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
        ui->srcChannel->clear();
        ui->srcChannel->addItem("---");
        mListChannel.lock();
        foreach (ChannelOptions * ch, listChannels) {
//            if(ch->enable)
//            {
                ui->srcChannel->addItem(ch->GetChannelName() + " (" + QString::number(ch->getNum()) + ")");
//            }
        }
        mListChannel.unlock();
        fInitCompoCopyChannels = true;

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
            if(mes.value("C") != cLogger::SERVICE &&\
                    (mes.value("C") != cLogger::ERR))
            {
                QString num = QString("%1").arg(var+1, 4, 10, QChar('0'));
                ui->listWidget->addItem(num + " | " + mes.value("D").toString() \
                                        + " " +  mes.value("T").toString() \
                                        + " | "+ mes.value("M").toString());
            }
        }
        ui->listWidget->scrollToBottom();
    }
    else if(ui->stackedWidget->currentIndex() == 2)
    {
        h = 44;



        //страница архива
        ui->saveButton->hide();
        ui->nameSubMenu->setText("АРХИВ");

//        initComboChannels();

//        ui->combo1ChannelArch->setCurrentIndex(1);
//        ui->combo2ChannelArch->setCurrentIndex(2);
//        ui->combo3ChannelArch->setCurrentIndex(3);
//        ui->combo4ChannelArch->setCurrentIndex(4);


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
        periodShift = 0;
        ui->period->setEnabled(false);
        fNeadRescale = true;
        arch->load(archivePeriod, periodShift);


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
        ui->nameSubMenu->setText("<html><head/><body><p>АРХИВ<br>ЗАМЕРОВ</p></body></html>");
        h = 72;
        QString noData = "Нет данных";
        ui->nameSteelTech->setText(noData);
        ui->steelTemp->setText(noData);
        ui->steelEmf->setText(noData);
        ui->steelAO->setText(noData);
        ui->steelAl->setText(noData);
        ui->steelC->setText(noData);

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

void dSettings::initComboChannels(void)
{
//    static bool fInit = false;

    if(!fInitComboChannles)
    {

        QList<QComboBox*> listCombo;
        listCombo.append(ui->combo1ChannelArch);
        listCombo.append(ui->combo2ChannelArch);
        listCombo.append(ui->combo3ChannelArch);
        listCombo.append(ui->combo4ChannelArch);

        QStringList channels;
        mListChannel.lock();
        foreach (ChannelOptions * channel, listChannels) {
            channels << QString(channel->GetChannelName() + " (" \
                                + QString::number(channel->getNum()) + ")");
        }
        mListChannel.unlock();

        foreach(QComboBox * combo, listCombo)
        {
            combo->clear();
            combo->addItem("ОТКЛ.");
            combo->addItems(channels);
        }

    }

    fInitComboChannles = true;
}

void dSettings::on_verticalScrollBar_sliderMoved(int position)
{
    ui->listWidget->setCurrentRow(position);
}

void dSettings::on_period_currentIndexChanged(int index)
{
    //               1 мин 10 мин 1 час 10 часов сутки  неделя  месяц    3 месяца год
    int periods[] = {60,   600,   3600, 36000,   86400, 604800, 2592000, 7776000, 31104000};
    if((sizeof(periods) / sizeof(int)) <= index) return;
    periodShift = 0;
    archivePeriod = periods[index];
    loadArchFromFile();
}

void dSettings::updateGraf(int period)
{

    assert(arch != NULL);
    if(arch == NULL) return;
    strLabel = "hh:mm:ss\ndd.MM.yy";
//                   1 мин 10 мин 1 час 10 часов сутки  неделя  месяц    3 месяца год
//    int periods[] = {60,   600,   3600, 36000,   86400, 604800, 2592000, 7776000, 31104000};
//    QStringList listLabels;
//    //            1 мин      10 мин        1 час      10 часов
//    listLabels << "hh:mm:ss" << "hh:mm:ss" << "hh:mm:ss" << "hh:mm:ss"
//    //              сутки       неделя     месяц      3 месяца      год
//               << "dd.MM hh:mm" << "dd.MM.yy" << "dd.MM.yy" << "dd.MM.yy" << "dd.MM.yy";

    /*QDateTime */firstTime = QDateTime::currentDateTime().addSecs(-period-periodShift);

    /*int */multiplier = 1;
    if(period >= 604800)
    {
        multiplier = 600;
    }
    int periodToGraf = period / multiplier;



    // копирование уже считанных с файлов данных
    Y_coordinates_Chanel_1 = arch->getVector(ui->combo1ChannelArch->currentIndex() - 1);
    Y_coordinates_Chanel_2 = arch->getVector(ui->combo2ChannelArch->currentIndex() - 1);
    Y_coordinates_Chanel_3 = arch->getVector(ui->combo3ChannelArch->currentIndex() - 1);
    Y_coordinates_Chanel_4 = arch->getVector(ui->combo4ChannelArch->currentIndex() - 1);

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

    //Расстановка меток времени согласно периоду
//    strLabel = "hh:mm:ss";
//    if(listLabels.size() == (sizeof(periods)/sizeof(int)))
//    {
//        for(int i = 0; i < listLabels.size(); i++)
//        {
//            strLabel = "dd.MM.yy\nhh:mm:ss";//listLabels.at(i);
//            if(period <= periods[i]) break;
//        }
//    }
//    //Генерация шкалы Х
    double first = firstTime.toTime_t();
    for(int i = 0; i < periodToGraf; i ++)
    {
        X_Coordinates.append(first + i * multiplier);
    }
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat(strLabel);
    ui->customPlot->xAxis->setAutoTickStep(true);
    ui->customPlot->xAxis->setTickLabelFont(QFont("Open Sans", 10));

    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");

    if(!Y_coordinates_Chanel_1.isEmpty())
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);


    // add the arrow:

    QPen graphPen;

    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(ColorCh1);

    ui->customPlot->graph()->setPen(graphPen);

    if(!Y_coordinates_Chanel_2.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(ColorCh2);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if(!Y_coordinates_Chanel_3.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(ColorCh3);
        ui->customPlot->graph()->setPen(graphPen);
    }

    if(!Y_coordinates_Chanel_4.isEmpty())
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(ColorCh4);
        ui->customPlot->graph()->setPen(graphPen);
    }


    if((!Y_coordinates_Chanel_1.isEmpty()) &&\
            !Y_coordinates_Chanel_2.isEmpty() &&\
            !Y_coordinates_Chanel_3.isEmpty() &&\
            !Y_coordinates_Chanel_4.isEmpty())
    {
        // авто масшабирование
        ui->customPlot->rescaleAxes();
        if(fNeadRescale)
        {
            ui->customPlot->xAxis->setRange(first, first+periodToGraf*multiplier);
        }
        else
        {
            ui->customPlot->xAxis->setRange(posPlotX, sizePlotX, Qt::AlignCenter);
        }
        ui->customPlot->replot();
        ui->customPlot->clearItems();// удаляем стрелочку а то она будет потом мешаться
    }

}

void dSettings::addChannel(QList<ChannelOptions *> channels, int num)
{
    //запоминаем канал, чтобы потом в него сохранить изменения
//    listChannels = channels;
    int ch_num = num;
    int ust_num = num;
    if(ch_num == 0) ch_num = 1;
    mListChannel.lock();
    if(ch_num > listChannels.size()) ch_num = listChannels.size();
    numChannel = ch_num;

    if(listChannels.size() > 0)
    {
        channel = listChannels.at(ch_num-1);
        ui->typeSignal->setCurrentIndex(getIndexSignalTypeTable(channel->GetSignalType()));
        ui->nameChannel->setText(channel->GetChannelName().toUtf8());
        ui->scaleUp->setValue(channel->GetHigherMeasureLimit());
        ui->scaleDown->setValue(channel->GetLowerMeasureLimit());
        ui->periodCh->setValue(channel->GetMeasurePeriod());
        ui->labelNumChannel->setText("КАНАЛ #" + QString::number(channel->getNum()));
        ui->bar->setNumChan(channel->getNum());
        ui->dempfer->setValue(channel->GetDempherValue());
        ui->typeReg->setCurrentIndex(channel->GetRegistrationType());
        ui->sensorShema->setCurrentIndex(channel->getShema() /*indexUiShemaFromSensorShema(channel->getShema())*/);
        ui->enableColdJunction->setCurrentIndex(channel->getStateColdJunction());
        ui->shiftColdJunction->setValue(channel->getShiftColdJunction());
        ui->comboTypeValue->setCurrentIndex(channel->getVoltageType());
        ui->comboPrecision->setCurrentIndex(getIndexTablePrecisions(channel->getPrecision()));
        ui->comboUnit->setCurrentIndex(channel->getIndexMultiplier());
//        ui->unit->setText(channel->GetUnitsName().toUtf8());
//        ui->comboCapacity->setCurrentIndex(channel->getCapacity());
    }
    mListChannel.unlock();

    //параметры уставок
    mListUstvok.lock();
    if(ust_num > listUstavok.size()) ust_num = listUstavok.size();
    if(listUstavok.size() > 0)
    {
        ustavka = listUstavok.at(ust_num-1);
        ui->ustavkaVol->setValue(ustavka->getHiStateValue());
        //    ui->ustavkaVolDown->setValue(ustavka->getLowStateValue());
        ui->gisterezis->setValue(ustavka->getHiHisteresis());
        ui->ustavkaChannel->setCurrentIndex(ustavka->getChannel());
        ui->typeFix->setCurrentIndex(ustavka->getTypeFix());
        ui->identifikator->setText(ustavka->getIdentifikator());
        ui->labelNumUsatvka->setText("УСТАВКИ #" + QString::number(ust_num));
        ui->reley->setCurrentIndex(ustavka->getnumRelayUp());
        //    ui->releyDown->setCurrentIndex(ustavka->getnumRelayDown());
        ui->messageOn->setText(ustavka->getMessInHigh());
        ui->messageOff->setText(ustavka->getMessNormHigh());
        ui->kvitir->setCurrentIndex(ustavka->getKvitirUp());
        //    ui->messageOnDown->setText(ustavka->getMessInLow());
        //    ui->messageOffDown->setText(ustavka->getMessNormHigh());
    }
    mListUstvok.unlock();

    connect(&tUpdateBar, SIGNAL(timeout()), this, SLOT(updateBar()));
    tUpdateBar.start(TIME_UPDATE_BAR);

}

void dSettings::setColorBar(QColor color, QColor colotLihgt)
{
    ui->bar->setColor(color, colotLihgt);
}

void dSettings::updateBar()
{
    int diapason = ui->sensorDiapazon->currentIndex();
    int valueType = ui->comboTypeValue->currentIndex();
//    int signalType = tableSignalTypes[ui->typeSignal->currentIndex()].st;
    int multIndex = ui->comboUnit->currentIndex();
    if(channel != NULL) {
        ui->bar->setExtr(channel->ConvertVisualValue(channel->GetMinimumChannelValue(), diapason, valueType, multIndex),\
                         channel->ConvertVisualValue(channel->GetMaximumChannelValue(), diapason, valueType, multIndex));
        //        ui->bar->resetLim();
        ui->bar->cleanMarker();
        mListUstvok.lock();
        foreach (Ustavka * ustavka, listUstavok) {
            if(ustavka->getChannel() == channel->getNum())
            {
//                ui->bar->setLim(ustavka->getLowStateValue(), ustavka->getHiStateValue());
                ui->bar->addMarker(ustavka->getHiStateValue(), ustavka->getTypeFix());
//                ui->bar->addMarker(ustavka->getLowStateValue(), false);
//                break;
            }
        }
        mListUstvok.unlock();
//        ui->bar->setColor(ColorCh1, ColorCh1Light); //Vag: переделать на QColor
        ui->bar->setText(ui->nameChannel->text(), ui->unit->text());
        ui->bar->setBarDiapazon(channel->ConvertVisualValue(channel->getMaxInDiapason(diapason), diapason, valueType, multIndex),\
                                channel->ConvertVisualValue(channel->getMinInDiapason(diapason), diapason, valueType, multIndex));
        ui->bar->setValue(channel->ConvertVisualValue(channel->GetCurrentChannelValue(), diapason, valueType, multIndex));
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
        mListChannel.lock();
        if(listChannels.size() > 0)
        {
            channel->SetChannelName(ui->nameChannel->text().toUtf8());
            channel->SetSignalType(tableSignalTypes[ui->typeSignal->currentIndex()].st);
            channel->SetHigherMeasureLimit(ui->scaleUp->value());
            channel->SetLowerMeasureLimit(ui->scaleDown->value());
            channel->SetUnitsName(ui->unit->text().toUtf8());
            channel->SetMeasurePeriod(ui->periodCh->value());
            channel->SetDempher(ui->dempfer->value());
            channel->SetRegistrationType(ui->typeReg->currentIndex());
            int diapasone = ui->sensorDiapazon->currentIndex();
            if(channel->GetSignalType() == VoltageMeasure)
            {
                diapasone = tableVoltageDiapasone[ui->sensorDiapazon->currentIndex()].diapason;
            }
            else if(channel->GetSignalType() == TermoCoupleMeasure)
            {
                diapasone = tableDiapasoneTC[ui->sensorDiapazon->currentIndex()].diapason;
            }
            else if(channel->GetSignalType() == TermoResistanceMeasure)
            {
                diapasone = tableDiapasoneRTD[ui->sensorDiapazon->currentIndex()].diapason;
            }
            channel->setShiftColdJunction(ui->shiftColdJunction->value());
            channel->enableColdJunction(ui->enableColdJunction->currentIndex());
            channel->SetDiapasonShema(diapasone, ui->sensorShema->currentIndex() /*sensorShemaFromUiShemaIndex(ui->sensorShema->currentIndex())*/);
            channel->SetUserDiapason(ui->sensorDiapazon->currentIndex());
            channel->setVolueVoltageType(ui->comboTypeValue->currentIndex());
            channel->setPrecision(tablePrecisions[ui->comboPrecision->currentIndex()].precision);
            channel->setIndexMultiplier(ui->comboUnit->currentIndex());
//            channel->setShema(sensorShemaFromUiShemaIndex(ui->sensorShema->currentIndex()));
        }
        mListChannel.unlock();

        mListUstvok.lock();
        if(listUstavok.size() > 0)
        {
            ustavka->setUstavka(\
                        ui->identifikator->text().toUtf8(), \
                        ui->ustavkaChannel->currentIndex(), \
                        ui->typeFix->currentIndex(), \
                        ui->ustavkaVol->value(), \
                        /*ui->ustavkaVolDown->value(),*/ \
                        ui->gisterezis->value(), \
                        /*ui->gisterezis->value(),*/ \
                        ui->reley->currentIndex() \
                        /*ui->releyDown->currentIndex()*/ \
                        );
            ustavka->setMessInHigh(ui->messageOn->text().toUtf8());
            ustavka->setMessNormHigh(ui->messageOff->text().toUtf8());
            ustavka->setKvitirUp(ui->kvitir->currentIndex());
            //        ustavka->setMessInLow(ui->messageOnDown->text().toUtf8());
            //        ustavka->setMessNormLow(ui->messageOffDown->text().toUtf8());
        }
        mListUstvok.unlock();
    }





}

void dSettings::saveParamToFile()
{
    ui->load->setHidden(false);
//     засекаем время записи настроек в файл или ждать сигнал о завершении
    timerLoad.start(1000);

    //сохраняемся
    saveParam();
    if(systemOptions.display == cSystemOptions::Steel)
    {
        if(cFileManager::writeSteelsSettings(pathtosteeloptions))
        {
            cLogger mk(pathtomessages, cLogger::CONFIG);
            mk.addMess("Error write to file " + QString(pathtosteeloptions), cLogger::ERR);
        }
    }
    else
    {
        if(cFileManager::writeChannelsSettings(pathtooptions/*, listChannels*/))
        {
            cLogger mk(pathtomessages, cLogger::CONFIG);
            mk.addMess("Error write to file " + QString(pathtooptions), cLogger::ERR);
        }
    }
}

void dSettings::loadArchFromFile()
{
    ui->loadArchive->setVisible(true);
    moArch.start();
    ui->period->setEnabled(false);
    //Сброс буфера перед новым запросом
    Y_coordinates_Chanel_1.reserve(0);
    Y_coordinates_Chanel_2.reserve(0);
    Y_coordinates_Chanel_3.reserve(0);
    Y_coordinates_Chanel_4.reserve(0);
    // Запрос данных из файлов архива
    fNeadRescale = true;
    arch->load(archivePeriod, periodShift);
}

//void dSettings::on_buttonBackUstavki_clicked()
//{
//    ui->stackedWidget->setCurrentIndex(0);
//    updateWidgets();
//}

void dSettings::on_saveButton_clicked()
{
    saveParamToFile();
}

void dSettings::on_exitButton_clicked()
{

        this->close();
}

void dSettings::timeoutLoad()
{
        this->close();
}

//void dSettings::on_buttonUstavk_clicked()
//{
//    //тут запускать окно настройки уставок или переключить вид главного фрэйма
//    ui->stackedWidget->setCurrentIndex(3);
//    updateWidgets();
//}

void dSettings::resizeEvent(QResizeEvent * s)
{
//    updateBar();
}

bool dSettings::eventFilter(QObject *watched, QEvent *event)
{
#ifndef Q_OS_WIN
//#endif
    if ( (event->type() == QEvent::MouseButtonRelease) && \
         (watched->property("enabled").toString() == "true") && \
         (/*( QString::fromLatin1(watched->metaObject()->className()) == "QSpinBox") ||*/ \
          (QString::fromLatin1(watched->metaObject()->className()) == "QLineEdit")/* ||*/ \
          /*(QString::fromLatin1(watched->metaObject()->className()) == "QDoubleSpinBox")*/))
    {
        //Vag: нужно переделать и передавать строку напрямую в конструктор клавиатуры
        keyboard::olderprop = watched->property("text").toString();
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

    if ( (event->type() == QEvent::MouseButtonRelease) && \
         (QString::fromLatin1(watched->metaObject()->className()) == "QComboBox"))
    {

    }


    if (watched == ui->customPlot && event->type() != QEvent::Paint \
            && event->type() != QEvent::Resize) {
        if (watched == ui->customPlot && event->type() == QEvent::MouseButtonPress) {
            reactOnMousePress();
        }

        if (watched == ui->customPlot && event->type() == QEvent::MouseButtonRelease) {
            reactOnMouseRelease();
        }

        if (watched == ui->customPlot && event->type() == QEvent::MouseMove) {
            ReactOnMouseSlide();
        }
        //Vag: Крайне не рекомендуется передавать управление событием в QCustomPlot,
        //      так как в текущей сборке Linux модуль работает некорректно,
        //      поэтому возвращаем true и завершаем обработку события
        return true;
    }


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
    if(index == NoMeasure)
    {
        ui->sensorDiapazon->hide();
        ui->sensorShema->hide();
        ui->labelDiapazon->hide();
        ui->labelShema->hide();
        ui->enableColdJunction->hide();
        ui->labelColdJunction->hide();
        ui->shiftColdJunction->hide();
        ui->labelShiftColdJunction->hide();
        ui->labelValue->hide();
        ui->comboTypeValue->hide();
        ui->comboUnit->hide();
        ui->labelUnit->hide();
    }
    else if(index == CurrentMeasure)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListCurrent);
        if((channel->GetUserDiapason() < 0) || (channel->GetUserDiapason() > StringListCurrent.size()))
        {
            ui->sensorDiapazon->setCurrentIndex(0);
        }
        else
        {
            ui->sensorDiapazon->setCurrentIndex(channel->GetUserDiapason());
        }
        ui->sensorDiapazon->show();
        ui->sensorShema->hide();
        ui->labelDiapazon->show();
        ui->labelShema->hide();
        ui->enableColdJunction->hide();
        ui->labelColdJunction->hide();
        ui->shiftColdJunction->hide();
        ui->labelShiftColdJunction->hide();
        ui->labelValue->show();
        ui->comboTypeValue->show();
        ui->comboUnit->hide();
        ui->labelUnit->hide();
    }
    else if(index == VoltageMeasure)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListNapryagenie);
        if((channel->GetUserDiapason() < 0) || (channel->GetUserDiapason() > StringListNapryagenie.size()))
        {
            ui->sensorDiapazon->setCurrentIndex(0);
        }
        else
        {
            ui->sensorDiapazon->setCurrentIndex(channel->GetUserDiapason());
        }
        ui->sensorDiapazon->setCurrentIndex(channel->GetUserDiapason());
        ui->sensorDiapazon->show();
        ui->labelDiapazon->show();
        ui->sensorShema->hide();
        ui->labelShema->hide();
        ui->enableColdJunction->hide();
        ui->labelColdJunction->hide();
        ui->shiftColdJunction->hide();
        ui->labelShiftColdJunction->hide();
        ui->labelValue->show();
        ui->comboTypeValue->show();
        ui->comboUnit->show();
        ui->labelUnit->show();
//        if((channel->GetUserDiapason() == ChannelOptions::Voltage150mV) || \
//                (channel->GetUserDiapason() == ChannelOptions::Voltage300mV) || \
//                (channel->GetUserDiapason() == ChannelOptions::Voltage1V) || \
//                (channel->GetUserDiapason() == ChannelOptions::Voltage0_1V))
//        {
//            ui->comboUnit->setCurrentIndex(0);
//        }
//        else
//        {
//            ui->comboUnit->setCurrentIndex(1);
//        }
    }
    else if(index == TermoCoupleMeasure)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListTC);
        ui->sensorDiapazon->setCurrentIndex(getIndexTableTC(channel->GetDiapason()));
        ui->sensorDiapazon->show();
        ui->labelDiapazon->show();
        ui->sensorShema->hide();
        ui->labelShema->hide();
        ui->enableColdJunction->show();
        ui->labelColdJunction->show();
        ui->shiftColdJunction->show();
        ui->labelShiftColdJunction->show();
        ui->labelValue->hide();
        ui->comboTypeValue->hide();
        ui->comboTypeValue->setCurrentIndex(ChannelOptions::Value_Real);
        ui->comboUnit->hide();
        ui->labelUnit->hide();
    }
    else if(index == TermoResistanceMeasure)
    {
        ui->sensorDiapazon->clear();
        ui->sensorDiapazon->addItems(StringListRTD);
        ui->sensorDiapazon->setCurrentIndex(getIndexTableRTD(channel->GetDiapason()));
        ui->sensorShema->setCurrentIndex(channel->getShema() /*indexUiShemaFromSensorShema(channel->getShema())*/);
        ui->sensorDiapazon->show();
        ui->labelDiapazon->show();
        ui->sensorShema->show();
        ui->labelShema->show();
        ui->enableColdJunction->hide();
        ui->labelColdJunction->hide();
        ui->shiftColdJunction->hide();
        ui->labelShiftColdJunction->hide();
        ui->labelValue->hide();
        ui->comboTypeValue->hide();
        ui->comboTypeValue->setCurrentIndex(ChannelOptions::Value_Real);
        ui->comboUnit->hide();
        ui->labelUnit->hide();
    }


    ui->comboTypeValue->setCurrentIndex(channel->getVoltageType());
    if(ui->comboTypeValue->currentIndex() == ChannelOptions::Value_Real)
    {
        ui->unit->setText(channel->getNameUnitByParam(index, ui->comboUnit->currentIndex()));
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

/*
 * Получение индекса в таблице по диапазону напряжений
 */
int dSettings::getIndexVoltageTable(int d)
{
    for(int i = 0; i < sizeof(tableVoltageDiapasone)/sizeof(typeTableDiapasone); i++)
    {
        if(tableVoltageDiapasone[i].diapason == d)
            return i;
    }
    return 0;
}

/*
 * Получение индекса в таблице по диапазону TC
 */
int dSettings::getIndexTableTC(int d)
{
    for(int i = 0; i < sizeof(tableDiapasoneTC)/sizeof(typeTableDiapasoneTC); i++)
    {
        if(tableDiapasoneTC[i].diapason == d)
            return i;
    }
    return 0;
}

/*
 * Получение индекса в таблице по диапазону TC
 */
int dSettings::getIndexTableRTD(int d)
{
    for(int i = 0; i < sizeof(tableDiapasoneRTD)/sizeof(typeTableDiapasoneRTD); i++)
    {
        if(tableDiapasoneRTD[i].diapason == d)
            return i;
    }
    return 0;
}

/*
 * Получение индекса в таблице по точности
 */
int dSettings::getIndexTablePrecisions(int p)
{
    for(int i = 0; i < sizeof(tablePrecisions)/sizeof(typeTablePrecisions); i++)
    {
        if(tablePrecisions[i].precision == p)
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

void dSettings::reactOnMousePress()
{
    xPos = ui->customPlot->mapFromGlobal(QCursor::pos()).x();
    yPos = ui->customPlot->mapFromGlobal(QCursor::pos()).y();
    int width = ui->customPlot->width();
    int height = ui->customPlot->height();
    if((xPos > (width / 10)) && (yPos < (height * 0.9)))
    {
        mouseOnMove = true;
    }
    else if((xPos < (width / 10)) && (yPos < (height * 0.9)))
    {
        mouseOnScalede = true;
    }
    else if((xPos > (width / 10)) && (yPos > (height * 0.9)))
    {
        mouseOnScaledeX = true;
    }

    sizePlot = ui->customPlot->yAxis->range().size();
    posPlot = ui->customPlot->yAxis->range().center();
    sizePlotX = ui->customPlot->xAxis->range().size();
    posPlotX = ui->customPlot->xAxis->range().center();
//    waitAutoScale = true;
}

void dSettings::reactOnMouseRelease()
{
    mouseOnScalede = false;
    mouseOnMove = false;
    mouseOnScaledeX= false;
//    timerScale.start(3000);
}

void dSettings::ReactOnMouseSlide()
{
    if(mouseOnScalede && !mouseOnMove)
    {
        int y = ui->customPlot->mapFromGlobal(QCursor::pos()).y();;
        double scale = 1 + (((double)y - (double)yPos) / 2000);
        double pos = ui->customPlot->yAxis->range().center();
        double size = sizePlot * scale * scale ;
        ui->customPlot->yAxis->setRange(pos, size, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    else if(mouseOnScaledeX && !mouseOnMove)
    {
        int x = ui->customPlot->mapFromGlobal(QCursor::pos()).x();;
        double scale = 1 + (((double)xPos - (double)x) / 2000);
        double pos = ui->customPlot->xAxis->range().center();
        double size = sizePlotX * scale * scale ;
        ui->customPlot->xAxis->setRange(pos, size, Qt::AlignCenter);

//        ui->customPlot->replot();
    }
    else if(mouseOnMove)
    {
        int y = ui->customPlot->mapFromGlobal(QCursor::pos()).y();
        int x = ui->customPlot->mapFromGlobal(QCursor::pos()).x();
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
    QStringList listRelayNames;
    int sizeR = listRelais.size();
    for (int i = 0; i < sizeR; i++) {
        listRelayNames.append("РЕЛЕ " + QString::number(i+1));
    }
    ui->steelRelayBreak->clear();
    ui->steelRelayReady->clear();
    ui->steelRelayMeasure->clear();
    ui->steelRelayTimeOut->clear();
    ui->steelRelayBreak->addItem("ОТКЛ.");
    ui->steelRelayReady->addItem("ОТКЛ.");
    ui->steelRelayMeasure->addItem("ОТКЛ.");
    ui->steelRelayTimeOut->addItem("ОТКЛ.");
    ui->steelRelayBreak->addItems(listRelayNames);
    ui->steelRelayReady->addItems(listRelayNames);
    ui->steelRelayMeasure->addItems(listRelayNames);
    ui->steelRelayTimeOut->addItems(listRelayNames);
    if(sizeR > 0)
    {
        ui->steelRelayBreak->setCurrentIndex(\
                    (curSteel->relais[0] < sizeR) ?\
                    (curSteel->relais[0] + 1) :\
                    0);
        ui->steelRelayReady->setCurrentIndex(\
                    (curSteel->relais[1] < sizeR) ?\
                    (curSteel->relais[1] + 1) :\
                    0);
        ui->steelRelayMeasure->setCurrentIndex(\
                    (curSteel->relais[2] < sizeR) ?\
                    (curSteel->relais[2] + 1) :\
                    0);
        ui->steelRelayTimeOut->setCurrentIndex(\
                    (curSteel->relais[3] < sizeR) ?\
                    (curSteel->relais[3] + 1) :\
                    0);
    }
    typeSteelTech * tech = curSteel->technology;
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
//    ui->customPlotSteel->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
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

        X_Steel.resize(0);
        for(int i = 0; i < max(Y_SteelTemp.size(), Y_SteelEds.size()); i++)
        {
//            if(!std::isnan(Y_SteelTemp.at(i)) && !std::isnan(Y_SteelEds.at(i)))
//            {
                X_Steel.append(i*0.1);
//            }
        }

        ui->customPlotSteel->clearGraphs();
        ui->customPlotSteel->addGraph();
        ui->customPlotSteel->graph()->setData(X_Steel, Y_SteelTemp);
        ui->customPlotSteel->xAxis->setLabel("ВРЕМЯ, сек.");
        ui->customPlotSteel->xAxis->setAutoTickStep(false);
//        if(X_Steel.size() > 20)
//        {
            ui->customPlotSteel->xAxis->setTickStep(1);
//        }
//        else
//        {
//            ui->customPlotSteel->xAxis->setTickStep(0.5);
//        }
        ui->customPlotSteel->yAxis2->setLabel("ЭДС, мВ");
        ui->customPlotSteel->yAxis->setLabel("ТЕМПЕРАТУРА, °C");
        ui->customPlotSteel->graph()->setPen(QPen(QBrush(ColorCh3), 2));

        ui->customPlotSteel->addGraph(ui->customPlotSteel->xAxis, ui->customPlotSteel->yAxis2);
        ui->customPlotSteel->graph()->setData(X_Steel, Y_SteelEds);
        ui->customPlotSteel->graph()->setPen(QPen(QBrush(ColorCh4), 2));
        ui->customPlotSteel->rescaleAxes(true);
        double pos = ui->customPlotSteel->yAxis->range().center();
        double size = ui->customPlotSteel->yAxis->range().size() * 1.1;
        ui->customPlotSteel->yAxis->setRange(pos, size, Qt::AlignCenter);
        ui->customPlotSteel->replot();
        ui->customPlotSteel->clearItems();

        QString noData = "Нет данных";
        ui->nameSteelTech->setText(noData);
        ui->nameSteelSmelt->setText(noData);
        ui->steelTemp->setText(noData);
        ui->steelEmf->setText(noData);
        ui->steelAO->setText(noData);
        ui->steelAl->setText(noData);
        ui->steelC->setText(noData);

        if((json["Technology"].toString() != "nan") && (json["Technology"].toString() != "32767"))
            ui->nameSteelTech->setText(json["Technology"].toString());
        if((json["Smelt"].toString() != "nan") && (json["Smelt"].toString() != "32767"))
            ui->nameSteelSmelt->setText(json["Smelt"].toString());
        if((json["Temp"].toString() != "nan") && (json["Temp"].toString() != "32767"))
            ui->steelTemp->setText(json["Temp"].toString());
        if((json["Eds"].toString() != "nan") && (json["Eds"].toString() != "32767"))
            ui->steelEmf->setText(json["Eds"].toString());
        if((json["OxActivity"].toString() != "nan") && (json["OxActivity"].toString() != "32767"))
            ui->steelAO->setText(json["OxActivity"].toString());
        if((json["MassAl"].toString() != "nan") && (json["MassAl"].toString() != "32767"))
            ui->steelAl->setText(json["MassAl"].toString());
        if((json["Carbon"].toString() != "nan") && (json["Carbon"].toString() != "32767"))
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

void dSettings::on_bDeleteUstavka_clicked()
{
    mListUstvok.lock();
    listUstavok.removeAt(ustavka->getNum());
    mListUstvok.unlock();
    saveParamToFile();
    this->close();
}

void dSettings::on_bUserPeriod_clicked()
{
    QDateTime start = QDateTime::currentDateTime().addSecs(-archivePeriod-periodShift);
    QDateTime finish = QDateTime::currentDateTime().addSecs(-periodShift);
    cCustomPeriod dialogPeriod(start, finish);
    dialogPeriod.setModal(true);
    dialogPeriod.exec();
    archivePeriod = cCustomPeriod::startDT.secsTo(cCustomPeriod::finishDT);
    periodShift = cCustomPeriod::finishDT.secsTo(QDateTime::currentDateTime());
    loadArchFromFile();
}

void dSettings::updateArchGraf()
{
    if(arch != NULL)
    {
        sizePlotX = ui->customPlot->xAxis->range().size();
        posPlotX = ui->customPlot->xAxis->range().center();
        fNeadRescale = false;
        drowGraf();
    }
}

void dSettings::on_combo1ChannelArch_currentIndexChanged(int index)
{
    if((index >= 0) && (index <= listChannels.size()))
        updateArchGraf();
}

void dSettings::on_combo2ChannelArch_currentIndexChanged(int index)
{
    if((index >=0) && (index <= listChannels.size()))
        updateArchGraf();
}

void dSettings::on_combo3ChannelArch_currentIndexChanged(int index)
{
    if((index >=0) && (index <= listChannels.size()))
        updateArchGraf();
}

void dSettings::on_combo4ChannelArch_currentIndexChanged(int index)
{
    if((index >=0) && (index <= listChannels.size()))
        updateArchGraf();
}

void dSettings::on_srcChannel_currentIndexChanged(int index)
{
    if((index > 0) && fInitCompoCopyChannels)
    {
//        channel->copyOptions(listChannels.at(index-1));
        ChannelOptions * srcChannel = listChannels.at(index-1);
        ui->typeSignal->setCurrentIndex(getIndexSignalTypeTable(srcChannel->GetSignalType()));
        updateUiSignalTypeParam(getIndexSignalTypeTable(srcChannel->GetSignalType()));
//        ui->nameChannel->setText(srcChannel->GetChannelName().toUtf8());
        ui->scaleUp->setValue(srcChannel->GetHigherMeasureLimit());
        ui->scaleDown->setValue(srcChannel->GetLowerMeasureLimit());
        ui->periodCh->setValue(srcChannel->GetMeasurePeriod());
        ui->dempfer->setValue(srcChannel->GetDempherValue());
        ui->typeReg->setCurrentIndex(srcChannel->GetRegistrationType());
        ui->comboTypeValue->setCurrentIndex(srcChannel->getVoltageType());
        ui->comboPrecision->setCurrentIndex(getIndexTablePrecisions(srcChannel->getPrecision()));
        ui->unit->setText(srcChannel->GetUnitsName().toUtf8());
        if(srcChannel->GetSignalType() == CurrentMeasure)
        {
            ui->sensorDiapazon->setCurrentIndex(srcChannel->GetUserDiapason());
        }
        else if(srcChannel->GetSignalType() == VoltageMeasure)
        {
//            ui->sensorDiapazon->setCurrentIndex(getIndexVoltageTable(srcChannel->GetDiapason()));
            ui->sensorDiapazon->setCurrentIndex(srcChannel->GetUserDiapason());
            ui->comboUnit->setCurrentIndex(srcChannel->getIndexMultiplier());
        }
        else if(srcChannel->GetSignalType() == TermoCoupleMeasure)
        {
            ui->sensorDiapazon->setCurrentIndex(getIndexTableTC(srcChannel->GetDiapason()));
            ui->enableColdJunction->setCurrentIndex(srcChannel->getStateColdJunction());
            ui->shiftColdJunction->setValue(srcChannel->getShiftColdJunction());
        }
        else if(srcChannel->GetSignalType() == TermoResistanceMeasure)
        {
            ui->sensorDiapazon->setCurrentIndex(getIndexTableRTD(srcChannel->GetDiapason()));
            ui->sensorShema->setCurrentIndex(srcChannel->getShema());
        }
    }
}

void dSettings::on_comboTypeValue_currentIndexChanged(int index)
{
    switch(index)
    {
    case ChannelOptions::Value_Real:
        ui->unit->setText(channel->getNameUnitByParam(tableSignalTypes[ui->typeSignal->currentIndex()].st, \
                          ui->comboUnit->currentIndex()));
        break;
    case ChannelOptions::Value_Procent:
        ui->unit->setText("%");
        break;
    default:
        break;
    }
}

void dSettings::on_sensorDiapazon_currentIndexChanged(int index)
{
    if(ui->comboTypeValue->currentIndex() != ChannelOptions::Value_Procent)
    {
        ui->unit->setText(channel->getNameUnitByParam(tableSignalTypes[ui->typeSignal->currentIndex()].st,\
                          ui->comboUnit->currentIndex()));
    }
}



void dSettings::on_comboUnit_currentIndexChanged(int index)
{
    ui->unit->setText(channel->getNameUnitByParam(tableSignalTypes[ui->typeSignal->currentIndex()].st,\
                      ui->comboUnit->currentIndex()));
}
