#include "pin.h"
#include "stdio.h"
#include "fcntl.h"
#include "string.h"
#include "unistd.h"
#include "linux/input.h"
//#include "asm/gpio.h"
#include "poll.h"


#define GPIO_EXPORT     "/sys/class/gpio/export"
#define GPIO_GPIO       "/sys/class/gpio/gpio"
#define GPIO_DIR        "/direction"
#define GPIO_VALUE      "/value"
#define GPIO_EDGE       "/edge"

FILE * f;


/*
 *      Инициализация пина
 * */
static int initPin(uint gpio)
{
    char s[200];
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_VALUE);
    if(!access(s, F_OK))
    {
        //пин уже инициализирован
        return 0;
    } else {
        //Файл не открылся, значит пин не проинициализирован
        fprintf(stderr, "gpio%d no init - make init\n", gpio);
        sprintf(s, GPIO_EXPORT);
        f = fopen(s, "w");
        fprintf(f, "%d", gpio);
        fclose(f);
        //проверяем, создался ли gpio
        sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_VALUE);
        if(access(s, F_OK))
        {
            //пин так и не "хочет" инициализироваться
            fprintf(stderr, "init error gpio%d\n", gpio);
            return -1;
        }
        return 0;
    }

}

/*
 *      Установка состояния пина
 * */
int setPin(uint gpio, bool state)
{
    if(initPin(gpio) == -1)
    {
        //пин не доступен для использования
        return -1;
    }
    char s[200];
    //установка пина на выход
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_DIR);
    f = fopen(s, "r+");
    fprintf(f, "out");
    fclose(f);
    //установка сигнала на выходе
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_VALUE);
    f = fopen(s, "r+");
    fprintf(f, "%d", state);
    fclose(f);
    //сигнал на пине должен установиться
    return 0;
}

/*
 *      Получение состояния пина
 * */
int getPin(uint gpio)
{

    if(initPin(gpio) == -1)
    {
        //пин не доступен для использования
        return -1;
    }
    char s[200];
    int state = 0;
    //установка пина на вход
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_DIR);
    f = fopen(s, "r+");
    fprintf(f, "in");
    fclose(f);
    //получение сигнала на входе
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_VALUE);
    f = fopen(s, "r");
    fscanf(f, "%d", &state);
    fclose(f);
    return (state != 0);
}

/*
 *      Установка прерывания по изменению на пине
 *      функция возвращает дескриптор на файл gpioN/value
 * */
int setPoll(uint gpio, const char * front)
{
    if(initPin(gpio) == -1)
    {
        //пин не доступен для использования
        return -1;
    }
    char s[200];
    int fd;
    char c;

    //установка пина на вход
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_DIR);
    f = fopen(s, "r+");
    fprintf(f, "in");
    fclose(f);
    //установка прерывания
    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_EDGE);
    f = fopen(s, "r+");
    fprintf(f, "%s", front);
    fclose(f);

    sprintf(s, "%s%d%s", GPIO_GPIO, gpio, GPIO_VALUE);
    fd = open(s, O_RDONLY);
    read(fd, &c, sizeof(c));
    fprintf(stderr, "read c=0x%x\n", c);

    return fd;
}

/*
 *      Функция ожидания изменения сигнала на пине
 * */
int getPoll(int fd)
{
    struct pollfd pollfd1[1];
    char c;
    int err;

    pollfd1[0].fd = fd;
    pollfd1[0].events = POLLPRI | POLLERR;
    pollfd1[0].revents = 0;

    err = poll(pollfd1, 1, 100000000);
    if(err != 1){
        fprintf(stderr, "poll err=%d\n", err);
        return -1;
    }

    lseek(fd, 0, SEEK_SET);

    err = read(fd, &c, sizeof(c));
    if(err != 1){
        fprintf(stderr, "read c=0x%x\n", c);
        return -1;
    }

    return c - '0';
}
