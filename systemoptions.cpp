#include <QList>
#include "systemoptions.h"


cSystemOptions::cSystemOptions(QObject *parent) : QObject(parent)
{
    listBauds << 9600 << 19200 << 38400 << 57600 << 115200;
}

