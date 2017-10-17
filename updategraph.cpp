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

int xoffset=0;

// пара 1

int color1rgbnormal[]={0, 137, 182};    // RAL 5012 colour
int color2rgbnormal[]={0, 131, 81};     // RAL 6024 colour
int color3rgbnormal[]={91, 104, 109};   // RAL 7031 colour
int color4rgbnormal[]={126, 75, 38};   // RAL 8003 colour

int color1rgbmaximum[]={61, 56, 85};       // RAL  5003
int color2rgbmaximum[]={0, 105, 76};       // RAL  6016
int color3rgbmaximum[]={56,62,66};         // 7016
int color4rgbmaximum[]={121,80,56};        // ral 8024

int color1rgbminimum[]={96,147,172};    // RAL 5024
int color2rgbminimum[]={97,153,59};     // RAL 6018
int color3rgbminimum[]={142,146,145};   // RAL 7042
int color4rgbminimum[]={157,98,43};    // RAL 8001


// пара 2

//int color1rgbnormal[]={144,51,115};    // RAL 4006 colour
//int color2rgbnormal[]={203,115,117};     // RAL 3014 colour
//int color3rgbnormal[]={218,110,0};   // RAL 2000 colour
//int color4rgbnormal[]={108,124,89};   // RAL 6011 colour

//int color1rgbmaximum[]={132,76,130};       // RAL  4008
//int color2rgbmaximum[]={134,26,34};        // RAL  3003
//int color3rgbmaximum[]={208,93,40};         // RAL  2010
//int color4rgbmaximum[]={94,110,59};        // RAL  6025

//int color1rgbminimum[]={129,97,131};    // RAL 4001
//int color2rgbminimum[]={216,160,166};   // RAL 3015
//int color3rgbminimum[]={255,155,0};   // RAL 1028
//int color4rgbminimum[]={138,153,119};    // RAL 6021

QColor ChannelColorHighState = QColor(0xFF,0x00,0x00);
QColor ChannelColorLowState  = QColor(0xFF,0xCA,0x00);

QColor Channel1Color = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2Color = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3Color = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4Color = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QColor Channel1ColorNormal = QColor(color1rgbnormal[0],color1rgbnormal[1],color1rgbnormal[2]);
QColor Channel2ColorNormal = QColor(color2rgbnormal[0],color2rgbnormal[1],color2rgbnormal[2]);
QColor Channel3ColorNormal = QColor(color3rgbnormal[0],color3rgbnormal[1],color3rgbnormal[2]);
QColor Channel4ColorNormal = QColor(color4rgbnormal[0],color4rgbnormal[1],color4rgbnormal[2]);

QColor Channel1ColorMaximum = QColor(color1rgbmaximum[0],color1rgbmaximum[1],color1rgbmaximum[2]);
QColor Channel2ColorMaximum = QColor(color2rgbmaximum[0],color2rgbmaximum[1],color2rgbmaximum[2]);
QColor Channel3ColorMaximum = QColor(color3rgbmaximum[0],color3rgbmaximum[1],color3rgbmaximum[2]);
QColor Channel4ColorMaximum = QColor(color4rgbmaximum[0],color4rgbmaximum[1],color4rgbmaximum[2]);

QColor Channel1ColorMinimum = QColor(color1rgbminimum[0],color1rgbminimum[1],color1rgbminimum[2]);
QColor Channel2ColorMinimum = QColor(color2rgbminimum[0],color2rgbminimum[1],color2rgbminimum[2]);
QColor Channel3ColorMinimum = QColor(color3rgbminimum[0],color3rgbminimum[1],color3rgbminimum[2]);
QColor Channel4ColorMinimum = QColor(color4rgbminimum[0],color4rgbminimum[1],color4rgbminimum[2]);


QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;

