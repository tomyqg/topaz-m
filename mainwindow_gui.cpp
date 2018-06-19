#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "Channels/channelOptions.h"
#include "uartdriver.h"
#include "filemanager.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPen>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>

QString inputstr = "";
QDateTime start(QDateTime::currentDateTime());
QString MainWindow::startdate = start.toString("dd/MM/yy");
QString MainWindow::starttime = start.toString("hh:mm:ss");
QString MainWindow::endtime = "";
QVector<QDateTime> MainWindow::Dates;


extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindowInitialization();
}

MainWindow::~MainWindow()
{
//    messwrite.LogAddMessage("Programm Closed");
//    messwrite.WriteAllLogToFile();
    logger->addMess("Programm Closed");
    delete timeUpdUst;
    delete ui;
}


void MainWindow::updateDateLabel()
{
    DateUpdate();
}

void MainWindow::on_WorkButton_clicked()
{
//    OpenWorkWindow();
    dMenu * menu = new dMenu();
    menu->addChannels(listCh, ustavkaObjectsList);
    connect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    menu->selectPageWork();
    menu->exec();
    disconnect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    menu->deleteLater();
    sendConfigChannelsToSlave();
}

void MainWindow::on_ArchiveButton_clicked()
{
//    OpenArchiveWindow();
    dialogSetingsChannel = new dSettings(listCh, ustavkaObjectsList, 1, 2);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
}


void MainWindow::on_EcoCheckBox_toggled(bool checked)
{
    SetEcoMode(checked);

    switch (checked) {
    case 0:
        ui->EcoCheckBox->setStyleSheet(" color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0123, 123, 123, 255), stop:1 rgba(0, 0, 0, 255)); ");
        break;
    case 1:
        ui->EcoCheckBox->setStyleSheet("background-color: rgb(0, 108, 217);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 108, 217, 255), stop:1 rgba(0, 170, 255, 255));color : white;");
        break;
    default:
        break;
    }

    ChangePalette(checked);


}

//void MainWindow::on_timeButton_clicked()
//{
//    ++dateindex;
//    if (dateindex>=datestrings.length())
//        dateindex = 0 ;

//    QDateTime local(QDateTime::currentDateTime());

//    ui->timeButton->setStyleSheet("background-color: rgb(0, 108, 217);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 108, 217, 255), stop:1 rgba(0, 170, 255, 255));color : white;");
//    ui->time_label->setText(local.date().toString(datestrings.at(dateindex) ) + local.time().toString());
//}

void MainWindow::on_pushButton_2_clicked()
{
//    OpenOptionsWindow(0);

    dMenu * menu = new dMenu();
    menu->addChannels(listCh, ustavkaObjectsList);
    connect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    menu->exec();
    disconnect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    menu->deleteLater();
    sendConfigChannelsToSlave();
}

void MainWindow::updateSystemOptions()
{
    cFileManager::readSystemOptionsFromFile(pathtosystemoptions, &systemOptions);
}

void MainWindow::on_pushButton_4_clicked()
{
//    OpenMessagesWindow();
    dialogSetingsChannel = new dSettings(listCh, ustavkaObjectsList, 1, 1);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
}


