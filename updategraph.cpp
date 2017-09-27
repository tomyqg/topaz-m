#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"
#include "stackedoptions.h"
#include "qglobal.h"
#include "defines.h"

int a=0;int b=0;

int color1rgbnormal[]={0xCF, 0xF7, 0x00};
int color2rgbnormal[]={0x00, 0xAE, 0x68};
int color3rgbnormal[]={0xFF, 0x4C, 0x00};
int color4rgbnormal[]={0xA1, 0x01, 0xA6};

QColor ChannelColorHighState = QColor(0xFF,0x00,0x00);
QColor ChannelColorLowState = QColor(0xFF,0xCA,0x00);

QColor Channel1Color = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2Color = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3Color = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4Color = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QColor ChannelColorNormal = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2ColorNormal = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3ColorNormal = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4ColorNormal = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;

int xyi;

void MainWindow::AddValuesToBuffer()
{

    startWorkSignal();
    X_Coordinates.append(b);
    X_Coordinates_archive.append(b);
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

    //    Y_coordinates_Chanel_1.append(UartDriver::channelinputbuffer[0]);

    //    UartDriver::channelinputbuffer[0] = b;
    Y_coordinates_Chanel_1.append(UartDriver::channelinputbuffer[0]);
    Y_coordinates_Chanel_2.append(UartDriver::channelinputbuffer[1]);
    Y_coordinates_Chanel_3.append(UartDriver::channelinputbuffer[2]);
    Y_coordinates_Chanel_4.append(UartDriver::channelinputbuffer[3]);

    Y_coordinates_Chanel_1_archive.append(UartDriver::channelinputbuffer[0]);
    Y_coordinates_Chanel_2_archive.append(UartDriver::channelinputbuffer[1]);
    Y_coordinates_Chanel_3_archive.append(UartDriver::channelinputbuffer[2]);
    Y_coordinates_Chanel_4_archive.append(UartDriver::channelinputbuffer[3]);

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
    //int maximumdots = GetGraphWidthInPixels()/2 ;

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
    stopWorkSignal();
}

void MainWindow::UpdateGraphics()
{
    needupdatePainter = 1;

    switch( StackedOptions::GetCurrentDisplayParametr() )
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
    graphPen.setWidth(GraphWidthinPixels);
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

    graphPen.setWidth(GraphWidthinPixels);

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


    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);


    ui->customPlot->replot();
}

void MainWindow::GrafsUpdateTrends()
{

    ui->customPlot->xAxis->setRange(b-GetXRange(), b+GetXRange());
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);


    //    // add the text label at the top:
    //    QCPItemText *textLabel = new QCPItemText(ui->customPlot);
    //    ui->customPlot->addItem(textLabel);
    //    textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    //    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    //    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    //    textLabel->setText("Text Item Demo");
    //    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    //    textLabel->setPen(QPen(Qt::black)); // show black border around text

    // add the arrow:

    if (ui->arrowscheckBox->checkState())
    {
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        //    arrow->start->setCoords(400,200);

        int ystart = 150;
        int xstart = 950;

        arrow->start->setPixelPoint(QPointF(xstart, ystart));
        arrow->end->setCoords(b, Y_coordinates_Chanel_1.last()); // point to (4, 1.6) in x-y-plot coordinates
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        arrow->setPen(QPen(channel1object.GetCurrentColor(),1,  Qt::DashLine));
        ui->customPlot->addItem(arrow);

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);

        arrow2->start->setPixelPoint(QPointF(xstart, ystart+105));
        arrow2->end->setCoords(b, Y_coordinates_Chanel_2.last()); // point to (4, 1.6) in x-y-plot coordinates
        arrow2->setHead(QCPLineEnding::esSpikeArrow);
        arrow2->setPen(QPen(channel2object.GetCurrentColor(),1,  Qt::DashLine));
        ui->customPlot->addItem(arrow2);

        QCPItemLine *arrow3 = new QCPItemLine(ui->customPlot);

        arrow3->start->setPixelPoint(QPointF(xstart, ystart+105*2));
        arrow3->end->setCoords(b, Y_coordinates_Chanel_3.last()); // point to (4, 1.6) in x-y-plot coordinates
        arrow3->setHead(QCPLineEnding::esSpikeArrow);
        arrow3->setPen(QPen(channel3object.GetCurrentColor(),1,  Qt::DashLine));
        ui->customPlot->addItem(arrow3);

        QCPItemLine *arrow4 = new QCPItemLine(ui->customPlot);

        arrow4->start->setPixelPoint(QPointF(xstart, ystart+105*3));
        arrow4->end->setCoords(b, Y_coordinates_Chanel_4.last()); // point to (4, 1.6) in x-y-plot coordinates
        arrow4->setHead(QCPLineEnding::esSpikeArrow);
        arrow4->setPen(QPen(channel4object.GetCurrentColor(),1,  Qt::DashLine));
        ui->customPlot->addItem(arrow4);

    }

    graphPen.setWidth(GraphWidthinPixels);
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


    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);

    // авто масшабирование
    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->rescaleAxes();
    }
    ui->customPlot->replot();
    ui->customPlot->clearItems();// удаляем стрелочку а то она будет потом мешаться
}

void MainWindow::GrafsUpdateNone()
{
    ui->MessagesWidget->update();

    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();

    //ui->customPlot->clearMask();
    //ui->customPlot->clearFocus();
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
    graphPen.setWidth(GraphWidthinPixels);
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


    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);


    ui->customPlot->replot();
}

void MainWindow::UpdateChannel1Slot()
{
    UartDriver::needtoupdatechannel[0] = 1;
    int period = channel1object.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
    channel1object.SetCurrentChannelValue(UartDriver::channelinputbuffer[0]);
    CheckState(channel1object);
}

void MainWindow::UpdateChannel2Slot()
{
    UartDriver::needtoupdatechannel[1] = 1;
    int period = channel2object.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
    channel2object.SetCurrentChannelValue(UartDriver::channelinputbuffer[1]);
    //    qDebug()<<UartDriver::channelinputbuffer[1];
    CheckState(channel2object);
}

void MainWindow::UpdateChannel3Slot()
{
    UartDriver::needtoupdatechannel[2] = 1;
    int period = channel3object.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
    channel3object.SetCurrentChannelValue(UartDriver::channelinputbuffer[2]);
    CheckState(channel3object);
}

void MainWindow::UpdateChannel4Slot()
{
    UartDriver::needtoupdatechannel[3] = 1;
    int period = channel4object.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
    channel4object.SetCurrentChannelValue(UartDriver::channelinputbuffer[3]);
    CheckState(channel4object);
}
