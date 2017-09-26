#include "stackedoptions.h"
#include "ui_stackedoptions.h"
#include "defines.h"

QString StackedOptions::calibrationprm = "3383 3962 234 599";
QString StackedOptions::displayResolution = "1280x800";
QString StackedOptions::MonitorResolution = "35";
int StackedOptions::DisplayParametr = DisplayParametrEnum::Polar;


#define MainMenuIndex 0
#define OptionsIndex 1
#define WorkIndex 2
#define DiagnisticIndex 3
#define ExpertIndex 4
#define ChangeDateTimeIndex 5
#define ExtendedOptionsIndex 6
#define SystemIndex 7
#define InputsIndex 8
#define UniversalInputsIndex 9
#define Channel1Index 10
#define Channel2Index 11
#define Channel3Index 12
#define Channel4Index 13
#define RegimOtobrageniaIndex 14
#define PrilogenieIndex 15
#define MatematicaIndex 16
#define PredelnieZnacheniaIndex 17
#define UstavkiKanal1Index 18
#define UstavkiKanal2Index 19
#define UstavkiKanal3Index 20
#define UstavkiKanal4Index 21
#define Vyhody 22


StackedOptions::StackedOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StackedOptions)
{
    ui->setupUi(this);

    ui->DiapasonChannel_1->clear();
    ui->DiapasonChannel_2->clear();
    ui->DiapasonChannel_3->clear();
    ui->DiapasonChannel_4->clear();

    StringListNone.clear();
    StringListNone.append("Нет");

    StringListTok.clear();
    StringListTok.append("4-20 мA");
    StringListTok.append("0-20 мA");
    StringListTok.append("0-5  мA");
    StringListTok.append("0-20 мA с корнеизвлеч.");
    StringListTok.append("4-20 мA с корнеизвлеч.");
    StringListTok.append("± 20 мA");

    StringListNapryagenie.clear();
    StringListNapryagenie.append("0-1  В");
    StringListNapryagenie.append("0-10 В");
    StringListNapryagenie.append("0-5  В");
    StringListNapryagenie.append("1-5  В");
    StringListNapryagenie.append("±150 мВ");
    StringListNapryagenie.append("±1   В");
    StringListNapryagenie.append("±10  В");
    StringListNapryagenie.append("±30  В");
    StringListNapryagenie.append("0-1  В с корнеизвлеч.");
    StringListNapryagenie.append("0-10 В с корнеизвлеч.");
    StringListNapryagenie.append("1-5  В с корнеизвлеч.");

    StringListTC.clear();
    StringListTC.append("Pt100  (IEC)");
    StringListTC.append("Pt100  (JIS)");
    StringListTC.append("Pt100  (GOST)");
    StringListTC.append("Pt500  (IEC)");
    StringListTC.append("Pt500  (JIS)");
    StringListTC.append("Pt1000 (IEC)");
    StringListTC.append("Pt1000 (JIS)");
    StringListTC.append("Pt45   (GOST)");
    StringListTC.append("Pt50   (GOST)");
    StringListTC.append("Cu50   (GOST, a=4260)");
    StringListTC.append("Cu50   (GOST, a=4280)");
    StringListTC.append("Cu53   (GOST, a=4280)");
    StringListTC.append("Cu100  (GOST, a=4280)");

    StringListRTD.clear();
    StringListRTD.append("Тип А (W5Re-W20Re)");
    StringListRTD.append("Тип B (Pt30Rh-Pt60Rh)");
    StringListRTD.append("Тип C (W50Re-W26Re)");
    StringListRTD.append("Тип D (W30Re-W25Re)");
    StringListRTD.append("Тип J (Fe-CuNi)");
    StringListRTD.append("Тип K (NiCr-Ni)");
    StringListRTD.append("Тип L (Fe-CuNi)");
    StringListRTD.append("Тип L (NiCr-CuNi, GOST)");
    StringListRTD.append("Тип N (NiCrSi-NiSi)");
    StringListRTD.append("Тип R (Pt13Rh-Pt)");
    StringListRTD.append("Тип S (Pt10Rh-Pt)");
    StringListRTD.append("Тип T (Cu-CuNi)");

    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(SetLabelIndex(int)) );
    SetStackIndex(MainMenuIndex);


    // устанавливаем евент фильтры чтобы при нажатии на поле появлялась клавиатура

    QList<QSpinBox*> spinList = StackedOptions::findChildren<QSpinBox*> ();

    for (int i = 0; i < spinList.count(); ++i) {
        QSpinBox *sb = spinList.at(i);
        sb->installEventFilter(this);
    }

    QList<QLineEdit*> lineeditList = StackedOptions::findChildren<QLineEdit*> (  );

    for (int i = 0; i < lineeditList.count(); ++i) {
        QLineEdit *le = lineeditList.at(i);
        le->installEventFilter(this);
    }

    connect(ui->buttonGroup,SIGNAL(buttonClicked( int )), this, SLOT(Channel1TypeChange( int )) );
    connect(ui->buttonGroup_2,SIGNAL(buttonClicked( int )), this, SLOT(Channel2TypeChange( int )) );
    connect(ui->buttonGroup_3,SIGNAL(buttonClicked( int )), this, SLOT(Channel3TypeChange( int )) );
    connect(ui->buttonGroup_4,SIGNAL(buttonClicked( int )), this, SLOT(Channel4TypeChange( int )) );

    ReadSystemOptionsFromFile();
    ReadChannelsOptionsFromFile();
    ApplyNewSettingstoOptionsUI();

}

StackedOptions::~StackedOptions()
{
    delete ui;
}

void StackedOptions::on_pushButton_9_clicked()
{
    SetStackIndex(Vyhody );
}

void StackedOptions::SetLabelIndex(int newindex)
{
    ui->indexLabel->setText("Index " + QString::number(newindex));
}

void StackedOptions::SetStackIndex(int newstackindex)
{
    ui->stackedWidget->setCurrentIndex(newstackindex);
}

void StackedOptions::on_pushButton_10_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_clicked()
{
    SetStackIndex(WorkIndex);
}

void StackedOptions::on_pushButton_2_clicked()
{
    SetStackIndex(OptionsIndex);
}

