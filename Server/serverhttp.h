#ifndef CSERVERHTTP_H
#define CSERVERHTTP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <socket_thread.h>


class cServerHttp : public QTcpServer
{
    Q_OBJECT
public:
    explicit cServerHttp(QObject *parent = 0);
    void incomingConnection(qintptr handle);

public slots:
    void onReadyRead();
    void onDisconnected();
};

#endif // CSERVERHTTP_H
