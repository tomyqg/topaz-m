#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
//#include "options.h"
#include "defines.h"
#include "stackedoptions.h"
#include "Channels/group_channels.h"
#include "Channels/freq_channel.h"

//extern QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern QList<cGroupChannels*> listGroup;
extern QList<ChannelOptions *> listChannels;
extern QList<cMathChannel *> listMath;
extern QList<cFreqChannel *> listFreq;


// полярные координаты
void MainWindow::PaintPolarDiagramm()
{
//    QPainter painter;

    painter.begin(ui->MessagesWidget);
    int widgetHeight = ui->MessagesWidget->height();
    int widgetWidth = ui->MessagesWidget->width();
    int widgetSize = min(widgetHeight, widgetWidth);
    int diagramSize = widgetSize - 50;

    painter.setRenderHint(QPainter::Antialiasing, true);

    int channel1value = GetPolarAngle();
    int channel2value = GetPolarAngle();
    int channel3value = GetPolarAngle();
    int channel4value = GetPolarAngle();

    if(curGroupChannel >= listGroup.size()) return;
    //получение текущих значений в процентах
    cGroupChannels * group = listGroup.at(curGroupChannel);
    float channelLenght[MAX_NUM_CHAN_GROUP];
//    float channel1length = NAN;
//    float channel2length = NAN;
//    float channel3length = NAN;
//    float channel4length = NAN;

    for(int i = 0; i < MAX_NUM_CHAN_GROUP; i ++)
    {
        if((group->typeInput[i] == cGroupChannels::Input_Analog) && (group->channel[i] != -1))
        {
            ChannelOptions * channel = listChannels.at(group->channel[i]);
#ifdef RANDOM_CHAN
            if((channel->getNum() <= NUM_CHAN_IN_4AI) || channel->enable)
#else
            if(channel->enable)
#endif
            {
                channelLenght[i] = channel->GetValuePercent();
            }
        }
        else if((group->typeInput[i] == cGroupChannels::Input_Math) && (group->mathChannel[i] != -1))
        {
            if((group->mathChannel[i] < listMath.size()) && (listMath.size() != 0))
            {
                cMathChannel * math = listMath.at(group->mathChannel[i]);
                channelLenght[i] = math->GetValuePercent();
            }
        }
        else if((group->typeInput[i] == cGroupChannels::Input_Freq) && (group->freqChannel[i] != -1))
        {
            cFreqChannel * channel = listFreq.at(group->freqChannel[i]);
            if(channel->enable)
            {
                channelLenght[i] = channel->GetValuePercent();
            }
        }
        else
        {
            channelLenght[i] = NAN;
        }
        //корректировка данных для диаграммы
        if(channelLenght[i] < 0) channelLenght[i] = 0;
        if(channelLenght[i] > 100) channelLenght[i] = 100;

        //перевод процентов в координатную размерность
        channelLenght[i] = channelLenght[i] * diagramSize / 100;
    }


    QColor color1,color2,color3,color4;

    color1 = ColorCh1;
    color2 = ColorCh2;
    color3 = ColorCh3;
    color4 = ColorCh4;
    
#define SHIFT_DIAGRAM_X 5
#define SHIFT_DIAGRAM_Y (-5)

    int centerx1,centerx2,centerx3,centerx4;
    int centery1,centery2,centery3,centery4;
    int newxcenter = SHIFT_DIAGRAM_X, newycenter = widgetHeight + SHIFT_DIAGRAM_Y;
    
    centerx1 = centerx2 = centerx3 = centerx4 = 1;
    centery1 = centery2 = centery3 = centery4 = 1;
    
    painter.translate(newxcenter, newycenter);
    /* Create the line object: */
//    painter.setPen(QPen(Qt::black, 1));

    if (GetEcoMode()){
        painter.setBrush(QBrush(EcoColor, Qt::SolidPattern));
        painter.setPen(QPen(NotEcoColor,1,  Qt::DotLine));
    }
    else
    {
        painter.setBrush(QBrush(NotEcoColor, Qt::SolidPattern));
        painter.setPen(QPen(EcoColor,1,  Qt::DotLine));
    }
//    painter.drawRect(0, -newycenter,ui->MessagesWidget->width()-1,ui->MessagesWidget->height());
    painter.drawLine(0, -diagramSize - 20, 0, 0);
    painter.drawLine(0, 0, diagramSize + 40, 0);
    
    // Рисуем пять дуг разметочки
    painter.drawArc(-diagramSize, -diagramSize, 2*diagramSize, 2*diagramSize, 0, 90 * 16); // 100%
    painter.drawArc(-diagramSize*4/5, -diagramSize*4/5, 2*diagramSize*4/5, 2*diagramSize*4/5, 0, 90 * 16);// 80%
    painter.drawArc(-diagramSize*3/5, -diagramSize*3/5, 2*diagramSize*3/5, 2*diagramSize*3/5, 0, 90 * 16);// 60%
    painter.drawArc(-diagramSize*2/5, -diagramSize*2/5, 2*diagramSize*2/5, 2*diagramSize*2/5, 0, 90 * 16);// 40%
    painter.drawArc(-diagramSize/5, -diagramSize/5, 2*diagramSize/5, 2*diagramSize/5, 0, 90 * 16);// 20%

    //ещё 5 дуг дополнительно
    painter.drawArc(-diagramSize*9/10, -diagramSize*9/10, \
                    2*diagramSize*9/10, 2*diagramSize*9/10, 0, 90 * 16); // 100%
    painter.drawArc(-diagramSize*7/10, -diagramSize*7/10, \
                    2*diagramSize*7/10, 2*diagramSize*7/10, 0, 90 * 16);// 80%
    painter.drawArc(-diagramSize*5/10, -diagramSize*5/10, \
                    2*diagramSize*5/10, 2*diagramSize*5/10, 0, 90 * 16);// 60%
    painter.drawArc(-diagramSize*3/10, -diagramSize*3/10, \
                    2*diagramSize*3/10, 2*diagramSize*3/10, 0, 90 * 16);// 40%
    painter.drawArc(-diagramSize*1/10, -diagramSize*1/10, \
                    2*diagramSize*1/10, 2*diagramSize*1/10, 0, 90 * 16);// 20%

    QLineF Channel1Line;
    
    //очистка хвостов
    if (GetPolarAngle()>=90) // если больше 90 градусов то поворачиваем диск
    {
        painter.rotate(GetPolarAngle()-90);
        if (GetPolarAngle()>=179)   // оставляем только 180 точек (на 180 градусов)
        {
            PolarChartPointsChannel1.removeFirst();
            PolarChartPointsChannel2.removeFirst();
            PolarChartPointsChannel3.removeFirst();
            PolarChartPointsChannel4.removeFirst();
            //SetPolarAngle(180);
        }
    }
    

    // радиальные линии
    for(int i=0; i < 36; i++)
    {
        QLineF Line;
        Line.setP1(QPointF(centerx1, centery1));
        Line.setAngle(10*(i));
        Line.setLength(diagramSize);
        painter.drawLine(Line);
    }
    
    // отсюда начинаем рисовать значения на круговой диаграмме
//    painter.setPen(QPen(Qt::green, 1));
    /* Set the origin: */
    Channel1Line.setP1(QPointF(centerx1, centery1));
    Channel1Line.setAngle(channel1value);
    Channel1Line.setLength(channelLenght[0]);
    
    int x1 = Channel1Line.x2(); // мы берем координаты `1 точки
    int y1 = Channel1Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel2Line;
//    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel2Line.setP1(QPointF(centerx2, centery2));
    Channel2Line.setAngle(channel2value);
    Channel2Line.setLength(channelLenght[1]);
    
    int x2 = Channel2Line.x2(); // мы берем координаты `1 точки
    int y2 = Channel2Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel3Line;
//    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel3Line.setP1(QPointF(centerx3, centery3));
    Channel3Line.setAngle(channel3value);
    Channel3Line.setLength(channelLenght[2]);
    int x3 = Channel3Line.x2(); // мы берем координаты `1 точки
    int y3 = Channel3Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel4Line;
//    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel4Line.setP1(QPointF(centerx4, centery4) );
    Channel4Line.setAngle(channel4value);
    Channel4Line.setLength(channelLenght[3]);
    
    int x4 = Channel4Line.x2(); // мы берем координаты `1 точки
    int y4 = Channel4Line.y2(); // мы берем координаты второй точки
    

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
    
    painter.setPen(QPen(color1, 2));
    if(PolarChartPointsChannel1.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel1);
    painter.setPen(QPen(color2, 2));
    if(PolarChartPointsChannel2.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel2);
    painter.setPen(QPen(color3, 2));
    if(PolarChartPointsChannel3.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel3);
    painter.setPen(QPen(color4, 2));
    if(PolarChartPointsChannel4.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel4);

    painter.resetTransform(); // все что дальше - не поворачивается динамически

    painter.setFont(QFont(Font, 10, QFont::Bold));
    painter.translate(newxcenter, newycenter);

    painter.drawText(0,               -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "0%");
    painter.drawText(diagramSize/5,   -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "20%");
    painter.drawText(diagramSize*2/5, -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "40%");
    painter.drawText(diagramSize*3/5, -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "60%");
    painter.drawText(diagramSize*4/5, -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "80%");
    painter.drawText(diagramSize,     -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "100%");

    painter.drawText(0, -30-diagramSize/5,   40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "20%");
    painter.drawText(0, -30-diagramSize*2/5, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "40%");
    painter.drawText(0, -30-diagramSize*3/5, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "60%");
    painter.drawText(0, -30-diagramSize*4/5, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "80%");
    painter.drawText(0, -30-diagramSize,     40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "100%");

    //стилизация круговой диаграммы (короче понты всякие)
    //вертикальные понты
    painter.setPen(ColorBlue);
    painter.setBrush(QBrush(ColorBlue, Qt::SolidPattern));
    painter.drawRect(0, 0, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh1);
    painter.setBrush(QBrush(ColorCh1, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh1Light);
    painter.setBrush(QBrush(ColorCh1Light, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize*2/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh3);
    painter.setBrush(QBrush(ColorCh3, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize*3/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh3Light);
    painter.setBrush(QBrush(ColorCh3Light, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize*4/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    //горизонтальные понты
    painter.setPen(ColorBlue);
    painter.setBrush(QBrush(ColorBlue, Qt::SolidPattern));
    painter.drawRect(0, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh1);
    painter.setBrush(QBrush(ColorCh1, Qt::SolidPattern));
    painter.drawRect(diagramSize/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh1Light);
    painter.setBrush(QBrush(ColorCh1Light, Qt::SolidPattern));
    painter.drawRect(diagramSize*2/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh3);
    painter.setBrush(QBrush(ColorCh3, Qt::SolidPattern));
    painter.drawRect(diagramSize*3/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh3Light);
    painter.setBrush(QBrush(ColorCh3Light, Qt::SolidPattern));
    painter.drawRect(diagramSize*4/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    //специальный белый квадратик в начале осей координат
    painter.setPen(Qt::white);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(0, 0, -SHIFT_DIAGRAM_X, -SHIFT_DIAGRAM_Y);

    painter.end();

}

void MainWindow::PaintOnWidget()
{
    switch(systemOptions.display)
    {
    case cSystemOptions::Cyfra:
        break;
    case cSystemOptions::TrendsCyfra:
        break;
    case cSystemOptions::Trends:
        break;
    case cSystemOptions::TrendsCyfraBars:
        break;
    case cSystemOptions::BarsCyfra:
        break;
    case cSystemOptions::PolarBars:
    case cSystemOptions::PolarCyfra:
//        PaintStatesAndAlertsAtTop();
        PaintPolarDiagramm();
        break;
    default:
        break;
    }
}

//void MainWindow::ReactOnTouch()
//{
//    //пока просто возвращаем

//    return;
//    // подкрашиваем квадраты куда коснулись в желтый цвет
//    // отступ  сверху и слева в пикселях
//    // высчитываются
    
//    int xcenter  = 652;// общая точка четырех квадратов по иксу в пикселях
//    int ycenter  = 384; // ширина всей области построения в пикселях
    
//    int xpos = QCursor::pos().x();
//    int ypos = QCursor::pos().y();
    
//    QString x = QString::number(xpos);
//    QString y = QString::number(ypos);

//    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
//    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
//    int confirmwindowwidth = widgwidth/4;
//    int confirmwindowheight  = widgheight/4;
//    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
//    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
//    int confirmwindowposx2 = confirmwindowposx  +  confirmwindowwidth;
//    int confirmwindowposy2 = confirmwindowposy + confirmwindowheight ;

//    if      ((xpos>confirmwindowposx) &&
//             (xpos<confirmwindowposx2) &&
//             (ypos>confirmwindowposy) &&
//             (ypos<confirmwindowposy2) )
//    {
////        if (channel4.GetConfirmationNeed() == true) {
////            channel4.SetConfirmationNeed(false);
////        }
////        if (channel3.GetConfirmationNeed() == true) {
////            channel3.SetConfirmationNeed(false);
////        }
////        if (channel2.GetConfirmationNeed() == true) {
////            channel2.SetConfirmationNeed(false);
////        }
////        if (channel1.GetConfirmationNeed() == true) {
////            channel1.SetConfirmationNeed(false);
////        }
//    }
    
//    int x1right  = 940;
//    int x2right  = 1240;
//    int y0right  = 162;
//    int y1right  = 258;
    
//    if      ((xpos>x1right) &&
//             (xpos<x2right) &&
//             (ypos>y0right) &&
//             (ypos<y1right) )
//    {
//        {
//            QPainter painter;
//            painter.begin(ui->MessagesWidget);
//            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
//            int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
//            int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
//            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
//            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
//            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,"Allelua");
//            painter.end();
//        }
//    }
//}
