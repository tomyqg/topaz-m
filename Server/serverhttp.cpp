#include "serverhttp.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include "httplistener.h"
#include "templatecache.h"
#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "src/requestmapper.h"


cServerHttp::cServerHttp(QObject *parent) :
    QTcpServer(parent)
{
    if(listen(QHostAddress::Any, 80))
    {
        qDebug() << "Listening...";
    }
    else
    {
        qDebug() << "Error while starting ..." << errorString();
    }
}

void cServerHttp::incomingConnection(qintptr handle)
{
    qDebug() << "incomingConnection...";
    cSocketThread* thread = new cSocketThread(handle);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void cServerHttp::onReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << socket->readAll();

    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";
    socket->write(response.arg(QDateTime::currentDateTime().toString()).toLocal8Bit());
    socket->disconnectFromHost();
}

void cServerHttp::onDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    socket->close();
    socket->deleteLater();
}
