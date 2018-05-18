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

private slots:
    void on_button_clicked(bool checked);

private:
    Ui::wButtonStyled *ui;
};

#endif // BUTTONSTYLED_H
