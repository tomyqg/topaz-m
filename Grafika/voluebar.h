#ifndef VOLUEBAR_H
#define VOLUEBAR_H

#include <QFrame>

namespace Ui {
class wVolueBar;
}

class wVolueBar : public QFrame
{
    Q_OBJECT

public:
    explicit wVolueBar(/*int num = 0, */QWidget *parent = 0);
    ~wVolueBar();
    void setBarDiapazon(double diap);
    void setVolue(double vol);
    void setExtr(double min, double max);
    void setLim(double low, double hi);
    void setText(QString type, QString mes);
    void changeNum(int num);

public slots:
    void setColor(QString cssColor, QString cssColorLight);
    void resizeEvent(QResizeEvent * s);


private:
    Ui::wVolueBar *ui;
    double razmah;  //размах, значение бара в крайних точках виджета
    int numBar;
};

#endif // VOLUEBAR_H
