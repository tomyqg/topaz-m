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

QVector<double> xx1, yy1, yy2, yy3, yy4;

void MainWindow::AddValuesToBuffer()
{
    UartDriver UD;
    xx1.append(b);
    yy1.append(UD.channelinputbuffer[0]);
    yy2.append(UD.channelinputbuffer[1]);
    yy3.append(UD.channelinputbuffer[2]);
    yy4.append(UD.channelinputbuffer[3]);
    b++;
}

void MainWindow::UpdateGraphics()
{
    switch( Options::DisplayParametr )
    {
    case Options::Trends:
        updatetrends();break;
    case Options::TrendsCyfra:
        updatetrends();break;
    case Options::Bars :
        updatebargraf();break;
    case Options::BarsCyfra :
        updatebargraf();break;
    case Options::TrendsBars:
        updatetrendsngrafs();break;
    case Options::TrendsCyfraBars:
        updatetrendsngrafs();break;
    case Options::Polar:
        justupdategraf();break;
    default:
        break;
    }
}

void MainWindow::updatetrendsngrafs()
{
    while (xx1.length()>300)
    {
        xx1.remove(0);yy1.remove(0);yy2.remove(0);yy3.remove(0);yy4.remove(0);
    }

    ui->customPlot->xAxis->setRange(b-300, b+300);

    if (b%60==0)
    {
        LabelsCorrect();
    }

    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(xx1, yy1);
    graphPen.setWidth(6);
    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {ui->customPlot->graph()->setData(xx1, yy2);
        graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx1, yy3);
        graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);}

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx1, yy4);
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

    int lastindex = xx1.at(xx1.length()-1);

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

void MainWindow::updatetrends()
{

    while (xx1.length()>300)
    {
        xx1.remove(0);yy1.remove(0);yy2.remove(0);yy3.remove(0);yy4.remove(0);
    }

    ui->customPlot->xAxis->setRange(b-300, b+300);

    if (b%60==0)
    {
        LabelsCorrect();
    }

    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(xx1, yy1);
    graphPen.setWidth(6);
    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));

    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();

    {ui->customPlot->graph()->setData(xx1, yy2);
        graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx1, yy3);
        graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);}

    {ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx1, yy4);
        graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
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

void MainWindow::justupdategraf()
{
    ui->customPlot->clearGraphs();
    ui->MessagesWidget->update();

    //    ui->customPlot->clearGraphs();
    //    ui->customPlot->replot();
}

