#include "keyboard.h"
#include "ui_keyboard.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "options.h"

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
}

keyboard::~keyboard()
{
    MessageWrite mr ("Keyboard Close");
    delete ui;
}

/*void keyboard::on_buttonBox_accepted()
{
}*/

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

void keyboard::on_comboBox_currentIndexChanged(int index)
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
            qDebug() << widget;


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
    {}
}
