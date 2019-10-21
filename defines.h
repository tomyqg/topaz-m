#include <QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "Channels/channelOptions.h"
#include "uartdriver.h"
#include "worker.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"

#ifndef DEFINES_H
#define DEFINES_H

#ifdef Q_OS_WIN32
    //#define RANDOM_CHAN
#endif
//#define RANDOM_CHAN

#define VER "1.108"

//#define CURRENT_VER "1.84" //версия ПО

#ifdef RANDOM_CHAN
#define CURRENT_VER VER"r" //версия ПО
#else
#define CURRENT_VER VER
#endif

#define PROTOCOL_VER "13.0"  //версия протокола внутреннего Modbus
#define HARDWARE_VERSION "1.0" // версия железа
#define SOFTWARE_REVISION "0001"  //ревизия ПО
#define TYPE_DEVICE "Multigraph"    //Тип/название прибора
#define MANUFACTURER "Teplopribor"
#define SITE "tpchel.ru"
#define EXT_MODBUS_VER "1.2"
#define BOARD_MODEL "MYD-AM335X"

#define DEBUG_RELAY
#define MYD
//#define Demo // раскоментить если делаем демо программу на РС

#ifndef Q_OS_WIN32
#define comportname "/dev/ttyO1" // com port for MYD board
#define comportextmodbusrtu "/dev/ttyO3"    //port for External Modbus RTU
#define pathtofiles  "/opt/"
#define pathtodirarchive "/opt/archive/"
#define pathtosystemoptions  "/opt/systemoptions.txt"
#define pathtosystemoptionsdef  "/opt/Defaults/systemoptions.txt"
#define pathtoarchive  "/opt/archive.txt"
#define pathtoarchivedata  "/opt/archive/archive.dat"
#define pathtooptions  "/opt/options.txt"
#define pathtooptionsdef  "/opt/Defaults/options.txt"
#define pathtolog  "/opt/Log/"
#define pathtomessages  "/opt/Log.txt"
#define pathtologotip  "/opt/logotpchel.png"
#define pathtoqrcodetip "/opt/qr-code.jpg"
#define pathtohilimico  "/opt/hilim.png"
#define pathtolowlimico  "/opt/lowlim.png"
#define pathtoloadgif   "/opt/load.gif"
#define pathtoleftarrow   "/opt/left.png"
#define pathtorightarrow   "/opt/right.png"
#define pathtomodbusconfigfile  "/opt/MODBusConfigFile.txt"
#define pathtosteeloptions "/opt/steeloptions.txt"
#define pathtoupdates  "/opt/updates/"
#define uartsleep DelayMsec(80);
#define threadsleep DelayMsec(100);
#define longsleep DelayMsec(1000);
#define ETH_NAME    "eth0"

#define ETOUT 110   /* Connection timed out */

#define IS_LINUX

#else
#define comportname "COM6"
#define comportextmodbusrtu "COM9"    //port for External Modbus RTU
#define uartsleep Sleep(80);
#define longsleep Sleep(1000);
#define ETH_NAME  "eth0"

#ifndef Demo
#define pathtofiles  "C:/Work/"
#define pathtodirarchive "C:/Work/archive/"
#define pathtosystemoptions  "C:/Work/systemoptions.txt"
#define pathtosystemoptionsdef  "C:/Work/Defaults/systemoptions.txt"
#define pathtooptions  "C:/Work/options.txt"
#define pathtooptionsdef  "C:/Work/Defaults/options.txt"
#define pathtolog  "C:/Work/txtes/"
#define pathtomodbusconfigfile  "C:/Work/MODBusConfigFile.txt"
#define pathtomessages  "C:/Work/Log.txt"
#define pathtoarchive  "C:/Work/archive.txt"
#define pathtoarchivedata  "C:/Work/archive/archive.dat"
#define pathtologotip  "C:/Work/logotpchel.png"
#define pathtoqrcodetip "C:/Work/qr-code.jpg"
#define pathtohilimico  "C:/Work/hilim.png"
#define pathtolowlimico  "C:/Work/lowlim.png"
#define pathtoloadgif   "C:/Work/load.gif"
#define pathtoleftarrow   "C:/Work/left.png"
#define pathtorightarrow   "C:/Work/right.png"
//#define pathtosteeloptions QCoreApplication::applicationDirPath() + "/steeloptions.txt"
#define pathtosteeloptions "C:/Work/steeloptions.txt"
#define pathtoupdates  "C:/Work/updates/"
#else
#define pathtofiles  QCoreApplication::applicationDirPath()
#define pathtodirarchive QCoreApplication::applicationDirPath() + "/archive/"
#define pathtosystemoptions  QCoreApplication::applicationDirPath() + "/systemoptions.txt"
#define pathtosystemoptionsdef  QCoreApplication::applicationDirPath() + "/Defaults/systemoptions.txt"
#define pathtooptions  QCoreApplication::applicationDirPath() + "/options.txt"
#define pathtooptionsdef  QCoreApplication::applicationDirPath() + "/Defaults/options.txt"
#define pathtolog  QCoreApplication::applicationDirPath() + "/txtes/"
#define pathtomodbusconfigfile  QCoreApplication::applicationDirPath() + "/MODBusConfigFile.txt"
#define pathtomessages  QCoreApplication::applicationDirPath() + "/Log.txt"
#define pathtoarchive  QCoreApplication::applicationDirPath() + "/archive.txt"
#define pathtoarchivedata  QCoreApplication::applicationDirPath() + "/archive.dat"
#define pathtologotip  QCoreApplication::applicationDirPath() + "/logotpchel.png"
#define pathtoqrcodetip QCoreApplication::applicationDirPath() + "/qr-code.jpg"
#define pathtohilimico  QCoreApplication::applicationDirPath() + "/hilim.png"
#define pathtolowlimico  QCoreApplication::applicationDirPath() + "/lowlim.png"
#define pathtoloadgif   QCoreApplication::applicationDirPath() + "/load.gif"
#define pathtoleftarrow   QCoreApplication::applicationDirPath() + "left.png"
#define pathtorightarrow   QCoreApplication::applicationDirPath() + "right.png"
#define pathtosteeloptions QCoreApplication::applicationDirPath() + "/steeloptions.txt"
#endif
#endif

