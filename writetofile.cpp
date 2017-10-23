#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "ui_options.h"
#include "messages.h"
#include "keyboard.h"
#include "channelOptions.h"
#include "defines.h"

extern QVector<double> X_Coordinates,Y_coordinates_Chanel_1,Y_coordinates_Chanel_2,Y_coordinates_Chanel_3,Y_coordinates_Chanel_4;

//  extern QString pathtofile ;

void MainWindow::WriteGpio(int num, bool val)
{
    QFile filedir("/sys/class/gpio/gpio" + QString::number(num) + "/direction");
    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);
    outdir << "out";
    filedir.close();
    QFile file("/sys/class/gpio/gpio" + QString::number(num) + "/value");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << val;
    file.close();
}

void Options::WriteSystemOptionsToFile()
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

void Options::WriteAllChannelsOptionsToFile()
{
    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&options_channel1);
    ChannelsObjectsList.append(&options_channel2);
    ChannelsObjectsList.append(&options_channel3);
    ChannelsObjectsList.append(&options_channel4);

    QJsonObject channeljsonobj,channels;
    QJsonArray settings;

    foreach (ChannelOptions * Channel, ChannelsObjectsList) {
            channeljsonobj["Type"] = Channel->GetSignalType();
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
            channeljsonobj["RegistrationType"] = Channel->GetRegistrationType();
            settings.append(channeljsonobj);
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

void MessageWrite::WriteAllLogToFile()
{
    QJsonObject archive;
    Options opt;
    double maxmes  = opt.GetLogMessagesLimit();

    while (messagesqueue.count()>maxmes) // удаляем все значения что были раньше чем нужно
    {
        messagesqueue.removeFirst();
    }
    archive["messages"] = messagesqueue;
    archive["totalmsg"] = messagesqueue.count();
    QString setstr = QJsonDocument(archive).toJson(QJsonDocument::Compact);
    QFile file(pathtomessages);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();

    opt.deleteLater();
}

void MessageWrite::LogAddMessage(QString nm)
{
    QJsonObject themessage;
    QDateTime local (QDateTime::currentDateTime());
    themessage["T"] = local.time().toString();
    themessage["D"] = local.date().toString("dd/MM/yy");
    themessage["M"] = nm;
    messagesqueue.append(themessage);
}

void MessageWrite::LogClear()
{
    QFile file(pathtomessages);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    file.close();
}

void MainWindow::WriteArchiveToFile() // пишет архив в файл каждые пять сек... вроде...
{
    this->update(); // мы апдейтим нашу главную форму

    // и начинаем архивацию
    QJsonObject archivechannel1;
    QJsonObject archivechannel2;
    QJsonObject archivechannel3;
    QJsonObject archivechannel4;
    QJsonObject archive;
    QJsonArray archives;
    QJsonArray valuesarray1;
    QJsonArray valuesarray2;
    QJsonArray valuesarray3;
    QJsonArray valuesarray4;

    for(int y=0; y<Y_coordinates_Chanel_1.size(); y++)
        valuesarray1.append(QString::number( Y_coordinates_Chanel_1.at(y), 'f', 3)); // округляем до 3 знаков после запятой
    for(int y=0; y<Y_coordinates_Chanel_2.size(); y++)
        valuesarray2.append(QString::number( Y_coordinates_Chanel_2.at(y), 'f', 3)); // округляем до 3 знаков после запятой
    for(int y=0; y<Y_coordinates_Chanel_3.size(); y++)
        valuesarray3.append(QString::number( Y_coordinates_Chanel_3.at(y), 'f', 3)); // округляем до 3 знаков после запятой
    for(int y=0; y<Y_coordinates_Chanel_4.size(); y++)
        valuesarray4.append(QString::number( Y_coordinates_Chanel_4.at(y), 'f', 3)); // округляем до 3 знаков после запятой

    QString channel1period = QString::number( channel1.GetMeasurePeriod(), 'f', 1);
    QString channel2period = QString::number( channel2.GetMeasurePeriod(), 'f', 1);
    QString channel3period = QString::number( channel3.GetMeasurePeriod(), 'f', 1);
    QString channel4period = QString::number( channel4.GetMeasurePeriod(), 'f', 1);

    archivechannel1["size"] = valuesarray1.size();
    archivechannel1["values"] = valuesarray1;
    archivechannel1["name"] = "AI4ch1";
    archivechannel1["T"] = channel1period;


    archivechannel2["size"] = valuesarray2.size();
    archivechannel2["values"] = valuesarray2;
    archivechannel2["name"] = "AI4ch2";
    archivechannel2["T"] = channel2period;


    archivechannel3["size"] = valuesarray3.size();
    archivechannel3["values"] = valuesarray3;
    archivechannel3["name"] = "AI4ch3";
    archivechannel3["T"] = channel3period;


    archivechannel4["size"] = valuesarray4.size();
    archivechannel4["values"] = valuesarray4;
    archivechannel4["name"] = "AI4ch4";
    archivechannel4["T"] = channel4period;

    archives.append(archivechannel1);
    archives.append(archivechannel2);
    archives.append(archivechannel3);
    archives.append(archivechannel4);

    QDateTime CurrentDateTime(QDateTime::currentDateTime());
    archive["Archives"] = archives;
    archive["StartDate"] = MainWindow::startdate;
    archive["StartTime"] = MainWindow::starttime;
    archive["EndDate"] = CurrentDateTime.toString("dd/MM/yy");
    archive["EndTime"] = CurrentDateTime.toString("hh:mm:ss");

    MainWindow::startdate = CurrentDateTime.toString("dd/MM/yy");
    MainWindow::starttime = CurrentDateTime.toString("hh:mm:ss");

    QString setstr = QJsonDocument(archive).toJson(QJsonDocument::Compact);

    QString pathtologs = pathtolog;
    // создаем файл с именем текстового файла архива
    pathtologs.append(CurrentDateTime.toString("ddMMyy"));
    pathtologs.append(CurrentDateTime.toString("hhmmss"));
    pathtologs.append(".txt");
    QFile file(pathtologs);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}

void MainWindow::CreateMODBusConfigFile() // создает файл дескрипотор системы
{
    QJsonArray Devices;
    QJsonObject Device1;
    QJsonObject Device2;
    QJsonObject Device;

    QJsonArray Device1ReadCommands;
    QJsonArray Device1WriteCommands;
    QJsonArray Registration1,Registration2;

    QJsonArray Device2ReadCommands;
    QJsonArray Device2WriteCommands;

    // конфиг для платы 4AI
    Device1ReadCommands.append(0x04); // на чтение аналогового ввода? Команда 0x04
    Device1WriteCommands.append(0x06); // на запись аналогового вывода? Команда 0x06

    Registration1.append("Screen");
    Registration1.append("Memory");

    Device1["Address"] = 0x01;
    Device1["Name"] = "4AI";
    Device1["Speed"] = 9600;
    Device1["Other"] = "Other";
    Device1["Stuff"] = "Stuff";
    Device1["ReadCommands"] = Device1ReadCommands;
    Device1["WriteCommands"] = Device1WriteCommands;
    Device1["DataType"] = "float";
    Device1["Registration"] = Registration1;


    // конфиг для платы Relays
    Device2ReadCommands.append(0x01); //чтение дискретного вывода? Команда 0x01
    Device2WriteCommands.append(0x05); // на запись дискретного вывода? Команда 0x05
    Registration1.append("Memory");

    Device2["Address"] = 0x02;
    Device2["Name"] = "Relay";
    Device2["Speed"] = 115200;
    Device2["Other"] = "Relay";
    Device2["Stuff"] = "Module";
    Device2["ReadCommands"] = Device2ReadCommands;
    Device2["WriteCommands"] = Device2WriteCommands;
    Device2["DataType"] = "bool";
    Device2["Registration"] = Registration2;

    Devices.append(Device1);
    Devices.append(Device2);

    Device["Count"] = Devices.count();
    Device["Devices"] = Devices;

    QString setstr = QJsonDocument(Device).toJson(QJsonDocument::Compact);
    QFile file(pathtomodbusconfigfile);
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << setstr;
    file.close();
}
