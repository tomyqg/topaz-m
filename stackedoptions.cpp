#include "stackedoptions.h"
#include "ui_stackedoptions.h"

#define MainMenuIndex 0
#define OptionsIndex 1
#define WorkIndex 2
#define DiagnisticIndex 3
#define ExpertIndex 4
#define ChangeDateTimeIndex 5
#define ExtendedOptionsIndex 6
#define SystemIndex 7
#define InputsIndex 8
#define UniversalInputsIndex 9
#define Channel1Index 10
#define Channel2Index 11
#define Channel3Index 12
#define Channel4Index 13

StackedOptions::StackedOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StackedOptions)
{

    ui->setupUi(this);
    connect(ui->stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(SetLabelIndex(int)) );
        SetStackIndex(MainMenuIndex);

}

StackedOptions::~StackedOptions()
{
    delete ui;
}

void StackedOptions::on_pushButton_9_clicked()
{

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
    SetStackIndex(WorkIndex);
}

void StackedOptions::on_pushButton_2_clicked()
{
    SetStackIndex(OptionsIndex);
}

void StackedOptions::on_pushButton_12_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_11_clicked()
{
    SetStackIndex(MainMenuIndex);
}

void StackedOptions::on_pushButton_6_clicked()
{
    SetStackIndex(DiagnisticIndex);
}

void StackedOptions::on_pushButton_7_clicked()
{
    SetStackIndex(ExpertIndex);
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

void StackedOptions::on_ExtendedOptionsButton_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

void StackedOptions::on_pushButton_16_clicked()
{
    SetStackIndex(OptionsIndex);
}

void StackedOptions::on_ChangeDateButton_2_clicked()
{
    SetStackIndex(ChangeDateTimeIndex);
}

void StackedOptions::on_pushButton_3_clicked()
{

    SetStackIndex(SystemIndex);
}

void StackedOptions::on_pushButton_4_clicked()
{
    SetStackIndex(InputsIndex);
}

void StackedOptions::on_pushButton_20_clicked()
{
    SetStackIndex(ExtendedOptionsIndex);
}

void StackedOptions::on_pushButton_23_clicked()
{
    SetStackIndex(InputsIndex);
}

void StackedOptions::on_pushButton_21_clicked()
{
    SetStackIndex(Channel1Index);
}

void StackedOptions::on_pushButton_26_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_5_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_22_clicked()
{
    SetStackIndex(Channel2Index);
}

void StackedOptions::on_pushButton_24_clicked()
{
    SetStackIndex(Channel3Index);
}

void StackedOptions::on_pushButton_25_clicked()
{
    SetStackIndex(Channel4Index);
}

void StackedOptions::on_pushButton_27_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_28_clicked()
{
    SetStackIndex(UniversalInputsIndex);
}

void StackedOptions::on_pushButton_29_clicked()
{
      SetStackIndex(UniversalInputsIndex);
}