void StackedOptions::on_pushButton_12_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_11_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_6_clicked()
{
    SetStackIndex(DiagnisticIndex);
}

void StackedOptions::on_pushButton_7_clicked()
{
    SetStackIndex(ExpertIndex);
}

void StackedOptions::on_pushButton_8_clicked()
{
    this->close();
}

void StackedOptions::on_pushButton_13_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_ChangeDateButton_clicked()
{
    SetStackIndex(ChangeDateTimeIndex);
}

void StackedOptions::on_pushButton_15_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_ExtendedOptionsButton_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

void StackedOptions::on_pushButton_16_clicked()
{
    SetStackIndex(OptionsIndex);
}

void StackedOptions::on_ChangeDateButton_2_clicked()
{
    SetStackIndex(ChangeDateTimeIndex);
}

void StackedOptions::on_pushButton_3_clicked()
{

    SetStackIndex(SystemIndex);
}

void StackedOptions::on_pushButton_4_clicked()
{
    SetStackIndex(InputsIndex);
}

void StackedOptions::on_pushButton_20_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

void StackedOptions::on_pushButton_23_clicked()
{
    SetStackIndex(InputsIndex);
}

void StackedOptions::on_pushButton_21_clicked()
{
    SetStackIndex(Channel1Index);
}

void StackedOptions::on_pushButton_26_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_5_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_22_clicked()
{
    SetStackIndex(Channel2Index);
}

void StackedOptions::on_pushButton_24_clicked()
{
    SetStackIndex(Channel3Index);
}

void StackedOptions::on_pushButton_25_clicked()
{
    SetStackIndex(Channel4Index);
}

void StackedOptions::on_pushButton_27_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_28_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_29_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_30_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

void StackedOptions::on_pushButton_36_clicked()
{
    SetStackIndex(WorkIndex);
}

void StackedOptions::on_pushButton_33_clicked()
{
    SetStackIndex(RegimOtobrageniaIndex);
}

void StackedOptions::on_pushButton_18_clicked()
{
    SetStackIndex(PrilogenieIndex);
}

void StackedOptions::on_pushButton_39_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

void StackedOptions::on_pushButton_41_clicked()
{
    SetStackIndex(PrilogenieIndex);
}

void StackedOptions::on_pushButton_37_clicked()
{
    SetStackIndex(MatematicaIndex);
}

void StackedOptions::on_pushButton_45_clicked()
{
    SetStackIndex(PrilogenieIndex);
}

void StackedOptions::on_pushButton_38_clicked()
{
    SetStackIndex(PredelnieZnacheniaIndex);
}

void StackedOptions::on_pushButton_46_clicked()
{
    SetStackIndex(UstavkiKanal1Index);
}

void StackedOptions::on_pushButton_47_clicked()
{
    SetStackIndex(PredelnieZnacheniaIndex);
}

void StackedOptions::on_pushButton_48_clicked()
{
    SetStackIndex(PredelnieZnacheniaIndex);
}

void StackedOptions::on_pushButton_49_clicked()
{
    SetStackIndex(PredelnieZnacheniaIndex);
}

void StackedOptions::on_pushButton_50_clicked()
{
    SetStackIndex(PredelnieZnacheniaIndex);
}

void StackedOptions::on_pushButton_44_clicked()
{
    SetStackIndex(UstavkiKanal2Index);
}

void StackedOptions::on_pushButton_42_clicked()
{
    SetStackIndex(UstavkiKanal3Index);
}

void StackedOptions::on_pushButton_43_clicked()
{
    SetStackIndex(UstavkiKanal4Index);
}


void StackedOptions::ReadSystemOptionsFromFile()
{
    QFile infile(pathtosystemoptions);
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    StackedOptions::calibrationprm = json["Calibration"].toString();
    StackedOptions::DisplayParametr = json["Display"].toInt();
    StackedOptions::displayResolution = json["Resolution"].toString();

    infile.close();
}


void StackedOptions::ReadChannelsOptionsFromFile()
{
    QFile infile(pathtooptions);
    infile.open(QIODevice::ReadOnly);
    QTextStream in(&infile);
    QString sss = in.readLine();
    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray array = json["channels"].toArray();
    infile.close();


    QJsonArray settings;

    QList<ChannelOptions *> ChannelsObjectsList;

    // Запихиваем адреса объектов каналов чтобы работать не с копиями а с оригиналами объектов
    ChannelsObjectsList.append(&options_channel1);
    ChannelsObjectsList.append(&options_channel2);
    ChannelsObjectsList.append(&options_channel3);
    ChannelsObjectsList.append(&options_channel4);
    QJsonObject jsonobj;

    int index = 0;


    foreach (ChannelOptions * Channel, ChannelsObjectsList) {
        jsonobj = array.at(index).toObject();

        Channel->SetHigherLimit(jsonobj.value("HigherLimit").toDouble());
        Channel->SetLowerLimit(jsonobj.value("LowerLimit").toDouble());
        Channel->SetHigherMeasureLimit(jsonobj.value("HigherMeasLimit").toDouble());
        Channel->SetLowerMeasureLimit(jsonobj.value("LowerMeasLimit").toDouble());
        Channel->SetSignalType(jsonobj.value("Type").toInt());
        Channel->SetUnitsName(jsonobj.value("Units").toString());
        Channel->SetMeasurePeriod(jsonobj.value("Period").toDouble());
        Channel->SetState1HighMessage(jsonobj.value("State1HighMessage").toString());
        Channel->SetState1LowMessage(jsonobj.value("State1LowMessage").toString());
        Channel->SetState2HighMessage(jsonobj.value("State2HighMessage").toString());
        Channel->SetState2LowMessage(jsonobj.value("State2LowMessage").toString());
        Channel->SetState1Value(jsonobj.value("State1Value").toDouble());
        Channel->SetState2Value(jsonobj.value("State2Value").toDouble());
        Channel->SetChannelName(jsonobj.value("Name").toString());
        Channel->SetMathEquation(jsonobj.value("MathString").toString());
        Channel->SetMathematical(jsonobj.value("MathWork").toBool());
        Channel->SetDiapason(jsonobj.value("Diapason").toInt());
        Channel->SetDempher(jsonobj.value("Dempher").toDouble());

//        qDebug() <<  index << ":" << jsonobj.value("Type").toInt() <<  " jsonobj.value(Type).toDouble() ";

        qDebug() <<  index << Channel->GetSignalType() << "xxx Channel->GetSignalType()";

        index ++ ;
    }
}

