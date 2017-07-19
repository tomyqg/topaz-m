#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "channel1.h"
#include "uartdriver.h"

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
    MessageWrite mr1 ("Programm Closed");
    mr1.WriteAllLogToFile();
    delete ui;
}

QColor MainWindow::GetChannel1Color()
{
    return Channel1Color;
}

void MainWindow::SetChannel1Color(QColor newcolor)
{
    Channel1Color = newcolor;
}

void MainWindow::SetChannel2Color(QColor newcolor)
{
    Channel2Color = newcolor;
}

QColor MainWindow::GetChannel2Color()
{
    return Channel2Color;
}

void MainWindow::SetChannel3Color(QColor newcolor)
{
    Channel3Color = newcolor;
}

QColor MainWindow::GetChannel3Color()
{
    return Channel3Color;
}

void MainWindow::SetChannel4Color(QColor newcolor)
{
    Channel4Color = newcolor;
}

QColor MainWindow::GetChannel4Color()
{
    return Channel4Color;
}

void MainWindow::updateDateLabel()
{
    DateUpdate();
}

void MainWindow::on_pushButton_clicked()
{
    PowerOff();
}

void MainWindow::on_pushButton_2_clicked()
{
    OpenOptionsWindow();
}

void MainWindow::on_pushButton_3_clicked()
{
    CloseApplication();
}

void MainWindow::on_pushButton_4_clicked()
{
    OpenMessagesWindow();
}

void MainWindow::on_radioButton_clicked()
{
    InitProcessorMaxFreq();
}

void MainWindow::on_radioButton_2_clicked()
{
    InitProcessorMinFreq();
}

void  MainWindow::updateText( const QString text ) // этот слот обновляет текстовое окно, когда мы эмитируем сигнал
{
    ui->textEdit_2->setText(text);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->MessagesWidget && event->type() == QEvent::Paint) {

        if (needupdatePainter ==1)
        {PaintOnWidget();
            needupdatePainter = 0;}
    }

    if (watched == ui->MessagesWidget && event->type() == QEvent::MouseButtonPress) {
        ReactOnTouch();
    }

    if (watched == ui->MessagesWidget && event->type() == QEvent::MouseMove) {

        ReactOnMouseSlide();
    }

    return true;
}

void MainWindow::ShowMessageBox (QString title,QString message)
{
    QMessageBox::information(this, title, message);
}


void MainWindow::resizeWindow(QWidget & qw, qreal mratio)
{

    // ratio to calculate correct sizing
    /*qreal mratio_bak = mratio;

        if(qw.m_ratio != 0)
            mratio /= qw.m_ratio;

        // this all was done so that if its called 2 times with ratio = 2, total is not 4 but still just 2 (ratio is absolute)
        qw.m_ratio = mratio_bak;
        */
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
        //qw.resizeFont(w);
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
/*
 *
void MainWindow::resizeWidgets(QObject & qobj, qreal xratio, qreal yratio)
{
    QList<QWidget *> widgets = qobj.findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {

        QRect g = widget->geometry();

        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);

        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
        //qw.resizeFont(w);
//        qDebug() << widget;
    }
    return;
}*/

void MainWindow::resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution)
{
    qreal xratio = xresolution / 1280;
    qreal yratio = yresolution / 800;

    QList<QWidget *> widgets = qobj.findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {

        QRect g = widget->geometry();

        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);

        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
        //qw.resizeFont(w);
//        qDebug() << widget;
    }
    return;
}


int MainWindow::GetWindowHeightPixels()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    return height;
}

int MainWindow::GetWindowWidthPixels()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    return width;
}


