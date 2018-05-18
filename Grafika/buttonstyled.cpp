#include "buttonstyled.h"
#include "ui_buttonstyled.h"

wButtonStyled::wButtonStyled(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wButtonStyled)
{
    ui->setupUi(this);

}

wButtonStyled::~wButtonStyled()
{
    delete ui;
}

void wButtonStyled::on_button_clicked(bool checked)
{

}