void MainWindow::AddValuesToBuffer()
{
    startWorkSignal(); // сигнал который запускает воркер . без него воркер не запустится
    X_Coordinates.append(xoffset); // добавляем смещение по иксу
    X_Coordinates_archive.append(xoffset);

    Y_coordinates_Chanel_1.append(channel1object.GetCurrentChannelValue());
    Y_coordinates_Chanel_2.append(channel2object.GetCurrentChannelValue());
    Y_coordinates_Chanel_3.append(channel3object.GetCurrentChannelValue());
    Y_coordinates_Chanel_4.append(channel4object.GetCurrentChannelValue());

    Y_coordinates_Chanel_1_archive.append(channel1object.GetCurrentChannelValue());
    Y_coordinates_Chanel_2_archive.append(channel2object.GetCurrentChannelValue());
    Y_coordinates_Chanel_3_archive.append(channel3object.GetCurrentChannelValue());
    Y_coordinates_Chanel_4_archive.append(channel4object.GetCurrentChannelValue());

    while (X_Coordinates.length()>GetXRange())
    {
        X_Coordinates.removeFirst();Y_coordinates_Chanel_1.removeFirst();Y_coordinates_Chanel_2.removeFirst();Y_coordinates_Chanel_3.removeFirst();Y_coordinates_Chanel_4.removeFirst();
    }

    int tickstep = GetTickStep();

    if (xoffset%tickstep==0) // если последняя точка по иксу попала на тайм-лейбел, его нужно корректировать
    {
        LabelsCorrect();
    }

    xoffset++;
    stopWorkSignal(); // стопим воркер если не нужно считывать данные
}