void StackedOptions::ApplyNewSettingstoOptionsUI()
{
    //    qDebug() << options_channel1.GetDiapason() << " options_channel1.GetDiapason() ";
    //    qDebug() << options_channel2.GetDiapason() << " options_channel2.GetDiapason() ";
    //    qDebug() << options_channel3.GetDiapason() << " options_channel3.GetDiapason() ";
    //    qDebug() << options_channel4.GetDiapason() << " options_channel4.GetDiapason() ";

    ui->PriborIdentifier->setText(CURRENT_VER);

    if (GetCurrentDisplayParametr() == Trends)
    {
        ui->SetTrendsBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == Bars)
    {
        ui->SetBarsBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == Polar)
    {
        ui->SetPolarBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == TrendsBars)
    {
        ui->SetTrendsBarsBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == Cyfra)
    {
        ui->SetCyfraBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == TrendsCyfraBars)
    {
        ui->SetTrendsBarsCyfraBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == TrendsCyfra)
    {
        ui->SetTrendsCyfraBtn->setChecked(true);
    }
    if (GetCurrentDisplayParametr() == BarsCyfra)
    {
        ui->SetBarsCyfraBtn->setChecked(true);
    }

    int sigtype  = options_channel1.GetSignalType();
    int sigtype2 = options_channel2.GetSignalType();
    int sigtype3 = options_channel3.GetSignalType();
    int sigtype4 = options_channel4.GetSignalType();

    //    qDebug() << sigtype <<  " sigtype ";
    //    qDebug() << sigtype2 << " sigtype2 ";
    //    qDebug() << sigtype3 << " sigtype3 ";
    //    qDebug() << sigtype4 << " sigtype4 ";



    //    ui->DiapasonChannel_1->clear();
    //    ui->DiapasonChannel_2->clear();
    //    ui->DiapasonChannel_3->clear();
    //    ui->DiapasonChannel_4->clear();

    // channel 1
    {
        if (sigtype == ModBus::NoMeasure)
        {
            ui->ButonOtklChannel_1->setChecked(true);

            ui->UnitsChannel_1->setText("None");
            ui->UnitsChannel_1->setEnabled(false);
            ui->VerhnPredelChannel_1->setEnabled(false);
            ui->NignPredelChannel_1->setEnabled(false);
            ui->NignPredIzmerChannel_1->setEnabled(false);
            ui->VerhnPredIzmerChannel_1->setEnabled(false);
            ui->PeriodIzmerChannel_1->setEnabled(false);
        }
        else
        {
            ui->UnitsChannel_1->setEnabled(true);
            ui->VerhnPredelChannel_1->setEnabled(true);
            ui->NignPredelChannel_1->setEnabled(true);
            ui->NignPredIzmerChannel_1->setEnabled(true);
            ui->VerhnPredIzmerChannel_1->setEnabled(true);
            ui->PeriodIzmerChannel_1->setEnabled(true);
        }

        if (sigtype == ModBus::CurrentMeasure)
        {
            ui->ButonTokChannel_1->setChecked(true);
            ui->DiapasonChannel_1->addItems(StringListTok);
        }

        if (sigtype == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_1->setChecked(true);
            ui->DiapasonChannel_1->addItems(StringListNapryagenie);

        }

        if (sigtype == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_1->setChecked(true);
            ui->DiapasonChannel_1->addItems(StringListRTD);
        }

        if (sigtype == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_1->setChecked(true);
            ui->DiapasonChannel_1->addItems(StringListTC);
        }

        if (sigtype == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_1->setChecked(true);
        }


        ui->DiapasonChannel_1->setCurrentIndex(options_channel1.GetDempherValue());
    }

    // channel 2
    {
        if (sigtype2 == ModBus::NoMeasure)
        {
            ui->ButonOtklChannel_2->setChecked(true);
            ui->UnitsChannel_2->setText("None");
            ui->UnitsChannel_2->setEnabled(false);
            ui->VerhnPredelChannel_2->setEnabled(false);
            ui->NignPredelChannel_2->setEnabled(false);
            ui->NignPredIzmerChannel_2->setEnabled(false);
            ui->VerhnPredIzmerChannel_2->setEnabled(false);
            ui->PeriodIzmerChannel_2->setEnabled(false);
        }
        else
        {
            ui->UnitsChannel_2->setEnabled(true);
            ui->VerhnPredelChannel_2->setEnabled(true);
            ui->NignPredelChannel_2->setEnabled(true);
            ui->NignPredIzmerChannel_2->setEnabled(true);
            ui->VerhnPredIzmerChannel_2->setEnabled(true);
            ui->PeriodIzmerChannel_2->setEnabled(true);
        }

        if (sigtype2 == ModBus::CurrentMeasure)
        {
            ui->ButonTokChannel_2->setChecked(true);
            ui->DiapasonChannel_2->clear();
            ui->DiapasonChannel_2->addItems(StringListTok);
        }

        if (sigtype2 == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_2->setChecked(true);
            ui->DiapasonChannel_2->addItems(StringListNapryagenie);
        }

        if (sigtype2 == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_2->setChecked(true);
            ui->DiapasonChannel_2->addItems(StringListRTD);
        }

        if (sigtype2 == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_2->setChecked(true);
            ui->DiapasonChannel_2->addItems(StringListTC);
        }

        if (sigtype2 == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_2->setChecked(true);
        }

        ui->DiapasonChannel_2->setCurrentIndex(options_channel2.GetDempherValue());
    }

    // channel 3
    {
        if (sigtype3 == ModBus::NoMeasure)
        {
            ui->ButonOtklChannel_3->setChecked(true);

            ui->UnitsChannel_3->setText("None");
            ui->UnitsChannel_3->setEnabled(false);
            ui->VerhnPredelChannel_3->setEnabled(false);
            ui->NignPredelChannel_3->setEnabled(false);
            ui->NignPredIzmerChannel_3->setEnabled(false);
            ui->VerhnPredIzmerChannel_3->setEnabled(false);
            ui->PeriodIzmerChannel_3->setEnabled(false);
        }
        else
        {
            ui->UnitsChannel_3->setEnabled(true);
            ui->VerhnPredelChannel_3->setEnabled(true);
            ui->NignPredelChannel_3->setEnabled(true);
            ui->NignPredIzmerChannel_3->setEnabled(true);
            ui->VerhnPredIzmerChannel_3->setEnabled(true);
            ui->PeriodIzmerChannel_3->setEnabled(true);
        }

        if (sigtype3 == ModBus::CurrentMeasure)
        {
            ui->ButonTokChannel_3->setChecked(true);
            ui->DiapasonChannel_3->clear();
            ui->DiapasonChannel_3->addItems(StringListTok);
        }

        if (sigtype3 == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_3->setChecked(true);
            ui->DiapasonChannel_3->addItems(StringListNapryagenie);
        }

        if (sigtype3 == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_3->setChecked(true);
            ui->DiapasonChannel_3->addItems(StringListRTD);
        }

        if (sigtype3 == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_3->setChecked(true);
            ui->DiapasonChannel_3->addItems(StringListTC);
        }

        if (sigtype3 == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_3->setChecked(true);
        }

        ui->DiapasonChannel_3->setCurrentIndex(options_channel3.GetDempherValue());
    }

    // channel 4
    {
        if (sigtype4 == ModBus::NoMeasure)
        {
            ui->ButonOtklChannel_4->setChecked(true);

            ui->UnitsChannel_4->setText("None");
            ui->UnitsChannel_4->setEnabled(false);
            ui->VerhnPredelChannel_4->setEnabled(false);
            ui->NignPredelChannel_4->setEnabled(false);
            ui->NignPredIzmerChannel_4->setEnabled(false);
            ui->VerhnPredIzmerChannel_4->setEnabled(false);
            ui->PeriodIzmerChannel_4->setEnabled(false);
        }
        else
        {
            ui->UnitsChannel_4->setEnabled(true);
            ui->VerhnPredelChannel_4->setEnabled(true);
            ui->NignPredelChannel_4->setEnabled(true);
            ui->NignPredIzmerChannel_4->setEnabled(true);
            ui->VerhnPredIzmerChannel_4->setEnabled(true);
            ui->PeriodIzmerChannel_4->setEnabled(true);
        }

        if (sigtype4 == ModBus::CurrentMeasure)
        {
            ui->ButonTokChannel_4->setChecked(true);
            ui->DiapasonChannel_4->clear();
            ui->DiapasonChannel_4->addItems(StringListTok);
        }

        if (sigtype4 == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_4->setChecked(true);
            ui->DiapasonChannel_4->addItems(StringListNapryagenie);
        }

        if (sigtype4 == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_4->setChecked(true);
            ui->DiapasonChannel_4->addItems(StringListRTD);
        }

        if (sigtype4 == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_4->setChecked(true);
            ui->DiapasonChannel_4->addItems(StringListTC);
        }

        if (sigtype4 == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_4->setChecked(true);
        }

        ui->DiapasonChannel_4->setCurrentIndex(options_channel4.GetDempherValue());
    }

    ui->UnitsChannel_1->setText(options_channel1.GetUnitsName());
    ui->VerhnPredelChannel_1->setValue(options_channel1.GetHigherLimit());
    ui->NignPredelChannel_1->setValue(options_channel1.GetLowerLimit());
    ui->VerhnPredIzmerChannel_1->setValue(options_channel1.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_1->setValue(options_channel1.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_1->setValue(options_channel1.GetMeasurePeriod());
    ui->State1ValueChannel_1->setValue(options_channel1.GetState1Value());
    ui->State2ValueChannel_1->setValue(options_channel1.GetState2Value());
    ui->State1HighMessageChannel_1->setText(options_channel1.GetState1HighMessage());
    ui->State1LowMessageChannel_1->setText(options_channel1.GetState1LowMessage());
    ui->State2HighMessageChannel_1->setText(options_channel1.GetState2HighMessage());
    ui->State2LowMessageChannel_1->setText(options_channel1.GetState2LowMessage());
    ui->Name_Channel_1->setText(options_channel1.GetChannelName());
    ui->math_text_channel_1->setText(options_channel1.GetMathString());
    ui->math_checkbox_channel_1->setChecked(options_channel1.IsChannelMathematical());
    ui->DemphirChannel_1->setValue(options_channel1.GetDempherValue());
    ui->DiapasonChannel_1->setCurrentIndex(options_channel1.GetDiapason());

    ui->UnitsChannel_2->setText(options_channel2.GetUnitsName());
    ui->VerhnPredelChannel_2->setValue(options_channel2.GetHigherLimit());
    ui->NignPredelChannel_2->setValue(options_channel2.GetLowerLimit());
    ui->VerhnPredIzmerChannel_2->setValue(options_channel2.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_2->setValue(options_channel2.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_2->setValue(options_channel2.GetMeasurePeriod());
    ui->State1ValueChannel_2->setValue(options_channel2.GetState1Value());
    ui->State2ValueChannel_2->setValue(options_channel2.GetState2Value());
    ui->State1HighMessageChannel_2->setText(options_channel2.GetState1HighMessage());
    ui->State1LowMessageChannel_2->setText(options_channel2.GetState1LowMessage());
    ui->State2HighMessageChannel_2->setText(options_channel2.GetState2HighMessage());
    ui->State2LowMessageChannel_2->setText(options_channel2.GetState2LowMessage());
    ui->Name_Channel_2->setText(options_channel2.GetChannelName());
    ui->math_text_channel_2->setText(options_channel2.GetMathString());
    ui->math_checkbox_channel_2->setChecked(options_channel2.IsChannelMathematical());
    ui->DemphirChannel_2->setValue(options_channel2.GetDempherValue());
    ui->DiapasonChannel_2->setCurrentIndex(options_channel2.GetDiapason());

    ui->UnitsChannel_3->setText(options_channel3.GetUnitsName());
    ui->VerhnPredelChannel_3->setValue(options_channel3.GetHigherLimit());
    ui->NignPredelChannel_3->setValue(options_channel3.GetLowerLimit());
    ui->VerhnPredIzmerChannel_3->setValue(options_channel3.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_3->setValue(options_channel3.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_3->setValue(options_channel3.GetMeasurePeriod());
    ui->State1ValueChannel_3->setValue(options_channel3.GetState1Value());
    ui->State2ValueChannel_3->setValue(options_channel3.GetState2Value());
    ui->State1HighMessageChannel_3->setText(options_channel3.GetState1HighMessage());
    ui->State1LowMessageChannel_3->setText(options_channel3.GetState1LowMessage());
    ui->State2HighMessageChannel_3->setText(options_channel3.GetState2HighMessage());
    ui->State2LowMessageChannel_3->setText(options_channel3.GetState2LowMessage());
    ui->Name_Channel_3->setText(options_channel3.GetChannelName());
    ui->math_text_channel_3->setText(options_channel3.GetMathString());
    ui->math_checkbox_channel_3->setChecked(options_channel3.IsChannelMathematical());
    ui->DemphirChannel_3->setValue(options_channel3.GetDempherValue());
    ui->DiapasonChannel_3->setCurrentIndex(options_channel3.GetDiapason());

    ui->UnitsChannel_4->setText(options_channel4.GetUnitsName());
    ui->VerhnPredelChannel_4->setValue(options_channel4.GetHigherLimit());
    ui->NignPredelChannel_4->setValue(options_channel4.GetLowerLimit());
    ui->VerhnPredIzmerChannel_4->setValue(options_channel4.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_4->setValue(options_channel4.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_4->setValue(options_channel4.GetMeasurePeriod());
    ui->State1ValueChannel_4->setValue(options_channel4.GetState1Value());
    ui->State2ValueChannel_4->setValue(options_channel4.GetState2Value());
    ui->State1HighMessageChannel_4->setText(options_channel4.GetState1HighMessage());
    ui->State1LowMessageChannel_4->setText(options_channel4.GetState1LowMessage());
    ui->State2HighMessageChannel_4->setText(options_channel4.GetState2HighMessage());
    ui->State2LowMessageChannel_4->setText(options_channel4.GetState2LowMessage());
    ui->Name_Channel_4->setText(options_channel4.GetChannelName());
    ui->math_text_channel_4->setText(options_channel4.GetMathString());
    ui->math_checkbox_channel_4->setChecked(options_channel4.IsChannelMathematical());
    ui->DemphirChannel_4->setValue(options_channel4.GetDempherValue());
    ui->DiapasonChannel_4->setCurrentIndex(options_channel4.GetDiapason());

    // реальное разрешение монитора

    //    ui->resolutionlabel->setText(Options::MonitorResolution);
    //    if (Options::displayResolution == "1024x768")
    //        ui->comboBox->setCurrentIndex(0);
    //    if (Options::displayResolution == "1280x800")
    //        ui->comboBox->setCurrentIndex(1);
    //    if (Options::displayResolution == "1920x1080")
    //        ui->comboBox->setCurrentIndex(2);
}

void StackedOptions::on_pushButton_51_clicked()
{
    ApplyNewSettingstoAllChannels();
    WriteSystemOptionsToFile();
    WriteAllChannelsOptionsToFile();
    this->close();
}

void StackedOptions::WriteAllChannelsOptionsToFile()
{
    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&options_channel1);
    ChannelsObjectsList.append(&options_channel2);
    ChannelsObjectsList.append(&options_channel3);
    ChannelsObjectsList.append(&options_channel4);

    QJsonObject channeljsonobj,channels;
    QJsonArray settings;

    int m = 0 ;
    foreach (ChannelOptions * Channel, ChannelsObjectsList) {

        channeljsonobj["Type"] = Channel->GetSignalType();
//        channeljsonobj["Type"] = 5-m;
        channeljsonobj["Name"] = Channel->GetChannelName();
        channeljsonobj["Units"] = Channel->GetUnitsName();
        channeljsonobj["HigherLimit"] = Channel->GetHigherLimit();
        channeljsonobj["LowerLimit"] = Channel->GetLowerLimit();
        channeljsonobj["HigherMeasLimit"] = Channel->GetHigherMeasureLimit();
        channeljsonobj["LowerMeasLimit"] = Channel->GetLowerMeasureLimit();
        channeljsonobj["Period"] = Channel->GetMeasurePeriod();
        channeljsonobj["State1HighMessage"] = Channel->GetState1HighMessage();
        channeljsonobj["State1LowMessage"] = Channel->GetState1LowMessage();
        channeljsonobj["State2HighMessage"] = Channel->GetState2HighMessage();
        channeljsonobj["State2LowMessage"] = Channel->GetState2LowMessage();
        channeljsonobj["State1Value"] = Channel->GetState1Value();
        channeljsonobj["State2Value"] = Channel->GetState2Value();
        channeljsonobj["MathString"] = Channel->GetMathString();
        channeljsonobj["MathWork"] = Channel->IsChannelMathematical();
        channeljsonobj["Diapason"] = Channel->GetDiapason();
        channeljsonobj["Dempher"] = Channel->GetDempherValue();

//        qDebug() <<  m << ":" << Channel->GetDempherValue() <<  "  Channel->GetDempherValue() ";

        settings.append(channeljsonobj);

        ++m;
    }

    channels["count"] = ChannelsObjectsList.length();
    channels["channels"] = settings;

    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    QFile file(pathtooptions);
    QTextStream out(&file);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    out << setstr;
    file.close();
}

void StackedOptions::WriteSystemOptionsToFile()
{
    UpdateCurrentDisplayParametr();

    QJsonObject systemoptions;
    systemoptions["Time"] = GetNewTimeString();
    systemoptions["Date"] = GetNewDateString();
    systemoptions["Display"] = GetCurrentDisplayParametr();
    systemoptions["Calibration"] = GetCalibration();
    systemoptions["Resolution"] = GetNewDisplayResolution();
    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);
    QFile file(pathtosystemoptions);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}

QString StackedOptions::GetNewDateString() {return ui->timeEdit->time().toString();}
QString StackedOptions::GetNewTimeString() {return ui->timeEdit->time().toString();}
QString StackedOptions::GetNewDisplayResolution() {return ui->comboBox->itemText(ui->comboBox->currentIndex());}

void StackedOptions::ApplyNewSettingstoAllChannels()
{
    options_channel1.SetUnitsName(ui->UnitsChannel_1->text());
    options_channel1.SetHigherLimit(ui->VerhnPredelChannel_1->value());
    options_channel1.SetLowerLimit(ui->NignPredelChannel_1->value());
    options_channel1.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_1->value());
    options_channel1.SetLowerMeasureLimit(ui->NignPredIzmerChannel_1->value());
    options_channel1.SetMeasurePeriod(ui->PeriodIzmerChannel_1->value());
    options_channel1.SetState1HighMessage(ui->State1HighMessageChannel_1->text());
    options_channel1.SetState1LowMessage(ui->State1LowMessageChannel_1->text());
    options_channel1.SetState2HighMessage(ui->State2HighMessageChannel_1->text());
    options_channel1.SetState2LowMessage(ui->State2LowMessageChannel_1->text());
    options_channel1.SetState1Value(ui->State1ValueChannel_1->value());
    options_channel1.SetState2Value(ui->State2ValueChannel_1->value());
    options_channel1.SetChannelName(ui->Name_Channel_1->text());
    options_channel1.SetMathEquation(ui->math_text_channel_1->text());
    options_channel1.SetMathematical(ui->math_checkbox_channel_1->isChecked());
    options_channel1.SetDempher(ui->DemphirChannel_1->value());
    options_channel1.SetDiapason(ui->DiapasonChannel_1->currentIndex());



    options_channel2.SetUnitsName(ui->UnitsChannel_2->text());
    options_channel2.SetHigherLimit(ui->VerhnPredelChannel_2->value());
    options_channel2.SetLowerLimit(ui->NignPredelChannel_2->value());
    options_channel2.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_2->value());
    options_channel2.SetLowerMeasureLimit(ui->NignPredIzmerChannel_2->value());
    options_channel2.SetMeasurePeriod(ui->PeriodIzmerChannel_2->value());
    options_channel2.SetState1HighMessage(ui->State1HighMessageChannel_2->text());
    options_channel2.SetState1LowMessage(ui->State1LowMessageChannel_2->text());
    options_channel2.SetState2HighMessage(ui->State2HighMessageChannel_2->text());
    options_channel2.SetState2LowMessage(ui->State2LowMessageChannel_2->text());
    options_channel2.SetState1Value(ui->State1ValueChannel_2->value());
    options_channel2.SetState2Value(ui->State2ValueChannel_2->value());
    options_channel2.SetChannelName(ui->Name_Channel_2->text());
    options_channel2.SetMathEquation(ui->math_text_channel_2->text());
    options_channel2.SetMathematical(ui->math_checkbox_channel_2->isChecked());
    options_channel2.SetDempher(ui->DemphirChannel_2->value());
    options_channel2.SetDiapason(ui->DiapasonChannel_2->currentIndex());

    options_channel3.SetUnitsName(ui->UnitsChannel_3->text());
    options_channel3.SetHigherLimit(ui->VerhnPredelChannel_3->value());
    options_channel3.SetLowerLimit(ui->NignPredelChannel_3->value());
    options_channel3.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_3->value());
    options_channel3.SetLowerMeasureLimit(ui->NignPredIzmerChannel_3->value());
    options_channel3.SetMeasurePeriod(ui->PeriodIzmerChannel_3->value());
    options_channel3.SetState1HighMessage(ui->State1HighMessageChannel_3->text());
    options_channel3.SetState1LowMessage(ui->State1LowMessageChannel_3->text());
    options_channel3.SetState2HighMessage(ui->State2HighMessageChannel_3->text());
    options_channel3.SetState2LowMessage(ui->State2LowMessageChannel_3->text());
    options_channel3.SetState1Value(ui->State1ValueChannel_3->value());
    options_channel3.SetState2Value(ui->State2ValueChannel_3->value());
    options_channel3.SetChannelName(ui->Name_Channel_3->text());
    options_channel3.SetMathEquation(ui->math_text_channel_3->text());
    options_channel3.SetMathematical(ui->math_checkbox_channel_3->isChecked());
    options_channel3.SetDempher(ui->DemphirChannel_3->value());
    options_channel3.SetDiapason(ui->DiapasonChannel_3->currentIndex());

    options_channel4.SetUnitsName(ui->UnitsChannel_4->text());
    options_channel4.SetHigherLimit(ui->VerhnPredelChannel_4->value());
    options_channel4.SetLowerLimit(ui->NignPredelChannel_4->value());
    options_channel4.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_4->value());
    options_channel4.SetLowerMeasureLimit(ui->NignPredIzmerChannel_4->value());
    options_channel4.SetMeasurePeriod(ui->PeriodIzmerChannel_4->value());
    options_channel4.SetState1HighMessage(ui->State1HighMessageChannel_4->text());
    options_channel4.SetState1LowMessage(ui->State1LowMessageChannel_4->text());
    options_channel4.SetState2HighMessage(ui->State2HighMessageChannel_4->text());
    options_channel4.SetState2LowMessage(ui->State2LowMessageChannel_4->text());
    options_channel4.SetState1Value(ui->State1ValueChannel_4->value());
    options_channel4.SetState2Value(ui->State2ValueChannel_4->value());
    options_channel4.SetChannelName(ui->Name_Channel_4->text());
    options_channel4.SetMathEquation(ui->math_text_channel_4->text());
    options_channel4.SetMathematical(ui->math_checkbox_channel_4->isChecked());
    options_channel4.SetDempher(ui->DemphirChannel_4->value());
    options_channel4.SetDiapason(ui->DiapasonChannel_4->currentIndex());


    qDebug() << options_channel1.GetDempherValue() <<  " DempherValue()  1";
    qDebug() << options_channel2.GetDempherValue() <<  " DempherValue()  2";
    qDebug() << options_channel3.GetDempherValue() <<  " DempherValue()  3";
    qDebug() << options_channel4.GetDempherValue() <<  " DempherValue()  4";

    //    SetLogMessagesLimit(ui->spinBox->value());
}

void StackedOptions::UpdateCurrentDisplayParametr()
{
    if (ui->SetTrendsBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::Trends);
    }
    if (ui->SetPolarBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::Polar);
    }
    if (ui->SetBarsBtn->isChecked())
    {

        SetCurrentDisplayParametr(StackedOptions::Bars);
    }
    if (ui->SetTrendsBarsBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::TrendsBars);
    }

    if (ui->SetCyfraBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::Cyfra);
    }

    if (ui->SetTrendsBarsCyfraBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::TrendsCyfraBars);
    }

    if (ui->SetTrendsCyfraBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::TrendsCyfra);
    }

    if (ui->SetBarsCyfraBtn->isChecked())
    {
        SetCurrentDisplayParametr(StackedOptions::BarsCyfra);
    }
}

