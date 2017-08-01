#ifndef WORKER_H
#define WORKER_H

#include "uartdriver.h"


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
  void StopWork();
  void StartWork();

private slots:
  void do_Work();

private:
  bool isrunning,isstopped;
};

#endif // WORKER_H
