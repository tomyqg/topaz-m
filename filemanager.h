#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include <QObject>
#include <QList>
#include <QFile>
#include <Channels/channelOptions.h>
#include <ustavka.h>


class cFileManager : public QObject
{
    Q_OBJECT
public:
    explicit cFileManager(QObject *parent = 0);
    static int writeChannelsSettings(QString path, QList<ChannelOptions*> listChannels, QList<Ustavka*> listUstavok);
    static int readChannelsSettings(QString path, QList<ChannelOptions*> listChannels, QList<Ustavka *> listUstavok);
signals:

public slots:
};

#endif // CFILEMANAGER_H
