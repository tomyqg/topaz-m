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
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(SetLabelIndex(int)) );
    SetStackIndex(MainMenuIndex);
    ReadSystemOptionsFromFile();
    ReadChannelsOptionsFromFile();
    ApplyNewSettingstoOptionsUI();

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

    StringListNone.clear();
    StringListNone.append("Нет");

    StringListTok.clear();
    StringListTok.append("4-20 мA");
    StringListTok.append("0-20 мA");
    StringListTok.append("0-5  мA");
    StringListTok.append("0-20 мA с корнеизвлеч.");
    StringListTok.append("0-20 мA с корнеизвлеч.");
    StringListTok.append("± 20 мA");

    StringListNapryagenie.clear();
    StringListNapryagenie.append("0-1  В");
    StringListNapryagenie.append("0-10 В");
    StringListNapryagenie.append("0-5  В");
    StringListNapryagenie.append("1-5  В");
    StringListNapryagenie.append("±150 мВ");
    StringListNapryagenie.append("±1   В");;
    StringListNapryagenie.append("±10  В");
    StringListNapryagenie.append("±30  В");
    StringListNapryagenie.append("0-1  В с корнеизвлеч.");
    StringListNapryagenie.append("0-10 В с корнеизвлеч.");
    StringListNapryagenie.append("1-5  В с корнеизвлеч.");

    StringListRTD.clear();
    StringListRTD.append("Pt100  (IEC)");
    StringListRTD.append("Pt100  (JIS)");
    StringListRTD.append("Pt100  (GOST)");
    StringListRTD.append("Pt500  (IEC)");
    StringListRTD.append("Pt500  (JIS)");
    StringListRTD.append("Pt1000 (IEC)");
    StringListRTD.append("Pt1000 (JIS)");
    StringListRTD.append("Pt45   (GOST)");
    StringListRTD.append("Pt50   (GOST)");
    StringListRTD.append("Cu50   (GOST, α=4260)");
    StringListRTD.append("Cu50   (GOST, α=4280)");
    StringListRTD.append("Cu53   (GOST, α=4280)");
    StringListRTD.append("Cu100  (GOST, α=4280)");

    StringListTC.clear();
    StringListTC.append("Тип А (W5Re-W20Re)");
    StringListTC.append("Тип B (Pt30Rh-Pt60Rh)");
    StringListTC.append("Тип C (W50Re-W26Re)");
    StringListTC.append("Тип D (W30Re-W25Re)");
    StringListTC.append("Тип J (Fe-CuNi)");
    StringListTC.append("Тип K (NiCr-Ni)");
    StringListTC.append("Тип L (Fe-CuNi)");
    StringListTC.append("Тип L (NiCr-CuNi, GOST)");
    StringListTC.append("Тип N (NiCrSi-NiSi)");
    StringListTC.append("Тип R (Pt13Rh-Pt)");
    StringListTC.append("Тип S (Pt10Rh-Pt)");
    StringListTC.append("Тип T (Cu-CuNi)");
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

    QJsonObject ch1 = array.at(0).toObject();
    options_channel1.SetHigherLimit(ch1.value("HigherLimit").toDouble());
    options_channel1.SetLowerLimit(ch1.value("LowerLimit").toDouble());
    options_channel1.SetHigherMeasureLimit(ch1.value("HigherMeasLimit").toDouble());
    options_channel1.SetLowerMeasureLimit(ch1.value("LowerMeasLimit").toDouble());
    options_channel1.SetSignalType(ch1.value("Type").toDouble());
    options_channel1.SetUnitsName(ch1.value("Units").toString());
    options_channel1.SetMeasurePeriod(ch1.value("Period").toDouble());
    options_channel1.SetState1HighMessage(ch1.value("State1HighMessage").toString());
    options_channel1.SetState1LowMessage(ch1.value("State1LowMessage").toString());
    options_channel1.SetState2HighMessage(ch1.value("State2HighMessage").toString());
    options_channel1.SetState2LowMessage(ch1.value("State2LowMessage").toString());
    options_channel1.SetState1Value(ch1.value("State1Value").toDouble());
    options_channel1.SetState2Value(ch1.value("State2Value").toDouble());
    options_channel1.SetChannelName(ch1.value("Name").toString());
    options_channel1.SetMathEquation(ch1.value("MathString").toString());
    options_channel1.SetMathematical(ch1.value("MathWork").toBool());

    QJsonObject ch2 = array.at(1).toObject();
    options_channel2.SetHigherLimit(ch2.value("HigherLimit").toDouble());
    options_channel2.SetLowerLimit(ch2.value("LowerLimit").toDouble());
    options_channel2.SetHigherMeasureLimit(ch2.value("HigherMeasLimit").toDouble());
    options_channel2.SetLowerMeasureLimit(ch2.value("LowerMeasLimit").toDouble());
    options_channel2.SetSignalType(ch2.value("Type").toDouble());
    options_channel2.SetUnitsName(ch2.value("Units").toString());
    options_channel2.SetMeasurePeriod(ch2.value("Period").toDouble());
    options_channel2.SetState1HighMessage(ch2.value("State1HighMessage").toString());
    options_channel2.SetState1LowMessage(ch2.value("State1LowMessage").toString());
    options_channel2.SetState2HighMessage(ch2.value("State2HighMessage").toString());
    options_channel2.SetState2LowMessage(ch2.value("State2LowMessage").toString());
    options_channel2.SetState1Value(ch2.value("State1Value").toDouble());
    options_channel2.SetState2Value(ch2.value("State2Value").toDouble());
    options_channel2.SetChannelName(ch2.value("Name").toString());
    options_channel2.SetMathEquation(ch2.value("MathString").toString());
    options_channel2.SetMathematical(ch2.value("MathWork").toBool());

    QJsonObject ch3 = array.at(2).toObject();
    options_channel3.SetHigherLimit(ch3.value("HigherLimit").toDouble());
    options_channel3.SetLowerLimit(ch3.value("LowerLimit").toDouble());
    options_channel3.SetHigherMeasureLimit(ch3.value("HigherMeasLimit").toDouble());
    options_channel3.SetLowerMeasureLimit(ch3.value("LowerMeasLimit").toDouble());
    options_channel3.SetSignalType(ch3.value("Type").toDouble());
    options_channel3.SetUnitsName(ch3.value("Units").toString());
    options_channel3.SetMeasurePeriod(ch3.value("Period").toDouble());
    options_channel3.SetState1HighMessage(ch3.value("State1HighMessage").toString());
    options_channel3.SetState1LowMessage(ch3.value("State1LowMessage").toString());
    options_channel3.SetState2HighMessage(ch3.value("State2HighMessage").toString());
    options_channel3.SetState2LowMessage(ch3.value("State2LowMessage").toString());
    options_channel3.SetState1Value(ch3.value("State1Value").toDouble());
    options_channel3.SetState2Value(ch3.value("State2Value").toDouble());
    options_channel3.SetChannelName(ch3.value("Name").toString());
    options_channel3.SetMathEquation(ch3.value("MathString").toString());
    options_channel3.SetMathematical(ch3.value("MathWork").toBool());

    QJsonObject ch4 = array.at(3).toObject();
    options_channel4.SetHigherLimit(ch4.value("HigherLimit").toDouble());
    options_channel4.SetLowerLimit(ch4.value("LowerLimit").toDouble());
    options_channel4.SetHigherMeasureLimit(ch4.value("HigherMeasLimit").toDouble());
    options_channel4.SetLowerMeasureLimit(ch4.value("LowerMeasLimit").toDouble());
    options_channel4.SetSignalType(ch4.value("Type").toDouble());
    options_channel4.SetUnitsName(ch4.value("Units").toString());
    options_channel4.SetMeasurePeriod(ch4.value("Period").toDouble());
    options_channel4.SetState1HighMessage(ch4.value("State1HighMessage").toString());
    options_channel4.SetState1LowMessage(ch4.value("State1LowMessage").toString());
    options_channel4.SetState2HighMessage(ch4.value("State2HighMessage").toString());
    options_channel4.SetState2LowMessage(ch4.value("State2LowMessage").toString());
    options_channel4.SetState1Value(ch4.value("State1Value").toDouble());
    options_channel4.SetState2Value(ch4.value("State2Value").toDouble());
    options_channel4.SetChannelName(ch4.value("Name").toString());
    options_channel4.SetMathEquation(ch4.value("MathString").toString());
    options_channel4.SetMathematical(ch4.value("MathWork").toBool());
}


