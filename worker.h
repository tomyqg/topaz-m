#ifndef WORKER_H
#define WORKER_H

#include "uartdriver.h"
#include "channel1.h"

class worker : public QObject
{
  Q_OBJECT
public:
  explicit worker(QObject *parent = 0);


signals:
  void SignalToObj_mainThreadGUI();
  void running();
  void stopped();

public slots:
  void StopWorkSlot();
  void StartWorkSlot();
  void GetObectsSlot(ChannelOptions* c1,ChannelOptions* c2,ChannelOptions* c3 ,ChannelOptions* c4);

private slots:
  void do_Work();

private:
  bool isrunning,isstopped;
};

#endif // WORKER_H