void MainWindow::UpdateGraphics()
{
    needupdatePainter = 1;

    //StackedOptions::SetCurrentDisplayParametr(StackedOptions::Bars ); // Bars

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
    while (X_Coordinates.length()>GetXRange())
    {
        X_Coordinates.remove(0);Y_coordinates_Chanel_1.remove(0);Y_coordinates_Chanel_2.remove(0);Y_coordinates_Chanel_3.remove(0);Y_coordinates_Chanel_4.remove(0);
    }

    ui->customPlot->xAxis->setRange(xoffset-GetXRange(), xoffset+GetXRange());
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);
    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(channel1object.GetStateDependentColor());

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
    graphPen.setColor(channel2object.GetStateDependentColor());
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
    graphPen.setColor(channel3object.GetStateDependentColor());
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
    graphPen.setColor(channel4object.GetStateDependentColor());
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    /// bars

    QVector<double> x1,x2,x3,x4;
    QVector<double> y1,y2,y3,y4;

    QVector<double> x1lim,x2lim,x3lim,x4lim;
    QVector<double> y1max,y2max,y3max,y4max;
    QVector<double> y1min,y2min,y3min,y4min;

    int lastindex = X_Coordinates.at(X_Coordinates.length()-1);

    // делаем чтоб штрихпунктиром отображалась верхняя и нижняя величина на графике за  период

    double chan1higherstate = channel1object.GetMaximumChannelValue();
    double chan2higherstate = channel2object.GetMaximumChannelValue();
    double chan3higherstate = channel3object.GetMaximumChannelValue();
    double chan4higherstate = channel4object.GetMaximumChannelValue();

    double chan1lowerstate = channel1object.GetMinimumChannelValue();
    double chan2lowerstate = channel2object.GetMinimumChannelValue();
    double chan3lowerstate = channel3object.GetMinimumChannelValue();
    double chan4lowerstate = channel4object.GetMinimumChannelValue();

    y1max.append(chan1higherstate);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);

    y2max.append(chan2higherstate);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);

    y3max.append(chan3higherstate);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);

    y4max.append(chan4higherstate);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);

    y1min.append(chan1lowerstate);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);

    y2min.append(chan2lowerstate);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);

    y3min.append(chan3lowerstate);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);

    y4min.append(chan4lowerstate);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);

    x1.append(430-300-100+25+lastindex);
    x1.append(460-300-100+25+lastindex);
    x2.append(470-300-75+25+lastindex);
    x2.append(500-300-75+25+lastindex);
    x3.append(510-300-50+25+lastindex);
    x3.append(540-300-50+25+lastindex);
    x4.append(550-300-25+25+lastindex);
    x4.append(580-300-25+25+lastindex);

    x1lim.append(x1.at(0));
    x1lim.append(x1.at(1));
    x1lim.append(x1.at(0));
    x1lim.append(x1.at(0));
    x1lim.append(x1.at(1));
    x1lim.append(x1.at(1));

    x2lim.append(x2.at(0));
    x2lim.append(x2.at(1));
    x2lim.append(x2.at(0));
    x2lim.append(x2.at(0));
    x2lim.append(x2.at(1));
    x2lim.append(x2.at(1));

    x3lim.append(x3.at(0));
    x3lim.append(x3.at(1));
    x3lim.append(x3.at(0));
    x3lim.append(x3.at(0));
    x3lim.append(x3.at(1));
    x3lim.append(x3.at(1));

    x4lim.append(x4.at(0));
    x4lim.append(x4.at(1));
    x4lim.append(x4.at(0));
    x4lim.append(x4.at(0));
    x4lim.append(x4.at(1));
    x4lim.append(x4.at(1));

    y1.append(channel1object.GetCurrentChannelValue());
    y1.append(channel1object.GetCurrentChannelValue());
    y2.append(channel2object.GetCurrentChannelValue());
    y2.append(channel2object.GetCurrentChannelValue());
    y3.append(channel3object.GetCurrentChannelValue());
    y3.append(channel3object.GetCurrentChannelValue());
    y4.append(channel4object.GetCurrentChannelValue());
    y4.append(channel4object.GetCurrentChannelValue());

    graphPen.setWidth(GraphWidthinPixels);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1, y1);
    ui->customPlot->graph()->setBrush(QBrush(channel1object.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2, y2);
    ui->customPlot->graph()->setBrush(QBrush(channel2object.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3, y3);
    ui->customPlot->graph()->setBrush(QBrush(channel3object.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4, y4);
    ui->customPlot->graph()->setBrush(QBrush(channel4object.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    // рисуем границы каналов каждого барграфа

    QPen dottedpen = QPen(Qt::red, 1, Qt::DashLine);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4max);
    ui->customPlot->graph()->setPen(dottedpen);


    dottedpen = QPen(Qt::green, 1, Qt::DashLine);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4min);
    ui->customPlot->graph()->setPen(dottedpen);


    // add the arrows:

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);

    QList<int> arrowsendcoords;

    arrowsendcoords.append(x1.at(0)-10);
    arrowsendcoords.append(x2.at(0)-10);
    arrowsendcoords.append(x3.at(0)-10);
    arrowsendcoords.append(x4.at(0)-10);

    // рисуем стрелки для каждой уставки

    int barindex = 0 ;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList)
    {
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        arrow->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        arrow->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState1Value() );
        arrow->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->GetState1Value() );
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow);

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
        arrow2->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        arrow2->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState2Value() );
        arrow2->end->setCoords(arrowsendcoords.at(barindex++)-1,Chanel->GetState2Value() );
        arrow2->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow2);


        QPointF Label1PixPoint = arrow->start->pixelPoint();
        QPointF Label2PixPoint = arrow2->start->pixelPoint();

        Label1PixPoint.setY(Label1PixPoint.y() - 20);
        Label2PixPoint.setY(Label2PixPoint.y() - 20);

        // add the text label at the top limit:
        QCPItemText *textLabelHi = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelHi);
        textLabelHi->position->setPixelPoint(Label1PixPoint);
        textLabelHi->setText(QString::number(Chanel->GetState1Value() ));
        textLabelHi->setFont(QFont(Font, 8, QFont::Bold));
        textLabelHi->setColor(QColor(Qt::red));

        // add the text label at the bottom limit

        QCPItemText *textLabelLo = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelLo);
        textLabelLo->position->setPixelPoint(Label2PixPoint);
        textLabelLo->setText(QString::number(Chanel->GetState2Value() ));
        textLabelLo->setFont(QFont(Font, 8, QFont::Bold));
        textLabelLo->setColor(QColor(Qt::green));
    }


    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->yAxis->rescale();
    }

    // add the helper arrows:

    if (ui->arrowscheckBox->checkState()) // if it s needed
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1object);
        ChannelsObjectsList.append(&channel2object);
        ChannelsObjectsList.append(&channel3object);
        ChannelsObjectsList.append(&channel4object);

        QList<int> arrowsendcoords;

        arrowsendcoords.append(x1.at(0));
        arrowsendcoords.append(x2.at(0));
        arrowsendcoords.append(x3.at(0));
        arrowsendcoords.append(x4.at(0));

        // рисуем стрелки для каждой уставки

        int index = 0 ;
        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
            //    arrow->start->setCoords(400,200);

            int ystart = 150;
            int xstart = 600;

            arrow->start->setPixelPoint(QPointF(xstart, ystart+100*index));
            arrow->end->setCoords(xoffset, Chanel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(Chanel->GetNormalColor(),1,  Qt::DashLine));
            ui->customPlot->addItem(arrow);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(arrow->start->pixelPoint());
            NameLabel->setText(Chanel->GetChannelName() );
            NameLabel->setFont(QFont(Font, 8, QFont::Bold));
            NameLabel->setColor(Chanel->GetNormalColor());

            ++index;
        }
    }

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateTrends()
{

    ui->customPlot->xAxis->setRange(xoffset-GetXRange(), xoffset+GetXRange());
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);

    // add the helper arrow:

    if (ui->arrowscheckBox->checkState())
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1object);
        ChannelsObjectsList.append(&channel2object);
        ChannelsObjectsList.append(&channel3object);
        ChannelsObjectsList.append(&channel4object);

        QList<int> arrowsendcoords;

        // рисуем стрелки для каждой уставки

        int index = 0 ;
        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
            //    arrow->start->setCoords(400,200);

            int ystart = 150;
            int xstart = 600;

            arrow->start->setPixelPoint(QPointF(xstart, ystart+100*index));
            arrow->end->setCoords(xoffset, Chanel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(Chanel->GetNormalColor(),1,  Qt::DashLine));
            arrow->setAntialiased(true);
            ui->customPlot->setAntialiasedElements(QCP::aeAll);
            ui->customPlot->addItem(arrow);

            arrow->setAntialiased(true);
            ui->customPlot->setAntialiasedElements(QCP::aeAll);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(arrow->start->pixelPoint());
            NameLabel->setText(Chanel->GetChannelName() );
            NameLabel->setFont(QFont(Font, 8, QFont::Bold));
            NameLabel->setColor(Chanel->GetNormalColor());

            ++index;
        }

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

    // авто масшабирование
    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->yAxis->rescale();
    }


    ui->customPlot->setAntialiasedElements(QCP::aeAll);

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateNone()
{
    ui->MessagesWidget->update();

    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
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

    Bar1_Y_Coord.append(DataBuffer::readchannelvalue(0));
    Bar1_Y_Coord.append(DataBuffer::readchannelvalue(0));
    Bar2_Y_Coord.append(DataBuffer::readchannelvalue(1));
    Bar2_Y_Coord.append(DataBuffer::readchannelvalue(1));
    Bar3_Y_Coord.append(DataBuffer::readchannelvalue(2));
    Bar3_Y_Coord.append(DataBuffer::readchannelvalue(2));
    Bar4_Y_Coord.append(DataBuffer::readchannelvalue(3));
    Bar4_Y_Coord.append(DataBuffer::readchannelvalue(3));

    QVector<double> x1lim,x2lim,x3lim,x4lim;
    QVector<double> y1max,y2max,y3max,y4max;
    QVector<double> y1min,y2min,y3min,y4min;

    // делаем чтоб штрихпунктиром отображалась верхняя и нижняя величина на графике за  период

    double chan1higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_1);
    double chan2higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_2);
    double chan3higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_3);
    double chan4higherstate = mathresolver::dGetMaximumValue(Y_coordinates_Chanel_4);

    double chan1lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_1);
    double chan2lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_2);
    double chan3lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_3);
    double chan4lowerstate = mathresolver::dGetMinimumValue(Y_coordinates_Chanel_4);

    y1max.append(chan1higherstate);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);
    y1max.append(0);
    y1max.append(chan1higherstate);

    y2max.append(chan2higherstate);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);
    y2max.append(0);
    y2max.append(chan2higherstate);


    y3max.append(chan3higherstate);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);
    y3max.append(0);
    y3max.append(chan3higherstate);

    y4max.append(chan4higherstate);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);
    y4max.append(0);
    y4max.append(chan4higherstate);

    y1min.append(chan1lowerstate);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);
    y1min.append(0);
    y1min.append(chan1lowerstate);

    y2min.append(chan2lowerstate);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);
    y2min.append(0);
    y2min.append(chan2lowerstate);

    y3min.append(chan3lowerstate);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);
    y3min.append(0);
    y3min.append(chan3lowerstate);

    y4min.append(chan4lowerstate);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);
    y4min.append(0);
    y4min.append(chan4lowerstate);

    x1lim.append(Bar1_X_Coord.at(0));
    x1lim.append(Bar1_X_Coord.at(1));
    x1lim.append(Bar1_X_Coord.at(0));
    x1lim.append(Bar1_X_Coord.at(0));
    x1lim.append(Bar1_X_Coord.at(1));
    x1lim.append(Bar1_X_Coord.at(1));

    x2lim.append(Bar2_X_Coord.at(0));
    x2lim.append(Bar2_X_Coord.at(1));
    x2lim.append(Bar2_X_Coord.at(0));
    x2lim.append(Bar2_X_Coord.at(0));
    x2lim.append(Bar2_X_Coord.at(1));
    x2lim.append(Bar2_X_Coord.at(1));

    x3lim.append(Bar3_X_Coord.at(0));
    x3lim.append(Bar3_X_Coord.at(1));
    x3lim.append(Bar3_X_Coord.at(0));
    x3lim.append(Bar3_X_Coord.at(0));
    x3lim.append(Bar3_X_Coord.at(1));
    x3lim.append(Bar3_X_Coord.at(1));

    x4lim.append(Bar4_X_Coord.at(0));
    x4lim.append(Bar4_X_Coord.at(1));
    x4lim.append(Bar4_X_Coord.at(0));
    x4lim.append(Bar4_X_Coord.at(0));
    x4lim.append(Bar4_X_Coord.at(1));
    x4lim.append(Bar4_X_Coord.at(1));

    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();

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

    // рисуем границы каналов каждого барграфа

    QPen dottedpen = QPen(Qt::red, 1, Qt::DashLine);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3max);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4max);
    ui->customPlot->graph()->setPen(dottedpen);

    dottedpen = QPen(Qt::green, 1, Qt::DashLine);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1lim, y1min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2lim, y2min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3lim, y3min);
    ui->customPlot->graph()->setPen(dottedpen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4lim, y4min);
    ui->customPlot->graph()->setPen(dottedpen);

    // add the arrows:

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);

    QList<int> arrowsendcoords;

    arrowsendcoords.append(Bar1_X_Coord.at(0));
    arrowsendcoords.append(Bar2_X_Coord.at(0));
    arrowsendcoords.append(Bar3_X_Coord.at(0));
    arrowsendcoords.append(Bar4_X_Coord.at(0));

    // рисуем стрелки для каждой уставки

    int barindex = 0 ;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList)
    {
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        arrow->setPen(QPen(Qt::red, 3, Qt::SolidLine));
        arrow->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState1Value() );
        arrow->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->GetState1Value() );
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow);

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
        arrow2->setPen(QPen(Qt::green, 3, Qt::SolidLine));
        arrow2->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->GetState2Value() );
        arrow2->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->GetState2Value() );
        arrow2->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow2);

        QPointF Label1PixPoint = arrow->start->pixelPoint();
        QPointF Label2PixPoint = arrow2->start->pixelPoint();

        Label1PixPoint.setY(Label1PixPoint.y() - 20);
        Label2PixPoint.setY(Label2PixPoint.y() - 20);

        // add the text label at the top limit:
        QCPItemText *textLabelHi = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelHi);
        textLabelHi->position->setPixelPoint(Label1PixPoint);
        textLabelHi->setText(QString::number(Chanel->GetState1Value() ));
        textLabelHi->setFont(QFont(Font, 8, QFont::Bold));
        textLabelHi->setColor(QColor(Qt::red));

        // add the text label at the bottom limit
        QCPItemText *textLabelLo = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelLo);
        textLabelLo->position->setPixelPoint(Label2PixPoint);
        textLabelLo->setText(QString::number(Chanel->GetState2Value() ));
        textLabelLo->setFont(QFont(Font, 8, QFont::Bold));
        textLabelLo->setColor(QColor(Qt::green));
        ++barindex;
    }

    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->yAxis->rescale();
    }

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::UpdateChannel1Slot()
{
    DataBuffer::needtoupdatechannel[0] = 1;
    int period = channel1object.GetMeasurePeriod()*1000;
    channeltimer1->setInterval(period);
    channel1object.SetCurrentChannelValue(DataBuffer::readchannelvalue(0));
    CheckState(channel1object);
}

void MainWindow::UpdateChannel2Slot()
{
    DataBuffer::needtoupdatechannel[1] = 1;
    int period = channel2object.GetMeasurePeriod()*1000;
    channeltimer2->setInterval(period);
    channel2object.SetCurrentChannelValue(DataBuffer::readchannelvalue(1));
    CheckState(channel2object);
}

void MainWindow::UpdateChannel3Slot()
{
    DataBuffer::needtoupdatechannel[2] = 1;
    int period = channel3object.GetMeasurePeriod()*1000;
    channeltimer3->setInterval(period);
    channel3object.SetCurrentChannelValue(DataBuffer::readchannelvalue(2));
    CheckState(channel3object);
}

void MainWindow::UpdateChannel4Slot()
{
    DataBuffer::needtoupdatechannel[3] = 1;
    int period = channel4object.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
    channel4object.SetCurrentChannelValue(DataBuffer::readchannelvalue(3));
    CheckState(channel4object);
}
