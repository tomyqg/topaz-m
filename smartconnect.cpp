#include <QtCore>
#include "smartconnect.h"

SmartConnect::SmartConnect(QObject *sender, const char *signal, void (*pFunc)()){
  pVoidFunc = pFunc;
  QObject::connect(sender,signal,this,SLOT(voidSlot()));
}

SmartConnect::SmartConnect(QObject *sender, const char *signal, void (*pFunc)(QString)){
  pQStringFunc = pFunc;
  QObject::connect(sender,signal,this,SLOT(QStringSlot(QString)));
}

void SmartConnect::voidSlot(){
  pVoidFunc();
}

void SmartConnect::QStringSlot(QString str){
  pQStringFunc(str);
}
