#ifndef STACKEDOPTIONS_H
#define STACKEDOPTIONS_H

#include <QWidget>

namespace Ui {
class StackedOptions;
}

class StackedOptions : public QWidget
{
    Q_OBJECT

public:
    explicit StackedOptions(QWidget *parent = 0);
    ~StackedOptions();

private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::StackedOptions *ui;
};

#endif // STACKEDOPTIONS_H
