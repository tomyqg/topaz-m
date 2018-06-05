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

int xoffset=0;

double randVal[4] = {0.0, 0.0, 0.0, 0.0};


QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
QVector<QDateTime> X_Date_Coordinates;

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

#ifdef Q_OS_WIN32

#endif
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
                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
                else
                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
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
            if (Chanel->IsChannelMathematical()) // учесть позже матем.канал.
                ;//painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);

            ch++;
        }
    }


}

void MainWindow::DrawSceneBottom()
{
    if ( (Options::GetCurrentDisplayParametr() != Options::Polar) && \
         (Options::GetCurrentDisplayParametr() != Options::Cyfra) && \
         (Options::GetCurrentDisplayParametr() != Options::TrendsBars) && \
         (Options::GetCurrentDisplayParametr() != Options::Bars) && \
         (Options::GetCurrentDisplayParametr() != Options::TrendsCyfra) && \
         (Options::GetCurrentDisplayParametr() != Options::Trends) )
    {
        ui->customPlot->resize(ui->left->width()/*1024*//*533*/,ui->left->height() * 0.9/*550*/);
//        ui->graphicsView->show();
        DrawScene();   // Add vertical line via center

    }
    else
    {
//        ui->customPlot->resize(ui->left->width()/*1024*//*533*/,ui->left->height() * 0.9/*550*/);
//        ui->graphicsView->hide();

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
//    while (X_Coordinates_archive.length()>15000)
//    {
//        X_Coordinates_archive.removeFirst();
//        Y_coordinates_Chanel_1_archive.removeFirst();
//        Y_coordinates_Chanel_2_archive.removeFirst();
//        Y_coordinates_Chanel_3_archive.removeFirst();
//        Y_coordinates_Chanel_4_archive.removeFirst();
//    }

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

    needupdatePainter = 1;

    switch( Options::GetCurrentDisplayParametr() )
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
//        updateBars();
        updateWidgetsVols();
        GrafsUpdateNone();break;
    case Options::Cyfra:
        updateBars();
        GrafsUpdateNone();break;
    default:
        break;
    }

    selectWidgetDiagram();
//    DrawSceneBottom();
}

//void MainWindow::UpdateLog()
//{
////    messwrite.WriteAllLogToFile();
//    updLogTimer->setInterval(LogUpdTimer);
//}

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


    if (ui->showdots->checkState()) {
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel1.GetStateDependentColor(),Qt::red, 3));
        ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel2.GetStateDependentColor(),Qt::red, 3));
        ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel3.GetStateDependentColor(),Qt::red, 3));
        ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, channel4.GetStateDependentColor(),Qt::red, 3));
    }


    if (ui->showcenterdots->checkState()) {
        ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
        ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
        ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsStepCenter);
        ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsStepCenter);
    }


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


    if (ui->autoscalecheckbox->checkState())
    {
        ui->customPlot->yAxis->rescale();
    }

    // add the helper arrows:

    if (ui->arrowscheckBox->checkState()) // if it s needed
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

            arrow->deleteLater();
            NameLabel->deleteLater();
        }
    }

    int start = clock();

    ui->customPlot->replot();

    int endd = clock();
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

    // add the helper arrow:

    if (ui->arrowscheckBox->checkState())
    {
        // add the arrows:

        QList<ChannelOptions *> ChannelsObjectsList;

        ChannelsObjectsList.append(&channel1);
        ChannelsObjectsList.append(&channel2);
        ChannelsObjectsList.append(&channel3);
        ChannelsObjectsList.append(&channel4);

        // рисуем стрелки для каждой уставки

        int index = 0;
        foreach (ChannelOptions * Chanel, ChannelsObjectsList)
        {
            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);

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

            arrow->deleteLater();
            NameLabel->deleteLater();
        }
    }

    updateBars();

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
    if(channel1.GetSignalType() == 0) return;
    DataBuffer::writeupdatestatus(0,true);
    int period = channel1.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(0);
    int slot = csc.getSlotByChannel(devCh);
