#include "digital_input.h"

cDigitalInput::cDigitalInput(QObject *parent) : QObject(parent)
{
    function = NoneFunction;
    controlObject = ControlGroup;
    numDiscretOutpit = 0;
    action = 1;
    mode = ModeSignal;
    timeoutOn = 100;
    timeoutOff = 100;
}
