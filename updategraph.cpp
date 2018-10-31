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
#include <stdlib.h>
#include "registersmap.h"
#include "assert.h"



#define PERIOD_MEASURE_STEEL 100    //msec период точек на графике анализа стали

int xoffset=0;

double randVal[4] = {((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10)};


QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
QVector<QDateTime> X_Date_Coordinates;
QVector<double> X_Coord_Steel, Y_Coord_SteelTemp, Y_Coord_SteelEds;

extern cChannelSlotController csc;
extern QList<cSteel*> listSteel;
extern cSteelController ssc;
extern cSystemOptions systemOptions;  //класс хранения состемных опций


int MainWindow::GetXOffset(int smallrectinglewidth, QGraphicsTextItem *ChannelValueText)
{
    ChannelValueText->setTextWidth(ChannelValueText->boundingRect().width());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = ChannelValueText->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    ChannelValueText->setTextCursor(cursor);
    int GW = smallrectinglewidth;
    int lw =  ChannelValueText->boundingRect().width();
    int xoffset = (GW  - lw) /2;

    return xoffset;
}

void MainWindow::DrawScene()
{
    scene->clear(); // очищаем чтобы не было утечек памяти
    //    ui->graphicsView->setScene(scene);  // Set graphics scene into graphicsView
    //    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    //    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // задается вручную
    int smallrectingleheight = 90; // высота прямоугольничка в пикселях задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int alerttextsize = smallrectingleheight/2;
    int smalltextsize = (smallrectingleheight - alerttextsize ) / 4;


    //redice size anyway
    alerttextsize/=1.5;
    smalltextsize/=1.2;
    // задаем координаты отображения квадратов
    channel1.xposition = 0;
    channel1.yposition = 0;
    channel1.w = smallrectinglewidth;
    channel1.h = smallrectingleheight;

    channel2.xposition = smallrectinglewidth;
    channel2.yposition = 0;
    channel2.w = smallrectinglewidth;
    channel2.h = smallrectingleheight;

    channel3.xposition = smallrectinglewidth*2;
    channel3.yposition = 0;
    channel3.w = smallrectinglewidth;
    channel3.h = smallrectingleheight;

    channel4.xposition = smallrectinglewidth*3;
    channel4.yposition = 0;
    channel4.w = smallrectinglewidth;
    channel4.h = smallrectingleheight;

    // создаем лист объектов для его отображения на графике


    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    int ch = 0;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();

            // рисуем прямоугольник  с заполненным цветом
            scene->addRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h,QPen(Qt::black),QBrush(Chanel->GetStateDependentColor(), Qt::SolidPattern));
            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
//                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
//                else
//                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }

            QGraphicsTextItem *ChannelValueText = scene->addText(ChannelValueString); //ChannelValueString
            ChannelValueText->setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

            int xoffset = GetXOffset(smallrectinglewidth, ChannelValueText);
            ChannelValueText->setPos(Chanel->xposition + xoffset, 5);

            QGraphicsTextItem *ChannelNameText = scene->addText(Chanel->GetChannelName());
            ChannelNameText->setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            xoffset = GetXOffset(smallrectinglewidth, ChannelNameText);
            ChannelNameText->setPos(Chanel->xposition + xoffset, -5);

            QGraphicsTextItem *UnitsNameText = scene->addText(Chanel->GetUnitsName());
            UnitsNameText->setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            xoffset = GetXOffset(smallrectinglewidth, UnitsNameText);
            UnitsNameText->setPos(Chanel->xposition + xoffset, 55);

            if (( isChannelInMaxNow(ch) || isChannelInMinNow(ch)/*Chanel->MinimumNow() || Chanel->MaximumNow()*/) )
            {
                ChannelValueText->setDefaultTextColor(Qt::red);
                ChannelNameText->setDefaultTextColor(Qt::red);
            }
            else
            {
                ChannelValueText->setDefaultTextColor(Qt::black);
                ChannelNameText->setDefaultTextColor(Qt::black);
            }
            //if (Chanel->IsChannelMathematical()) // учесть позже матем.канал.
            //painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);

            ch++;
        }
    }


}

void MainWindow::AddValuesToBuffer()
{
    X_Coordinates.append(xoffset); // добавляем смещение по иксу
    X_Coordinates_archive.append(xoffset);
    X_Date_Coordinates.append(QDateTime::currentDateTime());

    Y_coordinates_Chanel_1.append(channel1.GetCurrentChannelValue());
    Y_coordinates_Chanel_2.append(channel2.GetCurrentChannelValue());
    Y_coordinates_Chanel_3.append(channel3.GetCurrentChannelValue());
    Y_coordinates_Chanel_4.append(channel4.GetCurrentChannelValue());

    Y_coordinates_Chanel_1_archive.append(channel1.GetCurrentChannelValue());
    Y_coordinates_Chanel_2_archive.append(channel2.GetCurrentChannelValue());
    Y_coordinates_Chanel_3_archive.append(channel3.GetCurrentChannelValue());
    Y_coordinates_Chanel_4_archive.append(channel4.GetCurrentChannelValue());

    while (X_Coordinates.length()>150)
    {
        X_Coordinates.removeFirst();
        X_Date_Coordinates.removeFirst();
        Y_coordinates_Chanel_1.removeFirst();
        Y_coordinates_Chanel_2.removeFirst();
        Y_coordinates_Chanel_3.removeFirst();
        Y_coordinates_Chanel_4.removeFirst();
    }

    //пока нет ограничений на объём хранения
    while (X_Coordinates_archive.length()>15000)
    {
        X_Coordinates_archive.removeFirst();
        Y_coordinates_Chanel_1_archive.removeFirst();
        Y_coordinates_Chanel_2_archive.removeFirst();
        Y_coordinates_Chanel_3_archive.removeFirst();
        Y_coordinates_Chanel_4_archive.removeFirst();
    }

    int tickstep = GetTickStep();

    // если последняя точка по иксу попала на тайм-лейбел, его нужно корректировать
    if (xoffset%tickstep==0)
    {
        LabelsCorrect();
    }

    xoffset++;
    SetPolarAngle(GetPolarAngle()+1);
}

