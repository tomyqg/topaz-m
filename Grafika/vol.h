#ifndef VOL_H
#define VOL_H

#include <QWidget>

namespace Ui {
class wVol;
}

class wVol : public QWidget
{
    Q_OBJECT

public:
    explicit wVol(QWidget *parent = 0);
    ~wVol();
    void setVol(double vol);
    void setText(QString name, QString mes);
    void changeNum(int num);
    void setColor(QColor color);

public slots:
    void resizeEvent(QResizeEvent * s);

private:
    Ui::wVol *ui;
    int numBar;
};

#endif // VOL_H
