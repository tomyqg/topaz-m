#include "options.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"
#include "messages.h"

QString Options::calibrationprm = "3383 3962 234 599";
QString Options::olderprop = "";

double Options::maxmessageslimit=1000;

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("OPTIONS"));

    connect(ui->buttonGroup,   SIGNAL(buttonClicked(int)), this, SLOT(Channel1TypeChange()) );
    connect(ui->buttonGroup_2, SIGNAL(buttonClicked(int)), this, SLOT(Channel2TypeChange()) );
    connect(ui->buttonGroup_3, SIGNAL(buttonClicked(int)), this, SLOT(Channel3TypeChange()) );
    connect(ui->buttonGroup_4, SIGNAL(buttonClicked(int)), this, SLOT(Channel4TypeChange()) );

    readoptionsfromfile();
    applysettingstoUI();
    customizeUI();

    QList<QSpinBox*> spinList = Options::findChildren<QSpinBox*> (  );

    for (int i = 0; i < spinList.count(); ++i) {
        QSpinBox *sb = spinList.at(i);
        sb->installEventFilter(this);
    }

    QList<QLineEdit*> lineeditList = Options::findChildren<QLineEdit*> (  );

    for (int i = 0; i < lineeditList.count(); ++i) {
        QLineEdit *le = lineeditList.at(i);
        le->installEventFilter(this);
    }

    ui->textEdit->setReadOnly(1);
}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
    applynewsettings();
    WriteSystemOptionsToFile();
    WriteOptionsToFile();
    this->close();
}

void Options::on_pushButton_2_clicked()
{
    this->close();
}

