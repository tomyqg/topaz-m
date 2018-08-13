#ifndef CSTEELTECHNOLOGY_H
#define CSTEELTECHNOLOGY_H

#include <QObject>
#include <registersmap.h>

#define SIZE_TECH_NAME_STR 20

typedef struct {
    uint8_t num;    //номер группы (индекс с 0)
    uint8_t nSt;    //тип термопары
    uint8_t COH;    //тип первичного преобразователя
    uint16_t b1;    //температура кристаллизации для вычисления содержания углерода, град.
    float dSt;      //длительность площадки, сек.
    float dt;       //допустимое изменение выходного сигнала за время площадки, градус цельсия
    float tPt;      //время измерения по температуре в сек.
    int LPtl;       //нижний предел задания температуры начала замера в град.
    int LPth;       //верхний предел задания температуры начала замера в град.
    float dSE;      //длительность площадки по ЭДС, сек.
    float dE;       //допустимое изменение ЭДС за время площадки, мВ
    float tPE;      //время измерения ЭДС по активности кислорода, сек.
    uint8_t b2;     //весовой коэффициент вычисления содержания углерода
    uint8_t O;      //конечная окисленность, ppm
    uint8_t Y;      //степень усвоения аллюминия, %
    uint16_t G;     //масса плавки стали, контролируемой входной группой измерительных входов, т
    char name[SIZE_TECH_NAME_STR];    //имя группы технологии
} typeSteelTech;

const typeSteelTech defTech[] = {
//                   .     .    .                      .    .     .    .   .   .    .
//   num nSt         COH b1    dSt  dt    tPt  LPtl  LPth  dSE  dE    tPE  b2  O   Y    G
    {0,  TC_Type_B,  0,  1535, 2.0, 10.0, 5.0, 1300, 1800, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 1"},   //1
    {1,  TC_Type_B,  1,  1535, 2.0, 10.0, 7.0, 1300, 1800, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 2"},   //2
    {2,  TC_Type_B,  1,  1535, 2.0, 10.0, 7.0, 1300, 1800, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 3"},   //3
    {3,  TC_Type_S,  0,  1535, 2.0, 10.0, 5.0, 1100, 1700, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 4"},   //4
    {4,  TC_Type_S,  0,  1535, 2.0, 10.0, 5.0, 1100, 1700, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 5"},   //5
    {5,  TC_Type_A1, 0,  1535, 2.0, 10.0, 5.0, 1500, 2200, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 6"},   //6
    {6,  TC_Type_B,  2,  1535, 2.0, 10.0, 7.0, 1300, 1800, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 7"},   //7
    {7,  TC_Type_B,  2,  1535, 2.0, 10.0, 7.0, 1300, 1800, 2.0, 30.0, 8.0, 80, 20, 100, 100, "Group 8"},   //8
};

#endif // CSTEELTECHNOLOGY_H