void MainWindow::UpdateGraphics()
{
    needUpdatePainter = true;
    switch(systemOptions.display)
    {
    case cSystemOptions::TrendsCyfra:
        ui->stackedWidget->setCurrentIndex(1);
        GrafsUpdateTrends();
        break;
    case cSystemOptions::Bars :
        ui->stackedWidget->setCurrentIndex(0);
        GrafsUpdateBars();
        updateBars();
        break;
    case cSystemOptions::TrendsBars:
        ui->stackedWidget->setCurrentIndex(0);
        GrafsUpdateTrends();
        updateBars();
        break;
    case cSystemOptions::PolarBars:
        ui->stackedWidget->setCurrentIndex(0);
        updateBars();
        GrafsUpdateNone();
        break;
    case cSystemOptions::PolarCyfra:
        ui->stackedWidget->setCurrentIndex(1);
        GrafsUpdateNone();
        break;
    case cSystemOptions::Cyfra:
        ui->stackedWidget->setCurrentIndex(1);
//        GrafsUpdateNone();
        break;
    case cSystemOptions::Steel:
        updateSteelWidget();
        break;
    default:
        break;
    }
    selectWidgetDiagram();
}

void MainWindow::GrafsUpdateTrendsAndBars()
{
    // добавить проверку на нулевой входной массив, чтобы прилодение не вылетало!

    ui->customPlot->xAxis->setRange(xoffset-GetXRange(), xoffset+GetXRange());
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setName("graph #1");
    ui->customPlot->graph(0)->setData(channel1.GetChannelXBuffer(), channel1.GetChannelValuesBuffer());
    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(channel1.GetStateDependentColor());
    ui->customPlot->graph(0)->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(channel2.GetChannelXBuffer(),channel2.GetChannelValuesBuffer());
    graphPen.setColor(channel2.GetStateDependentColor());
    ui->customPlot->graph(1)->setPen(graphPen);


    ui->customPlot->addGraph();
    ui->customPlot->graph(2)->setData(channel3.GetChannelXBuffer(), channel3.GetChannelValuesBuffer());
    graphPen.setColor(channel3.GetStateDependentColor());
    ui->customPlot->graph(2)->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph(3)->setData(channel4.GetChannelXBuffer(), channel4.GetChannelValuesBuffer());
    graphPen.setColor(channel4.GetStateDependentColor());
    ui->customPlot->graph(3)->setPen(graphPen);


//    if (ui->showdots->checkState()) {
//        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel1.GetStateDependentColor(),Qt::red, 3));
//        ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel2.GetStateDependentColor(),Qt::red, 3));
//        ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel3.GetStateDependentColor(),Qt::red, 3));
//        ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel4.GetStateDependentColor(),Qt::red, 3));
//    }


//    if (ui->showcenterdots->checkState()) {
//        ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
//        ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
//        ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsStepCenter);
//        ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsStepCenter);
//    }


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

    int lastindex = 0;
    if(!X_Coordinates.isEmpty())
        lastindex = X_Coordinates.last();

    // делаем чтоб штрихпунктиром отображалась верхняя и нижняя величина на графике за  период

    double chan1higherstate = channel1.GetMaximumChannelValue();
    double chan2higherstate = channel2.GetMaximumChannelValue();
    double chan3higherstate = channel3.GetMaximumChannelValue();
    double chan4higherstate = channel4.GetMaximumChannelValue();

    double chan1lowerstate = channel1.GetMinimumChannelValue();
    double chan2lowerstate = channel2.GetMinimumChannelValue();
    double chan3lowerstate = channel3.GetMinimumChannelValue();
    double chan4lowerstate = channel4.GetMinimumChannelValue();

    updateBars();


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

    x1.append(430-300-36+lastindex);
    x1.append(460-300-36+lastindex);
    x2.append(470-300-24+lastindex);
    x2.append(500-300-24+lastindex);
    x3.append(510-300-12+lastindex);
    x3.append(540-300-12+lastindex);
    x4.append(550-300+lastindex);
    x4.append(580-300+lastindex);

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

    y1.append(channel1.GetCurrentChannelValue());
    y1.append(channel1.GetCurrentChannelValue());
    y2.append(channel2.GetCurrentChannelValue());
    y2.append(channel2.GetCurrentChannelValue());
    y3.append(channel3.GetCurrentChannelValue());
    y3.append(channel3.GetCurrentChannelValue());
    y4.append(channel4.GetCurrentChannelValue());
    y4.append(channel4.GetCurrentChannelValue());

    graphPen.setWidth(GraphWidthinPixels);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x1, y1);
    ui->customPlot->graph()->setBrush(QBrush(channel1.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x2, y2);
    ui->customPlot->graph()->setBrush(QBrush(channel2.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x3, y3);
    ui->customPlot->graph()->setBrush(QBrush(channel3.GetStateDependentColor()));
    graphPen.setColor(QColor(Qt::black));
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x4, y4);
    ui->customPlot->graph()->setBrush(QBrush(channel4.GetStateDependentColor()));
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

    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);

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
        //        arrow->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->ustavka1.getStateValue() );
        //        arrow->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->ustavka1.getStateValue() );
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow);
        arrow->deleteLater();

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
        arrow2->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        //        arrow2->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->ustavka2.getStateValue() );
        //        arrow2->end->setCoords(arrowsendcoords.at(barindex++)-1,Chanel->ustavka2.getStateValue() );
        arrow2->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow2);
        arrow2->deleteLater();


        QPointF Label1PixPoint = arrow->start->pixelPoint();
        QPointF Label2PixPoint = arrow2->start->pixelPoint();

        Label1PixPoint.setY(Label1PixPoint.y() - 20);
        Label2PixPoint.setY(Label2PixPoint.y() - 20);

        // add the text label at the top limit:
        QCPItemText *textLabelHi = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelHi);
        textLabelHi->position->setPixelPoint(Label1PixPoint);
        //        textLabelHi->setText(QString::number(Chanel->ustavka1.getStateValue() ));
        textLabelHi->setFont(QFont(Font, 8, QFont::Bold));
        textLabelHi->setColor(QColor(Qt::red));

        // add the text label at the bottom limit

        QCPItemText *textLabelLo = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelLo);
        textLabelLo->position->setPixelPoint(Label2PixPoint);
        //        textLabelLo->setText(QString::number(Chanel->ustavka2.getStateValue() ));
        textLabelLo->setFont(QFont(Font, 8, QFont::Bold));
        textLabelLo->setColor(QColor(Qt::green));

        textLabelHi->deleteLater();
        textLabelLo->deleteLater();
    }


    if (systemOptions.autoscale && !waitAutoScale /*ui->autoscalecheckbox->checkState()*/)
    {
        ui->customPlot->yAxis->rescale();
    }

    // add the helper arrows:

    if (systemOptions.arrows) // if it s needed
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1);
        ChannelsObjectsList.append(&channel2);
        ChannelsObjectsList.append(&channel3);
        ChannelsObjectsList.append(&channel4);

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

            int ystart = ui->customPlot->height() / 5;
            int xstart = ui->customPlot->width() - ui->customPlot->width() / 10;

            arrow->start->setPixelPoint(QPointF(xstart, ystart + ui->customPlot->height() * index / 5));
            arrow->end->setCoords(xoffset, Chanel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(Chanel->GetNormalColor(),1,  Qt::DashLine));
            ui->customPlot->addItem(arrow);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(QPointF(xstart+20, ystart + ui->customPlot->height() * index / 5));
            NameLabel->setText(Chanel->GetChannelName() );
            NameLabel->setFont(QFont(Font, 14, QFont::Bold));
            NameLabel->setColor(Chanel->GetNormalColor());

            ++index;

            arrow->deleteLater();
            NameLabel->deleteLater();
        }
    }

    //    int start = clock();

    ui->customPlot->replot();

    //    int endd = clock();
    //    qDebug() << endd - start ;
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems(); // если не сделать то стрелки будут дублироваться


}

