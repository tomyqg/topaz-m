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

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPen>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>
#include <QMetaType>

#ifndef DEFINES_H
#define DEFINES_H

#define CURRENT_VER "0.55" //версия ПО

//#define LinuxBoard // раздефайнить если мы ходим скомпилировать на плату Linux
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
#define uartsleep DelayMsec(160);
#define threadsleep DelayMsec(100);
#define longsleep DelayMsec(1000);

#else
#define comportname "COM3"
#define uartsleep Sleep(100);
#define longsleep Sleep(1000);
#define pathtofiles  "C:/Work/"
#define pathtosystemoptions  "C:/Work/systemoptions.txt"
#define pathtooptions  "C:/Work/options.txt"
#define pathtolog  "C:/Work/txtes/"
#define pathtomodbusconfigfile  "C:/Work/MODBusConfigFile.txt"
#define pathtomessages  "C:/Work/Log.txt"
#define pathtoarchive  "C:/Work/archive.txt"
#define pathtologotip  "C:/Work/logo.jpg"
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

#define Font "Open Sans Semibold"
#define NaNMessage "nan"
#define OKMessage "OK"
#define AlertSign "!"
#define MathString "math "

#define MultiThread
#define ValuesUpdateTimer 100
#define GraphicsUpdateTimer 300
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
