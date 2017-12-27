#ifndef USTAVKA_H
#define USTAVKA_H

//упрощённая реализация. В перспективе реализовать сигналы и контроль по таймеру

#include <QObject>
//#include <QTimer>

class Ustavka : public QObject
{
    Q_OBJECT
public:
    explicit Ustavka()
    {

    }

    void setUstavka(double val, double hist, int relayUp, int relayDown);
    int getUstavka(double cur); // при сработке возвращает номер реле, 0 - нет сработки уставки
    double getStateValue();
    double getHisteresis();
    int getnumRelayUp();
    int getnumRelayDown();

signals:
//    void workReleSignal(int);

private:
    double stateValue;  //уровень для переключения реле
    double histeresis;  //гистерезис на переключение
    int numRelayUp;     //номер переключаемого реле при превышении
    int numRelayDown;   //номер переключаемого реле при принижении
    bool setUp;         //превышение
    bool setDown;       //принижение
//    bool isSetted;      //признак срабатывания уставки
    //    QTimer timeFilter;  //фильтр по времени
    //    int interval;       //время устоявшегося значения

private slots:
//    void timeoutToWorkRelay();    //время отработать реле


};

#endif // USTAVKA_H
