#include "mainwindow.h"
#include "options.h"
#include "keyboard.h"
#include "stackedoptions.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>


//By Andrey Shmelev e-mail: andrshmel@gmail.com +79227112858
// Edit: Vagiz Abdulin

//void loadModules(QSplashScreen* psplash)
//{
//    QTime time;
//    time.start();

//    for (int i = 0; i < 100; ) {
//        if (time.elapsed() > 100) {
//            time.start();
//            ++i;
//        }
//        psplash->showMessage("Loading modules: "
//                             + QString::number(i) + "%",
//                             Qt::AlignHCenter | Qt::AlignBottom,
//                             Qt::black
//                            );
//    }
//}


int main(int argc, char *argv[])
{

#ifndef Q_OS_WIN32
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //    QTextCodec::setCodecForTr(codec);
    //    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
#endif

    QApplication a(argc, argv);

//    QTranslator translator;

//    // Предшествующее окно
//    QSplashScreen splash(QPixmap(pathtologotip));
//    splash.show();

    MainWindow w;
    // Передача размеров экрана в приложение
    w.desktopSize(a.desktop()->height(), a.desktop()->width());
    //делаем ресайз
    w.resizeWindow(w,w.GetWindowWidthPixels(),w.GetWindowHeightPixels());
    w.show();

//    splash.raise();
//    splash.activateWindow();
//    //функция загрузки
//    loadModules(&splash);
//    splash.finish(&w);

    return a.exec();
}