void MainWindow::GrafsUpdateTrends()
{

    ui->customPlot->xAxis->setRange(xoffset-GetXRange(), xoffset+GetXRange());
    ui->customPlot->clearGraphs();

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(channel1.GetChannelXBuffer(), channel1.GetChannelValuesBuffer());
    //    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_1);

    //    updateBars();

    graphPen.setWidth(GraphWidthinPixels);
    graphPen.setColor(ColorCh1);

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    ui->customPlot->graph()->setData(channel2.GetChannelXBuffer(), channel2.GetChannelValuesBuffer());
    //    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_2);
    graphPen.setColor(ColorCh2);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(channel3.GetChannelXBuffer(), channel3.GetChannelValuesBuffer());
    //    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_3);
    graphPen.setColor(ColorCh3);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(channel4.GetChannelXBuffer(), channel4.GetChannelValuesBuffer());
    //    ui->customPlot->graph()->setData(X_Coordinates, Y_coordinates_Chanel_4);
    graphPen.setColor(ColorCh4);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    // авто масшабирование
    if (systemOptions.autoscale && !waitAutoScale /*ui->autoscalecheckbox->checkState()*/)
    {
        ui->customPlot->yAxis->rescale();
    }

    // add the helper arrow:

    if (systemOptions.arrows)
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1);
        ChannelsObjectsList.append(&channel2);
        ChannelsObjectsList.append(&channel3);
        ChannelsObjectsList.append(&channel4);

        // рисуем стрелки для каждого канала

        int index = 0;
        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);

            int ystart = ui->customPlot->height() / 5;
            int xstart = ui->customPlot->width() - ui->customPlot->width() / 10;

            arrow->start->setPixelPoint(QPointF(xstart, ystart + ui->customPlot->height() * index / 5));
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
            NameLabel->position->setPixelPoint(QPointF(xstart+20, ystart + ui->customPlot->height() * index / 5));
            NameLabel->setText(Chanel->GetChannelName() );
            NameLabel->setFont(QFont(Font, 14, QFont::Bold));
            NameLabel->setColor(Chanel->GetNormalColor());

            ++index;

            arrow->deleteLater();
            NameLabel->deleteLater();
        }
    }

    ui->customPlot->setAntialiasedElements(QCP::aeAll);

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateNone()
{

    ui->MessagesWidget->update();

//    ui->customPlot->clearGraphs();
//    ui->customPlot->clearItems();

//    ui->customPlot->replot();
//    ui->customPlot->clearGraphs();
//    ui->customPlot->clearItems();
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

    Bar1_Y_Coord.append(channel1.GetCurrentChannelValue());
    Bar1_Y_Coord.append(channel1.GetCurrentChannelValue());
    Bar2_Y_Coord.append(channel2.GetCurrentChannelValue());
    Bar2_Y_Coord.append(channel2.GetCurrentChannelValue());
    Bar3_Y_Coord.append(channel3.GetCurrentChannelValue());
    Bar3_Y_Coord.append(channel3.GetCurrentChannelValue());
    Bar4_Y_Coord.append(channel4.GetCurrentChannelValue());
    Bar4_Y_Coord.append(channel4.GetCurrentChannelValue());

    QVector<double> x1lim,x2lim,x3lim,x4lim;
    QVector<double> y1max,y2max,y3max,y4max;
    QVector<double> y1min,y2min,y3min,y4min;

    // делаем чтоб штрихпунктиром отображалась верхняя и нижняя величина на графике за  период

    double chan1higherstate = channel1.GetMaximumChannelValue();
    double chan2higherstate = channel2.GetMaximumChannelValue();
    double chan3higherstate = channel3.GetMaximumChannelValue();
    double chan4higherstate = channel4.GetMaximumChannelValue();

    double chan1lowerstate = channel1.GetMinimumChannelValue();
    double chan2lowerstate = channel2.GetMinimumChannelValue();
    double chan3lowerstate = channel3.GetMinimumChannelValue();
    double chan4lowerstate = channel4.GetMinimumChannelValue();

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
    ui->customPlot->graph()->setBrush(QBrush(ColorCh1)); // first graph will be filled with translucent blue
    graphPen.setColor(ColorCh1 );
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar2_X_Coord, Bar2_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(ColorCh2)); // first graph will be filled with translucent blue
    graphPen.setColor(ColorCh2);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar3_X_Coord, Bar3_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(ColorCh3)); // first graph will be filled with translucent blue
    graphPen.setColor(ColorCh3);
    ui->customPlot->graph()->setPen(graphPen);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(Bar4_X_Coord, Bar4_Y_Coord);
    ui->customPlot->graph()->setBrush(QBrush(ColorCh4)); // first graph will be filled with translucent blue
    graphPen.setColor(ColorCh4);
    ui->customPlot->graph()->setPen(graphPen);

    LabelsBar.clear();
    LabelsBar.append("0");LabelsBar.append(channel1.GetChannelName());LabelsBar.append(channel2.GetChannelName());LabelsBar.append(channel3.GetChannelName());LabelsBar.append(channel4.GetChannelName());

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

    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);

    QList<int> arrowsendcoords;

    arrowsendcoords.append(Bar1_X_Coord.at(0) - 1);
    arrowsendcoords.append(Bar2_X_Coord.at(0) - 1);
    arrowsendcoords.append(Bar3_X_Coord.at(0) - 1);
    arrowsendcoords.append(Bar4_X_Coord.at(0) - 1);

    // рисуем стрелки для каждой уставки

    int barindex = 0 ;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList)
    {
        QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
        arrow->setPen(QPen(Qt::red, 3, Qt::SolidLine));
        //        arrow->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->ustavka1.getStateValue() );
        //        arrow->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->ustavka1.getStateValue() );
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        ui->customPlot->addItem(arrow);

        QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
        arrow2->setPen(QPen(Qt::green, 3, Qt::SolidLine));
        //        arrow2->start->setCoords(arrowsendcoords.at(barindex)-4,Chanel->ustavka2.getStateValue() );
        //        arrow2->end->setCoords(arrowsendcoords.at(barindex)-1,Chanel->ustavka2.getStateValue() );
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
        //        textLabelHi->setText(QString::number(Chanel->ustavka1.getStateValue() ));
        textLabelHi->setFont(QFont(Font, 8, QFont::Bold));
        textLabelHi->setColor(QColor(Qt::red));

        // add the text label at the bottom limit
        QCPItemText *textLabelLo = new QCPItemText(ui->customPlot);
        ui->customPlot->addItem(textLabelLo);
        textLabelLo->position->setPixelPoint(Label2PixPoint);
        //        textLabelLo->setText(QString::number(Chanel->ustavka2.getStateValue() ));
        textLabelLo->setFont(QFont(Font, 8, QFont::Bold));
        textLabelLo->setColor(QColor(Qt::green));

        ++barindex;

        arrow->deleteLater();
        arrow2->deleteLater();
        textLabelHi->deleteLater();
        textLabelLo->deleteLater();
    }

    if (systemOptions.autoscale && !waitAutoScale /*ui->autoscalecheckbox->checkState()*/)
    {
        ui->customPlot->yAxis->rescale();
    }

    ui->customPlot->replot();
    ui->customPlot->clearGraphs();
    ui->customPlot->clearItems();
}

