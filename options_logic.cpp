#include "options.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"
#include "messages.h"

#define MYD

QString Options::calibrationprm = "3383 3962 234 599";
QString Options::olderprop = "";
QString Options::displayResolution = "1280x800";
QString Options::MonitorResolution = "35";
extern QString pathtofile;
int Options::DisplayParametr = DisplayParametrEnum::Polar;

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

    ReadSystemOptionsFromFile();
    ReadChannelsOptionsFromFile();
    ApplyNewSettingstoOptionsUI();
    CustomizeOptionsUI();

    QList<QSpinBox*> spinList = Options::findChildren<QSpinBox*> ();

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
    ApplyNewSettingstoAllChannels();
    WriteSystemOptionsToFile();
    WriteAllChannelsOptionsToFile();
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
        options_channel1.SetSignalType(1);

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
        options_channel1.SetSignalType(2);
    }
    
    if (ui->ButonNapryagenieChannel_1->isChecked())
    {
        options_channel1.SetSignalType(3);
        ui->UnitsChannel_1->setText("V");
    }
    
    if (ui->ButonResistorChannel_1->isChecked())
    {
        options_channel1.SetSignalType(4);
        ui->UnitsChannel_1->setText("Om");
    }
    
    if (ui->ButonTermoparaChannel_1->isChecked())
    {
        options_channel1.SetSignalType(5);
        ui->UnitsChannel_1->setText("mV");
    }
    
    if (ui->ButonImpulseChannel_1->isChecked())
    {
        options_channel1.SetSignalType(6);
        ui->UnitsChannel_1->setText("1");
    }
}

void Options::Channel2TypeChange()
{
    if (ui->ButonOtklChannel_2->isChecked())
    {
        options_channel2.SetSignalType(1);
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
        options_channel2.SetSignalType(2);
        ui->UnitsChannel_2->setText("mA");
    }
    
    if (ui->ButonNapryagenieChannel_2->isChecked())
    {
        options_channel2.SetSignalType(3);
        ui->UnitsChannel_2->setText("V");
    }
    
    if (ui->ButonResistorChannel_2->isChecked())
    {
        options_channel2.SetSignalType(4);
        ui->UnitsChannel_2->setText("Om");
    }
    
    if (ui->ButonTermoparaChannel_2->isChecked())
    {
        options_channel2.SetSignalType(5);
        ui->UnitsChannel_2->setText("mV");
    }
    
    if (ui->ButonImpulseChannel_2->isChecked())
    {
        options_channel2.SetSignalType(6);
        ui->UnitsChannel_2->setText("1");
    }
}

void Options::Channel3TypeChange()
{
    if (ui->ButonOtklChannel_3->isChecked())
    {
        options_channel2.SetSignalType(1);
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
        options_channel3.SetSignalType(1);
    }

    if (ui->ButonTokChannel_3->isChecked())
    {
        options_channel3.SetSignalType(2);
    }

    if (ui->ButonNapryagenieChannel_3->isChecked())
    {
        options_channel3.SetSignalType(3);
    }

    if (ui->ButonResistorChannel_3->isChecked())
    {
        options_channel3.SetSignalType(4);
    }

    if (ui->ButonTermoparaChannel_3->isChecked())
    {
        options_channel3.SetSignalType(5);
    }

    if (ui->ButonImpulseChannel_3->isChecked())
    {
        options_channel3.SetSignalType(6);
    }
}

void Options::Channel4TypeChange()
{
    if (ui->ButonOtklChannel_4->isChecked())
    {
        options_channel2.SetSignalType(1);
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
        options_channel4.SetSignalType(1);
    }

    if (ui->ButonTokChannel_4->isChecked())
    {
        options_channel4.SetSignalType(2);
    }

    if (ui->ButonNapryagenieChannel_4->isChecked())
    {
        options_channel4.SetSignalType(3);
    }

    if (ui->ButonResistorChannel_4->isChecked())
    {
        options_channel4.SetSignalType(4);
    }

    if (ui->ButonTermoparaChannel_4->isChecked())
    {
        options_channel4.SetSignalType(5);
    }

    if (ui->ButonImpulseChannel_4->isChecked())
    {
        options_channel4.SetSignalType(6);
    }
}

void Options::ApplyNewSettingstoAllChannels()
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
    options_channel1.SetMathEquation(ui->math_text_ch_1->text());
    options_channel1.SetMathematical(ui->checkBox->isChecked());

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
    options_channel2.SetMathEquation(ui->math_text_ch_2->text());
    options_channel2.SetMathematical(ui->checkBox_2->isChecked());

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
    options_channel3.SetMathEquation(ui->math_text_ch_3->text());
    options_channel3.SetMathematical(ui->checkBox_3->isChecked());

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
    options_channel4.SetMathEquation(ui->math_text_ch_4->text());
    options_channel4.SetMathematical(ui->checkBox_4->isChecked());

    SetLogMessagesLimit(ui->spinBox->value());
}

