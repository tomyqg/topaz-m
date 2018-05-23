#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
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

#define CURRENT_VER "0.6" //версия ПО
#define MYD
//#define Demo // раскоментить если делаем демо программу на РС

#ifndef Q_OS_WIN32
#define comportname "/dev/ttyO1" // com port for MYD board
#define pathtofiles  "/opt/"
#define pathtosystemoptions  "/opt/systemoptions.txt"
#define pathtoarchive  "/opt/archive.txt"
#define pathtoarchivedata  "/opt/archive.dat"
#define pathtooptions  "/opt/options.txt"
#define pathtolog  "/opt/Log/"
#define pathtomessages  "/opt/Log.txt"
#define pathtologotip  "/opt/logotpchel.png"
#define pathtoqrcodetip "/opt/qr-code.jpg"
#define pathtohilimico  "/opt/hilim.png"
#define pathtolowlimico  "/opt/lowlim.png"
#define pathtomodbusconfigfile  "/opt/MODBusConfigFile.txt"
#define uartsleep DelayMsec(80);
#define threadsleep DelayMsec(100);
#define longsleep DelayMsec(1000);

#define ETOUT 110   /* Connection timed out */

#define IS_LINUX

#else
#define comportname "COM3"
#define uartsleep Sleep(80);
#define longsleep Sleep(1000);

#ifndef Demo
#define pathtofiles  "C:/Work/"
#define pathtosystemoptions  "C:/Work/systemoptions.txt"
#define pathtooptions  "C:/Work/options.txt"
#define pathtolog  "C:/Work/txtes/"
#define pathtomodbusconfigfile  "C:/Work/MODBusConfigFile.txt"
#define pathtomessages  "C:/Work/Log.txt"
#define pathtoarchive  "C:/Work/archive.txt"
#define pathtoarchivedata  "C:/Work/archive.dat"
#define pathtologotip  "C:/Work/logotpchel.png"
#define pathtoqrcodetip "C:/Work/qr-code.jpg"
#define pathtohilimico  "C:/Work/hilim.png"
#define pathtolowlimico  "C:/Work/lowlim.png"
#else

#define pathtofiles  QCoreApplication::applicationDirPath()
#define pathtosystemoptions  QCoreApplication::applicationDirPath() + "/systemoptions.txt"
#define pathtooptions  QCoreApplication::applicationDirPath() + "/options.txt"
#define pathtolog  QCoreApplication::applicationDirPath() + "/txtes/"
#define pathtomodbusconfigfile  QCoreApplication::applicationDirPath() + "/MODBusConfigFile.txt"
#define pathtomessages  QCoreApplication::applicationDirPath() + "/Log.txt"
#define pathtoarchive  QCoreApplication::applicationDirPath() + "/archive.txt"
#define pathtoarchivedata  QCoreApplication::applicationDirPath() + "C:/Work/archive.dat"
#define pathtologotip  QCoreApplication::applicationDirPath() + "/logotpchel.png"
#define pathtoqrcodetip QCoreApplication::applicationDirPath() + "/qr-code.jpg"
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
#define ValuesUpdateTimer 200        // время в мсек для обновления значений отрисовываемых на графике
#define GraphicsUpdateTimer 200      // время в мсек для обновления самого графика
#define ArchiveUpdateTimer 30000     // время архивации на флешку в мсекундах
#define DateLabelUpdateTimer 1000    // время обновления времени
//#define LogUpdTimer 10000             // время обновления журнала сообщений (Log.txt)
#define XRange 300                   // изначальный размер графика по иксу в 1 сторону
#define YRange 20                   // изначальный размер графика по игреку в 1 сторону
#define GraphWidthinPixels 2         // толщина графика линий в пикселях
#define UstavkiUpdateTimer 500      // период мсек обновления состояний уставок
#define ParsingReceiveTrans 5       // период парсинга входящих транзакций от Workera в мсек

#endif

//#define stylesheetclicked "background-color: rgb(0, 108, 217);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 108, 217, 255), stop:1 rgba(0, 170, 255, 255));color : white;"
#define stylesheetUnclicked " color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0123, 123, 123, 255), stop:1 rgba(0, 0, 0, 255)); "
//#define stylesheetclicked "background-color: rgb(135,210,240);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(207,232,252, 255), stop:1 rgba(0, 170, 255, 255));color : white;"
#define stylesheetclicked "color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255)); "
#define SpinboxstylesheetUnclicked "background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 255)); "
#define Spinboxstylesheetclicked "background-color: rgb(255, 128, 179);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 128, 179, 255), stop:1 rgba(153, 0, 61, 255));color : white;"

#define StyleSheetCh1 "color: rgb(255, 255, 255); background-color: #1cb99a;" // #33cccc;"
#define StyleSheetCh2 "color: rgb(255, 255, 255); background-color: #6666ff;"
#define StyleSheetCh3 "color: rgb(255, 255, 255); background-color: #e84c3d;"    //#cc3333;"
#define StyleSheetCh4 "color: rgb(255, 255, 255); background-color: #2b3e4c;"   //#9900cc;"
#define StyleSheetCh1Light "background-color: #57e9c5;" //#00ffaa;"
#define StyleSheetCh2Light "background-color: #9999ff;"
#define StyleSheetCh3Light "background-color: #eb9f85;" //#ff6655;"
#define StyleSheetCh4Light "background-color: #809fbc;"

#define ButtonStyleNormal "color: #FFFFFF; background-color: #2c3d4d; border: 0px;"
#define ButtonStyleDown "color: #FFFFFF; background-color: #1c2d3d; border: 0px;"

#define ColorCh1 QColor(0x1c,0xb9,0x9a) //QColor(0x33,0xcc,0xcc)
#define ColorCh2 QColor(0x66,0x66,0xff)
#define ColorCh3 QColor(0xe8,0x4c,0x3d)
#define ColorCh4 QColor(0x2b,0x3e,0x4c)
#define ColorCh1Light QColor(0x57,0xe9,0xc5)    //QColor(0x00,0xff,0xaa)
#define ColorCh2Light QColor(0x99,0x99,0xff)
#define ColorCh3Light QColor(0xeb,0x9f,0x85)
#define ColorCh4Light QColor(0x80,0x9f,0xbc)

#define ColorButtonNormal   QColor(0x2b,0x3e,0x4c)
#define ColorButtonDown     QColor(0x1b,0x2e,0x3c)
#define ColorBlue           QColor(14,210,226)


#define EcoColor QColor(0x00,0x00,0x4d)
#define NotEcoColor QColor(0xff,0xff,0xff)

#define TOTAL_NUM_USTAVKI   4

#define BASE_OFFSET_DEVICE      0x4000
#define BASE_OFFSET_CHANNEL_1   0x8000
#define BASE_OFFSET_CHANNEL_2   0x8080
#define BASE_OFFSET_CHANNEL_3   0x8100
#define BASE_OFFSET_CHANNEL_4   0x8180

#define max( x1, x2) ( ( x1 ) > ( x2 ) ? ( x1 ) : ( x2 ) )

//#define new new(/* _NORMAL_BLOCK, */__FILE__, __LINE__)

// DEFINES_H