void MainWindow::UpdateChannel1Slot()
{
//    qDebug() << "1Slot" << "in";
    if(channel1.GetSignalType() == 0) return;
    if(systemOptions.display == cSystemOptions::Steel) return;
    DataBuffer::writeupdatestatus(0,true);
    int period = channel1.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(0);
    int slot = csc.getSlotByChannel(devCh);
    //    uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = cRegistersMap::getOffsetByName(/*"DataChan0"*/"chan0Data");
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
    //    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
    //    channel1.SetCurrentChannelValue(DataBuffer::readchannelvalue(0));
    //    CheckAndLogginStates(channel1);

    /* Test */
#ifdef RANDOM_CHAN
    randVal[0] += ((double)((rand()%101) - 50) / 100);
    channel1.SetCurrentChannelValue(randVal[0]);
    ui->wBar_1->setVolue(randVal[0]);
    ui->widgetVol1->setVol(randVal[0]);
#endif

    channeltimer1->setInterval(period);
//    qDebug() << "1Slot" << "out";
}

void MainWindow::UpdateChannel2Slot()
{
//    qDebug() << "2Slot" << "in";
    if(channel2.GetSignalType() == 0) return;
    if(systemOptions.display == cSystemOptions::Steel) return;
    DataBuffer::writeupdatestatus(1,true);
    int period = channel2.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(1);
    int slot = csc.getSlotByChannel(devCh);
    //uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = cRegistersMap::getOffsetByName(/*"DataChan1"*/"chan1Data");
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
//        qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
//        qDebug() << "sendTransToWorker";
    //    channel2.SetCurrentChannelValue(DataBuffer::readchannelvalue(1));
    //    CheckAndLogginStates(channel2);

    /* Test */
#ifdef RANDOM_CHAN
    randVal[1] += ((double)((rand()%101) - 50) / 100);
    channel2.SetCurrentChannelValue(randVal[1]);
    ui->wBar_2->setVolue(randVal[1]);
    ui->widgetVol2->setVol(randVal[1]);
#endif

    channeltimer2->setInterval(period);
//    qDebug() << "2Slot" << "out";
}

