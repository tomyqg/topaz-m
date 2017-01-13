#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);

    setWindowTitle(tr("OPTIONS"));

}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
    this->close();
}

void Options::on_pushButton_2_clicked()
{
    this->close();
}


void Options::checkboxchange(QString newstring)
{
    ui->lineEdit_5->setText(newstring);
}

void Options::on_radioButton_2_clicked()
{
    checkboxchange("В");
}
