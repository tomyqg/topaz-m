#ifndef USTAVKA_H
#define USTAVKA_H

//упрощённая реализация. В перспективе реализовать сигналы и контроль по таймеру

#include <QObject>
#include <QTimer>
#include <QString>
#include <QDialog>

#define DELAY_RELAY 100

class Ustavka : public QObject
{
    Q_OBJECT
public:
    explicit Ustavka(QWidget *parent = 0);
    ~Ustavka();
//    static int ResID;   // Счётчик номеров уставок
//    int getID();        // Функция возврата номера уставки
    void setUstavka(int ch, double hi, double low, double hiHist, double lowHist, int relayUp, int relayDown);
    int getUstavka(); // при сработке возвращает номер реле, 0 - нет сработки уставки
    void update(double cur);    // обновление состояний уставки. вызывать периодически из главного потока
    double getHiStateValue() {return stateHiValue; }
    double getLowStateValue() { return stateLowValue; }
    double getHiHisteresis() { return hiHisteresis; }
    double getLowHisteresis() { return lowHisteresis; }
    int getnumRelayUp() { return numRelayUp; }
    int getnumRelayDown() { return numRelayDown; }
    int getChannel() { return numChannel; }
    bool isUp() { return setUp; }
    bool isDown() { return setDown; }
    QString getMessInHigh() { return stateInHighMess; }
    QString getMessNormHigh() { return stateNormHighMess; }
    QString getMessInLow() { return stateInLowMess; }
    QString getMessNormLow() { return stateNormLowMess; }
    void setMessInHigh(QString s) { stateInHighMess = s; }
    void setMessNormHigh(QString s) { stateNormHighMess = s; }
    void setMessInLow(QString s) { stateInLowMess = s; }
    void setMessNormLow(QString s) { stateNormLowMess = s; }

signals:
    void workReleSignal(int relay, bool state);
    void messToLogSignal(int channel, QString mess);

private:
    int numChannel;         // номер выбранного канала
    double stateHiValue;    // верхний уровень для переключения реле
    double stateLowValue;   // нижний уровень для переключения реле
    double hiHisteresis;    // гистерезис верхнего уровня
    double lowHisteresis;   // гистерезис нижнего уровня
    int numRelayUp;     //номер переключаемого реле при превышении верхнего уровня. 0 - нет реле
    int numRelayDown;   //номер переключаемого реле при принижении нижнего уровня. 0 - нет реле
    bool setUp;         //превышение
    bool setDown;       //принижение
    bool fNewState;     //признак изменения состояния
    bool fConfirm;      //признак подтверждения установки состояния реле
    QTimer *timeFilter;  //фильтр по времени
    QString stateInHighMess;
    QString stateNormHighMess;
    QString stateInLowMess;
    QString stateNormLowMess;
    bool kvitirEnUp;      //включение квитирования превышения
    bool kvitirEnDown;      //включение квитирования принижения

private slots:
    void timeoutToWorkRelay();    //время отработать реле


};

#endif // USTAVKA_H
