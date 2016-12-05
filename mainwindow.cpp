#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtMath>

int a=0;
int b=0;
int color1rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color2rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color3rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color4rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color5rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color6rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color7rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color8rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color9rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color10rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};

int color11rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color12rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color13rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color14rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color15rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color16rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color17rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color18rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color19rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color20rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};

//y1 offset

QVector<double> xx1;
QVector<double> yy1;

QVector<double> xx2;
QVector<double> yy2;

QVector<double> xx3;
QVector<double> yy3;

QVector<double> xx4;
QVector<double> yy4;

QVector<double> xx5;
QVector<double> yy5;

QVector<double> xx6;
QVector<double> yy6;

QVector<double> xx7;
QVector<double> yy7;

QVector<double> xx8;
QVector<double> yy8;

QVector<double> xx9;
QVector<double> yy9;

QVector<double> xx10;
QVector<double> yy10;

QVector<double> xx11;
QVector<double> yy11;

QVector<double> xx12;
QVector<double> yy12;

QVector<double> xx13;
QVector<double> yy13;

QVector<double> xx14;
QVector<double> yy14;

QVector<double> xx15;
QVector<double> yy15;

QVector<double> xx16;
QVector<double> yy16;

QVector<double> xx17;
QVector<double> yy17;

QVector<double> xx18;
QVector<double> yy18;

QVector<double> xx19;
QVector<double> yy19;

QVector<double> xx20;
QVector<double> yy20;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("/usr/inc/logo.jpg");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    QTimer *timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(updategraph()));

    timer->start(888);
    timer2->start(101);
    timer3->start(99);

    ui->customPlot->xAxis->setRange(-8, 200);
    ui->customPlot->yAxis->setRange(-5, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dial_sliderMoved(int position)
{
    ui->lcdNumber->display(ui->dial->value());

}

void MainWindow::on_lcdNumber_overflow()
{

}

void MainWindow::on_dial_actionTriggered(int action)
{

}

void MainWindow::updateCaption()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->textEdit_2->setText(local.toString());

}


