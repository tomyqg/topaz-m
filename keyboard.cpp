#include "keyboard.h"
#include "ui_keyboard.h"

#include <QtWidgets>

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);
}

keyboard::~keyboard()
{
    delete ui;
}

void keyboard::on_buttonBox_accepted()
{

    qDebug() << "acc";

    QList<QLabel*> spinList = QObject::findChildren<QLabel*> (  );

    // apply style to all widgets
    for (int i = 0; i < spinList.count(); ++i) {
        QLabel *sb = spinList.at(i);
        sb->setText("alloha");
    }
}
