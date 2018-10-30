#include "buttonstyled.h"
#include "ui_buttonstyled.h"
#include "defines.h"

#define AlignLeftStyle "text-align: left;\n padding-left: 40px;\n"
#define AlignRightStyle "text-align: right;\n padding-right: 40px;\n"
#define AlignCenterStyle "text-align: center;\n"
#define BorderStyle "border: 0px solid;\n"

wButtonStyled::wButtonStyled(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wButtonStyled)
{
    ui->setupUi(this);
    strColorText = "color: rgb(255, 255, 255);\n";
    strColorBg = "background-color:rgb(" \
            + QString::number(ColorButtonNormal.red()) + "," \
            + QString::number(ColorButtonNormal.green()) + "," \
            + QString::number(ColorButtonNormal.blue()) + ");\n";
    strAlign = AlignCenterStyle;
}

wButtonStyled::~wButtonStyled()
{
    delete ui;
}


void wButtonStyled::setText(QString text)
{
    ui->button->setText(text);
}


void wButtonStyled::resizeEvent(QResizeEvent * s)
{
    ui->button->setGeometry(0, 0, ui->frame->width() - 10, ui->frame->height() - 10);
    ui->shadow->setGeometry(7, 7, ui->frame->width() - 10, ui->frame->height() - 10);
}


void wButtonStyled::setColorText(QColor color)
{
    colorText = color;
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    strColorText = "color: rgb(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::setColorBg(QColor color)
{
    colorBg = color;
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    colorBgDown = QColor(r - 20, g - 20, b - 20);

    strColorBg = "background-color: rgb(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::on_button_pressed()
{

    int r = colorBgDown.red() - 20;
    int g = colorBgDown.green() - 20;
    int b = colorBgDown.blue() - 20;

    strColorBg = "background-color: rgb(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
    ui->shadow->setGeometry(3, 3, ui->shadow->width(), ui->shadow->height());
}

void wButtonStyled::on_button_released()
{
    int r = colorBg.red();
    int g = colorBg.green();
    int b = colorBg.blue();
    strColorBg = "background-color: rgb(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
    ui->shadow->setGeometry(7, 7, ui->shadow->width(), ui->shadow->height());
}

void wButtonStyled::setAlignLeft()
{
    strAlign = AlignLeftStyle;
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::setAlignCenter()
{
    strAlign = AlignCenterStyle;
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::setAlignRight()
{
    strAlign = AlignRightStyle;
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::on_button_clicked()
{
    emit clicked(index);
}

void wButtonStyled::setFontSize(int s)
{
    ui->button->setFont(QFont(Font,s));
}