void Options::Channel1TypeChange()
{
    if (ui->ButonOtklChannel_1->isChecked())
    {
        options1.SetSignalType(1);

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
    
    if (ui->ButonTokChannel_1->isChecked())
    {
        ui->UnitsChannel_1->setText("mA");
        options1.SetSignalType(2);
    }
    
    if (ui->ButonNapryagenieChannel_1->isChecked())
    {
        options1.SetSignalType(3);
        ui->UnitsChannel_1->setText("V");
    }
    
    if (ui->ButonResistorChannel_1->isChecked())
    {
        options1.SetSignalType(4);
        ui->UnitsChannel_1->setText("Om");
    }
    
    if (ui->ButonTermoparaChannel_1->isChecked())
    {
        options1.SetSignalType(5);
        ui->UnitsChannel_1->setText("mV");
    }
    
    if (ui->ButonImpulseChannel_1->isChecked())
    {
        options1.SetSignalType(6);
        ui->UnitsChannel_1->setText("1");
    }
}

void Options::Channel2TypeChange()
{
    if (ui->ButonOtklChannel_2->isChecked())
    {
        options2.SetSignalType(1);
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
    
    if (ui->ButonTokChannel_2->isChecked())
    {
        options2.SetSignalType(2);
        ui->UnitsChannel_2->setText("mA");
    }
    
    if (ui->ButonNapryagenieChannel_2->isChecked())
    {
        options2.SetSignalType(3);
        ui->UnitsChannel_2->setText("V");
    }
    
    if (ui->ButonResistorChannel_2->isChecked())
    {
        options2.SetSignalType(4);
        ui->UnitsChannel_2->setText("Om");
    }
    
    if (ui->ButonTermoparaChannel_2->isChecked())
    {
        options2.SetSignalType(5);
        ui->UnitsChannel_2->setText("mV");
    }
    
    if (ui->ButonImpulseChannel_2->isChecked())
    {
        options2.SetSignalType(6);
        ui->UnitsChannel_2->setText("1");
    }
}

void Options::Channel3TypeChange()
{
    if (ui->ButonOtklChannel_3->isChecked())
    {
        options2.SetSignalType(1);
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

    if (ui->ButonOtklChannel_3->isChecked())
    {
        options3.SetSignalType(1);
    }

    if (ui->ButonTokChannel_3->isChecked())
    {
        options3.SetSignalType(2);
    }

    if (ui->ButonNapryagenieChannel_3->isChecked())
    {
        options3.SetSignalType(3);
    }

    if (ui->ButonResistorChannel_3->isChecked())
    {
        options3.SetSignalType(4);
    }

    if (ui->ButonTermoparaChannel_3->isChecked())
    {
        options3.SetSignalType(5);
    }

    if (ui->ButonImpulseChannel_3->isChecked())
    {
        options3.SetSignalType(6);
    }
}

void Options::Channel4TypeChange()
{

    if (ui->ButonOtklChannel_4->isChecked())
    {
        options2.SetSignalType(1);
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

    if (ui->ButonOtklChannel_4->isChecked())
    {
        options4.SetSignalType(1);
    }

    if (ui->ButonTokChannel_4->isChecked())
    {
        options4.SetSignalType(2);
    }

    if (ui->ButonNapryagenieChannel_4->isChecked())
    {
        options4.SetSignalType(3);
    }

    if (ui->ButonResistorChannel_4->isChecked())
    {
        options4.SetSignalType(4);
    }

    if (ui->ButonTermoparaChannel_4->isChecked())
    {
        options4.SetSignalType(5);
    }

    if (ui->ButonImpulseChannel_4->isChecked())
    {
        options4.SetSignalType(6);
    }
}

QString Options::GetSignalUnits()
{
    return "Signal units";
}

void Options::applynewsettings()
{
    options1.SetUnitsName(ui->UnitsChannel_1->text());
    options1.SetHigherLimit(ui->VerhnPredelChannel_1->value());
    options1.SetLowerLimit(ui->NignPredelChannel_1->value());
    options1.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_1->value());
    options1.SetLowerMeasureLimit(ui->NignPredIzmerChannel_1->value());
    options1.SetMeasurePeriod(ui->PeriodIzmerChannel_1->value());
    options1.SetState1HighMessage(ui->State1HighMessageChannel_1->text());
    options1.SetState1LowMessage(ui->State1LowMessageChannel_1->text());
    options1.SetState2HighMessage(ui->State2HighMessageChannel_1->text());
    options1.SetState2LowMessage(ui->State2LowMessageChannel_1->text());
    options1.SetState1Value(ui->State1ValueChannel_1->value());
    options1.SetState2Value(ui->State2ValueChannel_1->value());
    options1.SetChannelName(ui->Name_Channel_1->text());
    options1.SetMathEquation(ui->math_text_ch_1->text());
    options1.SetMathematical(ui->checkBox->isChecked());

    options2.SetUnitsName(ui->UnitsChannel_2->text());
    options2.SetHigherLimit(ui->VerhnPredelChannel_2->value());
    options2.SetLowerLimit(ui->NignPredelChannel_2->value());
    options2.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_2->value());
    options2.SetLowerMeasureLimit(ui->NignPredIzmerChannel_2->value());
    options2.SetMeasurePeriod(ui->PeriodIzmerChannel_2->value());
    options2.SetState1HighMessage(ui->State1HighMessageChannel_2->text());
    options2.SetState1LowMessage(ui->State1LowMessageChannel_2->text());
    options2.SetState2HighMessage(ui->State2HighMessageChannel_2->text());
    options2.SetState2LowMessage(ui->State2LowMessageChannel_2->text());
    options2.SetState1Value(ui->State1ValueChannel_2->value());
    options2.SetState2Value(ui->State2ValueChannel_2->value());
    options2.SetChannelName(ui->Name_Channel_2->text());
    options2.SetMathEquation(ui->math_text_ch_2->text());
    options2.SetMathematical(ui->checkBox_2->isChecked());

    options3.SetUnitsName(ui->UnitsChannel_3->text());
    options3.SetHigherLimit(ui->VerhnPredelChannel_3->value());
    options3.SetLowerLimit(ui->NignPredelChannel_3->value());
    options3.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_3->value());
    options3.SetLowerMeasureLimit(ui->NignPredIzmerChannel_3->value());
    options3.SetMeasurePeriod(ui->PeriodIzmerChannel_3->value());
    options3.SetState1HighMessage(ui->State1HighMessageChannel_3->text());
    options3.SetState1LowMessage(ui->State1LowMessageChannel_3->text());
    options3.SetState2HighMessage(ui->State2HighMessageChannel_3->text());
    options3.SetState2LowMessage(ui->State2LowMessageChannel_3->text());
    options3.SetState1Value(ui->State1ValueChannel_3->value());
    options3.SetState2Value(ui->State2ValueChannel_3->value());
    options3.SetChannelName(ui->Name_Channel_3->text());
    options3.SetMathEquation(ui->math_text_ch_3->text());
    options3.SetMathematical(ui->checkBox_3->isChecked());

    options4.SetUnitsName(ui->UnitsChannel_4->text());
    options4.SetHigherLimit(ui->VerhnPredelChannel_4->value());
    options4.SetLowerLimit(ui->NignPredelChannel_4->value());
    options4.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_4->value());
    options4.SetLowerMeasureLimit(ui->NignPredIzmerChannel_4->value());
    options4.SetMeasurePeriod(ui->PeriodIzmerChannel_4->value());
    options4.SetState1HighMessage(ui->State1HighMessageChannel_4->text());
    options4.SetState1LowMessage(ui->State1LowMessageChannel_4->text());
    options4.SetState2HighMessage(ui->State2HighMessageChannel_4->text());
    options4.SetState2LowMessage(ui->State2LowMessageChannel_4->text());
    options4.SetState1Value(ui->State1ValueChannel_4->value());
    options4.SetState2Value(ui->State2ValueChannel_4->value());
    options4.SetChannelName(ui->Name_Channel_4->text());
    options4.SetMathEquation(ui->math_text_ch_4->text());
    options4.SetMathematical(ui->checkBox_4->isChecked());

    setmaxmessageslimit(ui->spinBox->value());
}

