#include "channel1.h"


int Channel1Options::count222 = 7;

int Channel1Options::GetSignalType1()
{
    return 1;
}

int Channel1Options::Ret()
{
    return Channel1Options::count222;
}


void Channel1Options::setprop()

{

    Channel1Options::count222 = 10;

//    count = 2 ;

//    qDebug() << this->type1;
//    type1 = 1;
//    qDebug() << this->type1;
}

void Channel1Options::setprop2()

{
//    qDebug() << this->type1;
//    type1 = 2;
//    qDebug() << this->type1;
}
