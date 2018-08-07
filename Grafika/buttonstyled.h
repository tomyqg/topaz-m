#ifndef BUTTONSTYLED_H
#define BUTTONSTYLED_H

#include <QWidget>

namespace Ui {
class wButtonStyled;
}

class wButtonStyled : public QWidget
{
    Q_OBJECT

public:
    explicit wButtonStyled(QWidget *parent = 0);
    ~wButtonStyled();
    void setText(QString text);
    void setColorText(QColor color);
    void setColorBg(QColor color);
    void setAlignLeft();
    void setAlignCenter();
    void setAlignRight();
    void setFontSize(int s);


private slots:
    void on_button_clicked();
    void on_button_pressed();
    void on_button_released();

signals:
    void clicked();

private:
    Ui::wButtonStyled *ui;
    QColor colorText;
    QColor colorBg;
    QColor colorBgDown;
    QString strColorText;
    QString strColorBg;
    QString strAlign;

//    bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent * s);
};

#endif // BUTTONSTYLED_H