void MainWindow::UpdateChannel3Slot()
{
//    qDebug() << "3Slot" << "in";
    if(channel3.GetSignalType() == 0) return;
    if(systemOptions.display == cSystemOptions::Steel) return;
    DataBuffer::writeupdatestatus(2,true);
    int period = channel3.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(2);
    int slot = csc.getSlotByChannel(devCh);
    //    uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = cRegistersMap::getOffsetByName(/*"DataChan2"*/"chan2Data");
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
    //    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
    //    channel3.SetCurrentChannelValue(DataBuffer::readchannelvalue(2));
    //    CheckAndLogginStates(channel3);

    /* Test */
#ifdef RANDOM_CHAN
    randVal[2] += ((double)((rand()%101) - 50) / 100);
    channel3.SetCurrentChannelValue(randVal[2]);
    ui->wBar_3->setVolue(randVal[2]);
    ui->widgetVol3->setVol(randVal[2]);
#endif

    channeltimer3->setInterval(period);
//    qDebug() << "3Slot" << "out";
}

void MainWindow::UpdateChannel4Slot()
{
//    qDebug() << "4Slot" << "in";
    if(channel4.GetSignalType() == 0) return;
    if(systemOptions.display == cSystemOptions::Steel) return;
    DataBuffer::writeupdatestatus(3,true);
    int period = channel4.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(3);
    int slot = csc.getSlotByChannel(devCh);
    //    uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = cRegistersMap::getOffsetByName(/*"DataChan3"*/"chan3Data");
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
    //    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
    //    channel4.SetCurrentChannelValue(DataBuffer::readchannelvalue(3));
    //    CheckAndLogginStates(channel4);

    /* Test */
#ifdef RANDOM_CHAN
    randVal[3] += ((double)((rand()%101) - 50) / 100);
    channel4.SetCurrentChannelValue(randVal[3]);
    ui->wBar_4->setVolue(randVal[3]);
    ui->widgetVol4->setVol(randVal[3]);
#endif

    channeltimer4->setInterval(period);
//    qDebug() << "4Slot" << "out";
}



void MainWindow::updateBars(void)
{
    ui->wBar_1->setExtr(channel1.GetMinimumChannelValue(), channel1.GetMaximumChannelValue());
    ui->wBar_2->setExtr(channel2.GetMinimumChannelValue(), channel2.GetMaximumChannelValue());
    ui->wBar_3->setExtr(channel3.GetMinimumChannelValue(), channel3.GetMaximumChannelValue());
    ui->wBar_4->setExtr(channel4.GetMinimumChannelValue(), channel4.GetMaximumChannelValue());

    ui->wBar_1->setBarDiapazon(max(abs(channel1.GetHigherMeasureLimit()), \
                                   abs(channel1.GetLowerMeasureLimit())));
    ui->wBar_2->setBarDiapazon(max(abs(channel2.GetHigherMeasureLimit()), \
                                   abs(channel2.GetLowerMeasureLimit())));
    ui->wBar_3->setBarDiapazon(max(abs(channel3.GetHigherMeasureLimit()), \
                                   abs(channel3.GetLowerMeasureLimit())));
    ui->wBar_4->setBarDiapazon(max(abs(channel4.GetHigherMeasureLimit()), \
                                   abs(channel4.GetLowerMeasureLimit())));

//    ui->wBar_1->resetLim();
//    ui->wBar_2->resetLim();
//    ui->wBar_3->resetLim();
//    ui->wBar_4->resetLim();
    ui->wBar_1->cleanMarker();
    ui->wBar_2->cleanMarker();
    ui->wBar_3->cleanMarker();
    ui->wBar_4->cleanMarker();
    foreach(Ustavka * ust, ustavkaObjectsList)
    {
        if(ust->getChannel() == 1)
        {
//            ui->wBar_1->setLim(ust->getLowStateValue(), \
//                               ust->getHiStateValue());
            ui->wBar_1->addMarker(ust->getHiStateValue(), ust->getTypeFix());
//            ui->wBar_1->addMarker(ust->getLowStateValue(), false);
        }
        if(ust->getChannel() == 2)
        {
//            ui->wBar_2->setLim(ust->getLowStateValue(), \
//                               ust->getHiStateValue());
            ui->wBar_2->addMarker(ust->getHiStateValue(), ust->getTypeFix());
//            ui->wBar_2->addMarker(ust->getLowStateValue(), false);
        }
        if(ust->getChannel() == 3)
        {
//            ui->wBar_3->setLim(ust->getLowStateValue(), \
//                               ust->getHiStateValue());
            ui->wBar_3->addMarker(ust->getHiStateValue(), ust->getTypeFix());
//            ui->wBar_3->addMarker(ust->getLowStateValue(), false);
        }
        if(ust->getChannel() == 4)
        {
//            ui->wBar_4->setLim(ust->getLowStateValue(), \
//                               ust->getHiStateValue());
            ui->wBar_4->addMarker(ust->getHiStateValue(), ust->getTypeFix());
//            ui->wBar_4->addMarker(ust->getLowStateValue(), false);
        }
    }
}

