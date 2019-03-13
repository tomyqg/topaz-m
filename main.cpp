#include "mainwindow.h"
//#include "options.h"
#include "keyboard.h"
#include "stackedoptions.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
#include <QDir>
#include "httplistener.h"
#include "templatecache.h"
#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "src/requestmapper.h"

//By Andrey Shmelev e-mail: andrshmel@gmail.com +79227112858
// Edit: Vagiz Abdulin


/** Cache for template files */
TemplateCache* templateCache;

/** Storage for session cookies */
HttpSessionStore* sessionStore;

/** Controller for static files */
StaticFileController* staticFileController;

/** Redirects log messages to a file */
FileLogger* logger;

/** Search the configuration file */
QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName(appName+".ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append("C:/Work");
    searchList.append(binDir+"/../DVRQT5Project");
    searchList.append(binDir+"/../../DVRQT5Project");
    searchList.append("/opt");
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../../etc"); // for development without shadow build
    searchList.append(binDir+"/../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(binDir+"/../../../../../"+appName+"/etc"); // for development with shadow build
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            // found
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return 0;
}

int main(int argc, char *argv[])
{

#ifndef Q_OS_WIN32
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //    QTextCodec::setCodecForTr(codec);
    //    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
#endif

    QApplication app(argc, argv);

    app.setApplicationName("Multigraph");
    app.setOrganizationName("Teplopribor-Sensor");
    app.setOrganizationDomain("tpchel.ru");

    QString configFileName = searchConfigFile();

//    QTranslator translator;

//    // Предшествующее окно
//    QSplashScreen splash(QPixmap(pathtologotip));
//    splash.show();

    MainWindow w;
    // Передача размеров экрана в приложение
    w.desktopSize(app.desktop()->height(), app.desktop()->width());
    //делаем ресайз
    w.resizeWindow(w,w.GetWindowWidthPixels(),w.GetWindowHeightPixels());
    w.show();

    // Configure template loader and cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    templateSettings->beginGroup("templates");
    templateCache=new TemplateCache(templateSettings,&app);

    // Configure session store
    QSettings* sessionSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings,&app);

    // Configure static file controller
    QSettings* fileSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    fileSettings->beginGroup("docroot");
    staticFileController=new StaticFileController(fileSettings,&app);

    QNetworkProxyFactory::setUseSystemConfiguration(false); //не использовать системный прокси

    // Configure and start the TCP listener
    QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,&app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);


    return app.exec();
}



