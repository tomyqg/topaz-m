#include "stackedoptions.h"
#include "ui_stackedoptions.h"

StackedOptions::StackedOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StackedOptions)
{
    ui->setupUi(this);
}

StackedOptions::~StackedOptions()
{
    delete ui;
}

void StackedOptions::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StackedOptions::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StackedOptions::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void StackedOptions::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StackedOptions::on_pushButton_12_clicked()
{
      ui->stackedWidget->setCurrentIndex(0);
}

void StackedOptions::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StackedOptions::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void StackedOptions::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
