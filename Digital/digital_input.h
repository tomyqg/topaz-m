#ifndef CDIGITALINPUT_H
#define CDIGITALINPUT_H

#include <QObject>
#include <QString>

class cDigitalInput : public QObject
{
    Q_OBJECT
public:
    explicit cDigitalInput(QObject *parent = 0);

signals:

public slots:

public:
    enum TypeFunctional{
        NoneFunction = 0,
        Control = 1,
        Transition = 2,
        TotalFunctions
    };
    Q_ENUM(TypeFunctional)
    TypeFunctional function;   // функция(назначение) входа

    enum TypeControlObject{
        ControlGroup = 0,
        ControlMath = 1,
        ControlUstavka = 2,
        TotalControlObject
    };
    Q_ENUM(TypeControlObject)
    TypeControlObject controlObject;   // Тип управляемого объекта

    int numDiscretOutpit;   // номер управляемого дискретного выхода, 0 - выход не используется

    QString messChangeToOn; // сообщение при переходе в 1
    QString messChangeToOff; // сообщение при переходе в 0
    int action;             // действие при сигнале на входе. 1 - включать выход (повторить), 0 - выключить (инвертировать сигнал)

    enum TypeMode{
        ModeSignal = 0,     // передача сигнала как есть
        ModeTransition = 1  // действие по переходу в течение заданного времени
    };
    Q_ENUM(TypeMode)
    TypeMode mode;      // режим действия, повоторять или однократно реагировать

    int timeoutOn;      // время действия при переходе в 1, мс
    int timeoutOff;      // время действия при переходе в 0, мс

};

#endif // CDIGITALINPUT_H