#define comportparity 'N'
#define comportbaud 9600
#define comportdatabit 8
#define comportstopbit 1

#define extmodbus_parity 'N'
#define extmodbus_baud 9600
#define extmodbus_databit 8
#define extmodbus_stopbit 1
#define extmodbus_server_id 17

#define BADCRCCODE -9999
#define CONNECTERRORCODE -9998

#define ConnectionErrorMessage "ош.соед"
#define CRCErrorMessage "ош.crc"
#define ObryvErrorMessage "обрыв"
#define StyleError "border: 2px solid rgb(170, 0, 0);"

#define Font "Open Sans"
#define NaNMessage "отказ"
#define OKMessage "норма"
#define AlertSign ""
#define MathString "math "

#define MultiThread
#define ValuesUpdateTimer 200        // период чтения данных в архив
#define GraphicsUpdateTimer 200      // время в мсек для обновления самого графика
#define ArchiveUpdateTimer 30000     // время архивации на флешку в мсекундах
#define DateLabelUpdateTimer 500    // время обновления времени
//#define LogUpdTimer 10000             // время обновления журнала сообщений (Log.txt)
#define XRange 300                   // изначальный размер графика по иксу в 1 сторону
#define YRange 20                   // изначальный размер графика по игреку в 1 сторону
#define GraphWidthinPixels 2         // толщина графика линий в пикселях
#define UstavkiUpdateTimer 500      // период мсек обновления состояний уставок
#define ParsingReceiveTrans 1       // период парсинга входящих транзакций от Workera в мсек
#define UpdateSteelTime 500
#define timeInitDevices 1000       // период инициализации комплекта плат, мсек
#define timeUpdateDevices 10000     // период обновления комплекта плат, мсек

#endif


/* Цветовая палитра */
// Базовые цвета. Только их использовать во всей программе. Чёрный и белый само собой
#define COLOR_1 QColor(0x1c,0xb9,0x9a)
#define COLOR_2 QColor(0x66,0x66,0xff)
#define COLOR_3 QColor(0xe8,0x4c,0x3d)
#define COLOR_4 QColor(0x2b,0x3e,0x4c)
#define COLOR_LIGHT_1 QColor(0x50,0xdc,0xc1)
#define COLOR_LIGHT_2 QColor(0x8c,0x8c,0xff)
#define COLOR_LIGHT_3 QColor(0xF3,0x7A,0x6D)
#define COLOR_LIGHT_4 QColor(0x70,0x8F,0xA5)
//#define COLOR_5 QColor(0xff,0x7c,0x27)
//#define COLOR_6 QColor(0xff,0xd2,0x4b)
//#define COLOR_7 QColor(0x2e,0xaf,0x4b)
//#define COLOR_8 QColor(0x76,0x5F,0x40)
//#define COLOR_LIGHT_5 QColor(0xff,0x9d,0x5d)
//#define COLOR_LIGHT_6 QColor(0xff,0xdd,0x78)
//#define COLOR_LIGHT_7 QColor(0x60,0xD7,0x7A)
//#define COLOR_LIGHT_8 QColor(0xBB,0x9F,0x7A)
#define COLOR_5 QColor(0x2b,0x3e,0x4c)
#define COLOR_6 QColor(0xe8,0x4c,0x3d)
#define COLOR_7 QColor(0x66,0x66,0xff)
#define COLOR_8 QColor(0x1c,0xb9,0x9a)
#define COLOR_LIGHT_5 QColor(0x70,0x8F,0xA5)
#define COLOR_LIGHT_6 QColor(0xF3,0x7A,0x6D)
#define COLOR_LIGHT_7 QColor(0x8c,0x8c,0xff)
#define COLOR_LIGHT_8 QColor(0x50,0xdc,0xc1)
//QColor rgb(96, 215, 122);