void StackedOptions::on_pushButton_52_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

bool StackedOptions::eventFilter(QObject *object, QEvent *event)
{
    if ( (event->type() == event->MouseButtonRelease)&&(object->property("enabled").toString() == "true") )
    {
        Options::olderprop = object->property("text").toString();
        keyboard kb;
        kb.setModal(true);
        kb.exec();
        object->setProperty("value", kb.getcustomstring() );
        object->setProperty("text",kb.getcustomstring() );
        ui->pushButton->setFocus();
        kb.close();
        kb.deleteLater();
    }
    return QObject::eventFilter(object, event);
}

void StackedOptions::Channel1TypeChange(int i)
{

    qDebug() << i << "index";
    QStringList qlist;
    ui->UnitsChannel_1->setEnabled(true);
    ui->VerhnPredelChannel_1->setEnabled(true);
    ui->NignPredelChannel_1->setEnabled(true);
    ui->NignPredIzmerChannel_1->setEnabled(true);
    ui->VerhnPredIzmerChannel_1->setEnabled(true);
    ui->PeriodIzmerChannel_1->setEnabled(true);

    switch (i) {
    case -2:
    {
        options_channel1.SetSignalType(ModBus::NoMeasure);

        ui->UnitsChannel_1->setText("Нет");
        ui->UnitsChannel_1->setEnabled(false);
        ui->VerhnPredelChannel_1->setEnabled(false);
        ui->NignPredelChannel_1->setEnabled(false);
        ui->NignPredIzmerChannel_1->setEnabled(false);
        ui->VerhnPredIzmerChannel_1->setEnabled(false);
        ui->PeriodIzmerChannel_1->setEnabled(false);

        ui->DiapasonChannel_1->clear();
        ui->DiapasonChannel_1->addItems(StringListNone);

        break;
    }

    case -3: // ButonTokChannel_1
    {
        options_channel1.SetSignalType(ModBus::CurrentMeasure);
        ui->UnitsChannel_1->setText("mA");
        ui->DiapasonChannel_1->clear();
        ui->DiapasonChannel_1->addItems(StringListTok);
        break;
    }

    case -4: //(ui->ButonNapryagenieChannel_1->isChecked())
    {
        options_channel1.SetSignalType(ModBus::VoltageMeasure);
        ui->UnitsChannel_1->setText("V");
        ui->DiapasonChannel_1->clear();
        ui->DiapasonChannel_1->addItems(StringListNapryagenie);
        break;
    }

    case -5: //(ui->ButonResistorChannel_1->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoResistanceMeasure);
        ui->UnitsChannel_1->setText("°C (RTD)");
        ui->DiapasonChannel_1->clear();
        ui->DiapasonChannel_1->addItems(StringListRTD);
        break;
        break;
    }

    case -6: //(ui->ButonTermoparaChannel_1->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoCoupleMeasure);
        ui->UnitsChannel_1->setText("°C (TC)");
        ui->DiapasonChannel_1->clear();
        ui->DiapasonChannel_1->addItems(StringListTC);
        break;
    }

    case -7:
    {
        options_channel1.SetSignalType(ModBus::ImpulseCounterMeasure);
        ui->UnitsChannel_1->setText("imp.cnt");
        break;
    }
    default:

        break;
    }
}