void MainWindow::setStyleBars()
{
    ui->wBar_1->setColor(ColorCh1, ColorCh1Light);
    ui->wBar_2->setColor(ColorCh2, ColorCh2Light);
    ui->wBar_3->setColor(ColorCh3, ColorCh3Light);
    ui->wBar_4->setColor(ColorCh4, ColorCh4Light);

    ui->wBar_1->setText(channel1.GetChannelName(), channel1.GetUnitsName());
    ui->wBar_2->setText(channel2.GetChannelName(), channel2.GetUnitsName());
    ui->wBar_3->setText(channel3.GetChannelName(), channel3.GetUnitsName());
    ui->wBar_4->setText(channel4.GetChannelName(), channel4.GetUnitsName());
}

void MainWindow::updateWidgetsVols(void)
{
    ui->widgetVol1->setText(channel1.GetChannelName(), channel1.GetUnitsName());
    ui->widgetVol2->setText(channel2.GetChannelName(), channel2.GetUnitsName());
    ui->widgetVol3->setText(channel3.GetChannelName(), channel3.GetUnitsName());
    ui->widgetVol4->setText(channel4.GetChannelName(), channel4.GetUnitsName());
    ui->widgetVol1->setColor(channel1.GetNormalColor());
    ui->widgetVol2->setColor(channel2.GetNormalColor());
    ui->widgetVol3->setColor(channel3.GetNormalColor());
    ui->widgetVol4->setColor(channel4.GetNormalColor());
}

void MainWindow::selectWidgetDiagram(void)
{
    if ( (systemOptions.display == cSystemOptions::BarsCyfra) || \
         (systemOptions.display == cSystemOptions::TrendsCyfraBars) || \
         (systemOptions.display == cSystemOptions::TrendsBars) || \
         (systemOptions.display == cSystemOptions::TrendsCyfra) || \
         (systemOptions.display == cSystemOptions::Trends) )
    {
        ui->stackedWidgetDiagram->setCurrentIndex(0);
        ui->right->show();
        ui->left->show();
        ui->frameSteel->hide();
    }
    else if((systemOptions.display == cSystemOptions::Cyfra) || \
            (systemOptions.display == cSystemOptions::Bars))
    {
        ui->left->hide();
    }
    else if(systemOptions.display == cSystemOptions::Polar ||\
            systemOptions.display == cSystemOptions::PolarBars ||\
            systemOptions.display == cSystemOptions::PolarCyfra)
    {
        ui->stackedWidgetDiagram->setCurrentIndex(1);
        ui->right->show();
        ui->left->show();
        ui->frameSteel->hide();
    }
    else if(systemOptions.display == cSystemOptions::Steel)
    {
        ui->right->hide();
        ui->left->hide();
        ui->frameSteel->show();
    }
}

void MainWindow::updateSteel()
{
    if(!slotSteelOnline) return;

    /*
     * Управление реле
     */
    for(int i = 0; i<listSteel.size(); i++)
    {
        cSteel * steel = listSteel.at(i);
        int countRele = SUM_RELAYS;
        uint8_t relayStates[countRele];
        memset(relayStates, 0, countRele * sizeof(relayStates[0]));
        if(steel->status == StatusCh_SteelWaitData)
        {
            relayStates[1] = 1;
        }
        else if(steel->status == StatusCh_SteelUpdateData)
        {
            relayStates[2] = 1;
        }
        else if((steel->status == StatusCh_SteelNotFoundSquareTemp)\
                || (steel->status == StatusCh_SteelNotFoundSquareEds)\
                || (steel->status == StatusCh_SteelNotFoundSquares)\
                || (steel->status == StatusCh_SteelSquaresOK))
        {
            relayStates[3] = 1;
        }
        else if(steel->status == StatusCh_WaitConf) //обрыв датчика
        {
            relayStates[0] = 1;
        }

        for(int i = 0; i < countRele; i++)
        {
            int8_t relay = steel->relais[i];
            if(relay != -1)
            {
                if(listRelais.at(relay)->newState != relayStates[i])
                {
                    listRelais.at(relay)->newState = relayStates[i];
                    Transaction tr(Transaction::W, (uint8_t)listRelais.at(relay)->mySlot);
                    uint8_t numDevRelay = listRelais.at(relay)->myPhysicalNum;
                    if(relay%2)
                        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyLo");
                    else
                        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyHi");
                    tr.volInt = relayStates[i];
                    emit sendTransToWorker(tr);
                    steel->countRelayTime = 4000 / UpdateSteelTime;
                }
                if((listRelais.at(relay)->newState == 1) && (i == 3))
                {
                    if(steel->countRelayTime > 0)
                        steel->countRelayTime--;
                    else
                    {
                        Transaction tr(Transaction::W, (uint8_t)listRelais.at(relay)->mySlot);
                        uint8_t numDevRelay = listRelais.at(relay)->myPhysicalNum;
                        if(relay%2)
                            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyLo");
                        else
                            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyHi");
                        tr.volInt = 0;
                        emit sendTransToWorker(tr);
                    }
                }

            }

        }
    }

    //Запрос статусов входных групп внезависимости от текущего состояния виджетов
    Transaction tr(Transaction::R, (uint8_t)ssc.getSlotBySteel(steelReadyNum));
    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(0)) + "Status");
    emit sendTransToWorker(tr);
    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(1)) + "Status");
    emit sendTransToWorker(tr);
