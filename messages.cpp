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

QJsonArray MessageWrite::messagesqueue;

Messages::Messages(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Messages)
{

//    MessageWrite mr;
//    mr.WriteAllLogToFile();
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("MESSAGES"));
    MessageWrite mr2 ("Messages open");
    QJsonArray messagesarray = mr2.LogMessageRead();
    for (int var = 0; var < messagesarray.count() ; ++var) {
        QJsonObject mes = messagesarray.at(var).toObject();
        ui->listWidget->addItem(QString::number((var+1)) + ": " + mes.value("Date").toString() +" "+  mes.value("Time").toString()+" "+ mes.value("Message").toString());
    }
    ui->listWidget->setStyleSheet("QListWidget { background-color: #CCFFFF }" "QListWidget::item:selected {border: 1px solid #6a6ea9;}" );
    ui->listWidget->scrollToBottom();
}

Messages::~Messages()
{
    MessageWrite mr ("Messages close");
    delete ui;
}

void Messages::on_pushButton_clicked()
{
    this->close();
}

MessageWrite::MessageWrite()
{
    LogMessageRead();
    LogMessageWrite("message");
}

MessageWrite::MessageWrite( QString nm)
{
    LogMessageRead();
    LogMessageWrite(nm);
}

void Messages::on_dial_valueChanged(int value)
{
    ui->listWidget->setCurrentRow(ui->listWidget->count()/100*ui->dial->value());
}
