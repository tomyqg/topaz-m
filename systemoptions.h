#ifndef CSYSTEMOPTIONS_H
#define CSYSTEMOPTIONS_H

#include <QObject>


class cSystemOptions : public QObject
{
    Q_OBJECT
public:
    explicit cSystemOptions(QObject *parent = 0);
    int display;
    bool arrows;
    enum DisplayParametrEnum {
        Bars = 0x00,
        TrendsBars = 0x01,
        PolarBars = 0x02,
        Cyfra = 0x04,
        TrendsCyfra = 0x05,
        PolarCyfra = 0x06,
        //исключить
        Trends = 0xF1,
        Polar = 0xF2,
        BarsCyfra = 0xF7,
        TrendsCyfraBars = 0xF8
    };

signals:

public slots:
};

#endif // CSYSTEMOPTIONS_H