//    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(2)) + "Status");
//    emit sendTransToWorker(tr);
//    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(3)) + "Status");
//    emit sendTransToWorker(tr);

    if(stateWidgetSteel == STEEL_READY)
    {   //в плате STEEL имеются данные для получения

//        if((listSteel.at(steelReadyNum)->vectorEdsReceived) && \
//                (listSteel.at(steelReadyNum)->vectorTempReceived))
//        {
//            if(!listSteel.at(steelReadyNum)->allVectorsReceived)
//            {
//                //запись в журнал
//                logginSteel(steelReadyNum);
//                writeArchiveSteel(steelReadyNum);
//            }
//            listSteel.at(steelReadyNum)->allVectorsReceived = true;
//        }
//        else
//        {
//            listSteel.at(steelReadyNum)->allVectorsReceived = false;
//        }

        if(listSteel.at(steelReadyNum)->allVectorsReceived)
        {
            //запись в журнал
            logginSteel(steelReadyNum);
            writeArchiveSteel(steelReadyNum);
            listSteel.at(steelReadyNum)->allVectorsReceived = false;
        }

        if((listSteel.at(steelReadyNum)->status == StatusCh_SteelUpdateData)\
                || (listSteel.at(steelReadyNum)->status == StatusCh_SteelErrorTC)\
                || (listSteel.at(steelReadyNum)->status == StatusCh_SteelErrorEds))
        {
//            steelReady = false;
//            steelMeasure = false;
            stateWidgetSteel = STEEL_WAIT;
        }

//        return;
    }
    else if(stateWidgetSteel == STEEL_MEASURE)
    {   //началось измерение температуры и поиск площадки
        Transaction tr(Transaction::R, (uint8_t)ssc.getSlotBySteel(steelReadyNum));
        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(0)) + "Data");
        emit sendTransToWorker(tr);
//        for(int i = 1; i <= 10; i ++)
//        {
        if((numArraySteel < 5) && askNewArray)
        {
            //запрос текущего участка графика ЭДС
            tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(numArraySteel+1));
            emit sendTransToWorker(tr);
            //запрос текущего участка графика температуры
            tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(numArraySteel+6));
            emit sendTransToWorker(tr);
            askNewArray = false;
            listSteel.at(steelReadyNum)->lastItemEds = false;
            listSteel.at(steelReadyNum)->lastItemTemp = false;
        }
//        }
        if((listSteel.at(steelReadyNum)->status == StatusCh_SteelWaitData)\
                || (listSteel.at(steelReadyNum)->status == StatusCh_WaitConf))
        {
            stateWidgetSteel = STEEL_WAIT;
        }
    }

    for(int i = 0; i<listSteel.size(); i++)
    {   // поочерёдный опрос статуса всех входных групп
        cSteel * steel = listSteel.at(i);
        int slot = ssc.getSlotBySteel(i);
        int devCh = ssc.getDevSteel(i);
        Transaction tr(Transaction::R, (uint8_t)slot, 0, 0);

        if((steel->status == StatusCh_SteelSquaresOK) ||\
                (steel->status == StatusCh_SteelNotFoundSquareTemp) || \
                (steel->status == StatusCh_SteelNotFoundSquareEds) || \
                (steel->status == StatusCh_SteelNotFoundSquares))
        {   // i-тая входная группа нашла площадку и имеются готовые данные
            QList<QString> str;
            str << "Data" << "OxActivity" << "MassAl" << "MassAl" << "PrimaryActivity" << "Carbon";
            if(listSteel.at(steelReadyNum)->technology->COH != 0)
            {
                foreach (QString s, str) {
                    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(devCh) + s);
                    emit sendTransToWorker(tr);
                }
            }
            else
            {
                tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(devCh) + "Data");
                emit sendTransToWorker(tr);
            }
//            for(int i = 1; i <= 10; i ++)
//            {
//                tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(i));
//                emit sendTransToWorker(tr);
//            }
            //очистка векторов для новых данных
//            QVector<double> vec = QVector(SIZE_ARRAY, NAN);
//            steel->vectorTemp = QVector<double>(SIZE_ARRAY, NAN);
//            steel->vectorEds = QVector<double>(SIZE_ARRAY, NAN);
            //очистка признаков завершения получения векторов (только всё начинается)
//            steel->allVectorsReceived = false;
            steel->vectorTempReceived = false;
            steel->vectorEdsReceived = false;
            steel->allVectorsReceived = false;
            steel->alg = 0x7FFF;
            steel->ao = 0x7FFF;
            steel->temp = NAN;
            steel->eds = NAN;
            steel->cl = NAN;
            //данные на стороне платы STEEL готовы - запомним
//            steelReady = true;  //данные готовы
            stateWidgetSteel = STEEL_READY;
            steelReadyNum = i;  //запоминаем номер входной группы
//            steelSelectFrame = false;   //разрешаем показывать график
            steel->timeUpdateData = QDateTime::currentDateTime();
               break;  //выход из цикла, на первой же входной группе с площадкой
        }
        else if(steel->status == StatusCh_SteelUpdateData)
        {
            if(stateWidgetSteel != STEEL_MEASURE)
            {
                stateWidgetSteel = STEEL_MEASURE;
                steelSelectFrame = false;
                steelReadyNum = i;
                numArraySteel = 0;
                askNewArray = true;
                steel->vectorEds = QVector<double>(SIZE_ARRAY, NAN);
                steel->vectorTemp = QVector<double>(SIZE_ARRAY, NAN);
            }
        }
    }
}

