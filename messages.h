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

private:
    Ui::Messages *ui;
};

class MessageWrite
{
public:

//    QString GetUnitsName();
//    double GetSignalType();
    void WriteMessage(QString nm);
    explicit  MessageWrite();
    explicit MessageWrite(QString nm);

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
