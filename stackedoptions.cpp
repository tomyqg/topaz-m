#include "stackedoptions.h"
#include "ui_stackedoptions.h"

#define ChangeDateTimeIndex 5
#define MainMenuIndex 0

StackedOptions::StackedOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StackedOptions)
{

    ui->setupUi(this);
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(SetLabelIndex(int)) );
}

StackedOptions::~StackedOptions()
{
    delete ui;
}

void StackedOptions::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StackedOptions::SetLabelIndex(int newindex)
{
    ui->indexLabel->setText("Index " + QString::number(newindex));
}

void StackedOptions::SetStackIndex(int newstackindex)
{
    ui->stackedWidget->setCurrentIndex(newstackindex);
}


void StackedOptions::on_pushButton_10_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_clicked()
{
    SetStackIndex(2);
}

void StackedOptions::on_pushButton_2_clicked()
{
    SetStackIndex(1);
}

void StackedOptions::on_pushButton_12_clicked()
{
      SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_11_clicked()
{
    SetStackIndex(0);
}

void StackedOptions::on_pushButton_6_clicked()
{
    SetStackIndex(3);
}

void StackedOptions::on_pushButton_7_clicked()
{
    SetStackIndex(4);
}

void StackedOptions::on_pushButton_8_clicked()
{
    this->close();
}

void StackedOptions::on_pushButton_13_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_ChangeDateButton_clicked()
{
    SetStackIndex(ChangeDateTimeIndex);
}

void StackedOptions::on_pushButton_15_clicked()
{
     SetStackIndex(MainMenuIndex);
}
