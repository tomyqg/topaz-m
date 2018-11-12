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
    void setUstavka(QString name, int ch, bool type, double hi, double hiHist, int relayUp);
    int getUstavka(); // при сработке возвращает номер реле, 0 - нет сработки уставки
    void update(double cur);    // обновление состояний уставки. вызывать периодически из главного потока
    double getHiStateValue() {return stateHiValue; }
//    double getLowStateValue() { return stateLowValue; }
    double getHiHisteresis() { return hiHisteresis; }
//    double getLowHisteresis() { return lowHisteresis; }
    int getnumRelayUp() { return numRelayUp; }
//    int getnumRelayDown() { return numRelayDown; }
    int getChannel() { return numChannel; }
    bool isUp() { return setUp; }
//    bool isDown() { return setDown; }
    QString getMessInHigh() { return stateInHighMess; }
    QString getMessNormHigh() { return stateNormHighMess; }
//    QString getMessInLow() { return stateInLowMess; }
//    QString getMessNormLow() { return stateNormLowMess; }
    void setMessInHigh(QString s) { stateInHighMess = s; }
    void setMessNormHigh(QString s) { stateNormHighMess = s; }
//    void setMessInLow(QString s) { stateInLowMess = s; }
//    void setMessNormLow(QString s) { stateNormLowMess = s; }
    void setKvitirUp(bool f) { kvitirEnUp = f; }
//    void setKvitirDown(bool f) { kvitirEnDown = f; }
    bool getKvitirUp() { return kvitirEnUp; }
//    bool getKvitirDown() { return kvitirEnDown; }
//    void setNameCh(QString name) { nameChannel = name; }
//    QString getNameCh() { return nameChannel; }
    QString getIdentifikator() { return identifikator; }
    void setIdentifikator(QString ident) { identifikator = ident; }
    void setNum(int i) { num = i; }
    int getNum() { return num; }
    void setTypeFix(bool t) { typeFix = t; }
    bool getTypeFix() { return typeFix; }

signals:
    void workReleSignal(int relay, bool state);
    void messToLogSignal(int channel, QString mess);

private:
    int num;                // номер ustavki
    QString identifikator;  // идентификатор уставки
    int numChannel;         // номер выбранного канала
    bool typeFix;           // тип (направление) срабатывания уставки
    double stateHiValue;    // верхний уровень для переключения реле
//    double stateLowValue;   // нижний уровень для переключения реле
    double hiHisteresis;    // гистерезис верхнего уровня
//    double lowHisteresis;   // гистерезис нижнего уровня
    int numRelayUp;     //номер переключаемого реле при превышении верхнего уровня. 0 - нет реле
//    int numRelayDown;   //номер переключаемого реле при принижении нижнего уровня. 0 - нет реле
    bool setUp;         //превышение
//    bool setDown;       //принижение
    bool fNewState;     //признак изменения состояния
    bool fConfirm;      //признак подтверждения установки состояния реле
    QTimer *timeFilter;  //фильтр по времени
    QString stateInHighMess;
    QString stateNormHighMess;
//    QString stateInLowMess;
//    QString stateNormLowMess;
    bool kvitirEnUp;        //включение квитирования превышения
//    bool kvitirEnDown;      //включение квитирования принижения
    bool waitKvitirUp;      //ожидания подтверждения от оператора
//    bool waitKvitirDown;    //ожидания подтверждения от оператора

private slots:
    void timeoutToWorkRelay();    //время отработать реле


};

#endif // USTAVKA_H
