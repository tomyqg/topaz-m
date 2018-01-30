#include "options.h"
#include "ui_options.h"
#include "Channels/channelOptions.h"
#include "keyboard.h"
#include "messages.h"
#include "defines.h"

void Options::ApplyNewSettingstoOptionsUI()
{
    ui->HashLine->setText(CURRENT_VER);

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
//    ui->State1ValueChannel_1->setValue(options_channel1.ustavka1.getStateValue());
//    ui->State2ValueChannel_1->setValue(options_channel1.ustavka2.getStateValue());
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
//    ui->State1ValueChannel_2->setValue(options_channel2.ustavka1.getStateValue());
//    ui->State2ValueChannel_2->setValue(options_channel2.ustavka2.getStateValue());
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
//    ui->State1ValueChannel_3->setValue(options_channel3.ustavka1.getStateValue());
//    ui->State2ValueChannel_3->setValue(options_channel3.ustavka2.getStateValue());
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
//    ui->State1ValueChannel_4->setValue(options_channel4.ustavka1.getStateValue());
//    ui->State2ValueChannel_4->setValue(options_channel4.ustavka2.getStateValue());
    ui->State1HighMessageChannel_4->setText(options_channel4.GetState1HighMessage());
    ui->State1LowMessageChannel_4->setText(options_channel4.GetState1LowMessage());
    ui->State2HighMessageChannel_4->setText(options_channel4.GetState2HighMessage());
    ui->State2LowMessageChannel_4->setText(options_channel4.GetState2LowMessage());
    ui->Name_Channel_4->setText(options_channel4.GetChannelName());
    ui->math_text_ch_4->setText(options_channel4.GetMathString());
    ui->checkBox_4->setChecked(options_channel4.IsChannelMathematical());

    // реальное разрешение монитора

    ui->resolutionlabel->setText(Options::MonitorResolution);

    if (Options::displayResolution == "1024x768")
        ui->comboBox->setCurrentIndex(0);

    if (Options::displayResolution == "1280x800")
        ui->comboBox->setCurrentIndex(1);

    if (Options::displayResolution == "1920x1080")
        ui->comboBox->setCurrentIndex(2);
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
QString Options::GetNewDisplayResolution() {return ui->comboBox->itemText(ui->comboBox->currentIndex());}

void Options::resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution)
{
    qreal xratio = xresolution / 1280;
    qreal yratio = yresolution / 800;

    QList<QWidget *> widgets = qobj.findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {
        QRect g = widget->geometry();

        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);

        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
        //qw.resizeFont(w);
        //qDebug() << widget;
    }
    return;
}

void Options::OpenKeyboard()
{

}
