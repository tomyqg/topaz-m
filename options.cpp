#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QtTest/QTestEvent>
#include <QtTest/QTestEventList>
#include <QtTest/QTest>
#include <QPoint>
#include <QEvent>
#include <QMouseEvent>
#include <QFocusEvent>

#include "options.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{

    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("OPTIONS"));

    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(Channel1TypeChange()) );
    connect(ui->buttonGroup_2, SIGNAL(buttonClicked(int)), this, SLOT(Channel2TypeChange()) );
    connect(ui->buttonGroup_3, SIGNAL(buttonClicked(int)), this, SLOT(Channel3TypeChange()) );
    connect(ui->buttonGroup_4, SIGNAL(buttonClicked(int)), this, SLOT(Channel4TypeChange()) );

    readoptionsfromfile();
    applysettingstoUI();
    customizeUI();

    ui->timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit->setDateTime(QDateTime::currentDateTime());


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
        ////qDebug() << "options1";
        ////qDebug() << options1.GetSignalType();
    }
    
    if (ui->ButonNapryagenieChannel_1->isChecked())
    {
        options1.SetSignalType(3);
        ui->UnitsChannel_1->setText("V");
        ////qDebug() << "options1";
        ////qDebug() << options1.GetSignalType();
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
        ////qDebug() << "options1";
        ////qDebug() << options1.GetSignalType();
    }
    
    if (ui->ButonImpulseChannel_1->isChecked())
    {
        options1.SetSignalType(6);
        ui->UnitsChannel_1->setText("1");
        ////qDebug() << "options1";
        ////qDebug() << options1.GetSignalType();
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

    //    qDebug() << options2.GetSignalType();
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

void Options::on_radioButton_2_clicked()
{
}

void Options::on_radioButton_3_clicked()
{
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

    options2.SetUnitsName(ui->UnitsChannel_2->text());
    options2.SetHigherLimit(ui->VerhnPredelChannel_2->value());
    options2.SetLowerLimit(ui->NignPredelChannel_2->value());
    options2.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_2->value());
    options2.SetLowerMeasureLimit(ui->NignPredIzmerChannel_2->value());

    options3.SetUnitsName(ui->UnitsChannel_3->text());
    options3.SetHigherLimit(ui->VerhnPredelChannel_3->value());
    options3.SetLowerLimit(ui->NignPredelChannel_3->value());
    options3.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_3->value());
    options3.SetLowerMeasureLimit(ui->NignPredIzmerChannel_3->value());

    options4.SetUnitsName(ui->UnitsChannel_4->text());
    options4.SetHigherLimit(ui->VerhnPredelChannel_4->value());
    options4.SetLowerLimit(ui->NignPredelChannel_4->value());
    options4.SetHigherMeasureLimit(ui->VerhnPredIzmerChannel_4->value());
    options4.SetLowerMeasureLimit(ui->NignPredIzmerChannel_4->value());

    // apply new time

    QProcess process;
    QDateTime newuidate = ui->dateEdit->dateTime();
    QTime newuitime = ui->timeEdit->time();

    QString newdate = QString::number(newuidate.date().year()) + "-" + QString::number(newuidate.date().month()) + "-" + QString::number(newuidate.date().day()) ;
    QString newtime = newuitime.toString();

    process.startDetached("sudo date --set " + newdate);
    process.startDetached("sudo date --set " + newtime); // max freq on
}

