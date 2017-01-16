#include "options.h"
#include "ui_options.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QtTest/QTestEvent>
#include <QtTest/QTestEventList>
#include <QtTest/QTest>
#include <QPoint>
#include <QEvent>
#include <QMouseEvent>

#include <QFocusEvent>

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("OPTIONS"));

    connect(ui->radioButton_2, SIGNAL(pressed()), this, SLOT(checkboxchange(QString newstring)) );

}

Options::~Options()
{
    delete ui;
}

void Options::on_pushButton_clicked()
{
    savesettings();
    this->close();
}

void Options::on_pushButton_2_clicked()
{
    this->close();
}

void Options::checkboxchange(QString newstring)
{
    ui->lineEdit_5->setText("newstring");
}

void Options::on_radioButton_2_clicked()
{

}

void Options::on_radioButton_3_clicked()
{

}
