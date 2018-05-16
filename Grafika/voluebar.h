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
    explicit wVolueBar(QWidget *parent = 0);
    ~wVolueBar();

public slots:
    void setVolue(double vol);
//    void setColor(QColor)


private:
    Ui::wVolueBar *ui;
};

#endif // VOLUEBAR_H