#define COLOR_DARK  QColor(0x2b,0x3e,0x4c)
#define COLOR_LIGHT QColor(0x0e,0xd2,0xe2)
#define COLOR_GRAY  QColor(0xe6,0xe6,0xe6)

#define ColorToTextRgb(x) "rgb("+QString::number((x).red())+","+QString::number((x).green())+","+QString::number((x).blue())+")"
#define ColorToTextRgbWithShift(x, s) "rgb("+QString::number((x).red()+s)+","+QString::number((x).green()+s)+","+QString::number((x).blue()+s)+")"

// Цвета элементов
#define ColorCh1 COLOR_1
#define ColorCh2 COLOR_2
#define ColorCh3 COLOR_3
#define ColorCh4 COLOR_4
#define ColorCh5 COLOR_5
#define ColorCh6 COLOR_6
#define ColorCh7 COLOR_7
#define ColorCh8 COLOR_8
#define ColorCh1Light COLOR_LIGHT_1
#define ColorCh2Light COLOR_LIGHT_2
#define ColorCh3Light COLOR_LIGHT_3
#define ColorCh4Light COLOR_LIGHT_4
#define ColorCh5Light COLOR_LIGHT_5
#define ColorCh6Light COLOR_LIGHT_6
#define ColorCh7Light COLOR_LIGHT_7
#define ColorCh8Light COLOR_LIGHT_8
#define ColorButtonNormal   COLOR_DARK
#define ColorBlue           COLOR_LIGHT
#define EcoColor COLOR_DARK
#define NotEcoColor QColor(0xff,0xff,0xff)

// Нужно избавить от этих дефайнов
#define stylesheetUnclicked " color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0123, 123, 123, 255), stop:1 rgba(0, 0, 0, 255)); "
#define stylesheetclicked "color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255)); "
#define SpinboxstylesheetUnclicked "background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 255)); "
#define Spinboxstylesheetclicked "background-color: rgb(255, 128, 179);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 128, 179, 255), stop:1 rgba(153, 0, 61, 255));color : white;"

#define TOTAL_NUM_DEVICES   6       //максимальное количество плат
#define NUM_CHAN_DEFAULT    4
#define TOTAL_NUM_CHAN      24      //максимальное число подключаемых каналов
#define TOTAL_NUM_MATH      30      //максимальное число подключаемых мат. каналов
#define TOTAL_NUM_FREQ      36      //максимальное число подключаемых частотных каналов
#define NUM_CHAN_IN_4AI     4       // количество каналов в одной плате 4AI
#define NUM_CHAN_IN_6DI6RO  6       // количество каналов в одной плате 6DI6RO
#define NUM_RELAY_IN_6DI6RO 6       // количество твердотельных реле
#define TOTAL_NUM_USTAVKI   128     //максимальное число уставок (уловное ограничение)
#define NUM_STEEL           4
#define NUM_CHAN_IN_STEEL   2
#define NUM_TECHNOLOGIES    8       //количество групп технологий измерения стали
#define MAX_NUM_GROUP       10      //максимальное количество группировок каналов
#define TOTAL_NUM_RELAY     36      //максимальное число подключаемых реле
#define NUM_RELAY_IN_8RP    8       //количество реле в одной плате 8RP

#define BASE_OFFSET_DEVICE      0x4000
#define BASE_OFFSET_CHANNEL_1   0x8000
#define BASE_OFFSET_CHANNEL_2   0x8080
#define BASE_OFFSET_CHANNEL_3   0x8100
#define BASE_OFFSET_CHANNEL_4   0x8180

//макрос определения максимального из двух чисел
#define max( x1, x2) ( ( x1 ) > ( x2 ) ? ( x1 ) : ( x2 ) )
//макрос определения минимального из двух чисел
#define min( x1, x2) ( ( x1 ) < ( x2 ) ? ( x1 ) : ( x2 ) )

//#define new new(/* _NORMAL_BLOCK, */__FILE__, __LINE__)

//------Временно-----------
#ifdef Q_OS_WIN32
#define CONST_SLAVE_STEEL   5
#define CONST_SLAVE_STEEL_2 3
#define CONST_SLAVE_ADC     4
#define CONST_SLAVE_RELAY   6
#define TOTAL_NUM_RELAIS    8
#else
#define CONST_SLAVE_STEEL   5
#define CONST_SLAVE_STEEL_2 3
#define CONST_SLAVE_ADC     4
#define CONST_SLAVE_ADC_2   3
#define CONST_SLAVE_RELAY   6
#define TOTAL_NUM_RELAIS    8
#endif
//-----------------------






// DEFINES_H
