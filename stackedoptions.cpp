#include "stackedoptions.h"
#include "ui_stackedoptions.h"
#include "defines.h"


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
}

StackedOptions::~StackedOptions()
{
    delete ui;
}

void StackedOptions::on_pushButton_9_clicked()
{

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
    Options::calibrationprm = json["Calibration"].toString();
    Options::DisplayParametr = json["Display"].toInt();
    Options::displayResolution = json["Resolution"].toString();

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
/*
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
*/
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

}
