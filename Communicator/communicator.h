#ifndef CCOMMUNICATOR_H
#define CCOMMUNICATOR_H

#include <QObject>

#define TIMEOUT_COMMUNICATOR_MS 10

class cCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit cCommunicator(QObject *parent = 0);

public slots:
    void run();

private:
    int init();

};

#endif // CCOMMUNICATOR_H