//    uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = RegisterMap::getOffsetByName("DataChan0"/*"chan0Data"*/);
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
//    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
    //    channel1.SetCurrentChannelValue(DataBuffer::readchannelvalue(0));
//    CheckAndLogginStates(channel1);

    /* Test */
//    randVal[0] += ((double)((rand()%101) - 50) / 100);
//    channel1.SetCurrentChannelValue(randVal[0]);
//    ui->wBar_1->setVolue(randVal[0]);
//    ui->widgetVol1->setVol(randVal[0]);

    channeltimer1->setInterval(period);
}

void MainWindow::UpdateChannel2Slot()
{
    if(channel2.GetSignalType() == 0) return;
    DataBuffer::writeupdatestatus(1,true);
    int period = channel2.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(1);
    int slot = csc.getSlotByChannel(devCh);
    //uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = RegisterMap::getOffsetByName("DataChan1"/*"chan1Data"*/);
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
//    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
    //    channel2.SetCurrentChannelValue(DataBuffer::readchannelvalue(1));
//    CheckAndLogginStates(channel2);

        /* Test */
//    randVal[1] += ((double)((rand()%101) - 50) / 100);
//    channel2.SetCurrentChannelValue(randVal[1]);
//    ui->wBar_2->setVolue(randVal[1]);
//    ui->widgetVol2->setVol(randVal[1]);

    channeltimer2->setInterval(period);
}

void MainWindow::UpdateChannel3Slot()
{
    if(channel3.GetSignalType() == 0) return;
    DataBuffer::writeupdatestatus(2,true);
    int period = channel3.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(2);
    int slot = csc.getSlotByChannel(devCh);
//    uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = RegisterMap::getOffsetByName("DataChan2"/*"chan2Data"*/);
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
//    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
//    channel3.SetCurrentChannelValue(DataBuffer::readchannelvalue(2));
//    CheckAndLogginStates(channel3);

    /* Test */
//    randVal[2] += ((double)((rand()%101) - 50) / 100);
//    channel3.SetCurrentChannelValue(randVal[2]);
//    ui->wBar_3->setVolue(randVal[2]);
//    ui->widgetVol3->setVol(randVal[2]);

    channeltimer3->setInterval(period);
}

void MainWindow::UpdateChannel4Slot()
{
    if(channel4.GetSignalType() == 0) return;
    DataBuffer::writeupdatestatus(3,true);
    int period = channel4.GetMeasurePeriod()*1000;
    int devCh = csc.getDevChannel(3);
    int slot = csc.getSlotByChannel(devCh);
//    uint32_t offset = getDevOffsetByChannel(devCh, ChannelOptions::chanData);
    uint16_t offset = RegisterMap::getOffsetByName("DataChan3"/*"chan3Data"*/);
    Transaction tr(Transaction::R, (uint8_t)slot, offset/*devCh*2*/, 0);
//    qDebug() << "MainWindow SIGNAL" << tr.offset;
    emit sendTransToWorker(tr);
//    channel4.SetCurrentChannelValue(DataBuffer::readchannelvalue(3));
//    CheckAndLogginStates(channel4);

    /* Test */
//    randVal[3] += ((double)((rand()%101) - 50) / 100);
//    channel4.SetCurrentChannelValue(randVal[3]);
//    ui->wBar_4->setVolue(randVal[3]);
//    ui->widgetVol4->setVol(randVal[3]);

    channeltimer4->setInterval(period);
}

void MainWindow::readReleSlot(uint8_t code)
{
    Transaction tr(Transaction::R, 2, 32799, 0);
    uint16_t offset = 32810;
    switch(code)
    {
    case 0:
        offset = 32810;
        break;
    case 1:
        offset = 32811;
        break;
    case 2:
        offset = 32938;
        break;
    case 3:
        offset = 32939;
        break;
    case 4:
        offset = 33055;
        break;
    case 5:
        offset = 33056;
        break;
    case 6:
        offset = 33183;
        break;
    case 7:
        offset = 33184;
        break;
    default:
        break;
    }
    tr.offset = offset;
    emit sendTransToWorker(tr);
}

