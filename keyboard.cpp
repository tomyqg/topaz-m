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

void keyboard::resizeWidgets(QDialog & qw, qreal mratio)
{

    qDebug() << "widget";
    // ratio to calculate correct sizing
    /*qreal mratio_bak = mratio;

        if(qw.m_ratio != 0)
            mratio /= qw.m_ratio;

        // this all was done so that if its called 2 times with ratio = 2, total is not 4 but still just 2 (ratio is absolute)
        qw.m_ratio = mratio_bak;
        */
    //    QLayout * ql = qw.layout();

    //    if (ql == NULL)
    //        return;

    //    QWidget * pw = ql->parentWidget();

    //    if (pw == NULL)
    //        return;

    QList<QLayout *> layouts;


    QList<QWidget *> widgets = findChildren<QWidget *>();

    foreach(QWidget * widget, widgets)
    {
        QRect g = widget->geometry();

        widget->setMinimumSize(widget->minimumWidth() * mratio, widget->minimumHeight() * mratio);
        widget->setMaximumSize(widget->maximumWidth() * mratio, widget->maximumHeight() * mratio);

        //        w->setMinimumSize(100, 50);
        //        w->setMaximumSize(100, 50);

        widget->resize(widget->width() * mratio, widget->height() * mratio);
        widget->move(QPoint(g.x() * mratio, g.y() * mratio));
        //qw.resizeFont(w);
        qDebug() << widget;
    }

    return;
    //    foreach(QWidget *w, pw->findChildren<QWidget*>())

    //    {
    //        QRect g = w->geometry();

    ////        w->setMinimumSize(w->minimumWidth() * mratio, w->minimumHeight() * mratio);
    ////        w->setMaximumSize(w->maximumWidth() * mratio, w->maximumHeight() * mratio);

    //        w->setMinimumSize(100, 50);
    //        w->setMaximumSize(100, 50);

    //        w->resize(w->width() * mratio, w->height() * mratio);
    //        w->move(QPoint(g.x() * mratio, g.y() * mratio));
    //        //qw.resizeFont(w);
    //    }

    //    foreach(QLayout *l, pw->findChildren<QLayout*>())
    //    {
    //        if(l != NULL && !(l->objectName().isEmpty()))
    //            layouts.append(l);
    //    }

    //    foreach(QLayout *l, layouts) {
    //        QMargins m = l->contentsMargins();

    //        m.setBottom(m.bottom() * mratio);
    //        m.setTop(m.top() * mratio);
    //        m.setLeft(m.left() * mratio);
    //        m.setRight(m.right() * mratio);

    //        l->setContentsMargins(m);

    //        l->setSpacing(l->spacing() * mratio);

    //        if (l->inherits("QGridLayout")) {
    //            QGridLayout* gl = ((QGridLayout*)l);

    //            gl->setHorizontalSpacing(gl->horizontalSpacing() * mratio);
    //            gl->setVerticalSpacing(gl->verticalSpacing() * mratio);
    //        }
    //    }

    //    QMargins m = qw.contentsMargins();

    //    m.setBottom(m.bottom() * mratio);
    //    m.setTop(m.top() * mratio);
    //    m.setLeft(m.left() * mratio);
    //    m.setRight(m.right() * mratio);

    //    // resize accordingly main window
    //    qw.resize(qw.width() * mratio, qw.height() * mratio);
    //    qw.setContentsMargins(m);
    //    qw.adjustSize();
}
