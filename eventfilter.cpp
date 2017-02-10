
#include "options.h"
#include "ui_options.h"
#include "channel1.h"
#include "keyboard.h"


bool Options::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == event->MouseButtonPress) // && object == ui->NignPredelChannel_1
    {

        keyboard kb;
        kb.setModal(true);
        kb.exec();
        object->setProperty("value",kb.getcustomstring());
        object->setProperty("text",kb.getcustomstring());
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
