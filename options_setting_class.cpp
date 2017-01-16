#include "options_setting_class.h"
#include "options.h"
#include "ui_options.h"
#include "updategraph.h"

#include <QMessageBox>
#include <QString>

void Options::savesettings()
{
    QMessageBox::information(this,"info","Options are saved");
}

void Options::WriteOptionsToFile()
{
    QJsonObject channel1;
    QJsonObject channels;
    QJsonArray settings;

    channel1["Type"] = GetSignalType();
    channel1["Units"] = GetSignalUnits();
    channel1["HigherLimit"] = 100;
    channel1["LowerLimit"] = 0;
    channel1["HigherMeasLimit"] = 20;
    channel1["LowerMeasLimit"] = 4;

    settings.append(channel1);

    channels["channel1"] = settings;
    channels["channel2"] = settings;

    QString setstr = QJsonDocument(channels).toJson(QJsonDocument::Compact);

    qDebug() << QJsonDocument(channels).toJson(QJsonDocument::Compact);

    //    QFile file("/sys/class/gpio/gpio69/value");
    QFile file("C:/Work/options.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << setstr;
    file.close();
}
