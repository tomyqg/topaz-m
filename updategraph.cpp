#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"

int a=0;int b=0;
int color1rgb[]={219, 37, 37};
int color2rgb[]={43, 37, 219};
int color3rgb[]={37, 219, 49};
int color4rgb[]={59, 30, 7};

QColor Channel1Color = QColor(color1rgb[0],color1rgb[1],color1rgb[2]);
QColor Channel2Color = QColor(color2rgb[0],color2rgb[1],color2rgb[2]);
QColor Channel3Color = QColor(color3rgb[0],color3rgb[1],color3rgb[2]);
QColor Channel4Color = QColor(color4rgb[0],color4rgb[1],color4rgb[2]);

/*int color2rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color3rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color4rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};*/

QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;

void MainWindow::AddValuesToBuffer()
{
    UartDriver UD;
    X_Coordinates.append(b);
    Y_coordinates_Chanel_1.append(UD.channelinputbuffer[0]);
    Y_coordinates_Chanel_2.append(UD.channelinputbuffer[1]);
    Y_coordinates_Chanel_3.append(UD.channelinputbuffer[2]);
    Y_coordinates_Chanel_4.append(UD.channelinputbuffer[3]);
    b++;
}

void MainWindow::UpdateGraphics()
{
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

    ui->customPlot->xAxis->setRange(b-300, b+300);

    if (b%60<=5)
    {
        LabelsCorrect();
    }

    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);
    graphPen.setWidth(6);
    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);}

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(60); // 60 secs btw timestamp

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

    graphPen.setWidth(1);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1, y1);
    ui->customPlot->graph()->setBrush(QBrush(QColor(color1rgb[0],color1rgb[1],color1rgb[2]))); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2, y2);
    ui->customPlot->graph()->setBrush(QBrush(QColor(color2rgb[0],color2rgb[1],color2rgb[2]))); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3, y3);
    ui->customPlot->graph()->setBrush(QBrush(QColor(color3rgb[0],color3rgb[1],color3rgb[2]))); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4, y4);
    ui->customPlot->graph()->setBrush(QBrush(QColor(color4rgb[0],color4rgb[1],color4rgb[2]))); // first graph will be filled with translucent blue
    graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->replot();
}

void MainWindow::GrafsUpdateTrends()
{

    while (X_Coordinates.length()>300)
    {
        X_Coordinates.remove(0);Y_coordinates_Chanel_1.remove(0);Y_coordinates_Chanel_2.remove(0);Y_coordinates_Chanel_3.remove(0);Y_coordinates_Chanel_4.remove(0);
    }

    ui->customPlot->xAxis->setRange(b-300, b+300);

    if (b%60<=5)
    {
        LabelsCorrect();
    }

    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);
    graphPen.setWidth(6);
    graphPen.setColor(Channel1Color);

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
        graphPen.setColor(Channel2Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
        graphPen.setColor(Channel3Color);
        ui->customPlot->graph()->setPen(graphPen);}

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
        graphPen.setColor(Channel4Color);
        ui->customPlot->graph()->setPen(graphPen);
    }

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(60); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    /*  // add the text label at the top:
    QCPItemText *textLabel = new QCPItemText(ui->customPlot);
    ui->customPlot->addItem(textLabel);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    textLabel->setText("Dummy label");
    textLabel->setFont(QFont("Times New Roman", 16)); // make font a bit larger
    textLabel->setPen(QPen(Qt::black)); // show black border around text*/
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
    graphPen.setWidth(1);
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
    LabelsBar.append("0");LabelsBar.append("Channel 1");LabelsBar.append("Channel 2");LabelsBar.append("Channel 3");LabelsBar.append("Channel 4");

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(20); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(LabelsBar);

    ui->customPlot->replot();
}