void MainWindow::updateSteelWidget(void)
{
//    simulatorSteel();

    QList<QString> strings;
    strings << "ОТКЛЮЧЕНО" << " " << "ОБРЫВ" \
            << " " << " " << "ГОТОВ" \
            << "ИЗМЕРЕНИЕ" << " " << "ОШИБКА ТС" \
            << "ОШИБКА ЭДС" << "ВРЕМЯ" << "ВРЕМЯ" \
            << "ВРЕМЯ" << "ВРЕМЯ";
    QList<QString> colorStyles;
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_4.red()) + ", " + QString::number(COLOR_4.green()) + ", " + QString::number(COLOR_4.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_2.red()) + ", " + QString::number(COLOR_2.green()) + ", " + QString::number(COLOR_2.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    ui->steelStatus1->setText(strings.at(listSteel.at(0)->status));
    ui->steelStatus2->setText(strings.at(listSteel.at(1)->status));
    ui->steelStatus3->setText(strings.at(listSteel.at(2)->status));
    ui->steelStatus4->setText(strings.at(listSteel.at(3)->status));

    ui->steelStatus1->setStyleSheet(colorStyles.at(listSteel.at(0)->status));
    ui->steelStatus2->setStyleSheet(colorStyles.at(listSteel.at(1)->status));
    ui->steelStatus3->setStyleSheet(colorStyles.at(listSteel.at(2)->status));
    ui->steelStatus4->setStyleSheet(colorStyles.at(listSteel.at(3)->status));

    ui->PlavkaNum->setText(QString::number(listSteel.at(0)->numSmelt));
    ui->PlavkaNum_2->setText(QString::number(listSteel.at(1)->numSmelt));
    ui->PlavkaNum_3->setText(QString::number(listSteel.at(2)->numSmelt));
    ui->PlavkaNum_4->setText(QString::number(listSteel.at(3)->numSmelt));

    if((stateWidgetSteel == STEEL_READY) || (stateWidgetSteel == STEEL_MEASURE))
    {   //найдена площадка
        //steelReadyNum
        cSteel * steel = listSteel.at(steelReadyNum);
        ui->nameSteelTech->setText(steel->technology->name);

        if(stateWidgetSteel == STEEL_READY)
        {
            ui->labelTimeSteel->setText(steel->timeUpdateData.toString("dd.MM.yy hh:mm:ss"));
            if(!std::isnan(steel->temp))
                ui->steelTemp->setText(QString::number(steel->temp));
            if(!std::isnan(steel->eds))
                ui->steelEmf->setText(QString::number(steel->eds));
            if(steel->ao != 0x7FFF)
                ui->steelAO->setText(QString::number(steel->ao));
            if(steel->alg != 0x7FFF)
                ui->steelAl->setText(QString::number(steel->alg));
            if(!std::isnan(steel->cl))
                ui->steelC->setText(QString::number(steel->cl));
        }

        if(!steelSelectFrame)
        {
            ui->framePlotSteel->show();
            ui->frameSteelStatus->hide();
            ui->frameTemperature->hide();
        }
        else
        {
            ui->framePlotSteel->hide();
            ui->frameSteelStatus->show();
            ui->frameTemperature->hide();
        }

//        if(listSteel.at(steelReadyNum)->allVectorsReceived)
//        {
            Y_Coord_SteelTemp = listSteel.at(steelReadyNum)->vectorTemp;
//            if(listSteel.at(steelReadyNum)->technology->COH != 0)
//            {
                Y_Coord_SteelEds = listSteel.at(steelReadyNum)->vectorEds;
//            }
//            else
//            {
//                Y_Coord_SteelEds = QVector<double>(Y_Coord_SteelTemp.size(), NAN);
//            }
            X_Coord_Steel.resize(0);
            for(int i = 0; i < Y_Coord_SteelTemp.size(); i++)
            {
                X_Coord_Steel.append((i * PERIOD_MEASURE_STEEL) / 1000.0);
            }
            ui->plotSteel->clearGraphs();
            ui->plotSteel->addGraph();
            ui->plotSteel->graph()->setData(X_Coord_Steel, Y_Coord_SteelTemp);
            ui->plotSteel->xAxis->setLabel("t,sec");
            ui->plotSteel->yAxis2->setLabel("Emf, mV");
            ui->plotSteel->yAxis->setLabel("Temp, °C");
            ui->plotSteel->graph()->setPen(QPen(QBrush(ColorCh3), 2));

            ui->plotSteel->addGraph(ui->plotSteel->xAxis, ui->plotSteel->yAxis2);
            ui->plotSteel->graph()->setData(X_Coord_Steel, Y_Coord_SteelEds);
            ui->plotSteel->graph()->setPen(QPen(QBrush(ColorCh4), 2));
//            ui->plotSteel->rescaleAxes();
            double size = steel->technology->tPt;
            double position = size / 2;
            ui->plotSteel->xAxis->setRange(position, size, Qt::AlignCenter);
            size = (steel->technology->LPth - steel->technology->LPtl) * 1.1;
            position = (steel->technology->LPth + steel->technology->LPtl) / 2;
            ui->plotSteel->yAxis->setRange(position, size, Qt::AlignCenter);
            ui->plotSteel->yAxis2->setRange(100, 800, Qt::AlignCenter);
            ui->plotSteel->replot();
            ui->plotSteel->clearItems();
//        }
    }
    else
    {
        QString str = "Нет данных";
        ui->nameSteelTech->setText(str);
        ui->labelTimeSteel->setText(" ");
        ui->steelTemp->setText(str);
        ui->steelEmf->setText(str);
        ui->steelAO->setText(str);
        ui->steelAl->setText(str);
        ui->steelC->setText(str);
        ui->plotSteel->clearGraphs();

        ui->framePlotSteel->hide();
        ui->frameSteelStatus->show();
        ui->frameTemperature->hide();

        ui->steelTech1->setText(listSteel.at(0)->technology->name);
        ui->steelTech2->setText(listSteel.at(1)->technology->name);
        ui->steelTech3->setText(listSteel.at(2)->technology->name);
        ui->steelTech4->setText(listSteel.at(3)->technology->name);
    }
}

