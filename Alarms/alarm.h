#ifndef CALARM_H
#define CALARM_H

#include <QObject>
#include <QTimer>
#include "kvitirovanie.h"

class cAlarm : public QObject
{
    Q_OBJECT
public:
    explicit cAlarm(int ch, QObject *parent = 0);
    enum
    {
        ControlUp = 0,  //верхняя контрольная точка
        ControlDown = 1,//нихняя контрольная точка
        Gradient = 2,   //градиент
        InBand = 3,     //внутриполосный
        OutBand = 4,    //внеполосный
    };

signals:
    void alarm(int relay, bool state, QString mes);

public slots:

private slots:
    void setAlarm();

private:
    int channel;    //номер подключенного канала. Изменение параметра = копирование уставки в другой канал
    int reg;        //тип регистрации
    QString name;   //имя аварии (идентификатор уставки)
    double vol;     //значение уставки
    double gister;  //гистерезис (абсолютная величина)
    int delay;      //время задержки в мс
    int relay;      //номер подключенного реле. -1 - реле отключены
    bool fKvitir;   //признак необходимости квитировать
    QString mesOn;  //сообщение при срабатывании уставки
    QString mesOff; //сообщение при восстановлении уставки
    QTimer timer;   //таймер срабатывания уставки
    kvitirovanie kvitDialog;    //вплывающее окно квитирвания. Ождает подтверждения от пользователя
};

#endif // CALARM_H
