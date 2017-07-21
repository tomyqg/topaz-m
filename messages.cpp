#include <QTimer>
#include <QTime>
#include <QVector>
#include <QFile>
#include <QMessageBox>
#include <QtWidgets>

#include "messages.h"
#include "ui_messages.h"

QJsonArray MessageWrite::messagesqueue;

Messages::Messages(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Messages)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowTitle(tr("LOG"));
    MessageWrite mr2 ;
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
//    MessageWrite mr ("Messages close");
    delete ui;
}

void Messages::on_pushButton_clicked()
{
    this->close();
}

MessageWrite::MessageWrite()
{
//    LogMessageRead();
//    LogAddMessage("message");
}

MessageWrite::MessageWrite( QString nm)
{
    LogMessageRead();
    LogAddMessage(nm);
}

void Messages::on_dial_valueChanged(int value)
{
    ui->listWidget->setCurrentRow(ui->listWidget->count()/100*ui->dial->value());
}
