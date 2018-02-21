#ifndef KVITIROVANIE_H
#define KVITIROVANIE_H

#include <QDialog>
#include <QString>

namespace Ui {
class kvitirovanie;
}

class kvitirovanie : public QDialog
{
    Q_OBJECT

public:
    explicit kvitirovanie(QWidget *parent = 0);
    ~kvitirovanie();
    void showInfo(QString mess);

private:
    Ui::kvitirovanie *ui;
};

#endif // KVITIROVANIE_H
