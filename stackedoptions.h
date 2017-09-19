#ifndef STACKEDOPTIONS_H
#define STACKEDOPTIONS_H

#include <QDialog>

namespace Ui {
class StackedOptions;
}

class StackedOptions : public QDialog
{
    Q_OBJECT

public:
    explicit StackedOptions(QWidget *parent = 0);
    ~StackedOptions();

private slots:

    void SetLabelIndex(int newlabelindex);

    void SetStackIndex(int newstackindex);

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_13_clicked();

    void on_ChangeDateButton_clicked();

    void on_pushButton_15_clicked();

private:
    Ui::StackedOptions *ui;
};

#endif // STACKEDOPTIONS_H
