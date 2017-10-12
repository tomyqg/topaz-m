#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channel1.h"
#include "uartdriver.h"
#include "worker.h"
#include "src/modbus-private.h"
#include "qextserialenumerator.h"


#ifndef DEFINES_H
#define DEFINES_H

#define CURRENT_VER "0.6" //версия ПО
#define MYD


#ifndef Q_OS_WIN32
#define comportname "/dev/ttyO1" // com port for MYD board
#define pathtofiles  "/opt/"
#define pathtosystemoptions  "/opt/systemoptions.txt"
#define pathtoarchive  "/opt/archive.txt"
#define pathtooptions  "/opt/options.txt"
#define pathtolog  "/opt/Log/"
#define pathtomessages  "/opt/Log.txt"
#define pathtologotip  "/opt/logo.jpg"
#define pathtomodbusconfigfile  "/opt/MODBusConfigFile.txt"
#define uartsleep DelayMsec(80);
#define threadsleep DelayMsec(100);
#define longsleep DelayMsec(1000);

#else
#define comportname "COM3"
#define uartsleep Sleep(80);
#define longsleep Sleep(1000);

//#define Demo

#ifndef Demo
#define pathtofiles  "C:/Work/"
#define pathtosystemoptions  "C:/Work/systemoptions.txt"
#define pathtooptions  "C:/Work/options.txt"
#define pathtolog  "C:/Work/txtes/"
#define pathtomodbusconfigfile  "C:/Work/MODBusConfigFile.txt"
#define pathtomessages  "C:/Work/Log.txt"
#define pathtoarchive  "C:/Work/archive.txt"
#define pathtologotip  "C:/Work/logo.jpg"
#else

#define pathtofiles  QCoreApplication::applicationDirPath()
#define pathtosystemoptions  QCoreApplication::applicationDirPath() + "/systemoptions.txt"
#define pathtooptions  QCoreApplication::applicationDirPath() + "/options.txt"
#define pathtolog  QCoreApplication::applicationDirPath() + "/txtes/"
#define pathtomodbusconfigfile  QCoreApplication::applicationDirPath() + "/MODBusConfigFile.txt"
#define pathtomessages  QCoreApplication::applicationDirPath() + "/Log.txt"
#define pathtoarchive  QCoreApplication::applicationDirPath() + "/archive.txt"
#define pathtologotip  QCoreApplication::applicationDirPath() + "/logo.jpg"
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
#define ArchiveUpdateTimer 30000     // время архивации на флешку в секундах
#define DateLabelUpdateTimer 1000    // время обновления времени
#define XRange 300                   // изначальный размер графика по иксу в 1 сторону
#define YRange 180                   // изначальный размер графика по игреку в 1 сторону
#define GraphWidthinPixels 2         // толщина графика линий в пикселях

#define rect1coords smallrectinglewidth*3-5, otstupsverhu3, smallrectinglewidth, smallrectingleheight         // толщина графика линий в пикселях
#define rect2coords smallrectinglewidth*3-5, otstupsverhu2, smallrectinglewidth, smallrectingleheight         // толщина графика линий в пикселях
#define rect3coords smallrectinglewidth*3-5, otstupsverhu1, smallrectinglewidth, smallrectingleheight         // толщина графика линий в пикселях
#define rect4coords smallrectinglewidth*3-5, otstupsverhu, smallrectinglewidth, smallrectingleheight         // толщина графика линий в пикселях

#endif // DEFINES_H

#define stylesheetUnclicked " color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0123, 123, 123, 255), stop:1 rgba(0, 0, 0, 255)); "
#define stylesheetclicked "background-color: rgb(0, 108, 217);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 108, 217, 255), stop:1 rgba(0, 170, 255, 255));color : white;"


#define EcoColor QColor(0x00,0x00,0x4d)
#define NotEcoColor QColor(0xff,0xff,0xff)