void Options::customizeUI()
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

    {
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

}

void Options::applysettingstoUI()
{

    int sigtype = options1.GetSignalType();
    int sigtype2 = options2.GetSignalType();
    int sigtype3 = options3.GetSignalType();
    int sigtype4 = options4.GetSignalType();
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

    ui->UnitsChannel_1->setText(options1.GetUnitsName());
    ui->VerhnPredelChannel_1->setValue(options1.GetHigherLimit());
    ui->NignPredelChannel_1->setValue(options1.GetLowerLimit());
    ui->VerhnPredIzmerChannel_1->setValue(options1.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_1->setValue(options1.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_1->setValue(options1.GetMeasurePeriod());
    ui->State1ValueChannel_1->setValue(options1.GetState1Value());
    ui->State2ValueChannel_1->setValue(options1.GetState2Value());
    ui->State1HighMessageChannel_1->setText(options1.GetState1HighMessage());
    ui->State1LowMessageChannel_1->setText(options1.GetState1LowMessage());
    ui->State2HighMessageChannel_1->setText(options1.GetState2HighMessage());
    ui->State2LowMessageChannel_1->setText(options1.GetState2LowMessage());
    ui->Name_Channel_1->setText(options1.GetChannelName());
    ui->math_text_ch_1->setText(options1.GetMathString());
    ui->checkBox->setChecked(options1.IsMathematical());

    ui->UnitsChannel_2->setText(options2.GetUnitsName());
    ui->VerhnPredelChannel_2->setValue(options2.GetHigherLimit());
    ui->NignPredelChannel_2->setValue(options2.GetLowerLimit());
    ui->VerhnPredIzmerChannel_2->setValue(options2.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_2->setValue(options2.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_2->setValue(options2.GetMeasurePeriod());
    ui->State1ValueChannel_2->setValue(options2.GetState1Value());
    ui->State2ValueChannel_2->setValue(options2.GetState2Value());
    ui->State1HighMessageChannel_2->setText(options2.GetState1HighMessage());
    ui->State1LowMessageChannel_2->setText(options2.GetState1LowMessage());
    ui->State2HighMessageChannel_2->setText(options2.GetState2HighMessage());
    ui->State2LowMessageChannel_2->setText(options2.GetState2LowMessage());
    ui->Name_Channel_2->setText(options2.GetChannelName());
    ui->math_text_ch_2->setText(options2.GetMathString());
    ui->checkBox_2->setChecked(options2.IsMathematical());

    ui->UnitsChannel_3->setText(options3.GetUnitsName());
    ui->VerhnPredelChannel_3->setValue(options3.GetHigherLimit());
    ui->NignPredelChannel_3->setValue(options3.GetLowerLimit());
    ui->VerhnPredIzmerChannel_3->setValue(options3.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_3->setValue(options3.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_3->setValue(options3.GetMeasurePeriod());
    ui->State1ValueChannel_3->setValue(options3.GetState1Value());
    ui->State2ValueChannel_3->setValue(options3.GetState2Value());
    ui->State1HighMessageChannel_3->setText(options3.GetState1HighMessage());
    ui->State1LowMessageChannel_3->setText(options3.GetState1LowMessage());
    ui->State2HighMessageChannel_3->setText(options3.GetState2HighMessage());
    ui->State2LowMessageChannel_3->setText(options3.GetState2LowMessage());
    ui->Name_Channel_3->setText(options3.GetChannelName());
    ui->math_text_ch_3->setText(options3.GetMathString());
    ui->checkBox_3->setChecked(options3.IsMathematical());

    ui->UnitsChannel_4->setText(options4.GetUnitsName());
    ui->VerhnPredelChannel_4->setValue(options4.GetHigherLimit());
    ui->NignPredelChannel_4->setValue(options4.GetLowerLimit());
    ui->VerhnPredIzmerChannel_4->setValue(options4.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_4->setValue(options4.GetLowerMeasureLimit());
    ui->PeriodIzmerChannel_4->setValue(options4.GetMeasurePeriod());
    ui->State1ValueChannel_4->setValue(options4.GetState1Value());
    ui->State2ValueChannel_4->setValue(options4.GetState2Value());
    ui->State1HighMessageChannel_4->setText(options4.GetState1HighMessage());
    ui->State1LowMessageChannel_4->setText(options4.GetState1LowMessage());
    ui->State2HighMessageChannel_4->setText(options4.GetState2HighMessage());
    ui->State2LowMessageChannel_4->setText(options4.GetState2LowMessage());
    ui->Name_Channel_4->setText(options4.GetChannelName());
    ui->math_text_ch_4->setText(options4.GetMathString());
    ui->checkBox_4->setChecked(options4.IsMathematical());
}

void Options::on_UnitsChannel_1_editingFinished()
{
    options1.SetUnitsName(ui->UnitsChannel_1->text());
    ////qDebug() << options1.GetUnitsName();
}

void Options::on_pushButton_3_clicked()
{
    QProcess process1;
    process1.start("xinput_calibrator"); // max perfomance on

    QString stringtofind = "Option	\"Calibration\"";
    QString stringtofind2 = "Option	\"SwapAxes\"";

    process1.waitForFinished();

    QString output = QTextCodec::codecForMib(106)->toUnicode(process1.readAll());

    if (output.indexOf(stringtofind)>=0)
    {
        //ui->textEdit->setText(Options::calibrationprm);
        //QString a = Options::calibrationprm;
        QString pice = output.remove(0,(output.indexOf(stringtofind ) ) );
        pice = pice.remove(pice.indexOf(stringtofind2), pice.length() - pice.indexOf(stringtofind2) );
        pice = pice.simplified();
        pice = pice.remove(0, stringtofind.length() );
        Options::calibrationprm = pice.remove('\"');
    }
}

void Options::on_pushButton_4_clicked()
{
    MessageWrite ms;
    ms.LogClear();
}

double Options::getmaxmessageslimit()
{
    return maxmessageslimit;
}

void Options::setmaxmessageslimit(double n)
{
    maxmessageslimit = n;
}

void Options::on_pushButton_5_clicked()
{
    // apply new time
    QProcess process;
    QDateTime newuidate = ui->dateEdit->dateTime();
    QTime newuitime = ui->timeEdit->time();

    QString newdate = QString::number(newuidate.date().year()) + "-" + QString::number(newuidate.date().month()) + "-" + QString::number(newuidate.date().day()) ;
    QString newtime = newuitime.toString();

    process.startDetached("sudo date --set " + newdate);
    process.startDetached("sudo date --set " + newtime); // max freq on
}

void Options::on_horizontalScrollBar_sliderReleased()
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