void StackedOptions::ApplyNewSettingstoOptionsUI()
{
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
        }

        if (sigtype == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_1->setChecked(true);
        }

        if (sigtype == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_1->setChecked(true);
        }

        if (sigtype == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_1->setChecked(true);
        }

        if (sigtype == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_1->setChecked(true);
        }
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
        }

        if (sigtype2 == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_2->setChecked(true);
        }

        if (sigtype2 == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_2->setChecked(true);
        }

        if (sigtype2 == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_2->setChecked(true);
        }

        if (sigtype2 == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_2->setChecked(true);
        }
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
        }

        if (sigtype3 == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_3->setChecked(true);
        }

        if (sigtype3 == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_3->setChecked(true);
        }

        if (sigtype3 == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_3->setChecked(true);
        }

        if (sigtype3 == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_3->setChecked(true);
        }
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
        }

        if (sigtype4 == ModBus::VoltageMeasure)
        {
            ui->ButonNapryagenieChannel_4->setChecked(true);
        }

        if (sigtype4 == ModBus::TermoResistanceMeasure)
        {
            ui->ButonResistorChannel_4->setChecked(true);
        }

        if (sigtype4 == ModBus::TermoCoupleMeasure)
        {
            ui->ButonTermoparaChannel_4->setChecked(true);
        }

        if (sigtype4 == ModBus::ImpulseCounterMeasure)
        {
            ui->ButonImpulseChannel_4->setChecked(true);
        }
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
    QJsonObject channel1,channel2,channel3,channel4,channels;
    QJsonArray settings;

    channel1["Type"] = options_channel1.GetSignalType();
    channel1["Name"] = options_channel1.GetChannelName();
    channel1["Units"] = options_channel1.GetUnitsName();
    channel1["HigherLimit"] = options_channel1.GetHigherLimit();
    channel1["LowerLimit"] = options_channel1.GetLowerLimit();
    channel1["HigherMeasLimit"] = options_channel1.GetHigherMeasureLimit();
    channel1["LowerMeasLimit"] = options_channel1.GetLowerMeasureLimit();
    channel1["Period"] = options_channel1.GetMeasurePeriod();
    channel1["State1HighMessage"] = options_channel1.GetState1HighMessage();
    channel1["State1LowMessage"] = options_channel1.GetState1LowMessage();
    channel1["State2HighMessage"] = options_channel1.GetState2HighMessage();
    channel1["State2LowMessage"] = options_channel1.GetState2LowMessage();
    channel1["State1Value"] = options_channel1.GetState1Value();
    channel1["State2Value"] = options_channel1.GetState2Value();
    channel1["MathString"] = options_channel1.GetMathString();
    channel1["MathWork"] = options_channel1.IsChannelMathematical();

    settings.append(channel1);

    channel2["Type"] = options_channel2.GetSignalType();
    channel2["Name"] = options_channel2.GetChannelName();
    channel2["Units"] = options_channel2.GetUnitsName();
    channel2["HigherLimit"] = options_channel2.GetHigherLimit();
    channel2["LowerLimit"] = options_channel2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options_channel2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options_channel2.GetLowerMeasureLimit();
    channel2["Period"] = options_channel2.GetMeasurePeriod();
    channel2["State1HighMessage"] = options_channel2.GetState1HighMessage();
    channel2["State1LowMessage"] = options_channel2.GetState1LowMessage();
    channel2["State2HighMessage"] = options_channel2.GetState2HighMessage();
    channel2["State2LowMessage"] = options_channel2.GetState2LowMessage();
    channel2["State1Value"] = options_channel2.GetState1Value();
    channel2["State2Value"] = options_channel2.GetState2Value();
    channel2["MathString"] = options_channel2.GetMathString();
    channel2["MathWork"] = options_channel2.IsChannelMathematical();

    settings.append(channel2);

    channel3["Type"] = options_channel3.GetSignalType();
    channel3["Name"] = options_channel3.GetChannelName();
    channel3["Units"] = options_channel3.GetUnitsName();
    channel3["HigherLimit"] = options_channel3.GetHigherLimit();
    channel3["LowerLimit"] = options_channel3.GetLowerLimit();
    channel3["HigherMeasLimit"] = options_channel3.GetHigherMeasureLimit();
    channel3["LowerMeasLimit"] = options_channel3.GetLowerMeasureLimit();
    channel3["Period"] = options_channel3.GetMeasurePeriod();
    channel3["State1HighMessage"] = options_channel3.GetState1HighMessage();
    channel3["State1LowMessage"] = options_channel3.GetState1LowMessage();
    channel3["State2HighMessage"] = options_channel3.GetState2HighMessage();
    channel3["State2LowMessage"] = options_channel3.GetState2LowMessage();
    channel3["State1Value"] = options_channel3.GetState1Value();
    channel3["State2Value"] = options_channel3.GetState2Value();
    channel3["MathString"] = options_channel3.GetMathString();
    channel3["MathWork"] = options_channel3.IsChannelMathematical();

    settings.append(channel3);

    channel4["Type"] = options_channel4.GetSignalType();
    channel4["Name"] = options_channel4.GetChannelName();
    channel4["Units"] = options_channel4.GetUnitsName();
    channel4["HigherLimit"] = options_channel4.GetHigherLimit();
    channel4["LowerLimit"] = options_channel4.GetLowerLimit();
    channel4["HigherMeasLimit"] = options_channel4.GetHigherMeasureLimit();
    channel4["LowerMeasLimit"] = options_channel4.GetLowerMeasureLimit();
    channel4["Period"] = options_channel4.GetMeasurePeriod();
    channel4["State1HighMessage"] = options_channel4.GetState1HighMessage();
    channel4["State1LowMessage"] = options_channel4.GetState1LowMessage();
    channel4["State2HighMessage"] = options_channel4.GetState2HighMessage();
    channel4["State2LowMessage"] = options_channel4.GetState2LowMessage();
    channel4["State1Value"] = options_channel4.GetState1Value();
    channel4["State2Value"] = options_channel4.GetState2Value();
    channel4["MathString"] = options_channel4.GetMathString();
    channel4["MathWork"] = options_channel4.IsChannelMathematical();

    settings.append(channel4);

    channels["count"] = 4;
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
