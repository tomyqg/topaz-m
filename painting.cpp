#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"

extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;

extern QColor Channel1ColorNormal;
extern QColor Channel2ColorNormal ;
extern QColor Channel3ColorNormal;
extern QColor Channel4ColorNormal ;

extern QColor ChannelColorHighState;
extern QColor ChannelColorLowState;

extern QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;

void MainWindow::PaintCyfrasBottom()
{
    int widgwidth  = ui->MessagesWidget->width();
    int widgheight  = ui->MessagesWidget->height();

    QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    ModBus mb;

    // если связб плохая и прошло пол секунды то нужно мигнуть красным цветом
    if ( (GetHalfSecFlag() == 1)&&(mb.GetConnectFailureStatus() >0) )
    {
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+widgwidth*3/4, 2+561, widgwidth*1/4-4, 96);
    }
    else
    {
        painter.setBrush(QBrush(Channel1Color, Qt::Dense4Pattern));
        painter.drawRect(2, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Channel2Color, Qt::Dense4Pattern));
        painter.drawRect(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Channel3Color, Qt::Dense4Pattern));
        painter.drawRect(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Channel4Color, Qt::Dense4Pattern));
        painter.drawRect(2+widgwidth*3/4, 2+561, widgwidth*1/4-4, 96);
    }

    if (mb.GetConnectFailureStatus() == 1)
    {
        painter.setFont(QFont("Times New Roman", 25, QFont::ExtraBold));
        Channel1ValueString=Channel2ValueString=Channel3ValueString=Channel4ValueString = "Connection Fail";
    }

    else if (mb.GetConnectFailureStatus() == 2)
    {
        painter.setFont(QFont("Times New Roman", 25, QFont::ExtraBold));
        Channel1ValueString=Channel2ValueString=Channel3ValueString=Channel4ValueString = "CRC Error";
    }
    else {
        painter.setFont(QFont("Times New Roman", 50, QFont::ExtraBold));
        Channel1ValueString = QString::number(UartDriver::channelinputbuffer[0]);
        Channel2ValueString = QString::number(UartDriver::channelinputbuffer[1]);
        Channel3ValueString = QString::number(UartDriver::channelinputbuffer[2]);
        Channel4ValueString = QString::number(UartDriver::channelinputbuffer[3]);
    }

    //вычесление бесконечной суммы с нужной точностью eps

    /*
    double s = 0;
    double a = 0;
    double eps = 0.00001;
    int i=1;

    while (  i )
    {

        a = pow (i,2)/(pow(3,i));
        s += a;
        i++;
        if (a < eps)
        {
            qDebug() << i;
            qDebug() << s;
            break;

        }
    }
    */

    //Channel1ValueString=Channel2ValueString=Channel3ValueString=Channel4ValueString = QString::number(s);
    painter.drawText(2, 2+561, widgwidth*1/4, 96,     Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
    painter.drawText(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
    painter.drawText(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
    painter.drawText(2+widgwidth*3/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

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

void MainWindow::PaintStatesAndAlerts() // отрисовывает события на уставки
{
    QPainter painter;

    int widgwidth  = ui->MessagesWidget->width();
    int widgheight  = ui->MessagesWidget->height();

    int alertwindowwidth = widgwidth/2-2;
    int alertwindowheight = widgheight/2/4;
    int alerttextsize = 30;

    double channel1currentvalue = UartDriver::channelinputbuffer[0];
    double channel2currentvalue = UartDriver::channelinputbuffer[1];
    double channel3currentvalue = UartDriver::channelinputbuffer[2];
    double channel4currentvalue = UartDriver::channelinputbuffer[3];

    double channel1state1value = ch1.GetState1Value();
    double channel2state1value = ch2.GetState1Value();
    double channel3state1value = ch3.GetState1Value();
    double channel4state1value = ch4.GetState1Value();

    double channel1state2value = ch1.GetState2Value();
    double channel2state2value = ch2.GetState2Value();
    double channel3state2value = ch3.GetState2Value();
    double channel4state2value = ch4.GetState2Value();

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(Channel1Color, Qt::SolidPattern));
    painter.drawRect(2, 2, alertwindowwidth, alertwindowheight-2);
    painter.setBrush(QBrush(Channel2Color, Qt::SolidPattern));
    painter.drawRect(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight-2);
    painter.setBrush(QBrush(Channel3Color, Qt::SolidPattern));
    painter.drawRect(2, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2);
    painter.setBrush(QBrush(Channel4Color, Qt::SolidPattern));
    painter.drawRect(2+alertwindowwidth, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2);

    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 12, QFont::ExtraBold));
    painter.drawText(2, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignTop, ch1.GetChannelName());
    painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignTop,ch2.GetChannelName());
    painter.drawText(2, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignTop,ch3.GetChannelName());
    painter.drawText(2+alertwindowwidth, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignTop,ch4.GetChannelName());

    painter.setFont(QFont("Times New Roman", alerttextsize, QFont::ExtraBold));

    // увеличение уставки Channel 1
    if (channel1currentvalue>channel1state1value)
    {
        //        qDebug() << GetChannel1Color();
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch1.GetState1HighMessage());
        SetChannel1Color(ChannelColorHighState);
    }
    // уменьшение уставки  Channel 1
    else if (channel1currentvalue<channel1state2value)
    {

        painter.drawText(2, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch1.GetState2LowMessage());
        SetChannel1Color(ChannelColorLowState);
    }

    else
    {
         painter.drawText(2, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel1Color(Channel1ColorNormal);
    }

    // увеличение уставки Channel 2
    if (channel2currentvalue>channel2state1value)
    {
        //        qDebug() << GetChannel2Color();
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch2.GetState1HighMessage());
        SetChannel2Color(ChannelColorHighState);
    }
    // уменьшение уставки  Channel 2
    else if (channel2currentvalue<channel2state2value)
    {

        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch2.GetState2LowMessage());
        SetChannel2Color(ChannelColorLowState);
    }

    else
    {
         painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel2Color(Channel2ColorNormal);
    }

    // увеличение уставки Channel 3
    if (channel3currentvalue>channel3state1value)
    {
        //        qDebug() << GetChannel3Color();
        painter.drawText(2, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch3.GetState1HighMessage());
        SetChannel3Color(ChannelColorHighState);
    }
    // уменьшение уставки  Channel 3
    else if (channel3currentvalue<channel3state2value)
    {

        painter.drawText(2, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch3.GetState2LowMessage());
        SetChannel3Color(ChannelColorLowState);
    }

    else
    {
         painter.drawText(2, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel3Color(Channel3ColorNormal);
    }


    // увеличение уставки Channel 4
    if (channel4currentvalue>channel4state1value)
    {
        //        qDebug() << GetChannel4Color();
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch4.GetState1HighMessage());
        SetChannel4Color(ChannelColorHighState);
    }
    // уменьшение уставки  Channel 4
    else if (channel4currentvalue<channel4state2value)
    {

        painter.drawText(2+alertwindowwidth, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, ch4.GetState2LowMessage());
        SetChannel4Color(ChannelColorLowState);
    }

    else
    {
         painter.drawText(2+alertwindowwidth, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel4Color(Channel4ColorNormal);
    }


    if  (GetHalfSecFlag() == 1)
    {
        painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
        painter.setFont(QFont("Times New Roman", 70, QFont::ExtraBold));

        // если сработала какая-то уставка, то начинаем мигать восклицательным флагом
        if ((channel1currentvalue>channel1state1value) || (channel1currentvalue<channel1state2value))
            painter.drawText(2, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignRight | Qt::AlignVCenter, "!");
        if ((channel2currentvalue>channel2state1value) || (channel2currentvalue<channel2state2value))
            painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight-2, Qt::AlignRight | Qt::AlignVCenter,"!");
        if ((channel3currentvalue>channel3state1value) || (channel3currentvalue<channel3state2value))
            painter.drawText(2, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignRight | Qt::AlignVCenter,"!");
        if ((channel4currentvalue>channel4state1value) || (channel4currentvalue<channel4state2value))
            painter.drawText(2+alertwindowwidth, 2+alertwindowheight-2, alertwindowwidth, alertwindowheight-2, Qt::AlignRight | Qt::AlignVCenter,"!");
    }

    ModBus mb;
    // если связб плохая и прошло пол секунды то нужно мигнуть красным цветом
    if ( (GetHalfSecFlag() == 1)&&(mb.GetConnectFailureStatus() >0) )
    {
        QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;

        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+widgwidth*3/4, 2+561, widgwidth*1/4-4, 96);

        painter.setFont(QFont("Times New Roman", 25, QFont::ExtraBold));
        Channel1ValueString=Channel2ValueString=Channel3ValueString=Channel4ValueString = "Connection Fail";
        painter.drawText(2, 2+561, widgwidth*1/4, 96,     Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
        painter.drawText(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
        painter.drawText(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
        painter.drawText(2+widgwidth*3/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

        //    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
        painter.setFont(QFont("Times New Roman", 15, QFont::ExtraBold));
        painter.drawText(2, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop, ch1.GetChannelName());
        painter.drawText(2+widgwidth*1/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop,ch2.GetChannelName());
        painter.drawText(2+widgwidth*2/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop,ch3.GetChannelName());
        painter.drawText(2+widgwidth*3/4, 2+561, widgwidth*1/4, 96, Qt::AlignHCenter | Qt::AlignTop,ch4.GetChannelName());
    }
    painter.end();
}

void MainWindow::PaintPolarDiagramm()
{
    QPainter painter;
    int widgheight  = ui->MessagesWidget->height();
    double maximumradius;
    maximumradius = widgheight/4 - 10;
    painter.begin(ui->MessagesWidget);
    int channel1value = X_Coordinates.last();
    int channel2value = X_Coordinates.last();
    int channel3value = X_Coordinates.last();
    int channel4value = X_Coordinates.last();

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

    QPoint NewPolarPointChannel1,NewPolarPointChannel2,NewPolarPointChannel3,NewPolarPointChannel4;
    NewPolarPointChannel1.setX(x1);
    NewPolarPointChannel1.setY(y1);

    NewPolarPointChannel2.setX(x2);
    NewPolarPointChannel2.setY(y2);

    NewPolarPointChannel3.setX(x3);
    NewPolarPointChannel3.setY(y3);

    NewPolarPointChannel4.setX(x4);
    NewPolarPointChannel4.setY(y4);

    PolarChartPointsChannel1.append(NewPolarPointChannel1);
    PolarChartPointsChannel2.append(NewPolarPointChannel2);
    PolarChartPointsChannel3.append(NewPolarPointChannel3);
    PolarChartPointsChannel4.append(NewPolarPointChannel4);

    int a = X_Coordinates.last();
    if ( a%360 >= 0)
    {
        //X_Coordinates.clear();
        PolarChartPointsChannel1.clear(); //*** после продолжительной работы замедляется (тормзоит ) построение графика - проверить
        PolarChartPointsChannel2.clear();
        PolarChartPointsChannel3.clear();
        PolarChartPointsChannel4.clear();
    }

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Channel1Color, 4));
    painter.drawPolyline(PolarChartPointsChannel1);
    painter.setPen(QPen(Channel2Color, 4));
    painter.drawPolyline(PolarChartPointsChannel2);
    painter.setPen(QPen(Channel3Color, 4));
    painter.drawPolyline(PolarChartPointsChannel3);
    painter.setPen(QPen(Channel4Color, 4));
    painter.drawPolyline(PolarChartPointsChannel4);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.end();
}

void MainWindow::PaintOnWidget()
{
    PaintStatesAndAlerts();
    switch( Options::DisplayParametr )
    {
    case Options::Cyfra:
        PaintCyfrasFullScreen();break;
    case Options::TrendsCyfra:
        PaintCyfrasBottom();break;
    case Options::Trends:
        //        PaintStatesAndAlerts();break;
        PaintCyfrasFullScreen();break;
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
