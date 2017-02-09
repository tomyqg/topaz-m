#include "keyboard.h"
#include "ui_keyboard.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtWidgets>

keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);

    connect(ui->buttonGroup, SIGNAL(buttonPressed(int)), this, SLOT(textinput()) );
}

keyboard::~keyboard()
{
    delete ui;
}

void keyboard::on_buttonBox_accepted()
{



//    qDebug() << "acc";


    //    QTextEdit *textBox = MainWindow->findChild<QTextEdit *>("textBox");

    //    QList<QLabel*> spinList = keyboard::findChildren<QLabel*> (  );

    //    // apply style to all widgets
    //    for (int i = 0; i < spinList.count(); ++i) {
    //        QLabel *sb = spinList.at(i);
    //        sb->setText("alloha");
    //    }
}

QString keyboard::getcustomstring()
{
    return ui->textEdit->toPlainText();
//     ui->textEdit->toPlainText();
}

void keyboard::textinput()
{

    QWidget *widget = QApplication::focusWidget();

    QString textwas = ui->textEdit->toPlainText();

    QPushButton *button = static_cast<QPushButton*>(widget);

    QString textnew = textwas  + button->text();

    ui->textEdit->setText(textnew);



    //    qDebug() << button->text();

    //    if( strcmp( widget->metaObject()->className(), "QPushButton") == 0 ) {
    //        qDebug() << "this comes from a QPushButton, let me print the text on the button";
    //        QPushButton *button = static_cast<QPushButton*>(widget);
    //        qDebug() << button->text();
    //    }
}

void keyboard::on_pushButton_13_clicked()
{
    this->close();
}
