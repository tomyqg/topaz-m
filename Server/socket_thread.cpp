#include "socket_thread.h"

cSocketThread::cSocketThread(int descriptor, QObject *parent) :
    QThread(parent), socketDescriptor(descriptor)
{

}

cSocketThread::~cSocketThread()
{
    delete socket;
}

void cSocketThread::run()
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()), Qt::DirectConnection);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);

    sleep(1);

    exec();
}

void cSocketThread::onReadyRead()
{
    //qDebug() << socket->readAll();
    qDebug() << "onReadyRead";

    QString response = "HTTP/1.1 200 OK\r\n\r\n%1";
    socket->write(response.arg(QDateTime::currentDateTime().toString()).toLocal8Bit());
    socket->disconnectFromHost();
}

void cSocketThread::onDisconnected()
{
    socket->close();
    quit();
}

void cSocketThread::connected()
{
    qDebug() << "Client connected...";
}
