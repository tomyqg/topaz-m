#ifndef CEXPERTACCESS_H
#define CEXPERTACCESS_H

#include <QObject>

enum accessModeType
{
    ACCESS_USER = 0,
    ACCESS_EXPERT = 1,
    ACCESS_ADMIN = 2
};

class cExpertAccess : public QObject
{
    Q_OBJECT
public:
    explicit cExpertAccess(QObject *parent = 0);
    static bool accessRequest(QString pass);
    static void resetAccess();
    static accessModeType getMode() { return mode; }
signals:

public slots:

private:
    QString searchConfigFile();
    static QString passExpert;
    static QString passAdmin;
    static QString configFileName;
    static accessModeType mode;

};

#endif // CEXPERTACCESS_H
