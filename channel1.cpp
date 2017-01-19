#include "channel1.h"

int Channel1Options::GetSignalType1()
{

    if ( ui->UnChan1RadButOtkl->isChecked() )
        return 1;

    return 2;
}
