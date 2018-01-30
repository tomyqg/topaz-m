#include <assert.h>
#include "slot.h"

void Slot::initSlot(int n, int type)
{
    assert(n > 0);
    assert(n <= MAX_NUM_SLOTS);
    assert(type > 0);

    num = n;
    deviceType = type;
    state = 1;      // слот инициализирован
    needConfig = true;
}