void StackedOptions::Channel2TypeChange(int i)
{
    ui->UnitsChannel_2->setEnabled(true);
    ui->VerhnPredelChannel_2->setEnabled(true);
    ui->NignPredelChannel_2->setEnabled(true);
    ui->NignPredIzmerChannel_2->setEnabled(true);
    ui->VerhnPredIzmerChannel_2->setEnabled(true);
    ui->PeriodIzmerChannel_2->setEnabled(true);

    switch (i) {
    case -2:
    {
        options_channel1.SetSignalType(ModBus::NoMeasure);

        ui->UnitsChannel_2->setText("Нет");
        ui->UnitsChannel_2->setEnabled(false);
        ui->VerhnPredelChannel_2->setEnabled(false);
        ui->NignPredelChannel_2->setEnabled(false);
        ui->NignPredIzmerChannel_2->setEnabled(false);
        ui->VerhnPredIzmerChannel_2->setEnabled(false);
        ui->PeriodIzmerChannel_2->setEnabled(false);

        ui->DiapasonChannel_2->clear();
        ui->DiapasonChannel_2->addItems(StringListNone);

        break;
    }

    case -3: // ButonTokChannel_2
    {
        options_channel1.SetSignalType(ModBus::CurrentMeasure);
        ui->UnitsChannel_2->setText("mA");
        ui->DiapasonChannel_2->clear();
        ui->DiapasonChannel_2->addItems(StringListTok);
        break;
    }

    case -4: //(ui->ButonNapryagenieChannel_2->isChecked())
    {
        options_channel1.SetSignalType(ModBus::VoltageMeasure);
        ui->UnitsChannel_2->setText("V");
        ui->DiapasonChannel_2->clear();
        ui->DiapasonChannel_2->addItems(StringListNapryagenie);
        break;
    }

    case -5: //(ui->ButonResistorChannel_2->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoResistanceMeasure);
        ui->UnitsChannel_2->setText("°C (RTD)");
        ui->DiapasonChannel_2->clear();
        ui->DiapasonChannel_2->addItems(StringListRTD);
        break;
        break;
    }

    case -6: //(ui->ButonTermoparaChannel_2->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoCoupleMeasure);
        ui->UnitsChannel_2->setText("°C (TC)");
        ui->DiapasonChannel_2->clear();
        ui->DiapasonChannel_2->addItems(StringListTC);
        break;
    }

    case -7:
    {
        options_channel1.SetSignalType(ModBus::ImpulseCounterMeasure);
        ui->UnitsChannel_2->setText("imp.cnt");
        break;
    }
    default:

        break;
    }
}


