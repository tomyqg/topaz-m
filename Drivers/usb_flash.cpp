#include "usb_flash.h"
#include <QProcess>
#include <QDebug>

cUsbFlash::cUsbFlash(QObject *parent) : QObject(parent)
{

    firstScan = false;
    sizeListFlash = 0;
    connect(&timerScanFlash, SIGNAL(timeout()), this, SLOT(scanUsbFlash()));
    timerScanFlash.start(1000);
}

QString cUsbFlash::getNameDisk(uint8_t num)
{
    if(num < listDiscs.size())
        return listDiscs.at(num);
    else
        return "NAN";
}

int cUsbFlash::getNumDisk()
{
    return listDiscs.size();
}

void cUsbFlash::scanUsbFlash()
{
    QProcess process;
    QString string;
    QStringList listFlash;
    QString cmd;
    QChar separator;
#ifdef Q_OS_WIN32
    cmd = "fsutil fsinfo drives";   // команда которая покажет что подключено в данный момент
    separator = ' ';
#else
    cmd = "ls /media";
    separator = '\n';
#endif
    process.start(cmd);
    process.waitForFinished();
    string = process.readAll();
    listFlash = string.split(separator);
    if(listFlash.size() != sizeListFlash)
    {
        sizeListFlash = listFlash.size();
        listDiscs = listFlash;
#ifdef Q_OS_WIN32
        listDiscs.removeFirst();
#endif
        listDiscs.removeLast();
        foreach (QString disc, listDiscs) {
            qDebug() << disc.toStdString().c_str();
        }
        if(firstScan) emit newFlash(listDiscs.size());
        firstScan = true;
    }
}
