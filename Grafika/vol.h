#ifndef VOL_H
#define VOL_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class wVol;
}

class wVol : public QWidget
{
    Q_OBJECT

public:
    explicit wVol(QWidget *parent = 0);
    ~wVol();
    void setVol(double vol, int prec = 10);
    void setText(QString name, QString mes);
    void changeNum(int num);
    void setColor(QColor color);
    void setChanNum(int num) { numChan = num; }
    void setError(bool err) { errorStyle = err; }

public slots:
    void resizeEvent(QResizeEvent * s);
    bool eventFilter(QObject* watched, QEvent* event);

signals:
    void clickedLabel(int);

private slots:
    void slotErrorFlash();
private:
    Ui::wVol *ui;
    int numBar;
    int numChan;
    bool errorStyle;    // включить стиль оформления "Ошибка"
    QTimer flashTimer;  // таймер моргания ошибки
};

#endif // VOL_H