void Options::readoptionsfromfile()
{
        QFile infile("/usr/options.txt");

//    QFile infile("C:/Work/options.txt");

    infile.open(QIODevice::ReadOnly);

    QTextStream in(&infile);
    QString sss = in.readLine();

    QJsonDocument doc = QJsonDocument::fromJson(sss.toUtf8());

    QJsonObject json = doc.object();

    QJsonArray array = json["channels"].toArray();

    /*
    for (int x = 0; x < 2; ++x) {

        ch  = array.at(x).toObject();
    }*/

    QJsonObject ch1 = array.at(0).toObject();

    options1.SetHigherLimit(ch1.value("HigherLimit").toInt());
    options1.SetLowerLimit(ch1.value("LowerLimit").toInt());
    options1.SetHigherMeasureLimit(ch1.value("HigherMeasLimit").toInt());
    options1.SetLowerMeasureLimit(ch1.value("LowerMeasLimit").toInt());
    options1.SetSignalType(ch1.value("Type").toInt());
    options1.SetUnitsName(ch1.value("Units").toString());

    QJsonObject ch2 = array.at(1).toObject();

    options2.SetHigherLimit(ch2.value("HigherLimit").toInt());
    options2.SetLowerLimit(ch2.value("LowerLimit").toInt());
    options2.SetHigherMeasureLimit(ch2.value("HigherMeasLimit").toInt());
    options2.SetLowerMeasureLimit(ch2.value("LowerMeasLimit").toInt());
    options2.SetSignalType(ch2.value("Type").toInt());
    options2.SetUnitsName(ch2.value("Units").toString());

    QJsonObject ch3 = array.at(2).toObject();

    options3.SetHigherLimit(ch3.value("HigherLimit").toInt());
    options3.SetLowerLimit(ch3.value("LowerLimit").toInt());
    options3.SetHigherMeasureLimit(ch3.value("HigherMeasLimit").toInt());
    options3.SetLowerMeasureLimit(ch3.value("LowerMeasLimit").toInt());
    options3.SetSignalType(ch3.value("Type").toInt());
    options3.SetUnitsName(ch3.value("Units").toString());

    QJsonObject ch4 = array.at(3).toObject();

    options4.SetHigherLimit(ch4.value("HigherLimit").toInt());
    options4.SetLowerLimit(ch4.value("LowerLimit").toInt());
    options4.SetHigherMeasureLimit(ch4.value("HigherMeasLimit").toInt());
    options4.SetLowerMeasureLimit(ch4.value("LowerMeasLimit").toInt());
    options4.SetSignalType(ch4.value("Type").toInt());
    options4.SetUnitsName(ch4.value("Units").toString());

    //    qDebug() << json;
    //    qDebug() << ch1;
    //    qDebug() << ch2;
    //    qDebug() << ch3;
    //    qDebug() << ch4;

    infile.close();
}
void Options::customizeUI()
{
    //set tab height

    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 40px }");

    //    ui->NignPredelChannel_1->setStyleSheet( "QSpinBox::down-button { width: 10px;}""QSpinBox::up-button { width: 10px; }""QSpinBox { border: 2px solid red; border-radius: 5px; background-color: #e6ffff; height: 70px}");//height: 20px;

    ui->NignPredelChannel_1->setStyleSheet( "QSpinBox { border: 2px solid red; border-radius: 5px; background-color: #e6ffff; height: 60px}");//height: 20px;

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


    ui->UnitsChannel_2->setText(options2.GetUnitsName());
    ui->VerhnPredelChannel_2->setValue(options2.GetHigherLimit());
    ui->NignPredelChannel_2->setValue(options2.GetLowerLimit());
    ui->VerhnPredIzmerChannel_2->setValue(options2.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_2->setValue(options2.GetLowerMeasureLimit());


    ui->UnitsChannel_3->setText(options3.GetUnitsName());
    ui->VerhnPredelChannel_3->setValue(options3.GetHigherLimit());
    ui->NignPredelChannel_3->setValue(options3.GetLowerLimit());
    ui->VerhnPredIzmerChannel_3->setValue(options3.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_3->setValue(options3.GetLowerMeasureLimit());

    ui->UnitsChannel_4->setText(options4.GetUnitsName());
    ui->VerhnPredelChannel_4->setValue(options4.GetHigherLimit());
    ui->NignPredelChannel_4->setValue(options4.GetLowerLimit());
    ui->VerhnPredIzmerChannel_4->setValue(options4.GetHigherMeasureLimit());
    ui->NignPredIzmerChannel_4->setValue(options4.GetLowerMeasureLimit());

}

void Options::WriteSystemOptionsToFile()
{
    //    QJsonObject systemoptions;

    //    QDateTime local(QDateTime::currentDateTime());

    //    systemoptions["Time"] = local.time().toString();
    //    systemoptions["Date"] = local.date().toString();

    //    QString setstr = QJsonDocument(systemoptions).toJson(QJsonDocument::Compact);

    //    //qDebug() << setstr;

    //    QFile file("C:/Work/systemoptions.txt");

    //    file.open(QIODevice::ReadWrite);

    //    file.resize(0); // clear file

    //    QTextStream out(&file);
    //    out << setstr;
    //    file.close();

}

void Options::WriteOptionsToFile()
{
    QJsonObject channel1;
    QJsonObject channel2;
    QJsonObject channel3;
    QJsonObject channel4;
    QJsonObject channels;
    QJsonArray settings;
    
    channel1["Type"] = options1.GetSignalType();
    channel1["Units"] = options1.GetUnitsName();
    channel1["HigherLimit"] = options1.GetHigherLimit();
    channel1["LowerLimit"] = options1.GetLowerLimit();
    channel1["HigherMeasLimit"] = options1.GetHigherMeasureLimit();
    channel1["LowerMeasLimit"] = options1.GetLowerMeasureLimit();
    
    settings.append(channel1);
    
    channel2["Type"] = options2.GetSignalType();
    channel2["Units"] = options2.GetUnitsName();
    channel2["HigherLimit"] = options2.GetHigherLimit();
    channel2["LowerLimit"] = options2.GetLowerLimit();
    channel2["HigherMeasLimit"] = options2.GetHigherMeasureLimit();
    channel2["LowerMeasLimit"] = options2.GetLowerMeasureLimit();

    settings.append(channel2);

    channel3["Type"] = options3.GetSignalType();
    channel3["Units"] = options3.GetUnitsName();
    channel3["HigherLimit"] = options3.GetHigherLimit();
    channel3["LowerLimit"] = options3.GetLowerLimit();
    channel3["HigherMeasLimit"] = options3.GetHigherMeasureLimit();
    channel3["LowerMeasLimit"] = options3.GetLowerMeasureLimit();

    settings.append(channel3);

    channel4["Type"] = options4.GetSignalType();
    channel4["Units"] = options4.GetUnitsName();
    channel4["HigherLimit"] = options4.GetHigherLimit();
    channel4["LowerLimit"] = options4.GetLowerLimit();
    channel4["HigherMeasLimit"] = options4.GetHigherMeasureLimit();
    channel4["LowerMeasLimit"] = options4.GetLowerMeasureLimit();

    settings.append(channel4);
    
    channels["count"] = 4;
    channels["channels"] = settings;

    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);
    
    //    qDebug() << channel4;


        QFile file("/usr/options.txt");
//    QFile file("C:/Work/options.txt");
    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}

void Options::on_VerhnPredIzmerChannel_1_valueChanged(int arg1)
{
}

void Options::on_PeriodIzmerChannel_1_valueChanged(int arg1)
{
}

void Options::on_UnitsChannel_1_editingFinished()
{
    options1.SetUnitsName(ui->UnitsChannel_1->text());
    ////qDebug() << options1.GetUnitsName();
}

void Options::on_pushButton_3_clicked()
{
    //    ui->timeEdit->
    QProcess process1;
    process1.startDetached("xinput_calibrator"); // max perfomance on
}

void Options::on_NignPredelChannel_2_valueChanged(int arg1)
{

    //    ui->NignPredelChannel_2->setValue(kb.getcustomstring().toDouble());
}

void Options::on_tabWidget_2_tabBarClicked(int index)
{
    //    QWidget *newTab = new QWidget(ui->tabWidget_2);
    //    ui->tabWidget_2->addTab(newTab, tr("NewChannel"));
}
