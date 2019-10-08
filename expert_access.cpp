#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QByteArray>
#include "expert_access.h"
#include "log.h"
#include "defines.h"

QString cExpertAccess::configFileName = "";

QString cExpertAccess::passExpert = "0000"; //Пароль экперта. "0000" - по-умолчанию
QString cExpertAccess::passAdmin = "abrakadabra"; // Пароль доступа админа
accessModeType cExpertAccess::mode = Access_User;

QString cExpertAccess::passExtExpert = "0000"; //Пароль экперта. "0000" - по-умолчанию
QString cExpertAccess::passExtAdmin = "abrakadabra"; // Пароль доступа админа
accessModeType cExpertAccess::modeModbusExt = Access_User;

cExpertAccess::cExpertAccess(QObject *parent) : QObject(parent)
{

    QByteArray encryptedPwd;// = QCryptographicHash::hash(passExpert.toUtf8(), QCryptographicHash::Sha1).toHex();
    configFileName = searchConfigFile();
    QSettings* cryptoHash = new QSettings(configFileName, QSettings::IniFormat);
    //    cryptoHash->setValue("HashExpert", encryptedPwd);
    encryptedPwd = cryptoHash->value("HashExpert", QByteArray("0000")).toByteArray();
}

bool cExpertAccess::accessRequest(QString pass)
{
    if(pass == passExpert)
    {
        cLogger log(pathtomessages, cLogger::UI);
        log.addMess("Login Expert", cLogger::SERVICE);
        mode = Access_Expert;
        return true;
    }
    else if(pass == passAdmin)
    {
        cLogger log(pathtomessages, cLogger::UI);
        log.addMess("Login Admin", cLogger::SERVICE);
        mode = Access_Admin;
        return true;
    }
    else
    {
        cLogger log(pathtomessages, cLogger::UI);
        log.addMess("Login error pass", cLogger::SERVICE);
        mode = Access_User;
        return false;
    }
}

void cExpertAccess::resetAccess()
{
    mode = Access_User;
}

accessModeType cExpertAccess::accessExtRequest(QString pass)
{
    if(pass == passExtExpert)
    {
        cLogger log(pathtomessages, cLogger::UI);
        log.addMess("Login external Expert", cLogger::SERVICE);
        modeModbusExt = Access_Expert;
    }
    else if(pass == passExtAdmin)
    {
        cLogger log(pathtomessages, cLogger::UI);
        log.addMess("Login external Admin", cLogger::SERVICE);
        modeModbusExt = Access_Admin;
    }
    else
    {
        cLogger log(pathtomessages, cLogger::UI);
        log.addMess("External Login Error", cLogger::SERVICE);
        modeModbusExt = Access_User;
    }
    return modeModbusExt;
}

void cExpertAccess::resetExtAccess()
{
    modeModbusExt = Access_User;
}


/** Search the configuration file */
QString cExpertAccess::searchConfigFile()
{
    QString fileName("Multigraph.ini");

    QStringList searchList;
    searchList.append("C:/Work");
    searchList.append("/opt");
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return 0;
}
