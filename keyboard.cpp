#include "keyboard.h"
#include "ui_keyboard.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "options.h"
#include "defines.h"

#include <QtCore>
#include <QtWidgets>
#include <QDebug>

langSimbols keyboard::simbols [] = {
    { "q", "Q", "й", "Й" },     //0
    { "w", "W", "ц", "Ц" },     //1
    { "e", "E", "у", "У" },     //2
    { "r", "R", "к", "К" },     //3
    { "t", "T", "е", "Е" },     //4
    { "y", "Y", "н", "Н" },     //5
    { "u", "U", "г", "Г" },     //6
    { "i", "I", "ш", "Ш" },     //7
    { "o", "O", "щ", "Щ" },     //8
    { "p", "P", "з", "З" },     //9
    { "[", "{", "х", "Х" },     //10
    { "]", "}", "ъ", "Ъ" },     //11
    { "a", "A", "ф", "Ф" },     //12
    { "s", "S", "ы", "Ы" },     //13
    { "d", "D", "в", "В" },     //14
    { "f", "F", "а", "А" },     //15
    { "g", "G", "п", "П" },     //16
    { "h", "H", "р", "Р" },     //17
    { "j", "J", "о", "О" },     //18
    { "k", "K", "л", "Л" },     //19
    { "l", "L", "д", "Д" },     //20
    { ";", ";", "ж", "Ж" },     //21
    { "'", "'", "э", "Э" },     //22
    { "z", "Z", "я", "Я" },     //23
    { "x", "X", "ч", "Ч" },     //24
    { "c", "C", "с", "С" },     //25
    { "v", "V", "м", "М" },     //26
    { "b", "B", "и", "И" },     //27
    { "n", "N", "т", "Т" },     //28
    { "m", "M", "ь", "Ь" },     //29
    { "<", "<", "б", "Б" },     //30
    { "<", ">", "ю", "Ю" }      //31
};

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    shift = false;
    ui->setupUi(this);
    connect(ui->buttonGroup, SIGNAL(buttonPressed(int)), this, SLOT(textinput(int)) );

    ui->textEdit->setText(Options::olderprop);
    ui->textEdit->setFocus(); // чтобы при загрузке сразу активным было окошечко с вводом параметров
    ui->textEdit->installEventFilter(this);

//    MessageWrite mr ("Keyboard Open");
//    mr.deleteLater();
    cLogger mk(pathtomessages);
    mk.addMess("Keyboard Open");
    mk.deleteLater();

    QList<QPushButton*> ButtonList = keyboard::findChildren<QPushButton*> ();
    // добавляем все кнопошки в евентфильтр
    for (int i = 0; i < ButtonList.count(); ++i) {
        QPushButton *but = ButtonList.at(i);
        but->installEventFilter(this);
        // ставим везде стайлшит

        but->setStyleSheet(stylesheetUnclicked);
    }

}

keyboard::~keyboard()
{
//    MessageWrite mr ("Keyboard Close");
//    mr.deleteLater();
    cLogger mk(pathtomessages);
    mk.addMess("Keyboard Close");
    mk.deleteLater();
    delete ui;
}

QString keyboard::getcustomstring()
{
    return ui->textEdit->toPlainText();
}

void keyboard::textinput(int b)
{
    QWidget *widget = QApplication::focusWidget();
    QString textwas = ui->textEdit->toPlainText();
    QTextCursor cursor = ui->textEdit->textCursor();
    int pos = cursor.position();
//    qDebug() << "cursor" << pos;
    QPushButton *button = static_cast<QPushButton*>(widget);
    QString textnew = textwas.insert(pos, button->text());
    ui->textEdit->setText(textnew);
    cursor.setPosition(pos+1);
    ui->textEdit->setTextCursor(cursor);
}

void keyboard::on_pushButton_13_clicked()
{
    this->close();
}

void keyboard::on_pushButton_27_clicked()
{

}

void keyboard::on_pushButton_27_clicked(bool checked)
{
    shift = checked;
    if(shift)
        ui->pushButton_27->setStyleSheet(stylesheetclicked);
    else
        ui->pushButton_27->setStyleSheet(stylesheetUnclicked);

    QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QPushButton * widget, widgets)
    {
        for(int i = 0; i < (sizeof(simbols) / sizeof(langSimbols)); i++)
        {
            QString en = simbols[i].eng;
            QString EN = simbols[i].ENG;
            QString ru = simbols[i].rus;
            QString RU = simbols[i].RUS;

            if(shift)
            {
                if (widget->property("text").toString() == en)
                    widget->setProperty("text", EN);
                if (widget->property("text").toString() == ru)
                    widget->setProperty("text", RU);
            }
            else
            {
                if (widget->property("text").toString() == EN)
                    widget->setProperty("text", en);
                if (widget->property("text").toString() == RU)
                    widget->setProperty("text", ru);
            }
        }
    }
}


void keyboard::on_pushButton_28_clicked()
{
    QString textwas = ui->textEdit->toPlainText();
    QTextCursor cursor = ui->textEdit->textCursor();
    int pos = cursor.position();
    if(pos>0)
    {
        QString textnew = textwas.remove(pos-1,1);
        ui->textEdit->setText(textnew);
        cursor.setPosition(pos-1);
        ui->textEdit->setTextCursor(cursor);
    }
}


void keyboard::on_pushButton_44_clicked()
{
    QString textwas = ui->textEdit->toPlainText();
    QTextCursor cursor = ui->textEdit->textCursor();
    int pos = cursor.position();
    QString textnew = textwas.insert(pos, ' ');
    ui->textEdit->setText(textnew);
    cursor.setPosition(pos+1);
    ui->textEdit->setTextCursor(cursor);
}

void keyboard::on_comboBox_currentIndexChanged()
{
    ChangeLanguage(ui->comboBox->currentIndex() );
}

void keyboard::ChangeLanguage(int eng)
{
    QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и меняем язык

    foreach(QPushButton * widget, widgets)
    {
        for(int i = 0; i < (sizeof(simbols) / sizeof(langSimbols)); i++)
        {
            QString en = simbols[i].eng;
            QString EN = simbols[i].ENG;
            QString ru = simbols[i].rus;
            QString RU = simbols[i].RUS;
            if(eng == 1) // если русский язык
            {
                if (widget->property("text").toString() == en || \
                        widget->property("text").toString() == EN)
                {
                    if(shift)
                        widget->setProperty("text", RU);
                    else
                        widget->setProperty("text", ru);
                }
            }
            else if(eng == 0)   // если англ. язык
            {
                if (widget->property("text").toString() == ru || \
                        widget->property("text").toString() == RU)
                {
                    if(shift)
                        widget->setProperty("text", EN);
                    else
                        widget->setProperty("text", en);
                }
            }

        }
    }
}