void StackedOptions::Channel3TypeChange(int i)
{
    ui->UnitsChannel_3->setEnabled(true);
    ui->VerhnPredelChannel_3->setEnabled(true);
    ui->NignPredelChannel_3->setEnabled(true);
    ui->NignPredIzmerChannel_3->setEnabled(true);
    ui->VerhnPredIzmerChannel_3->setEnabled(true);
    ui->PeriodIzmerChannel_3->setEnabled(true);

    switch (i) {
    case -2:
    {
        options_channel1.SetSignalType(ModBus::NoMeasure);

        ui->UnitsChannel_3->setText("Нет");
        ui->UnitsChannel_3->setEnabled(false);
        ui->VerhnPredelChannel_3->setEnabled(false);
        ui->NignPredelChannel_3->setEnabled(false);
        ui->NignPredIzmerChannel_3->setEnabled(false);
        ui->VerhnPredIzmerChannel_3->setEnabled(false);
        ui->PeriodIzmerChannel_3->setEnabled(false);

        ui->DiapasonChannel_3->clear();
        ui->DiapasonChannel_3->addItems(StringListNone);

        break;
    }

    case -3: // ButonTokChannel_3
    {
        options_channel1.SetSignalType(ModBus::CurrentMeasure);
        ui->UnitsChannel_3->setText("mA");
        ui->DiapasonChannel_3->clear();
        ui->DiapasonChannel_3->addItems(StringListTok);
        break;
    }

    case -4: //(ui->ButonNapryagenieChannel_3->isChecked())
    {
        options_channel1.SetSignalType(ModBus::VoltageMeasure);
        ui->UnitsChannel_3->setText("V");
        ui->DiapasonChannel_3->clear();
        ui->DiapasonChannel_3->addItems(StringListNapryagenie);
        break;
    }

    case -5: //(ui->ButonResistorChannel_3->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoResistanceMeasure);
        ui->UnitsChannel_3->setText("°C (RTD)");
        ui->DiapasonChannel_3->clear();
        ui->DiapasonChannel_3->addItems(StringListRTD);
        break;
        break;
    }

    case -6: //(ui->ButonTermoparaChannel_3->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoCoupleMeasure);
        ui->UnitsChannel_3->setText("°C (TC)");
        ui->DiapasonChannel_3->clear();
        ui->DiapasonChannel_3->addItems(StringListTC);
        break;
    }

    case -7:
    {
        options_channel1.SetSignalType(ModBus::ImpulseCounterMeasure);
        ui->UnitsChannel_3->setText("imp.cnt");
        break;
    }
    default:

        break;
    }
}

