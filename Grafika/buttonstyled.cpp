#include "buttonstyled.h"
#include "ui_buttonstyled.h"

#define AlignLeftStyle "text-align: left;\n padding-left: 40px;\n"
#define AlignRightStyle "text-align: right;\n padding-right: 40px;\n"
#define AlignCenterStyle "text-align: center;\n padding-left: 20px;\n padding-right: 2  0px;\n"
#define BorderStyle "border: 0px solid rgba(255, 255, 255, 0);\n"

wButtonStyled::wButtonStyled(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wButtonStyled)
{
    ui->setupUi(this);
    strColorText = "color: rgb(255, 255, 255);\n";
    strColorBg = "background-color: #2c3d4d;\n";
    strAlign = AlignCenterStyle;
}

wButtonStyled::~wButtonStyled()
{
    delete ui;
}

void wButtonStyled::on_button_clicked(bool checked)
{

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
    int a = color.alpha();
    strColorText = "color: rgba(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ", " + \
            QString::number(a) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::setColorBg(QColor color)
{
    colorBg = color;
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    int a = color.alpha();
    colorBgDown = QColor(r - 20, g - 20, b - 20, a);

    strColorBg = "background-color: rgba(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ", " + \
            QString::number(a) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::on_button_pressed()
{

    int r = colorBgDown.red() - 20;
    int g = colorBgDown.green() - 20;
    int b = colorBgDown.blue() - 20;
    int a = colorBgDown.alpha();
    strColorBg = "background-color: rgba(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ", " + \
            QString::number(a) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

void wButtonStyled::on_button_released()
{
    int r = colorBg.red();
    int g = colorBg.green();
    int b = colorBg.blue();
    int a = colorBg.alpha();
    strColorBg = "background-color: rgba(" + \
            QString::number(r) + ", " + \
            QString::number(g) + ", " + \
            QString::number(b) + ", " + \
            QString::number(a) + ");\n";
    QString stringStyleSheet = strColorText + strColorBg + BorderStyle + strAlign;
    ui->button->setStyleSheet(stringStyleSheet);
}

//bool wButtonStyled::eventFilter(QObject *watched, QEvent *event)
//{
//    if(watched == ui->button && event->type() == QEvent::HoverMove)
//    {
//        int r = colorBg.red() - 10;
//        int g = colorBg.green() - 10;
//        int b = colorBg.blue() - 10;
//        int a = colorBg.alpha();
//        strColorBg = "background-color: rgba(" + \
//                QString::number(r) + ", " + \
//                QString::number(g) + ", " + \
//                QString::number(b) + ", " + \
//                QString::number(a) + ");\n";
//        QString stringStyleSheet = strColorText + strColorBg + "border: 0px solid rgba(255, 255, 255, 0);";
//        ui->button->setStyleSheet(stringStyleSheet);
//    } else if(watched == ui->button && event->type() == QEvent::HoverLeave)
//    {
//        int r = colorBg.red();
//        int g = colorBg.green();
//        int b = colorBg.blue();
//        int a = colorBg.alpha();
//        strColorBg = "background-color: rgba(" + \
//                QString::number(r) + ", " + \
//                QString::number(g) + ", " + \
//                QString::number(b) + ", " + \
//                QString::number(a) + ");\n";
//        QString stringStyleSheet = strColorText + strColorBg + "border: 0px solid rgba(255, 255, 255, 0);";
//        ui->button->setStyleSheet(stringStyleSheet);
//    }
//    return wButtonStyled::eventFilter(watched, event);
//}

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