void  MainWindow::destroyedslot(QObject *)
{
    //    ui->textEdit_2->setText(text);
    //    ui->textEdit_2->setText("Destroyed");
    //    qDebug() << "destroyed slot";
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->MessagesWidget && event->type() == QEvent::Paint)
    {
        if (needupdatePainter == 1)
        {
            PaintOnWidget();
            needupdatePainter = 0;
        }
    }

    if (watched == ui->MessagesWidget && event->type() == QEvent::MouseButtonPress) {
        ReactOnTouch();
    }

    if (watched == ui->MessagesWidget && event->type() == QEvent::MouseMove) {

        ReactOnMouseSlide();
    }

    if ( (event->type() == QEvent::MouseButtonPress)&& ( QString::fromLatin1(watched->metaObject()->className()) == "QPushButton" ))//)inherits("QPushButton")) // ("QWidgetWindow"))
    {

        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит

            if (widget->objectName() == watched->property("objectName"))
            {
                widget->setStyleSheet("color:#FFFFFF;background-color:rgb(" \
                                      + QString::number(ColorButtonNormal.red() - 20) + "," \
                                      + QString::number(ColorButtonNormal.green() - 20) + "," \
                                      + QString::number(ColorButtonNormal.blue() - 20) + ");" \
                                      + "border: 0px;");
            }
        }
    }


    if ( (event->type() == QEvent::MouseButtonRelease)&& ( QString::fromLatin1(watched->metaObject()->className()) == "QPushButton" ))//)inherits("QPushButton")) // ("QWidgetWindow"))
    {

        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит

            if (widget->objectName() == watched->property("objectName"))
            {
                widget->setStyleSheet("color:#FFFFFF;background-color:rgb(" \
                                      + QString::number(ColorButtonNormal.red()) + "," \
                                      + QString::number(ColorButtonNormal.green()) + "," \
                                      + QString::number(ColorButtonNormal.blue()) + ");" \
                                      + "border: 0px;");
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MainWindow::ShowMessageBox (QString title,QString message)
{
    QMessageBox::information(this, title, message);
}

void MainWindow::resizeWindow(QWidget & qw, qreal mratio)
{
    QLayout * ql = qw.layout();

    if (ql == NULL)
        return;

    QWidget * pw = ql->parentWidget();

    if (pw == NULL)
        return;

    QList<QLayout *> layouts;

    foreach(QWidget *w, pw->findChildren<QWidget*>())
    {
        QRect g = w->geometry();
        w->setMinimumSize(w->minimumWidth() * mratio, w->minimumHeight() * mratio);
        w->setMaximumSize(w->maximumWidth() * mratio, w->maximumHeight() * mratio);
        w->resize(w->width() * mratio, w->height() * mratio);
        w->move(QPoint(g.x() * mratio, g.y() * mratio));
    }

    foreach(QLayout *l, pw->findChildren<QLayout*>())
    {
        if(l != NULL && !(l->objectName().isEmpty()))
            layouts.append(l);
    }

    foreach(QLayout *l, layouts) {
        QMargins m = l->contentsMargins();

        m.setBottom(m.bottom() * mratio);
        m.setTop(m.top() * mratio);
        m.setLeft(m.left() * mratio);
        m.setRight(m.right() * mratio);
        l->setContentsMargins(m);
        l->setSpacing(l->spacing() * mratio);

        if (l->inherits("QGridLayout")) {
            QGridLayout* gl = ((QGridLayout*)l);

            gl->setHorizontalSpacing(gl->horizontalSpacing() * mratio);
            gl->setVerticalSpacing(gl->verticalSpacing() * mratio);
        }
    }

    QMargins m = qw.contentsMargins();

    m.setBottom(m.bottom() * mratio);
    m.setTop(m.top() * mratio);
    m.setLeft(m.left() * mratio);
    m.setRight(m.right() * mratio);

    // resize accordingly main window
    qw.resize(qw.width() * mratio, qw.height() * mratio);
    qw.setContentsMargins(m);
    qw.adjustSize();
}

void MainWindow::resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution)
{
    qreal xratio = xresolution / 1024;
    qreal yratio = yresolution / 768;

    QList<QWidget *> widgets = qobj.findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {
        QRect g = widget->geometry();
        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);
        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
    }
    return;
}

void MainWindow::resizeSelf(qreal xresolution, qreal yresolution)
{
    qreal xratio = xresolution / GetWindowWidthPixels();
    qreal yratio = yresolution / GetWindowHeightPixels();

    SetWindowHeightPixels(yresolution);
    SetWindowWidthPixels(xresolution);

    QList<QWidget *> widgets = findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {
        QRect g = widget->geometry();
        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);
        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
    }
    return;
}

int MainWindow::GetWindowHeightPixels()
{
    return windowheight;
}

int MainWindow::GetWindowWidthPixels()
{
    return windowwidth;
}

int MainWindow::GetMonitorWidthPixels()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    Options::MonitorResolution = QString::number(width) + "x"+QString::number(height) ;
    return width;
}

int MainWindow::GetMonitorHeightPixels()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    return height;
}

void MainWindow::SetWindowWidthPixels(int neww)
{
    windowwidth = neww;
}

void MainWindow::SetWindowHeightPixels(int newh)
{
    windowheight = newh;
}

//void MainWindow::on_bWriteTypeSignal_clicked()
//{
//    uint32_t tmp = ui->setTypeSignal->text().toInt();
////    typeSign1.dir = DIR_W;
////    typeSign1.param = device.channel0.SignalType;
////    typeSign1.id = 0;
////    typeSign1.volInt = tmp;
//    Transaction trans(Transaction::W, 1, 32781, tmp);
//    emit sendTransToWorker(trans);
//}

//void MainWindow::on_sendI2C_clicked()
//{

//}

//void MainWindow::on_readI2C_clicked()
//{
//    cDriverI2C i2c_1;
//    i2c_1.init(1);
//    i2c_1.detect();
//    cDriverI2C i2c_2;
//    i2c_2.init(2);
//    i2c_2.detect();
//}


//void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
//{
////        randVal[0] = arg1;
//        channel1.SetCurrentChannelValue(arg1);
//        ui->wBar_1->setVolue(arg1);
//}

void MainWindow::openSettingsChannel(int num)
{

    //проверка на наличие такого номера канала
    if((num <= 0) || (num > listCh.size())) return;

    dialogSetingsChannel = new dSettings(listCh, ustavkaObjectsList, num);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
    sendConfigChannelsToSlave();
}

