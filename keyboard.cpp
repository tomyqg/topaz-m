#include "keyboard.h"
#include "ui_keyboard.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "options.h"
#include "defines.h"

#include <QtCore>
#include <QtWidgets>

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);
    connect(ui->buttonGroup, SIGNAL(buttonPressed(int)), this, SLOT(textinput()) );

    ui->textEdit->setText(Options::olderprop);
    ui->textEdit->setFocus(); // чтобы при загрузке сразу активным было окошечко с вводом параметров
    ui->textEdit->installEventFilter(this);

    MessageWrite mr ("Keyboard Open");
    mr.deleteLater();

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
    MessageWrite mr ("Keyboard Close");
    mr.deleteLater();
    delete ui;
}

QString keyboard::getcustomstring()
{
    return ui->textEdit->toPlainText();
}

void keyboard::textinput()
{
    QWidget *widget = QApplication::focusWidget();
    QString textwas = ui->textEdit->toPlainText();
    QPushButton *button = static_cast<QPushButton*>(widget);

    QString textnew = textwas  + button->text();
    ui->textEdit->setText(textnew);
}

void keyboard::on_pushButton_13_clicked()
{
    this->close();
}

void keyboard::on_pushButton_27_clicked()
{
    QString textwas = ui->textEdit->toPlainText();
    QString textnew = textwas.remove(textwas.length()-1,1);
    ui->textEdit->setText(textnew);
}

void keyboard::on_pushButton_44_clicked()
{
    QString textwas = ui->textEdit->toPlainText();
    QString textnew = textwas.append(' ');
    ui->textEdit->setText(textnew);
}

void keyboard::on_comboBox_currentIndexChanged()
{
    ChangeLanguage(ui->comboBox->currentIndex() );
}

void keyboard::ChangeLanguage(char eng)
{
    if (eng == 1) //если русский язык
    {
        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            if (widget->property("text").toString() == "q")
                widget->setProperty("text", "й");

            if (widget->property("text").toString() == "w")
                widget->setProperty("text", "ц");

            if (widget->property("text").toString() == "e")
                widget->setProperty("text", "у");

            if (widget->property("text").toString() == "r")
                widget->setProperty("text", "к");

            if (widget->property("text").toString() == "t")
                widget->setProperty("text", "е");

            if (widget->property("text").toString() == "y")
                widget->setProperty("text", "н");

            if (widget->property("text").toString() == "u")
                widget->setProperty("text", "г");

            if (widget->property("text").toString() == "i")
                widget->setProperty("text", "ш");

            if (widget->property("text").toString() == "o")
                widget->setProperty("text", "щ");

            if (widget->property("text").toString() == "p")
                widget->setProperty("text", "з");

            if (widget->property("text").toString() == "a")
                widget->setProperty("text", "ф");

            if (widget->property("text").toString() == "s")
                widget->setProperty("text", "ы");

            if (widget->property("text").toString() == "d")
                widget->setProperty("text", "в");

            if (widget->property("text").toString() == "f")
                widget->setProperty("text", "а");

            if (widget->property("text").toString() == "g")
                widget->setProperty("text", "п");

            if (widget->property("text").toString() == "h")
                widget->setProperty("text", "р");

            if (widget->property("text").toString() == "j")
                widget->setProperty("text", "о");

            if (widget->property("text").toString() == "k")
                widget->setProperty("text", "л");

            if (widget->property("text").toString() == "l")
                widget->setProperty("text", "д");

            if (widget->property("text").toString() == ";")
                widget->setProperty("text", "ж");

            if (widget->property("text").toString() == "'")
                widget->setProperty("text", "э");

            if (widget->property("text").toString() == "z")
                widget->setProperty("text", "я");

            if (widget->property("text").toString() == "x")
                widget->setProperty("text", "ч");

            if (widget->property("text").toString() == "c")
                widget->setProperty("text", "с");

            if (widget->property("text").toString() == "v")
                widget->setProperty("text", "м");

            if (widget->property("text").toString() == "b")
                widget->setProperty("text", "и");

            if (widget->property("text").toString() == "n")
                widget->setProperty("text", "т");

            if (widget->property("text").toString() == "m")
                widget->setProperty("text", "ь");

            if (widget->property("text").toString() == "<")
                widget->setProperty("text", "б");

            if (widget->property("text").toString() == ">")
                widget->setProperty("text", "ю");
        }
    }

    if (eng == 0) // если английский
    {
        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты

        foreach(QPushButton * widget, widgets)
        {
            if (widget->property("text").toString() == "й")
                widget->setProperty("text", "q");

            if (widget->property("text").toString() == "ц")
                widget->setProperty("text", "w");

            if (widget->property("text").toString() == "у")
                widget->setProperty("text", "e");

            if (widget->property("text").toString() == "к")
                widget->setProperty("text", "r");

            if (widget->property("text").toString() == "е")
                widget->setProperty("text", "t");

            if (widget->property("text").toString() == "н")
                widget->setProperty("text", "y");

            if (widget->property("text").toString() == "г")
                widget->setProperty("text", "u");

            if (widget->property("text").toString() == "ш")
                widget->setProperty("text", "i");

            if (widget->property("text").toString() == "щ")
                widget->setProperty("text", "o");

            if (widget->property("text").toString() == "з")
                widget->setProperty("text", "p");

            if (widget->property("text").toString() == "ф")
                widget->setProperty("text", "a");

            if (widget->property("text").toString() == "ы")
                widget->setProperty("text", "s");

            if (widget->property("text").toString() == "в")
                widget->setProperty("text", "d");

            if (widget->property("text").toString() == "а")
                widget->setProperty("text", "f");

            if (widget->property("text").toString() == "п")
                widget->setProperty("text", "g");

            if (widget->property("text").toString() == "р")
                widget->setProperty("text", "h");

            if (widget->property("text").toString() == "о")
                widget->setProperty("text", "j");

            if (widget->property("text").toString() == "л")
                widget->setProperty("text", "k");

            if (widget->property("text").toString() == "д")
                widget->setProperty("text", "l");

            if (widget->property("text").toString() == "ж")
                widget->setProperty("text", ";");

            if (widget->property("text").toString() == "э")
                widget->setProperty("text", "'");

            if (widget->property("text").toString() == "я")
                widget->setProperty("text", "z");

            if (widget->property("text").toString() == "ч")
                widget->setProperty("text", "x");

            if (widget->property("text").toString() == "с")
                widget->setProperty("text", "c");

            if (widget->property("text").toString() == "м")
                widget->setProperty("text", "v");

            if (widget->property("text").toString() == "и")
                widget->setProperty("text", "b");

            if (widget->property("text").toString() == "т")
                widget->setProperty("text", "n");

            if (widget->property("text").toString() == "ь")
                widget->setProperty("text", "m");

            if (widget->property("text").toString() == "б")
                widget->setProperty("text", "<");

            if (widget->property("text").toString() == "ю")
                widget->setProperty("text", ">");
        }
    }
}

void keyboard::on_pushButton_28_clicked()
{
    QString textwas = ui->textEdit->toPlainText();
    QString textnew = textwas.remove(textwas.length()-1,1);
    ui->textEdit->setText(textnew);
}
