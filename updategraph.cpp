#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"
#include "qglobal.h"

int a=0;int b=0;

int color1rgbnormal[]={30, 120, 120 };  //dark cyan
int color2rgbnormal[]={40, 40, 220};  // dark blue
int color3rgbnormal[]={30, 100, 30};  // dark green
int color4rgbnormal[]={150, 50, 170}; // dark phiolet

QColor Channel1Color = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2Color = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3Color = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4Color = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QColor Channel1ColorNormal = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2ColorNormal = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3ColorNormal = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4ColorNormal = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QColor ChannelColorHighState = QColor(200,30,30);

QColor ChannelColorLowState = QColor(230,230,30);

QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;

//int xyi;

void MainWindow::AddValuesToBuffer()
{
    X_Coordinates.append(b);
    UartDriver UD;
// закомменитть чтобы не рисовать синусоиду
//    xyi++;
//    int  a = qRound( eee.SolveEquation("sin(x/6)*40-20",xyi) );
//    int  co = qRound( eee.SolveEquation("cos(x/3)*40-20",xyi) );
//    Y_coordinates_Chanel_1.append(a+5);
//    Y_coordinates_Chanel_2.append(co+30);
//    Y_coordinates_Chanel_3.append(a+55);
//    Y_coordinates_Chanel_4.append(co+70);

    //// закоментить посюда
// раскомментить чтобы рисовались нормальные графики

    Y_coordinates_Chanel_1.append(UD.channelinputbuffer[0]);
    Y_coordinates_Chanel_2.append(UD.channelinputbuffer[1]);
    Y_coordinates_Chanel_3.append(UD.channelinputbuffer[2]);
    Y_coordinates_Chanel_4.append(UD.channelinputbuffer[3]);

    /*
    //чтоб графики рисовались не постоянно а с периодом их обновления
    if (UartDriver::needtoupdatechannel[0] == 1)
        Y_coordinates_Chanel_1.append(UD.channelinputbuffer[0]);
    if (UartDriver::needtoupdatechannel[1] == 1)
        Y_coordinates_Chanel_2.append(UD.channelinputbuffer[1]);
    if (UartDriver::needtoupdatechannel[2] == 1)
        Y_coordinates_Chanel_3.append(UD.channelinputbuffer[2]);
    if (UartDriver::needtoupdatechannel[3] == 1)
        Y_coordinates_Chanel_4.append(UD.channelinputbuffer[3]);
    */

    // раскоментить посюда
    int maximumdots = GetGraphWidthInPixels()/2 ;

    while (X_Coordinates.length()>300)
    {
        X_Coordinates.removeFirst();Y_coordinates_Chanel_1.removeFirst();Y_coordinates_Chanel_2.removeFirst();Y_coordinates_Chanel_3.removeFirst();Y_coordinates_Chanel_4.removeFirst();
    }

    int tickstep = GetTickStep();

    if (b%tickstep==0)
    {
        LabelsCorrect();
    }

    b++;
}

void MainWindow::UpdateGraphics()
{

    needupdatePainter = 1;

    switch( Options::DisplayParametr )
    {
    case Options::Trends:
        GrafsUpdateTrends();break;
    case Options::TrendsCyfra:
        GrafsUpdateTrends();break;
    case Options::Bars :
        GrafsUpdateBars();break;
    case Options::BarsCyfra :
        GrafsUpdateBars();break;
    case Options::TrendsBars:
        GrafsUpdateTrendsAndBars();break;
    case Options::TrendsCyfraBars:
        GrafsUpdateTrendsAndBars();break;
    case Options::Polar:
        GrafsUpdateNone();break;
    case Options::Cyfra:
        GrafsUpdateNone();break;
    default:
        break;
    }
}

