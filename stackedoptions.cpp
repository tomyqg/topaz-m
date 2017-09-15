#include "stackedoptions.h"
#include "ui_stackedoptions.h"

StackedOptions::StackedOptions(QWidget *parent) :
    QWidget(parent),
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
