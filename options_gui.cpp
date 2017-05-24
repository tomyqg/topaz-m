#include "options.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"
#include "messages.h"

void Options::ApplyNewSettingstoOptionsUI()
{
    int sigtype = options_channel1.GetSignalType();
    int sigtype2 = options_channel2.GetSignalType();
    int sigtype3 = options_channel3.GetSignalType();
    int sigtype4 = options_channel4.GetSignalType();

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
    // channel 1
    {
        if (sigtype == 1)
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

        if (sigtype == 2)
        {
            ui->ButonTokChannel_1->setChecked(true);
        }

        if (sigtype == 3)
        {
            ui->ButonNapryagenieChannel_1->setChecked(true);
        }

        if (sigtype == 4)
        {
            ui->ButonResistorChannel_1->setChecked(true);
        }

        if (sigtype == 5)
        {
            ui->ButonTermoparaChannel_1->setChecked(true);
        }

        if (sigtype == 6)
        {
            ui->ButonImpulseChannel_1->setChecked(true);
        }
    }

    // channel 2
    {
        if (sigtype2 == 1)
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

        if (sigtype2 == 2)
        {
            ui->ButonTokChannel_2->setChecked(true);
        }

        if (sigtype2 == 3)
        {
            ui->ButonNapryagenieChannel_2->setChecked(true);
        }

        if (sigtype2 == 4)
        {
            ui->ButonResistorChannel_2->setChecked(true);
        }

        if (sigtype2 == 5)
        {
            ui->ButonTermoparaChannel_2->setChecked(true);
        }

        if (sigtype2 == 6)
        {
            ui->ButonImpulseChannel_2->setChecked(true);
        }
    }

    // channel 3
    {
        if (sigtype3 == 1)
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

        if (sigtype3 == 2)
        {
            ui->ButonTokChannel_3->setChecked(true);
        }

        if (sigtype3 == 3)
        {
            ui->ButonNapryagenieChannel_3->setChecked(true);
        }

        if (sigtype3 == 4)
        {
            ui->ButonResistorChannel_3->setChecked(true);
        }

        if (sigtype3 == 5)
        {
            ui->ButonTermoparaChannel_3->setChecked(true);
        }

        if (sigtype3 == 6)
        {
            ui->ButonImpulseChannel_3->setChecked(true);
        }
    }

    // channel 4
    {
        if (sigtype4 == 1)
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

        if (sigtype4 == 2)
        {
            ui->ButonTokChannel_4->setChecked(true);
        }

        if (sigtype4 == 3)
        {
            ui->ButonNapryagenieChannel_4->setChecked(true);
        }

        if (sigtype4 == 4)
        {
            ui->ButonResistorChannel_4->setChecked(true);
        }

        if (sigtype4 == 5)
        {
            ui->ButonTermoparaChannel_4->setChecked(true);
        }

        if (sigtype4 == 6)
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
    ui->math_text_ch_1->setText(options_channel1.GetMathString());
    ui->checkBox->setChecked(options_channel1.IsChannelMathematical());

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
    ui->math_text_ch_2->setText(options_channel2.GetMathString());
    ui->checkBox_2->setChecked(options_channel2.IsChannelMathematical());

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
    ui->math_text_ch_3->setText(options_channel3.GetMathString());
    ui->checkBox_3->setChecked(options_channel3.IsChannelMathematical());

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
    ui->math_text_ch_4->setText(options_channel4.GetMathString());
    ui->checkBox_4->setChecked(options_channel4.IsChannelMathematical());
}

void Options::on_UnitsChannel_1_editingFinished()
{
    options_channel1.SetUnitsName(ui->UnitsChannel_1->text());
}

void Options::on_horizontalScrollBar_sliderReleased() // разблокирует математический канал при проведении слайдера вправо
{
    if (ui->horizontalScrollBar->value() == ui->horizontalScrollBar->maximum())
    {
        ui->math_text_ch_1->setEnabled(1);
        ui->math_text_ch_2->setEnabled(1);
        ui->math_text_ch_3->setEnabled(1);
        ui->math_text_ch_4->setEnabled(1);
        ui->checkBox->setEnabled(1);
        ui->checkBox_3->setEnabled(1);
        ui->checkBox_4->setEnabled(1);
        ui->checkBox_2->setEnabled(1);
    }
    ui->horizontalScrollBar->setValue(0);
}


void Options::CustomizeOptionsUI() // кастомизирует контроллы во вкладке опшнс
{
    //set tab height

    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 40px }");

    //    ui->NignPredelChannel_1->setStyleSheet( "QSpinBox::down-button { width: 10px;}""QSpinBox::up-button { width: 10px; }""QSpinBox { border: 2px solid red; border-radius: 5px; background-color: #e6ffff; height: 70px}");//height: 20px;

    ui->NignPredelChannel_1->setStyleSheet( "QDoubleSpinBox { border: 2px solid red; border-radius: 5px; background-color: #e6ffff; height: 40px}""QSpinBox::down-button { width: 40px; height: 20px}""QDoubleSpinBox::down-button { width: 40px;height: 20px}" "QSpinBox::up-button { width: 40px; height: 20px}""QDoubleSpinBox::up-button { width: 40px;height: 20px}");//height: 20px;

    //    ui->pushButton_2->setIcon(QIcon("/usr/EXT.jpg"));
    //    ui->pushButton_2->setIconSize(QSize(211,45));

    QString commonstylesheet( ui->NignPredelChannel_1->styleSheet());
    ui->VerhnPredelChannel_1->setStyleSheet(commonstylesheet);

    // find qspin widgetss to apply new settings
    QList<QSpinBox*> spinList = QObject::findChildren<QSpinBox*> (  );

    // apply style to all widgets
    for (int i = 0; i < spinList.count(); ++i) {
        QSpinBox *sb = spinList.at(i);
        sb->setStyleSheet(commonstylesheet);
    }

    QList<QDoubleSpinBox*> dspinList = QObject::findChildren<QDoubleSpinBox*> (  );

    // apply style to all widgets
    for (int i = 0; i < dspinList.count(); ++i) {
        QDoubleSpinBox *dsb = dspinList.at(i);
        dsb->setStyleSheet(commonstylesheet);
        //qDebug() << dsb;
    }

    //this is a template
    /*("QSpinBox { border: 2px solid red; border-radius: 5px; background-color: yellow; }"
        "QSpinBox::up-arrow { border-left: 17px solid none;"
        "border-right: 17px solid none; border-bottom: 17px solid black; width: 0px; height: 0px; }"
        "QSpinBox::up-arrow:hover { border-left: 17px solid none;"
        "border-right: 17px solid none; border-bottom: 17px solid black; width: 0px; height: 0px; }"
        "QSpinBox::up-button { width: 50px; height: 37px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.25 "+getRGBhexColor(palspin.light().color())+", stop: 1 "+getRGBhexColor(palspin.midlight().color())+") }"
        "QSpinBox::up-button:hover { width: 50px; height: 37px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.25 "+getRGBhexColor(palspin.light().color())+", stop: 1 "+getRGBhexColor(palspin.shadow().color())+") }"

        "QSpinBox::down-arrow { border-left: 17px solid none;"
        "border-right: 17px solid none; border-top: 17px solid black; width: 0px; height: 0px; }"
        "QSpinBox::down-arrow:hover { border-left: 17px solid none;"
        "border-right: 17px solid none; border-top: 17px solid black; width: 0px; height: 0px; }"
        "QSpinBox::down-button { width: 50px; height: 37px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.25 "+getRGBhexColor(palspin.light().color())+", stop: 1 "+getRGBhexColor(palspin.midlight().color())+") }"
        "QSpinBox::down-button:hover { width: 50px; height: 37px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0.25 "+getRGBhexColor(palspin.light().color())+", stop: 1 "+getRGBhexColor(palspin.shadow().color())+") }"
        );*/
}


void Options::UpdateCurrentDisplayParametr()
{
    if (ui->SetTrendsBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::Trends);
        }
        if (ui->SetPolarBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::Polar);
        }
        if (ui->SetBarsBtn->isChecked())
        {

            SetCurrentDisplayParametr(Options::Bars);
        }
        if (ui->SetTrendsBarsBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::TrendsBars);
        }

        if (ui->SetCyfraBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::Cyfra);
        }

        if (ui->SetTrendsBarsCyfraBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::TrendsCyfraBars);
        }

        if (ui->SetTrendsCyfraBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::TrendsCyfra);
        }

        if (ui->SetBarsCyfraBtn->isChecked())
        {
            SetCurrentDisplayParametr(Options::BarsCyfra);
        }
}

QString Options::GetNewDateString() {return ui->timeEdit->time().toString();}
QString Options::GetNewTimeString() {return ui->timeEdit->time().toString();}