void MainWindow::GrafsUpdateTrendsAndBars()
{
    while (X_Coordinates.length()>300)
    {
        X_Coordinates.remove(0);Y_coordinates_Chanel_1.remove(0);Y_coordinates_Chanel_2.remove(0);Y_coordinates_Chanel_3.remove(0);Y_coordinates_Chanel_4.remove(0);
    }

    ui->customPlot->xAxis->setRange(b-GetXRange(), b+GetXRange());
    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);
    graphPen.setWidth(3);
    graphPen.setColor(GetChannel1Color());

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(GetChannel2Color());
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(GetChannel3Color());
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(GetChannel4Color());
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    /// bars

    QVector<double> x1,x2,x3,x4;
    QVector<double> y1,y2,y3,y4;

    int lastindex = X_Coordinates.at(X_Coordinates.length()-1);

    x1.append(430-300+lastindex);
    x1.append(460-300+lastindex);
    x2.append(470-300+lastindex);
    x2.append(500-300+lastindex);
    x3.append(510-300+lastindex);
    x3.append(540-300+lastindex);
    x4.append(550-300+lastindex);
    x4.append(580-300+lastindex);

    y1.append(UartDriver::channelinputbuffer[0]);
    y1.append(UartDriver::channelinputbuffer[0]);
    y2.append(UartDriver::channelinputbuffer[1]);
    y2.append(UartDriver::channelinputbuffer[1]);
    y3.append(UartDriver::channelinputbuffer[2]);
    y3.append(UartDriver::channelinputbuffer[2]);
    y4.append(UartDriver::channelinputbuffer[3]);
    y4.append(UartDriver::channelinputbuffer[3]);

    graphPen.setWidth(3);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1, y1);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel1Color())); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2, y2);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel2Color())); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3, y3);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel3Color())); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4, y4);
    ui->customPlot->graph()->setBrush(QBrush(GetChannel4Color())); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->replot();
}

void MainWindow::GrafsUpdateTrends()
{
    ui->customPlot->xAxis->setRange(b-GetXRange(), b+GetXRange());
    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);
    graphPen.setWidth(3);
    //    graphPen.set
    graphPen.setColor(Channel1Color);

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(Channel2Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(Channel3Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    {
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(Channel4Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    ui->customPlot->replot();
}

void MainWindow::GrafsUpdateNone()
{
    ui->MessagesWidget->update();
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
}

void MainWindow::GrafsUpdateBars()
{
    QVector<double> Bar1_X_Coord,Bar2_X_Coord,Bar3_X_Coord,Bar4_X_Coord;
    QVector<double> Bar1_Y_Coord,Bar2_Y_Coord,Bar3_Y_Coord,Bar4_Y_Coord;

    Bar1_X_Coord.append(15);
    Bar1_X_Coord.append(25);
    Bar2_X_Coord.append(35);
    Bar2_X_Coord.append(45);
    Bar3_X_Coord.append(55);
    Bar3_X_Coord.append(65);
    Bar4_X_Coord.append(75);
    Bar4_X_Coord.append(85);

    Bar1_Y_Coord.append(UartDriver::channelinputbuffer[0]);
    Bar1_Y_Coord.append(UartDriver::channelinputbuffer[0]);
    Bar2_Y_Coord.append(UartDriver::channelinputbuffer[1]);
    Bar2_Y_Coord.append(UartDriver::channelinputbuffer[1]);
    Bar3_Y_Coord.append(UartDriver::channelinputbuffer[2]);
    Bar3_Y_Coord.append(UartDriver::channelinputbuffer[2]);
    Bar4_Y_Coord.append(UartDriver::channelinputbuffer[3]);
    Bar4_Y_Coord.append(UartDriver::channelinputbuffer[3]);

    ui->customPlot->clearGraphs();
    ui->customPlot->xAxis->setRange(0, 100);
    graphPen.setWidth(2);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("Bargraf");

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar1_X_Coord, Bar1_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel1Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel1Color );
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar2_X_Coord, Bar2_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel2Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel2Color);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar3_X_Coord, Bar3_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel3Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel3Color);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar4_X_Coord, Bar4_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(Channel4Color)); // first graph will be filled with translucent blue
    graphPen.setColor(Channel4Color);
    ui->customPlot->graph()->setPen(graphPen);

    LabelsBar.clear();
    LabelsBar.append("0");LabelsBar.append(channel1object.GetChannelName());LabelsBar.append(channel2object.GetChannelName());LabelsBar.append(channel3object.GetChannelName());LabelsBar.append(channel4object.GetChannelName());

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(20); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(LabelsBar);

    ui->customPlot->replot();
}

void MainWindow::UpdateChannel1()
{
    UartDriver::needtoupdatechannel[0] = 1;
    int period = channel1object.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
    channel1object.SetCurrentValue(UartDriver::channelinputbuffer[0]);
    CheckState(channel1object);
}

void MainWindow::UpdateChannel2()
{
    UartDriver::needtoupdatechannel[1] = 1;
    int period = channel2object.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
    channel2object.SetCurrentValue(UartDriver::channelinputbuffer[1]);
    CheckState(channel2object);
}

void MainWindow::UpdateChannel3()
{
    UartDriver::needtoupdatechannel[2] = 1;
    int period = channel3object.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
    channel3object.SetCurrentValue(UartDriver::channelinputbuffer[2]);
    CheckState(channel3object);
}

void MainWindow::UpdateChannel4()
{
    UartDriver::needtoupdatechannel[3] = 1;
    int period = channel4object.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
    channel4object.SetCurrentValue(UartDriver::channelinputbuffer[3]);
    CheckState(channel4object);
}

