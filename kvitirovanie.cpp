#include "kvitirovanie.h"
#include "ui_kvitirovanie.h"
#include <QtCore>
#include <QtWidgets>

kvitirovanie::kvitirovanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kvitirovanie)
{
    ui->setupUi(this);
}

kvitirovanie::~kvitirovanie()
{
    delete ui;
}

void kvitirovanie::showInfo(QString mess)
{
    ui->messUstavka->setText(mess);
}
