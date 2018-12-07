//#include <QTextCodec>
#include <QDebug>
#include "ip_controller.h"

#define TIME_UPDATE_IP  2000

cIpController::cIpController(QString eth, QObject *parent) :
    ethName(eth),
    QObject(parent)
{
    online = false;
    enable = false;
    ipAddr = DEF_IP;
    netMask = DEF_MASK;
    gateWay = DEF_GW;
    connect(&timerUpdate, SIGNAL(timeout()), this, SLOT(updateStatus()));
    timerUpdate.start(TIME_UPDATE_IP);
    textCodec=QTextCodec::codecForLocale();
}

void cIpController::setConfig(QString ip, QString mask, QString gw)
{
    ipAddr = ip;
    netMask = mask;
    gateWay = gw;
    winDebugEnable = true;
    QString out;
    QProcess ifconfig;


        out = "ifconfig " + ethName + " " + ipAddr\
                + " netmask " + netMask + " broadcast " + gateWay;
#ifndef Q_OS_WIN
        ifconfig.start(out);
        ifconfig.waitForFinished();
        qDebug() << out.toStdString().c_str();
#else
        qDebug() << out.toStdString().c_str();
#endif
//        if(!enable)
//        {
//            out = "ifup " + ethName;
//#ifndef Q_OS_WIN
//            ifconfig.start(out);
//            ifconfig.waitForFinished();
//#else
//            qDebug() << out.toStdString().c_str();
//#endif
//        }
//    }
//    else
//    {
//        emit signalErrIp();
//    }
}

void cIpController::setOff()
{
    QProcess ifconfig;
    winDebugEnable = false;
    QString out = "ifdown " + ethName;
#ifndef Q_OS_WIN
    ifconfig.start(out);
    ifconfig.waitForFinished();
#else
    qDebug() << out.toStdString().c_str();
#endif
}

void cIpController::updateStatus()
{
//    QString out = "ifconfig " + ethName + " | grep \"BROADCAST\"";
//#ifndef Q_OS_WIN
//    ifconfig.start(out);
//    ifconfig.waitForFinished(500);
//#else
//    qDebug() << out.toStdString().c_str();
//#endif
//#ifndef Q_OS_WIN
//    QString in = ifconfig.readAll();
//#else
//    QString in;
//    if(winDebugEnable == true)
//    {
//        in = "UP BROADCAST RUNNING";
//    }
//    else
//    {
//        in = "";
//    }
//#endif
//    if(in.contains("RUNNING"))
//    {
//        online = true;
//    }
//    else
//    {
//        online = false;
//    }

//    if(!in.contains("UP"))
//    {
////        out = "ifup " + eth;
////        ifconfig.start(out);
//        enable = false;
//    }
//    else
//    {
//        enable = true;
//    }

//    emit signalStatus(online, enable);

//    if(online)
//    {
//        updateParamEternet();
//    }

}

void cIpController::updateParamEternet()
{
    QProcess ifconfig;
    QString out = "ifconfig " + ethName + " | grep inet";
#ifndef Q_OS_WIN
    ifconfig.start("sh", QStringList() << "-c" << out);
    ifconfig.waitForFinished();
    qDebug() << out.toStdString().c_str();
#else
    qDebug() << out.toStdString().c_str();
#endif
#ifndef Q_OS_WIN
    QString in = QString(ifconfig.readAll());
    ifconfig.close();
#else
    QString in = "inet addr:10.12.13.5  Bcast:10.12.13.255  Mask:255.255.255.0";
#endif
    qDebug() << in.toStdString().c_str();
    QStringList params = in.split(":");
    if(params.size() < 4) return;
    QString addr = params.at(1);
    QString bcast = params.at(2);
    QString mask = params.at(3);
    ipAddr = addr.split(" ").at(0);
    netMask = mask.split(" ").at(0);
    gateWay = bcast.split(" ").at(0);
}