void MainWindow::UpdateDataChannel1()
{
    UartDriver UD;
    ModBus modbus;
    mathresolver mathres;
    double currentdata;

    int period = channel1object.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);

    UD.needtoupdatechannel[0] = 1;

    currentdata = modbus.DataChannelRead(ModBus::UniversalChannel1);

    if (currentdata!=0)
    {

        if (channel1object.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(channel1object.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(1,currentdata);

        if ((currentdata>=channel1object.GetState1Value() ) && ( channel1object.HighState1Setted == false ))
        {
            channel1object.LowState1Setted = false;
            ui->listWidget->addItem(channel1object.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            channel1object.HighState1Setted = true;
            mr.LogAddMessage (channel1object.GetChannelName() + ":" + channel1object.GetState1HighMessage());
        }

        if ((currentdata<channel1object.GetState1Value() ) && ( channel1object.LowState1Setted == false ))
        {
            channel1object.LowState1Setted = true;
            ui->listWidget->addItem(channel1object.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            channel1object.HighState1Setted = false;
            mr.LogAddMessage (channel1object.GetChannelName() + ":" + channel1object.GetState1LowMessage());
        }
    }
}

void MainWindow::UpdateDataChannel2()
{
    UartDriver UD;
    ModBus modbus;
    mathresolver mathres;

    double currentdata;
    double pressure;
    currentdata = modbus.DataChannel1Read(); // тоже покатит:  modbus.DataChannelRead(ModBus::UniversalChannel1);
    if (currentdata!=0)
    {
        if (channel2object.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(channel2object.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        pressure = currentdata;
        UD.writechannelvalue(2,pressure);

        if ((pressure>=channel2object.GetState1Value() ) && ( channel2object.HighState1Setted == false ))
        {
            channel2object.LowState1Setted = false;
            ui->listWidget->addItem(channel2object.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            channel2object.HighState1Setted = true;
            mr.LogAddMessage (channel2object.GetChannelName() + ":" + channel2object.GetState1HighMessage());
        }

        if ((pressure<channel2object.GetState1Value() ) && ( channel2object.LowState1Setted == false ))
        {
            channel2object.LowState1Setted = true;
            ui->listWidget->addItem(channel2object.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            channel2object.HighState1Setted = false;
            mr.LogAddMessage (channel2object.GetChannelName() + ":" + channel2object.GetState1LowMessage());
        }
    }
    int period = channel2object.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
}

void MainWindow::UpdateDataChannel3()
{
    UartDriver UD;
    ModBus modbus;
    mathresolver mathres;
    double currentdata;

    currentdata = modbus.DataChannelRead(ModBus::UniversalChannel1);
    if (currentdata!=0)
    {
        if (channel3object.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(channel3object.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(3,currentdata);

        if ((currentdata>=channel3object.GetState1Value() ) && ( channel3object.HighState1Setted == false ))
        {
            channel3object.LowState1Setted = false;
            ui->listWidget->addItem(channel3object.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            channel3object.HighState1Setted = true;
            mr.LogAddMessage (channel3object.GetChannelName() + ":" + channel3object.GetState1HighMessage());
        }

        if ((currentdata<channel3object.GetState1Value() ) && ( channel3object.LowState1Setted == false ))
        {
            channel3object.LowState1Setted = true;
            ui->listWidget->addItem(channel3object.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            channel3object.HighState1Setted = false;
            mr.LogAddMessage (channel3object.GetChannelName() + ":" + channel3object.GetState1LowMessage());
        }
    }
    int period = channel3object.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
}

void MainWindow::UpdateDataChannel4()
{
    UartDriver UD;
    ModBus modbus;
    mathresolver mathres;
    double currentdata;

    currentdata = modbus.DataChannelRead(ModBus::UniversalChannel1);
    if (currentdata!=0)
    {
        if (channel4object.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(channel4object.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(4,currentdata);

        if ((currentdata>=channel4object.GetState1Value() ) && ( channel4object.HighState1Setted == false ))
        {
            channel4object.LowState1Setted = false;
            ui->listWidget->addItem(channel4object.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            channel4object.HighState1Setted = true;
            mr.LogAddMessage (channel4object.GetChannelName() + ":" + channel4object.GetState1HighMessage());
        }

        if ((currentdata<channel4object.GetState1Value() ) && ( channel4object.LowState1Setted == false ))
        {
            channel4object.LowState1Setted = true;
            ui->listWidget->addItem(channel4object.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            channel4object.HighState1Setted = false;
            mr.LogAddMessage (channel4object.GetChannelName() + ":" + channel4object.GetState1LowMessage());
        }
    }
    int period = channel4object.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
}