void MainWindow::updatebargraf()
{
    QVector<double> x1,x2,x3,x4;
    QVector<double> y1,y2,y3,y4;

    x1.append(15);
    x1.append(25);
    x2.append(35);
    x2.append(45);
    x3.append(55);
    x3.append(65);
    x4.append(75);
    x4.append(85);

    y1.append(UartDriver::channelinputbuffer[0]);
    y1.append(UartDriver::channelinputbuffer[0]);
    y2.append(UartDriver::channelinputbuffer[1]);
    y2.append(UartDriver::channelinputbuffer[1]);
    y3.append(UartDriver::channelinputbuffer[2]);
    y3.append(UartDriver::channelinputbuffer[2]);
    y4.append(UartDriver::channelinputbuffer[3]);
    y4.append(UartDriver::channelinputbuffer[3]);

    ui->customPlot->clearGraphs();
    ui->customPlot->xAxis->setRange(0, 100);
    graphPen.setWidth(1);
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("Bargraf");

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
        if (ch1.IsMathematical())
        {
            currentdata = mathres.Solve(ch1.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(1,currentdata);

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
        if (ch2.IsMathematical())
        {
            currentdata = mathres.Solve(ch2.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        pressure = currentdata;
        UD.writechannelvalue(2,pressure);

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
        if (ch3.IsMathematical())
        {
            currentdata = mathres.Solve(ch3.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(3,currentdata);

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
        if (ch4.IsMathematical())
        {
            currentdata = mathres.Solve(ch4.GetMathString(), currentdata); // + mathres.Solve("sin(x)*10", currentdata); //sqrt(abs(x))+20
        }
        UD.writechannelvalue(4,currentdata);

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
    }
    int period = ch4.GetMeasurePeriod()*1000;
    channeltimer4->setInterval(period);
}

void MainWindow::PaintOnWidget()
{
    switch( Options::DisplayParametr )
    {
    case Options::Cyfra:
        PaintCyfrasFullScreen();break;
    case Options::TrendsCyfra:
        PaintCyfrasBottom();break;
    case Options::TrendsCyfraBars:
        PaintCyfrasBottom();break;
    case Options::BarsCyfra:
        PaintCyfrasBottom();break;
    case Options::Polar:
        PaintPolarDiagramm();break;
    default:
        break;
    }
}

void MainWindow::PaintCyfrasBottom()
{
    int widgwidth  = ui->MessagesWidget->width();
    int widgheight  = ui->MessagesWidget->height();

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(Channel1Color, Qt::Dense4Pattern));
    painter.drawRect(2, 2+561, widgwidth*1/4, 96);
    painter.setBrush(QBrush(Channel2Color, Qt::Dense4Pattern));
    painter.drawRect(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96);
    painter.setBrush(QBrush(Channel3Color, Qt::Dense4Pattern));
    painter.drawRect(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96);
    painter.setBrush(QBrush(Channel4Color, Qt::Dense4Pattern));
    painter.drawRect(2+widgwidth*3/4, 2+561, widgwidth*1/4-4, 96);

    painter.setFont(QFont("Times New Roman", 50, QFont::ExtraBold));
    painter.drawText(2, 2+561, widgwidth*1/4, 96,     Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[0]));
    painter.drawText(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[1]));
    painter.drawText(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[2]));
    painter.drawText(2+widgwidth*3/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[3]));

    //    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 15, QFont::ExtraBold));
    painter.drawText(2, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop, ch1.GetChannelName());
    painter.drawText(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop,ch2.GetChannelName());
    painter.drawText(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop,ch3.GetChannelName());
    painter.drawText(2+widgwidth*3/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop,ch4.GetChannelName());

    painter.setFont(QFont("Times New Roman", 12, QFont::ExtraBold));
    painter.drawText(2, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignBottom, ch1.GetUnitsName());
    painter.drawText(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignBottom,ch2.GetUnitsName());
    painter.drawText(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignBottom,ch3.GetUnitsName());
    painter.drawText(2+widgwidth*3/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignBottom,ch4.GetUnitsName());
    painter.end();
}

void MainWindow::PaintCyfrasFullScreen()
{
    QPainter painter;

    int widgwidth  = ui->MessagesWidget->width();
    int widgheight  = ui->MessagesWidget->height();

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(Channel1Color, Qt::SolidPattern));
    painter.drawRect(2, 2, widgwidth/2-2, widgheight/2-2);
    painter.setBrush(QBrush(Channel2Color, Qt::SolidPattern));
    painter.drawRect(2+widgwidth/2-2, 2, widgwidth/2-2, widgheight/2-2);
    painter.setBrush(QBrush(Channel3Color, Qt::SolidPattern));
    painter.drawRect(2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2);
    painter.setBrush(QBrush(Channel4Color, Qt::SolidPattern));
    painter.drawRect(2+widgwidth/2-2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2);

    painter.setPen(QPen(Qt::cyan, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 110, QFont::ExtraBold));
    painter.drawText(2, 2, widgwidth/2-2, widgheight/2-2,     Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[0]));
    painter.drawText(2+widgwidth/2-2, 2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[1]));
    painter.drawText(2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[2]));
    painter.drawText(2+widgwidth/2-2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignVCenter,QString::number(UartDriver::channelinputbuffer[3]));

    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 50, QFont::ExtraBold));
    painter.drawText(2, 2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignTop, ch1.GetChannelName());
    painter.drawText(2+widgwidth/2-2, 2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignTop,ch2.GetChannelName());
    painter.drawText(2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignTop,ch3.GetChannelName());
    painter.drawText(2+widgwidth/2-2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignTop,ch4.GetChannelName());

    painter.setFont(QFont("Times New Roman", 50, QFont::ExtraBold));
    painter.drawText(2, 2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignBottom, ch1.GetUnitsName());
    painter.drawText(2+widgwidth/2-2, 2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignBottom,ch2.GetUnitsName());
    painter.drawText(2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignBottom,ch3.GetUnitsName());
    painter.drawText(2+widgwidth/2-2, 2+widgheight/2-2, widgwidth/2-2, widgheight/2-2, Qt::AlignHCenter | Qt::AlignBottom,ch4.GetUnitsName());
    painter.end();
}

