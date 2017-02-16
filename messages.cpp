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

#include "messages.h"
#include "ui_messages.h"

Messages::Messages(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Messages)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("MESSAGES"));

    QString s = "What are little boys made of, made of?What are little boysmade of?";
    for (int var = 0; var < 12; ++var) {
        ui->listWidget->addItem(s);
    }
    ui->listWidget->setStyleSheet("QListWidget { background-color: #CCFFFF }" "QListWidget::item:selected {border: 1px solid #6a6ea9;}" );

    MessageWrite mr;

}

Messages::~Messages()
{
    delete ui;
}

void Messages::on_pushButton_clicked()
{
    this->close();
}

MessageWrite::MessageWrite()
{
//    qDebug() << "none string";
}

MessageWrite::MessageWrite( QString nm)
{
//    qDebug() << nm;
}

void MessageWrite::WriteMessage(QString nm)
{}


