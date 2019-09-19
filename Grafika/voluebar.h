#ifndef VOLUEBAR_H
#define VOLUEBAR_H

#include <QFrame>
#include <QTimer>

namespace Ui {
class wVolueBar;
}

class cMarker : public QObject
{
    Q_OBJECT

public:
    double vol;
    bool dir;   // направление уставки: 0 - вниз, 1 - вверх
};

class wVolueBar : public QFrame
{
    Q_OBJECT

public:
    explicit wVolueBar(/*int num = 0, */QWidget *parent = 0);
    ~wVolueBar();
    void setBarDiapazon(double hi, double low);
    void setValue(double vol);
    void setExtr(double minimum, double maximum);
    void setLim(double low, double hi);
    void resetLim();
    void setText(QString type, QString mes);
    void changeNum(int num);
    void setNumChan(int num) { numChan = num; }
    void cleanMarker();
    void addMarker(double vol, bool dir);
    void setValueType(int type) { valueType = type; }
    int getValueType(void) { return valueType; }
    void setError(bool err) { errorStyle = err; }

    enum BarValueType {
        BarValue_Real = 0,
        BarValue_Procent = 1
    };
    Q_ENUM(BarValueType)
signals:
    void clickedLabel(int);

public slots:
    void setColor(QColor color, QColor colorL);
    void resizeEvent(QResizeEvent * s);
    bool eventFilter(QObject* watched, QEvent* event);


private slots:
    void slotErrorFlash();
private:
    Ui::wVolueBar *ui;
    double razmah;  //размах
    double higherMeasure;
    double lowerMeasure;
    int numBar;
    int numChan;
    QColor colorBar;    //цвет бара
    QColor colorLight;  //цвет светлой части бара
//    typedef struct {
//        double vol;
//        bool dir;   // направление уставки: 0 - вниз, 1 - вверх
//    } typeMarker;
    QList <cMarker*> listMarker;
    int valueType;  //тип отображения: 0 - абсолютная величина, 1 - %
    bool errorStyle;    // включить стиль оформления "Ошибка"
    QTimer flashTimer;  // таймер моргания ошибки
};



#endif // VOLUEBAR_H
