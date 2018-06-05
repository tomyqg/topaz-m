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

signals:
    void clickedLabel(int);

public slots:
    void setColor(QColor color, QColor colorL);
    void resizeEvent(QResizeEvent * s);
    bool eventFilter(QObject* watched, QEvent* event);


private:
    Ui::wVolueBar *ui;
    double razmah;  //размах, значение бара в крайних точках виджета
    int numBar;
    QColor colorBar;    //цвет бара
    QColor colorLight;  //цвет светлой части бара
};

#endif // VOLUEBAR_H
