#ifndef CSTEEL_H
#define CSTEEL_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include "steel_technology.h"


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
    uint16_t cl;    //массовая доля содержания углерода, %
    uint16_t alg;   //расчётное значение массы алюминия для раскисления, кг
    uint16_t cup;   //прогнозируемое содержание углерода на основе замеров температуры ликвидуса
    QVector<double> vectorTemp;    //массив измеренных температур для графика
    QVector<double> vectorEds;     //массив измеренных эдс для графика
    QDateTime timeUpdateData;       //время получения готовых данных по площадке
    bool vectorTempReceived;
    bool vectorEdsReceived;
    bool allVectorsReceived;        //массивы дял графиков получены полностью

signals:

public slots:

private:

};

#endif // CSTEEL_H