void MainWindow::updatepicture()
{

    QPen graphPen;

    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(xx1, yy1);


    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);


    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx2, yy2);
    graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);



    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx3, yy3);
    graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx4, yy4);
    graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx5, yy5);
    graphPen.setColor(QColor(color5rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx6, yy6);
    graphPen.setColor(QColor(color6rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx7, yy7);
    graphPen.setColor(QColor(color7rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx8, yy8);
    graphPen.setColor(QColor(color8rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx9, yy9);
    graphPen.setColor(QColor(color9rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx10, yy10);
    graphPen.setColor(QColor(color10rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx11, yy11);
    graphPen.setColor(QColor(color11rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx12, yy12);
    graphPen.setColor(QColor(color12rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx13, yy13);
    graphPen.setColor(QColor(color13rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx14, yy14);
    graphPen.setColor(QColor(color14rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx15, yy15);
    graphPen.setColor(QColor(color15rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx16, yy16);
    graphPen.setColor(QColor(color16rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx17, yy17);
    graphPen.setColor(QColor(color17rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx18, yy18);
    graphPen.setColor(QColor(color18rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx19, yy19);
    graphPen.setColor(QColor(color19rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xx20, yy20);
    graphPen.setColor(QColor(color20rgb[0],color5rgb[1],color5rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->replot();
}

void MainWindow::updategraph()
{
    int high=1;
    int low=-1;

    xx1.append(b);
    xx2.append(b);
    xx3.append(b);
    xx4.append(b);
    xx5.append(b);
    xx6.append(b);
    xx7.append(b);
    xx8.append(b);
    xx9.append(b);
    xx10.append(b);

    xx11.append(b);
    xx12.append(b);
    xx13.append(b);
    xx14.append(b);
    xx15.append(b);
    xx16.append(b);
    xx17.append(b);
    xx18.append(b);
    xx19.append(b);

    xx20.append(b);




    yy1.append(ui->dial_2->value()+qrand() % ((high + 1) - low) + low + 0);

    yy2.append(ui->dial_3->value()+qrand() % ((high + 1) - low) + low + 5);

    yy3.append(ui->dial_4->value()+qrand() % ((high + 1) - low) + low + 10);

    yy4.append(ui->dial_5->value()+qrand() % ((high + 1) - low) + low + 15);

    yy5.append(ui->dial_6->value()+qrand() % ((high + 1) - low) + low + 20);

    yy6.append(ui->dial_7->value()+qrand() % ((high + 1) - low) + low + 25);

    yy7.append(ui->dial_8->value()+qrand() % ((high + 1) - low) + low + 30);

    yy8.append(ui->dial_9->value()+qrand() % ((high + 1) - low) + low + 35);

    yy9.append(ui->dial_10->value()+qrand() % ((high + 1) - low) + low + 40);

    yy10.append(ui->dial_11->value()+qrand() % ((high + 1) - low) + low + 45);

    yy11.append(ui->dial_12->value()+qrand() % ((high + 1) - low) + low + 50);

    yy12.append(ui->dial_13->value()+qrand() % ((high + 1) - low) + low + 55);
    yy13.append(ui->dial_14->value()+qrand() % ((high + 1) - low) + low + 60);
    yy14.append(ui->dial_15->value()+qrand() % ((high + 1) - low) + low + 65);
    yy15.append(ui->dial_16->value()+qrand() % ((high + 1) - low) + low + 70);
    yy16.append(ui->dial_17->value()+qrand() % ((high + 1) - low) + low + 75);
    yy17.append(ui->dial_18->value()+qrand() % ((high + 1) - low) + low + 80);
    yy18.append(ui->dial_19->value()+qrand() % ((high + 1) - low) + low + 85);
    yy19.append(ui->dial_20->value()+qrand() % ((high + 1) - low) + low + 90);
    yy20.append(ui->dial_21->value()+qrand() % ((high + 1) - low) + low + 95);


    ///////////////
    b++;

    if (b==200)
    {
        b=0;
        xx1.clear();
        xx2.clear();
        xx3.clear();
        xx4.clear();
        xx5.clear();
        xx6.clear();
        xx7.clear();
        xx8.clear();
        xx9.clear();
        xx10.clear();

        xx11.clear();
        xx12.clear();
        xx13.clear();
        xx14.clear();
        xx15.clear();
        xx16.clear();
        xx17.clear();
        xx18.clear();
        xx19.clear();

        xx20.clear();

        yy1.clear();
        yy2.clear();
        yy3.clear();
        yy4.clear();
        yy5.clear();

        yy6.clear();
        yy7.clear();
        yy8.clear();
        yy9.clear();
        yy10.clear();

        yy11.clear();
        yy12.clear();
        yy13.clear();
        yy14.clear();
        yy15.clear();
        yy16.clear();
        yy17.clear();
        yy18.clear();
        yy19.clear();
        yy20.clear();

    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{/*
    QPainter painter(this);

    QPen pen1(Qt::black);
    pen1.setWidth(6);

painter.rotate(-45);
    QRect rec(20,60,20,60);
    painter.setPen(pen1);
    painter.drawRect(rec);*/

}

void MainWindow::on_pushButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushButton_2_clicked()
{

    ui->customPlot->clearGraphs();
    int high=-1;
    int low=1;


    for (int i=0; i<b; i++)
    {
        yy1[i] += qrand() % ((high + 1) - low) + low + 0;
        yy2[i] += qrand() % ((high + 1) - low) + low + 0;
        yy3[i] += qrand() % ((high + 1) - low) + low + 0;
        yy4[i] += qrand() % ((high + 1) - low) + low + 0;
        yy5[i] += qrand() % ((high + 1) - low) + low + 0;
        yy6[i] += qrand() % ((high + 1) - low) + low + 0;
        yy7[i] += qrand() % ((high + 1) - low) + low + 0;
        yy8[i] += qrand() % ((high + 1) - low) + low + 0;
        yy9[i] += qrand() % ((high + 1) - low) + low + 0;
        yy10[i] += qrand() % ((high + 1) - low) + low + 0;

        yy11[i] += qrand() % ((high + 1) - low) + low + 0;
        yy12[i] += qrand() % ((high + 1) - low) + low + 0;
        yy13[i] += qrand() % ((high + 1) - low) + low + 0;
        yy14[i] += qrand() % ((high + 1) - low) + low + 0;
        yy15[i] += qrand() % ((high + 1) - low) + low + 0;
        yy16[i] += qrand() % ((high + 1) - low) + low + 0;
        yy17[i] += qrand() % ((high + 1) - low) + low + 0;
        yy18[i] += qrand() % ((high + 1) - low) + low + 0;
        yy19[i] += qrand() % ((high + 1) - low) + low + 0;
        yy20[i] += qrand() % ((high + 1) - low) + low + 0;
    }
}

void MainWindow::on_pushButton_2_pressed()
{
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    QFile filedir("/sys/class/gpio/gpio69/direction");

    filedir.open(QIODevice::WriteOnly);
    QTextStream outdir(&filedir);

    outdir << "out";
    filedir.close();

    QFile file("/sys/class/gpio/gpio69/value");

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    if (ui->checkBox->checkState())
    {
        out << "1";
    }

    else
    {
        out << "0";

        //qDebug() << ui->checkBox->checkState();
    }
    file.close();
}

