#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QFile>

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
    if(event->type() == event->MouseButtonRelease)
    {
        keyboard kb;
        kb.setModal(true);
        kb.exec();
        object->setProperty("value", kb.getcustomstring() );
        object->setProperty("text",kb.getcustomstring() );
        ui->pushButton->setFocus();
        kb.close();
    }
    return QObject::eventFilter(object, event);
}

bool keyboard::eventFilter(QObject *object, QEvent *event)
{
    QKeyEvent* key = static_cast<QKeyEvent*>(event); // what key pressed

    if  (key->key()==Qt::Key_Enter) // if key == enter, then close
    {
        this->close();
    }
    return QObject::eventFilter(object, event);
}
