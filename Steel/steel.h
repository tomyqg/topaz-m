#ifndef CSTEEL_H
#define CSTEEL_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include "steel_technology.h"
#include "transaction.h"
#include <QTimer>

#define SIZE_ARRAY  160
#define SIZE_ONE_ARRAY  32
#define SUM_RELAYS   4

enum {
    STEEL_WAIT,             //ожидание измерения на одной из входных групп
    STEEL_MEASURE,          //измерение температуры
    STEEL_READY,            //измерения выполнены
};

class cSteel : public QObject
{
    Q_OBJECT
public:
    explicit cSteel(QObject *parent = 0);
//    int setTechnology(typeSteelTech * tech) { technology = tech; }

    int num; //номер объекта
    bool enable;    //состояние входной группы ВКЛ/ВЫКЛ
    int slot;       //слот, в котором установлена плата
    int slotIndex;  //номер входной группы в плате
    static int countSteel;
    typeSteelTech * technology;
    uint8_t status; //статус входной группы
    uint8_t verifStatus;    //подтверждение статуса
    float temp;     //измеряемая температура, град.
    float eds;      //ЭДС датчика активности кислорода, мВ
    float ao;    //активность кислорода, ppm
    float cl;    //массовая доля содержания углерода, %
    float alg;   //расчётное значение массы алюминия для раскисления, кг
    uint16_t cup;   //прогнозируемое содержание углерода на основе замеров температуры ликвидуса
    QVector<double> vectorTemp;    //массив измеренных температур для графика
    QVector<double> vectorEds;     //массив измеренных эдс для графика
    QDateTime timeUpdateData;       //время получения готовых данных по площадке
    bool vectorTempReceived;
    bool vectorEdsReceived;
    bool allVectorsReceived;        //массивы для графиков получены полностью
    bool fConfirm;                  //признак подтверждения получения всех данных
    bool lastItemEds;
    bool lastItemTemp;
    int8_t relais[SUM_RELAYS];
    uint8_t countRelayTime;         //обратный счётчик реле "ВРЕМЯ"
    bool fReleyTimeout;         //признак отключения состояния последнего реле "ВРЕМЯ" по таймауту
    uint8_t numSmelt;           // номер плавки
    float cj;       // температура холодного спая
    int state;      //состояние входной группы (из статуса)
    deviceModeEnum mode;   //режим работы: 0 - рабочий, 1 - диагностика

signals:
    void sendTransToWorker(Transaction tr);
    void signalDevicesPause(bool pause);
    void signalSteelFrame(bool steelFrame);
    void signalReady(int n);
    void signalMeasure(int n);
    void signalWait(int n);
    void signalArchive(int n);

public slots:
    void parserSteel(Transaction tr);

private slots:
    void update();
    void updateParam();

private:
    QTimer timerUpdate;
    QTimer timerUpdateParam;
    QStringList listBadParam;
    QStringList blackListParam;
    QList<QString> listUpdateParam;
    QList<QString> listDiagnosticParam;
    int countParam;
    int countDiagnostic;

    int getIndexArray();
    void readArrays();
    bool isBlackListParam(QString param);
    void addBadParam(QString param);
};

#endif // CSTEEL_H
