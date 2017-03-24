#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"

int a=0;int b=0;
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

    yy1.append(UD.channelinputbuffer[0]);
    yy2.append(UD.channelinputbuffer[1]);
    yy3.append(UD.channelinputbuffer[2]);
    yy4.append(UD.channelinputbuffer[3]);
    b++;

    if (b>=300&&b<900)
    {
        ui->customPlot->xAxis->setRange(b-300, b+300);
    }

    if (b==1200)
    {
        b=0;
        ui->customPlot->xAxis->setRange(0, 600);
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
    ui->customPlot->xAxis->setTickStep(60); // 60 secs btw timestamp
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);

    ui->customPlot->xAxis->setDateTimeSpec(Qt::OffsetFromUTC);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");// dd.MM.yyyy
    ui->customPlot->replot();
}

void MainWindow::UpdateDataChannel1()
{
    UartDriver UD;
    ModBus modbus;
    double currentdata ;
    currentdata = modbus.ReadVoltage(1);
    UD.writechannelvalue(1,currentdata);
//    modbus.ModBusMakeRequest2(0x01,0x03, 0xcaef,0xccae);

    if ((currentdata>=ch1.GetState1Value() ) && ( ch1.HighState1Setted == false ))
    {
        ch1.LowState1Setted = false;
        ui->listWidget->addItem(ch1.GetState1HighMessage());
        ui->listWidget->scrollToBottom();
        ch1.HighState1Setted = true;
        mr.LogMessageWrite (ch1.GetChannelName() + ":" + ch1.GetState1HighMessage());
    }

    if ((currentdata<ch1.GetState1Value() ) && ( ch1.LowState1Setted == false ))
    {
        ch1.LowState1Setted = true;
        ui->listWidget->addItem(ch1.GetState1LowMessage());
        ui->listWidget->scrollToBottom();
        ch1.HighState1Setted = false;
        mr.LogMessageWrite (ch1.GetChannelName() + ":" + ch1.GetState1LowMessage());
    }

    int period = ch1.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
}

void MainWindow::UpdateDataChannel2()
{
    UartDriver UD;
    ModBus modbus;
    mathresolver mathres;

    double currentdata ;
    double pressure ;
    currentdata = modbus.ReadTemperature(1);
    pressure = mathres.Solve("x/2", currentdata); //sqrt(abs(x))+20
    UD.writechannelvalue(2,pressure);
//    pressure = mathres.Solve("sqrt(abs(x))+3.33");

    if ((pressure>=ch2.GetState1Value() ) && ( ch2.HighState1Setted == false ))
    {
        ch2.LowState1Setted = false;
        ui->listWidget->addItem(ch2.GetState1HighMessage());
        ui->listWidget->scrollToBottom();
        ch2.HighState1Setted = true;
        mr.LogMessageWrite (ch2.GetChannelName() + ":" + ch2.GetState1HighMessage());
    }

    if ((pressure<ch2.GetState1Value() ) && ( ch2.LowState1Setted == false ))
    {
        ch2.LowState1Setted = true;
        ui->listWidget->addItem(ch2.GetState1LowMessage());
        ui->listWidget->scrollToBottom();
        ch2.HighState1Setted = false;
        mr.LogMessageWrite (ch2.GetChannelName() + ":" + ch2.GetState1LowMessage());
    }

    int period = ch2.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
}

void MainWindow::UpdateDataChannel3()
{
    double fakedata = ui->dial->value() + 30;
    UartDriver UD;
    UD.writechannelvalue(3,fakedata);
    double currentdata = fakedata;

    if ((currentdata>=ch3.GetState1Value() ) && ( ch3.HighState1Setted == false ))
    {
        ch3.LowState1Setted = false;
        ui->listWidget->addItem(ch3.GetState1HighMessage());
        ui->listWidget->scrollToBottom();
        ch3.HighState1Setted = true;
        mr.LogMessageWrite (ch3.GetChannelName() + ":" + ch3.GetState1HighMessage());
    }

    if ((currentdata<ch3.GetState1Value() ) && ( ch3.LowState1Setted == false ))
    {
        ch3.LowState1Setted = true;
        ui->listWidget->addItem(ch3.GetState1LowMessage());
        ui->listWidget->scrollToBottom();
        ch3.HighState1Setted = false;
        mr.LogMessageWrite (ch3.GetChannelName() + ":" + ch3.GetState1LowMessage());
    }

    int period = ch3.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
}

void MainWindow::UpdateDataChannel4()
{
    double fakedata = ui->dial->value() + 40;
    UartDriver UD;
    UD.writechannelvalue(4,fakedata);
    double currentdata = fakedata;

    if ((currentdata>=ch4.GetState1Value() ) && ( ch4.HighState1Setted == false ))
    {
        ch4.LowState1Setted = false;
        ui->listWidget->addItem(ch4.GetState1HighMessage());
        ui->listWidget->scrollToBottom();
        ch4.HighState1Setted = true;
        mr.LogMessageWrite (ch4.GetChannelName() + ":" + ch4.GetState1HighMessage());
    }

    if ((currentdata<ch4.GetState1Value() ) && ( ch4.LowState1Setted == false ))
    {
        ch4.LowState1Setted = true;
        ui->listWidget->addItem(ch4.GetState1LowMessage());
        ui->listWidget->scrollToBottom();
        ch4.HighState1Setted = false;
        mr.LogMessageWrite (ch4.GetChannelName() + ":" + ch4.GetState1LowMessage());
    }

    int period = ch4.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
}
