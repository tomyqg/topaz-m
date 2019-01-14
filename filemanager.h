#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include <QObject>
#include <QList>
#include <QFile>
#include <Channels/channelOptions.h>
#include <ustavka.h>
#include <systemoptions.h>
#include <mainwindow.h>


class cFileManager : public QObject
{
    Q_OBJECT
public:
    explicit cFileManager(QObject *parent = 0);
    static int writeChannelsSettings(QString path/*, QList<ChannelOptions*> listChannels*/);
    static int writeSteelsSettings(QString path);
    static int readChannelsSettings(QString path);
    static int readSteelsSettings(QString path);
    static int writeSystemOptionsToFile(QString path, cSystemOptions * opt);
    static int readSystemOptionsFromFile(QString path, cSystemOptions *opt);
signals:

public slots:
};

#endif // CFILEMANAGER_H
