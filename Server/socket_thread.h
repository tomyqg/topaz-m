#ifndef CSOCKETTHREAD_H
#define CSOCKETTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDateTime>

class cSocketThread : public QThread
{
    Q_OBJECT
public:
    explicit cSocketThread(int descriptor, QObject *parent = 0);

    ~cSocketThread();
    void run();

public slots:
    void onReadyRead();
    void onDisconnected();
    void connected();

private:
    int socketDescriptor;
    QTcpSocket * socket;

};

#endif // CSOCKETTHREAD_H