void MainWindow::PaintPolarDiagramm()
{
    QPainter painter;
    int widgheight  = ui->MessagesWidget->height();
    double maximumradius;

    maximumradius = widgheight/4 - 10;

    painter.begin(ui->MessagesWidget);
    //    painter.setRenderHint(QPainter::Antialiasing, true);
    //    int channel1value = UartDriver::channelinputbuffer[0]/200*180;
    //    int channel2value = UartDriver::channelinputbuffer[1]/200*180;
    //    int channel3value = UartDriver::channelinputbuffer[2]/200*180;
    //    int channel4value = UartDriver::channelinputbuffer[3]/200*180;

    int channel1value = xx1.last();
    int channel2value = xx1.last();
    int channel3value = xx1.last();
    int channel4value = xx1.last();

    int channel1length = UartDriver::channelinputbuffer[0]/200*maximumradius;
    int channel2length = UartDriver::channelinputbuffer[1]/200*maximumradius;
    int channel3length = UartDriver::channelinputbuffer[2]/200*maximumradius;
    int channel4length = UartDriver::channelinputbuffer[3]/200*maximumradius;

    /* Create the line object: */
    QLineF Channel1Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel1Line.setP1(QPointF(widgheight/2-2, maximumradius + 10));
    Channel1Line.setAngle(channel1value);
    Channel1Line.setLength(channel1length);

    int x1 = Channel1Line.x2(); // мы берем координаты `1 точки
    int y1 = Channel1Line.y2(); // мы берем координаты второй точки

    /* Create the line object: */
    QLineF Channel2Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel2Line.setP1(QPointF(widgheight-2, maximumradius + 10));
    Channel2Line.setAngle(channel2value);
    Channel2Line.setLength(channel2length);
    int x2 = Channel2Line.x2(); // мы берем координаты `1 точки
    int y2 = Channel2Line.y2(); // мы берем координаты второй точки

    /* Create the line object: */
    QLineF Channel3Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel3Line.setP1(QPointF(widgheight/2-2, widgheight - (maximumradius + 10)));
    Channel3Line.setAngle(channel3value);
    Channel3Line.setLength(channel3length);
    int x3 = Channel3Line.x2(); // мы берем координаты `1 точки
    int y3 = Channel3Line.y2(); // мы берем координаты второй точки

    /* Create the line object: */
    QLineF Channel4Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel4Line.setP1(QPointF(widgheight-2,  widgheight - (maximumradius + 10)) );
    Channel4Line.setAngle(channel4value);
    Channel4Line.setLength(channel4length);

    int x4 = Channel4Line.x2(); // мы берем координаты `1 точки
    int y4 = Channel4Line.y2(); // мы берем координаты второй точки

    //qDebug() << Channel1Line.y2(); // мы берем координаты второй точки

    painter.setPen(QPen(Qt::green,2,  Qt::DashLine)); //, Qt::DashDotLine, Qt::RoundCap));

    painter.drawLine(Channel1Line);
    painter.drawLine(Channel2Line);
    painter.drawLine(Channel3Line);
    painter.drawLine(Channel4Line);

    QPoint p1,p2,p3,p4;
    p1.setX(x1);
    p1.setY(y1);

    p2.setX(x2);
    p2.setY(y2);

    p3.setX(x3);
    p3.setY(y3);

    p4.setX(x4);
    p4.setY(y4);

    points1.append(p1);
    points2.append(p2);
    points3.append(p3);
    points4.append(p4);

    if (xx1.last()>=360)
    {
        points1.removeFirst();
        points2.removeFirst();
        points3.removeFirst();
        points4.removeFirst();
    }

    painter.setRenderHint(QPainter::Antialiasing, true);

    //graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));
    //painter.setPen(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));

    painter.setPen(QPen(Channel1Color, 4));
    painter.drawPolyline(points1);
    painter.setPen(QPen(Channel2Color, 4));
    painter.drawPolyline(points2);
    painter.setPen(QPen(Channel3Color, 4));
    painter.drawPolyline(points3);
    painter.setPen(QPen(Channel4Color, 4));
    painter.drawPolyline(points4);
    painter.setRenderHint(QPainter::Antialiasing, false);

    //qDebug() << x;qDebug() << y;
    //qDebug() << points;

    painter.end();
}
