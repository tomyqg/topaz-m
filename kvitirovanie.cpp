#include "kvitirovanie.h"
#include "ui_kvitirovanie.h"
#include <QtWidgets>

kvitirovanie::kvitirovanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kvitirovanie)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    }

kvitirovanie::~kvitirovanie()
{
    delete ui;
}

void kvitirovanie::showInfo(QString mess, QString title)
{
    ui->messUstavka->setText(mess);
    ui->title->setText(title);
}
