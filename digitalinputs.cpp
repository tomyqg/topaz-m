#include "digitalinputs.h"
#include "ui_digitalinputs.h"

DigitalInputs::DigitalInputs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DigitalInputs)
{
    ui->setupUi(this);
}

DigitalInputs::~DigitalInputs()
{
    delete ui;
}
