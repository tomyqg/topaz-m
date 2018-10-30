#include <QtCore>

class SmartConnect : public QObject
{
  Q_OBJECT
  void (*pVoidFunc)(void);
  void (*pQStringFunc)(QString);
public:
  SmartConnect(QObject* sender,const char* signal,void (*pFunc)(void));
  SmartConnect(QObject* sender,const char* signal,void (*pFunc)(QString));
private slots:
  void voidSlot();
  void QStringSlot(QString str);
};