void Options::on_pushButton_3_clicked()
{
    TouchScreenCalibrate();
}

void Options::on_pushButton_4_clicked()
{
    MessageWrite ms;
    ms.LogClear();
}

double Options::GetLogMessagesLimit()
{
    return maxmessageslimit;
}

void Options::SetLogMessagesLimit(double n)
{
    maxmessageslimit = n;
}

void Options::on_pushButton_5_clicked()
{
    QProcess process;
    QDateTime newuidate = ui->dateEdit->dateTime();
    QTime newuitime = ui->timeEdit->time();

#ifdef MYD


#endif

    // apply new time

    QString newdate = QString::number(newuidate.date().year()) + "-" + QString::number(newuidate.date().month()) + "-" + QString::number(newuidate.date().day()) ;
    QString newtime = newuitime.toString();

    process.startDetached("date --set " + newdate);
    process.startDetached("date --set " + newtime); // max freq on
}

void Options::on_comboBox_currentIndexChanged(int index)
{
    ui->lineEdit->setText(ui->comboBox->itemText(ui->comboBox->currentIndex()));
}

void Options::on_ArchivetoUSB_button_clicked()
{
    ArchiveToUSBFlashDrive();
}

void Options::TouchScreenCalibrate()
{

    QProcess process1;

#ifdef MYD // если плата MYD
    process1.start("ts_calibrate"); // max perfomance on
    process1.waitForFinished();
    return;
#endif

    // если плата BBB
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



void Options::ArchiveToUSBFlashDrive() // кастомизирует контроллы во вкладке опшнс
{

    //    return;

    // этот кусок кода ищет название куда смонтирована флешка
    QProcess process1,process2;
    process2.start("mount"); // команда которая покажет что подключено в данный момент
    process2.waitForFinished();

    QString output = QTextCodec::codecForMib(106)->toUnicode(process2.readAll());

    QString stringtofind = "/media/sd";
    QString stringtofind2 = " type vfat";

    if (output.indexOf(stringtofind )==-1) // если строка не найдена то устройство не вставлено
    {
        ui->outputtext->setText("Нет USB, или неверный формат\nВставьте USB носитель,подождите несколько секунд и повторите попытку\nформат FAT32\nот 4 до 32 Gb");
        return;
    }

    QString peace;

    peace = output;

    peace = peace.remove(0,(output.lastIndexOf(stringtofind)));

    peace = peace.remove(peace.lastIndexOf(stringtofind2), peace.length() - peace.lastIndexOf(stringtofind2) );

    //    ui->outputtext->setText(peace);
    //    return;

    if ( (peace == "") || (peace == NULL) )
    {
        ui->outputtext->setText("Нет USB, или неверный формат\nВставьте USB носитель,подождите несколько секунд и повторите попытку\nформат FAT32\nот 4 до 32 Gb");
        return;
    }
    else
    {
        ui->outputtext->setText("Найдено USB: " + peace);
    }

    // конец кода который ищет название куда смонтирована флешка//  что то на подобие /media/sda1/




    QString USBFlashPath = peace.trimmed().simplified();

    // тест что флешка живая

    process1.start("cat " + USBFlashPath  + "/testfile.txt"); // запускаем программу копирования файла на флешку
    process1.waitForFinished();

    if ((QTextCodec::codecForMib(106)->toUnicode(process1.readAll()) )!="testok")
    {
        ui->outputtext->setText("Ошибка тестирования USB\nОтсутствует файл testfile.txt\nСоздайте файл testfile.txt с содержимым \"testok\"\nи поместите в корень USB носителя\n");
        return;
    }

    QString SourceFilePath = pathtofile + "archive.txt";
    QDateTime local(QDateTime::currentDateTime());
    QString copycommandstring = "cp " + SourceFilePath + " " + USBFlashPath  + "/USBArchive" + local.time().toString("HH-mm-ss")+ "_" +  local.date().toString("dd-MM-yyyy") + ".txt";

    ui->outputtext->setText("Найдено USB: " + USBFlashPath);
    ui->outputtext->setText("Копирование ...");
    process1.start(copycommandstring); // запускаем программу копирования файла на флешку
    process1.waitForFinished(1000);
    ui->outputtext->setText("Копирование на USB завершено. -->" + USBFlashPath);
}
