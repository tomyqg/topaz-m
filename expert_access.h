#ifndef CEXPERTACCESS_H
#define CEXPERTACCESS_H

#include <QObject>

enum accessModeType
{
    Access_User = 0,
    Access_Expert = 1,
    Access_Admin = 2
};

class cExpertAccess : public QObject
{
    Q_OBJECT
public:
    explicit cExpertAccess(QObject *parent = 0);
    static bool accessRequest(QString pass);
    static void resetAccess();
    static accessModeType getMode() { return mode; }
    static void resetExtAccess();
    static accessModeType accessExtRequest(QString pass);
    static accessModeType getExtMode() { return modeModbusExt; }
signals:

public slots:

private:
    QString searchConfigFile();
    static QString passExpert;
    static QString passAdmin;
    static QString configFileName;
    static accessModeType mode;
    static QString passExtExpert;
    static QString passExtAdmin;
    static accessModeType modeModbusExt;

};

#endif // CEXPERTACCESS_H
