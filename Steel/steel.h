#ifndef CSTEEL_H
#define CSTEEL_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include "steel_technology.h"

#define SIZE_ARRAY  160
#define SUM_RELAYS   4


class cSteel : public QObject
{
    Q_OBJECT
public:
    explicit cSteel(QObject *parent = 0);
//    int setTechnology(typeSteelTech * tech) { technology = tech; }

    int num; //номер объекта
    static int countSteel;
    typeSteelTech * technology;
    uint8_t status; //статус входной группы
    float temp;     //измеряемая температура, град.
    float eds;      //ЭДС датчика активности кислорода, мВ
    uint16_t ao;    //активность кислорода, ppm
    float cl;    //массовая доля содержания углерода, %
    uint16_t alg;   //расчётное значение массы алюминия для раскисления, кг
    uint16_t cup;   //прогнозируемое содержание углерода на основе замеров температуры ликвидуса
    QVector<double> vectorTemp;    //массив измеренных температур для графика
    QVector<double> vectorEds;     //массив измеренных эдс для графика
    QDateTime timeUpdateData;       //время получения готовых данных по площадке
    bool vectorTempReceived;
    bool vectorEdsReceived;
    bool allVectorsReceived;        //массивы для графиков получены полностью
    bool lastItemEds;
    bool lastItemTemp;
    int8_t relais[SUM_RELAYS];
    uint8_t countRelayTime;         //обратнфый счётчик реле "ВРЕМЯ"
    uint8_t numSmelt;           // номер плавки

signals:

public slots:

private:

};

#endif // CSTEEL_H
