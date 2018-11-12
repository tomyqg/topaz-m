//#include "options.h"
#include "mainwindow.h"
//#include "ui_options.h"
#include "Channels/channelOptions.h"
#include "keyboard.h"
#include "defines.h"

bool Options::eventFilter(QObject *object, QEvent *event)
{
#ifndef Q_OS_WIN
    if ( (event->type() == QEvent::MouseButtonRelease)&&(object->property("enabled").toString() == "true") )
    {
        Options::olderprop = object->property("text").toString();
        keyboard kb;
        kb.setModal(true);
        kb.exec();
        object->setProperty("value", kb.getcustomstring() );
        object->setProperty("text",kb.getcustomstring() );
        ui->pushButton->setFocus();
        kb.close();
        kb.deleteLater();
    }
#endif

    return QObject::eventFilter(object, event);
}