void MainWindow::updateBars(void)
{
    ui->wBar_1->setExtr(channel1.GetMinimumChannelValue(), channel1.GetMaximumChannelValue());
    ui->wBar_2->setExtr(channel2.GetMinimumChannelValue(), channel2.GetMaximumChannelValue());
    ui->wBar_3->setExtr(channel3.GetMinimumChannelValue(), channel3.GetMaximumChannelValue());
    ui->wBar_4->setExtr(channel4.GetMinimumChannelValue(), channel4.GetMaximumChannelValue());

    ui->wBar_1->setColor(ColorCh1, ColorCh1Light);
    ui->wBar_2->setColor(ColorCh2, ColorCh2Light);
    ui->wBar_3->setColor(ColorCh3, ColorCh3Light);
    ui->wBar_4->setColor(ColorCh4, ColorCh4Light);

    ui->wBar_1->setText(channel1.GetChannelName(), channel1.GetUnitsName());
    ui->wBar_2->setText(channel2.GetChannelName(), channel2.GetUnitsName());
    ui->wBar_3->setText(channel3.GetChannelName(), channel3.GetUnitsName());
    ui->wBar_4->setText(channel4.GetChannelName(), channel4.GetUnitsName());

    ui->wBar_1->setBarDiapazon(max(abs(channel1.GetHigherMeasureLimit()), \
                                   abs(channel1.GetLowerMeasureLimit())));
    ui->wBar_2->setBarDiapazon(max(abs(channel2.GetHigherMeasureLimit()), \
                                   abs(channel2.GetLowerMeasureLimit())));
    ui->wBar_3->setBarDiapazon(max(abs(channel3.GetHigherMeasureLimit()), \
                                   abs(channel3.GetLowerMeasureLimit())));
    ui->wBar_4->setBarDiapazon(max(abs(channel4.GetHigherMeasureLimit()), \
                                   abs(channel4.GetLowerMeasureLimit())));

    ui->wBar_1->setLim(channel1.GetLowerLimit(), channel1.GetHigherLimit());
    ui->wBar_2->setLim(channel2.GetLowerLimit(), channel2.GetHigherLimit());
    ui->wBar_3->setLim(channel3.GetLowerLimit(), channel3.GetHigherLimit());
    ui->wBar_4->setLim(channel4.GetLowerLimit(), channel4.GetHigherLimit());
}

void MainWindow::updateWidgetsVols(void)
{
    ui->widgetVol1->setText(channel1.GetChannelName(), channel1.GetUnitsName());
    ui->widgetVol2->setText(channel2.GetChannelName(), channel2.GetUnitsName());
    ui->widgetVol3->setText(channel3.GetChannelName(), channel3.GetUnitsName());
    ui->widgetVol4->setText(channel4.GetChannelName(), channel4.GetUnitsName());
}

void MainWindow::selectWidgetDiagram(void)
{
    if ( (Options::GetCurrentDisplayParametr() == Options::BarsCyfra) || \
         (Options::GetCurrentDisplayParametr() == Options::TrendsCyfraBars) || \
         (Options::GetCurrentDisplayParametr() == Options::Cyfra) || \
         (Options::GetCurrentDisplayParametr() == Options::TrendsBars) || \
         (Options::GetCurrentDisplayParametr() == Options::Bars) || \
         (Options::GetCurrentDisplayParametr() == Options::TrendsCyfra) || \
         (Options::GetCurrentDisplayParametr() == Options::Trends) )
    {
        ui->stackedWidgetDiagram->setCurrentIndex(0);
    }
    else if(Options::GetCurrentDisplayParametr() == Options::Polar)
    {
        ui->stackedWidgetDiagram->setCurrentIndex(1);
    }
}
