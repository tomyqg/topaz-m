#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"


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
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QtScript/QScriptEngine>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QMutex>

int a=0;
int b=0;
int color1rgb[]={153, 0, 153}; //фиолетовый темный
int color2rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color3rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color4rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};


QVector<double> xx1;
QVector<double> yy1;
QVector<double> yy2;
QVector<double> yy3;
QVector<double> yy4;

void MainWindow::updategraph()
{
    UartDriver UD;

    xx1.append(b);

    double argument = ui->dial->value();

    yy1.append(UD.readchannelvalue(0));
    yy2.append(UD.readchannelvalue(1));
    yy3.append(UD.readchannelvalue(2));
    yy4.append(UD.readchannelvalue(3));

    b++;

    if (b>=300&&b<900)
    {
        ui->customPlot->xAxis->setRange(b-300, b+300);
    }
    
    if (b==1200)
    {
        ui->customPlot->xAxis->setRange(0, 600);
        
        b=0;
        xx1.clear();
        yy1.clear();
        yy2.clear();
        yy3.clear();
        yy4.clear();
    }
}

void MainWindow::updatepicture()
{
    
    QByteArray arr;
    
    QPen graphPen;

    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(xx1, yy1);
    graphPen.setWidth(2);
    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();
    
    {ui->customPlot->graph()->setData(xx1, yy2);
        graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
        graphPen.setWidth(2);
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx1, yy3);
        graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
        graphPen.setWidth(2);
        ui->customPlot->graph()->setPen(graphPen);}
    
    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx1, yy4);
        graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
        graphPen.setWidth(2);
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false);
    //    qDebug() << ui->customPlot->xAxis->tickStep();
    //    ui->customPlot->xAxis->set
    ui->customPlot->xAxis->setTickStep(60); // 60 secs btw timestamp
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);

    ui->customPlot->xAxis->setDateTimeSpec(Qt::OffsetFromUTC);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");// dd.MM.yyyy
    //    ui->customPlot->rescaleAxes(); // если мы хотим всегда график растянутый на всю длину и высоту
    ui->customPlot->replot();
}


void MainWindow::UpdateDataChannel1()
{
    double currentdata = ui->dial->value() + 5;
    UartDriver UD;
    UD.writechannelvalue(0,currentdata);
    if ((currentdata>=ch1.GetState1Value() ) && ( ch1.HighState1Setted == false ))
    {
        ch1.LowState1Setted = false;
        ui->listWidget->addItem(ch1.GetState1HighMessage());
        ui->listWidget->scrollToBottom();
        ch1.HighState1Setted = true;
        MessageWrite mr (ch1.GetChannelName() + ":" + ch1.GetState1HighMessage());
    }

    if ((currentdata<ch1.GetState1Value() ) && ( ch1.LowState1Setted == false ))
    {
        ch1.LowState1Setted = true;
        ui->listWidget->addItem(ch1.GetState1LowMessage());
        ui->listWidget->scrollToBottom();
        ch1.HighState1Setted = false;
        MessageWrite mr (ch1.GetChannelName() + ":" + ch1.GetState1LowMessage());
    }

    int period = ch1.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
}

void MainWindow::UpdateDataChannel2()
{
    UartDriver UD;
    ch2.readoptionsfromfile(2);
    UD.writechannelvalue(1,ui->dial->value() + 20 );
    int period = ch2.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
}

void MainWindow::UpdateDataChannel3()
{
    UartDriver UD;
    ch3.readoptionsfromfile(3);
    UD.writechannelvalue(2,ui->dial->value() + 30 );
    int period = ch3.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
}

void MainWindow::UpdateDataChannel4()
{
    UartDriver UD;
    ch4.readoptionsfromfile(4);
    UD.writechannelvalue(3,ui->dial->value() + 40 );
    int period = ch4.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
}