void StackedOptions::Channel4TypeChange(int i)
{
    ui->UnitsChannel_4->setEnabled(true);
    ui->VerhnPredelChannel_4->setEnabled(true);
    ui->NignPredelChannel_4->setEnabled(true);
    ui->NignPredIzmerChannel_4->setEnabled(true);
    ui->VerhnPredIzmerChannel_4->setEnabled(true);
    ui->PeriodIzmerChannel_4->setEnabled(true);

    switch (i) {
    case -2:
    {
        options_channel1.SetSignalType(ModBus::NoMeasure);

        ui->UnitsChannel_4->setText("Нет");
        ui->UnitsChannel_4->setEnabled(false);
        ui->VerhnPredelChannel_4->setEnabled(false);
        ui->NignPredelChannel_4->setEnabled(false);
        ui->NignPredIzmerChannel_4->setEnabled(false);
        ui->VerhnPredIzmerChannel_4->setEnabled(false);
        ui->PeriodIzmerChannel_4->setEnabled(false);

        ui->DiapasonChannel_4->clear();
        ui->DiapasonChannel_4->addItems(StringListNone);

        break;
    }

    case -3: // ButonTokChannel_4
    {
        options_channel1.SetSignalType(ModBus::CurrentMeasure);
        ui->UnitsChannel_4->setText("mA");
        ui->DiapasonChannel_4->clear();
        ui->DiapasonChannel_4->addItems(StringListTok);
        break;
    }

    case -4: //(ui->ButonNapryagenieChannel_4->isChecked())
    {
        options_channel1.SetSignalType(ModBus::VoltageMeasure);
        ui->UnitsChannel_4->setText("V");
        ui->DiapasonChannel_4->clear();
        ui->DiapasonChannel_4->addItems(StringListNapryagenie);
        break;
    }

    case -5: //(ui->ButonResistorChannel_4->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoResistanceMeasure);
        ui->UnitsChannel_4->setText("°C (RTD)");
        ui->DiapasonChannel_4->clear();
        ui->DiapasonChannel_4->addItems(StringListRTD);
        break;
        break;
    }

    case -6: //(ui->ButonTermoparaChannel_4->isChecked())
    {
        options_channel1.SetSignalType(ModBus::TermoCoupleMeasure);
        ui->UnitsChannel_4->setText("°C (TC)");
        ui->DiapasonChannel_4->clear();
        ui->DiapasonChannel_4->addItems(StringListTC);
        break;
    }

    case -7:
    {
        options_channel1.SetSignalType(ModBus::ImpulseCounterMeasure);
        ui->UnitsChannel_4->setText("imp.cnt");
        break;
    }

    default:
        break;

    }
}

void StackedOptions::on_pushButton_14_clicked()
{
#ifndef WIN32
    QProcess process;
    QDateTime newuidate = ui->dateEdit->dateTime();
    QTime newuitime = ui->timeEdit->time();

    QString newdate = QString::number(newuidate.date().year()) + "-" + QString::number(newuidate.date().month()) + "-" + QString::number(newuidate.date().day()) ;
    QString newtime = newuitime.toString();

    process.startDetached("date --set " + newdate);
    process.startDetached("date --set " + newtime); // max freq on
#endif
}
