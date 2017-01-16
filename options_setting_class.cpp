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
    QJsonArray settings;
    QJsonObject channel1;

    channel1["Type"] = GetSignalType();
    channel1["Units"] = GetSignalUnits();
    channel1["HigherLimit"] = 100;
    channel1["LowerLimit"] = 0;
    channel1["HigherMeasLimit"] = 20;
    channel1["LowerMeasLimit"] = 4;

    settings.append(channel1);

//    channel1["settings"] = settings;
//    channel1["settings2"] = settings;

    QString setstr = QJsonDocument(channel1).toJson(QJsonDocument::Compact);

    qDebug() << QJsonDocument(channel1).toJson(QJsonDocument::Compact);

    //    QFile file("/sys/class/gpio/gpio69/value");
    QFile file("C:/Work/options.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << setstr;
    file.close();
}
