ifndef DIGITALINPUTS_H
#define DIGITALINPUTS_H

#include <QDialog>

namespace Ui {
class DigitalInputs;
}

class DigitalInputs : public QDialog
{
    Q_OBJECT

public:
    explicit DigitalInputs(QWidget *parent = 0);
    ~DigitalInputs();

private:
    Ui::DigitalInputs *ui;
};

#endif // DIGITALINPUTS_H
