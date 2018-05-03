#ifndef PIN_H
#define PIN_H

#include <sys/types.h>
#include <stdbool.h>

int getPin(uint gpio);
int setPin(uint gpio, bool state);
int setPoll(uint gpio, const char * front);
int getPoll(int fd);

#endif // PIN_H