void MainWindow::UpdateDataChannel1()
{
    UartDriver UD;
    ModBus modbus;
    mathresolver mathres;
    double currentdata;

    UD.needtoupdatechannel[0] = 1;

    currentdata = modbus.DataChannelRead(ModBus::UniversalChannel1);

    if (currentdata!=0)
    {

        if (ch1.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(ch1.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(1,currentdata);

        if ((currentdata>=ch1.GetState1Value() ) && ( ch1.HighState1Setted == false ))
        {
            ch1.LowState1Setted = false;
            ui->listWidget->addItem(ch1.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            ch1.HighState1Setted = true;
            mr.LogAddMessage (ch1.GetChannelName() + ":" + ch1.GetState1HighMessage());
        }

        if ((currentdata<ch1.GetState1Value() ) && ( ch1.LowState1Setted == false ))
        {
            ch1.LowState1Setted = true;
            ui->listWidget->addItem(ch1.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            ch1.HighState1Setted = false;
            mr.LogAddMessage (ch1.GetChannelName() + ":" + ch1.GetState1LowMessage());
        }

    }
    else {

    }
    int period = ch1.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
}

void MainWindow::UpdateDataChannel111()
{
    UartDriver::needtoupdatechannel[0] = 1;
    int period = ch1.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
}

void MainWindow::UpdateDataChannel222()
{
    UartDriver::needtoupdatechannel[1] = 1;
    int period = ch2.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
}

void MainWindow::UpdateDataChannel333()
{
    UartDriver::needtoupdatechannel[2] = 1;
    int period = ch3.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
}

void MainWindow::UpdateDataChannel444()
{
    UartDriver::needtoupdatechannel[3] = 1;
    int period = ch4.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
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
        if (ch2.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(ch2.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        pressure = currentdata;
        UD.writechannelvalue(2,pressure);

        if ((pressure>=ch2.GetState1Value() ) && ( ch2.HighState1Setted == false ))
        {
            ch2.LowState1Setted = false;
            ui->listWidget->addItem(ch2.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            ch2.HighState1Setted = true;
            mr.LogAddMessage (ch2.GetChannelName() + ":" + ch2.GetState1HighMessage());
        }

        if ((pressure<ch2.GetState1Value() ) && ( ch2.LowState1Setted == false ))
        {
            ch2.LowState1Setted = true;
            ui->listWidget->addItem(ch2.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            ch2.HighState1Setted = false;
            mr.LogAddMessage (ch2.GetChannelName() + ":" + ch2.GetState1LowMessage());
        }
    }
    int period = ch2.GetMeasurePeriod()*1000;
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
        if (ch3.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(ch3.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(3,currentdata);

        if ((currentdata>=ch3.GetState1Value() ) && ( ch3.HighState1Setted == false ))
        {
            ch3.LowState1Setted = false;
            ui->listWidget->addItem(ch3.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            ch3.HighState1Setted = true;
            mr.LogAddMessage (ch3.GetChannelName() + ":" + ch3.GetState1HighMessage());
        }

        if ((currentdata<ch3.GetState1Value() ) && ( ch3.LowState1Setted == false ))
        {
            ch3.LowState1Setted = true;
            ui->listWidget->addItem(ch3.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            ch3.HighState1Setted = false;
            mr.LogAddMessage (ch3.GetChannelName() + ":" + ch3.GetState1LowMessage());
        }
    }
    int period = ch3.GetMeasurePeriod()*1000;
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
        if (ch4.IsChannelMathematical())
        {
            currentdata = mathres.SolveEquation(ch4.GetMathString(), currentdata); // + mathres.SolveEquation("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(4,currentdata);

        if ((currentdata>=ch4.GetState1Value() ) && ( ch4.HighState1Setted == false ))
        {
            ch4.LowState1Setted = false;
            ui->listWidget->addItem(ch4.GetState1HighMessage());
            ui->listWidget->scrollToBottom();
            ch4.HighState1Setted = true;
            mr.LogAddMessage (ch4.GetChannelName() + ":" + ch4.GetState1HighMessage());
        }

        if ((currentdata<ch4.GetState1Value() ) && ( ch4.LowState1Setted == false ))
        {
            ch4.LowState1Setted = true;
            ui->listWidget->addItem(ch4.GetState1LowMessage());
            ui->listWidget->scrollToBottom();
            ch4.HighState1Setted = false;
            mr.LogAddMessage (ch4.GetChannelName() + ":" + ch4.GetState1LowMessage());
        }
    }
    int period = ch4.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
}

