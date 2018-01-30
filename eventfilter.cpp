#include "options.h"
#include "mainwindow.h"
#include "ui_options.h"
#include "Channels/channelOptions.h"
#include "keyboard.h"
#include "defines.h"

bool Options::eventFilter(QObject *object, QEvent *event)
{
    if ( (event->type() == event->MouseButtonRelease)&&(object->property("enabled").toString() == "true") )
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
    return QObject::eventFilter(object, event);
}

bool keyboard::eventFilter(QObject *object, QEvent *event)
{
    QKeyEvent* key = static_cast<QKeyEvent*>(event); // what key pressed
    if  (key->key()==Qt::Key_Enter) // if key == enter, then close
    {
        this->close();
    }

    if ( (event->type() == QEvent::MouseButtonPress)&& ( QString::fromLatin1(object->metaObject()->className()) == "QPushButton" ))//)inherits("QPushButton")) // ("QWidgetWindow"))
    {

        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит

            if (widget->objectName() == object->property("objectName"))
            {
                widget->setStyleSheet(stylesheetclicked);
            }
        }
    }

    if ( (event->type() == QEvent::MouseButtonRelease)&& ( QString::fromLatin1(object->metaObject()->className()) == "QPushButton" ))//)inherits("QPushButton")) // ("QWidgetWindow"))
    {

        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит

            if (widget->objectName() == object->property("objectName"))
            {
                widget->setStyleSheet(stylesheetUnclicked);
            }
        }
    }

    return QObject::eventFilter(object, event);
}

void keyboard::setolderproperty(QString str)
{
    this->olderproperty = str;
}
