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

    int signaltype = Options::GetSignalType();

    QJsonObject obj;//root object
    QJsonArray settings;
    QJsonObject channel1;

    channel1["Type"] = "1";

    settings.append(channel1);

    obj["settings"] = channel1;

    QString setstr = QJsonDocument(obj).toJson(QJsonDocument::Compact);

    qDebug() << QJsonDocument(obj).toJson(QJsonDocument::Compact);

//    QString stype = "2";

    //    QFile file("/sys/class/gpio/gpio69/value");
    QFile file("C:/Work/options.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << setstr;
//    stype ="мАмперы";
//    out << "Measure: " +  stype;
    file.close();
//    ui->pushButton_2->setText("настрйки сохранены");
}
