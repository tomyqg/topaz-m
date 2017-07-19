#include "options.h"
#include "mainwindow.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"

bool Options::eventFilter(QObject *object, QEvent *event)
{
    if ( (event->type() == event->MouseButtonRelease)&&(object->property("enabled").toString() == "true") )
    {
        Options::olderprop = object->property("text").toString();
        keyboard kb;
        // подстраиваем размер окна
        resizeWindow(kb, 1280*0.8*1,800*0.96*1);
        kb.setMaximumHeight(370*0.96*1);
        kb.setMinimumHeight(370*0.96*1);
        kb.setMaximumWidth(760*0.8*1);
        kb.setMinimumWidth(760*0.8*1);
        kb.resize(760*0.8*1,370*0.96*1);
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

void keyboard::setolderproperty(QString str)
{
    this->olderproperty = str;
}


