#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

#include <mainwindow.h>

namespace Ui {
class Options;
class Channel1OptionClass;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

public slots:

    void WriteOptionsToFile();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void checkboxchange();

    void savesettings();

    int GetSignalType();

    QString GetSignalUnits();


private:

    Ui::Options *ui;

};





#endif // OPTIONS_H
