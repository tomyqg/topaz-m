#ifndef MESSAGES_H
#define MESSAGES_H

#include <QDialog>

namespace Ui {
class Messages;
}

class Messages : public QDialog
{
    Q_OBJECT

public:
    explicit Messages(QDialog *parent = 0);
    ~Messages();

private slots:
    void on_pushButton_clicked();

    void on_dial_valueChanged();

private:
    Ui::Messages *ui;
};

class MessageWrite : public QObject
{
public:

    QJsonArray LogMessageRead();
    void LogAddMessage(QString nm);
    void WriteAllLogToFile();
    explicit  MessageWrite(); // конструктор
    explicit MessageWrite(QString nm); // конструктор
    ~MessageWrite(); // деструктор

    static QJsonArray messagesqueue;
    static QString strstr;

    void LogClear();

private:

//    double signaltype;
//    double lowerlimit;
//    double higherlimit;
//    double lowermeasurelimit;
//    double highermeasurelimit;
//    double measureperiodsecond;
//    QString unitsname;
};

#endif // MESSAGES_H
