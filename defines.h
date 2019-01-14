#include <QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "options.h"
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

#define CURRENT_VER "1.62" //версия ПО
#define SOFTWARE_REVISION "0001"  //ревизия ПО

#define MYD
//#define Demo // раскоментить если делаем демо программу на РС

#ifndef Q_OS_WIN32
#define comportname "/dev/ttyO1" // com port for MYD board
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
#define uartsleep DelayMsec(80);
#define threadsleep DelayMsec(100);
#define longsleep DelayMsec(1000);
#define ETH_NAME    "eth0"

#define ETOUT 110   /* Connection timed out */

#define IS_LINUX

#else
#define comportname "COM6"
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

#define BADCRCCODE -9999
#define CONNECTERRORCODE -9998

#define ConnectionErrorMessage "ош.соед"
#define CRCErrorMessage "ош.crc"
#define ObryvErrorMessage "обрыв"

#define Font "Open Sans"
#define NaNMessage "nan"
#define OKMessage "норма"
#define AlertSign ""
#define MathString "math "

#define MultiThread
#define ValuesUpdateTimer 200        // период чтения данных в архив
#define GraphicsUpdateTimer 100      // время в мсек для обновления самого графика
#define ArchiveUpdateTimer 30000     // время архивации на флешку в мсекундах
#define DateLabelUpdateTimer 500    // время обновления времени
//#define LogUpdTimer 10000             // время обновления журнала сообщений (Log.txt)
#define XRange 300                   // изначальный размер графика по иксу в 1 сторону
#define YRange 20                   // изначальный размер графика по игреку в 1 сторону
#define GraphWidthinPixels 2         // толщина графика линий в пикселях
#define UstavkiUpdateTimer 500      // период мсек обновления состояний уставок
#define ParsingReceiveTrans 1       // период парсинга входящих транзакций от Workera в мсек
#define UpdateSteelTime 500
#define timeUpdateDevices 1000       // время обновления комплекта плат

#endif


/* Цветовая палитра */
// Базовые цвета. Только их использовать во всей программе. Чёрный и белый само собой
#define COLOR_1 QColor(0x1c,0xb9,0x9a)
#define COLOR_2 QColor(0x66,0x66,0xff)
#define COLOR_3 QColor(0xe8,0x4c,0x3d)
#define COLOR_4 QColor(0x2b,0x3e,0x4c)
#define COLOR_LIGHT_1 QColor(0x57,0xe9,0xc5)
#define COLOR_LIGHT_2 QColor(0x99,0x99,0xff)
#define COLOR_LIGHT_3 QColor(0xeb,0x9f,0x85)
#define COLOR_LIGHT_4 QColor(0x80,0x9f,0xbc)
#define COLOR_DARK  QColor(0x2b,0x3e,0x4c)
#define COLOR_LIGHT QColor(0x0e,0xd2,0xe2)
#define COLOR_GRAY  QColor(0xe6,0xe6,0xe6)

// Цвета элементов
#define ColorCh1 COLOR_1
#define ColorCh2 COLOR_2
#define ColorCh3 COLOR_3
#define ColorCh4 COLOR_4
#define ColorCh1Light COLOR_LIGHT_1
#define ColorCh2Light COLOR_LIGHT_2
#define ColorCh3Light COLOR_LIGHT_3
#define ColorCh4Light COLOR_LIGHT_4
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
#define TOTAL_NUM_USTAVKI   8
#define NUM_STEEL           4
#define NUM_TECHNOLOGIES    8       //количество групп технологий измерения стали
#define MAX_NUM_GROUP       10      //максимальное количество группировок каналов

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
