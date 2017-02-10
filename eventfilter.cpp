#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QEvent>

#include "options.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"


bool Options::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == event->MouseButtonRelease) // && object == ui->NignPredelChannel_1
    {

        keyboard kb;
        kb.setModal(true);
        kb.exec();
        object->setProperty("value", kb.getcustomstring() );
        object->setProperty("text",kb.getcustomstring() );

        qDebug() << object;
        qDebug() << object->objectName();

        kb.close();
        ui->pushButton->setFocus();
        kb.close();
        event->accept();
        kb.close();
        ui->pushButton->setFocus();
        event->accept();
        kb.close();
        event->accept();
        ui->pushButton->setFocus();
        event->accept();
        //        return true;
    }

    return QObject::eventFilter(object, event);
}
