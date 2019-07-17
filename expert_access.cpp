#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QByteArray>
#include "expert_access.h"
//#include "keyboard.h"

//Пароль экперта. "0000" - по-умолчанию
QString cExpertAccess::passExpert = "0000";
// Пароль доступа админа
QString cExpertAccess::passAdmin = "abrakadabra";
QString cExpertAccess::configFileName = "";
accessModeType cExpertAccess::mode = ACCESS_USER;
accessModeType cExpertAccess::modeModbusExt = ACCESS_USER;

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
        mode = ACCESS_EXPERT;
        return true;
    }
    else if(pass == passAdmin)
    {
        mode = ACCESS_ADMIN;
        return true;
    }
    else
    {
        mode = ACCESS_USER;
        return false;
    }
}

void cExpertAccess::resetAccess()
{
    mode = ACCESS_USER;
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
